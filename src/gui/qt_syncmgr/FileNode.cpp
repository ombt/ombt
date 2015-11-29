
#include "FileNode.h"

// ctors and dtor
FileNode::FileNode():
    name_(), path_() { }

FileNode::FileNode(const QString &name):
    name_(name), path_() { }

FileNode::FileNode(const QString &name, const QString &path):
    name_(name), path_(path) { }

FileNode::FileNode(const FileNode &src):
    name_(src.name_), path_(src.path_) { }

FileNode::~FileNode() { }

// assignment
FileNode &
FileNode::operator=(const FileNode &rhs)
{
    if (this != &rhs)
    {
        name_ = rhs.name_;
        path_ = rhs.path_;
    }
    return(*this);
}

    // access
const QString &
FileNode::getName() const
{
    return(name_);
}

void 
FileNode::setName(const QString &newname)
{
    name_ = newname;
}

const QString &
FileNode::getPath() const
{
    return(path_);
}

void 
FileNode::setPath(const QString &newpath)
{
    path_ = newpath;
}

// comparisons using name
bool 
FileNode::operator==(const FileNode &rhs)
{
    return(name_ == rhs.name_);
}

bool 
FileNode::operator!=(const FileNode &rhs)
{
    return(name_ != rhs.name_);
}

bool 
FileNode::operator<(const FileNode &rhs)
{
    return(name_ < rhs.name_);
}

bool 
FileNode::operator<=(const FileNode &rhs)
{
    return(name_ <= rhs.name_);
}

bool 
FileNode::operator>(const FileNode &rhs)
{
    return(name_ > rhs.name_);
}

bool 
FileNode::operator>=(const FileNode &rhs)
{
    return(name_ >= rhs.name_);
}
