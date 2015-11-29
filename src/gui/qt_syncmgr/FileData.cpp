
#include "FileData.h"

// ctor and dtor
FileData::FileData(): 
    root_(NULL)
{
    // do nothing
}
FileData::~FileData()
{
    clear(root_);
}

// access
void 
FileData::retrieve(const QString &path, Data &data, bool &retrieved)
{
    OperationStatus status = Searching;
    QVector<QString> pathvec(path.split(QDir::separator()).toVector());
    retrieve(root_, 0, pathvec.count()-1, pathvec, data, status);
    retrieved = (status == Found) ? true : false;
}

void 
FileData::insert(const QString &path, const Data &data, bool &inserted)
{
    OperationStatus status = Searching;
    QVector<QString> pathvec(path.split(QDir::separator()).toVector());
    insert(root_, 0, pathvec.count()-1, pathvec, data, status);
    inserted = (status == Inserted) ? true : false;
}

void 
FileData::update(const QString &path, const Data &data, bool &updated)
{
    OperationStatus status = Searching;
    QVector<QString> pathvec(path.split(QDir::separator()).toVector());
    update(root_, 0, pathvec.count()-1, pathvec, data, status);
    updated = (status == Updated) ? true : false;
}

void 
FileData::remove(const QString &path, bool &removed)
{
    OperationStatus status = Searching;
    QVector<QString> pathvec(path.split(QDir::separator()).toVector());
    remove(root_, 0, pathvec.count()-1, pathvec, status);
    removed = (status == Removed) ? true : false;
}

// output data
void
FileData::print(QStringList &path, const Node * const &node, bool output) const
{
    if (node == NULL)
    {
        if (output) qDebug() << path.join(QDir::separator());
    }
    else
    {
        path.push_back(node->name_);
        print(path, node->child_, true);
        path.pop_back();
        print(path, node->sibling_, false);
    }
}

void
FileData::print(const Node * const &node) const
{
    if (node != NULL)
    {
        QStringList path;
        print(path, node, true);
    }
}

void
FileData::print() const
{
    print(root_);
}

// utilities
void
FileData::clear(Node *&node)
{
    if (node != NULL)
    {
        clear(node->child_);
        clear(node->sibling_);
        delete node;
        node = NULL;
    }
}

void
FileData::clear()
{
    clear(root_);
}

void 
FileData::retrieve(Node *&node, int idx, int maxidx, const QVector<QString> &pathvec, Data &data, OperationStatus &status)
{
    // sanity check
    if (node == NULL)
    {
        status = NotFound;
        return;
    }

    // traverse this level by following siblings.
    for (Node *ps = node; ps != NULL; ps=ps->sibling_)
    {
        // look for a match at this level
        if (pathvec[idx] == ps->name_)
        {
            // names match 
            if (idx == maxidx)
            {
                // done with the path and we have a match.
                data = ps->data_;
                status = Found;
            }
            else if (ps->data_.isdir_)
            {
                // check the next level downs since the current
                // level is a directory. follow the children.
                for (Node *pc = ps->child_; 
                     pc != NULL && status == Searching; 
                     pc=pc->child_)
                {
                    retrieve(pc, idx+1, maxidx, pathvec, data, status);
                }
            }
            else
            {
                // not done with the path, but the file is not a
                // directory. cannot continue to search.
                status = NotFound;
            }
            return;
        }
    }

    // no match at this level.
    status = NotFound;
    return;
}

void 
FileData::insert(Node *&node, int idx, int maxidx, const QVector<QString> &pathvec, const Data &data, OperationStatus &status)
{
    // sanity check
    if (node == NULL)
    {
        addNewNodes(node, idx, maxidx, pathvec, data, status);
        return;
    }

    // traverse this level by following siblings.
    for (Node *ps = node; ps != NULL; ps=ps->sibling_)
    {
        // look for a match at this level
        if (pathvec[idx] == ps->name_)
        {
            // names match 
            if (idx == maxidx)
            {
                // done with the path and we have a match.
                // cannot overwrite
                status = NotInserted;
            }
            else if (ps->data_.isdir_)
            {
                // check the next level downs since the current
                // level is a directory. follow the children.
                for (Node *pc = ps->child_; 
                     pc != NULL && status == Searching; 
                     pc=pc->child_)
                {
                    insert(pc, idx+1, maxidx, pathvec, data, status);
                }
            }
            else
            {
                // not done with the path, but the file is not a
                // directory. cannot continue to search.
                status = NotInserted;
            }
            return;
        }
    }

    // add new node as a sibling
    Node *newnode;
    addNewNodes(newnode, idx, maxidx, pathvec, data, status);
    newnode->sibling_ = node->sibling_;
    node->sibling_ = newnode;
    return;
}

void 
FileData::update(Node *&node, int idx, int maxidx, const QVector<QString> &pathvec, const Data &data, OperationStatus &status)
{
    // sanity check
    if (node == NULL)
    {
        status = NotUpdated;
        return;
    }

    // traverse this level by following siblings.
    for (Node *ps = node; ps != NULL; ps=ps->sibling_)
    {
        // look for a match at this level
        if (pathvec[idx] == ps->name_)
        {
            // names match 
            if (idx == maxidx)
            {
                // done with the path and we have a match.
                // don't allow changing the type of file.
                if (ps->data_.isdir_ == data.isdir_)
                {
                    ps->data_ = data;
                    status = Updated;
                }
                else
                {
                    // don't allow changing the type of file.
                    status = NotUpdated;
                }
            }
            else if (ps->data_.isdir_)
            {
                // check the next level downs since the current
                // level is a directory. follow the children.
                for (Node *pc = ps->child_; 
                     pc != NULL && status == Searching; 
                     pc=pc->child_)
                {
                    update(pc, idx+1, maxidx, pathvec, data, status);
                }
            }
            else
            {
                // not done with the path, but the file is not a
                // directory. cannot continue to search.
                status = NotUpdated;
            }
            return;
        }
    }

    // no match at this level.
    status = NotUpdated;
    return;
}

void 
FileData::remove(Node *&node, int idx, int maxidx, const QVector<QString> &pathvec, OperationStatus &status)
{
    // sanity check
    if (node == NULL)
    {
        status = NotRemoved;
        return;
    }

    // traverse this level by following siblings.
    for (Node **pps = &node; *pps != NULL; pps=&((*pps)->sibling_))
    {
        // look for a match at this level
        if (pathvec[idx] == (*pps)->name_)
        {
            // names match 
            if (idx == maxidx)
            {
                // done with the path and we have a match.
                // delete all children and reconnect sibling list
                clear((*pps)->child_);
                Node *psave = (*pps)->sibling_;
                delete (*pps);
                (*pps) = psave;
                status = Removed;
            }
            else if ((*pps)->data_.isdir_)
            {
                // check the next level downs since the current
                // level is a directory. follow the children.
                for (Node **ppc = &((*pps)->child_); 
                     status == Searching && (*ppc) != NULL; 
                     ppc=&((*ppc)->child_))
                {
                    remove(*ppc, idx+1, maxidx, pathvec, status);
                    if (status != Searching) break;
                }
            }
            else
            {
                // not done with the path, but the file is not a
                // directory. cannot continue to search.
                status = NotRemoved;
            }
            return;
        }
    }

    // no match at this level.
    status = NotRemoved;
    return;
}

void 
FileData::addNewNodes(Node *&node, int idx, int maxidx, const QVector<QString> &pathvec, const Data &data, OperationStatus &status)
{
    if (idx < maxidx)
    {
        Node *pn = new Node;
        pn->name_ = pathvec[idx];
        pn->data_.isdir_ = true;
        addNewNodes(pn->child_, idx+1, maxidx, pathvec, data, status);
        node = pn;
    }
    else
    {
        Node *pn = new Node;
        pn->name_ = pathvec[idx];
        pn->data_ = data;
        node = pn;
        status = Inserted;
    }
}
