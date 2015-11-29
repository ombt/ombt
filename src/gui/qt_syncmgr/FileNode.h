#ifndef __FILE_NODE_H
#define __FILE_NODE_H

#include <QString>

class FileNode
{
public:
    // ctors and dtor
    FileNode();
    FileNode(const QString &name);
    FileNode(const QString &name, const QString &path);
    FileNode(const FileNode &src);
    ~FileNode();

    // assignment
    FileNode &operator=(const FileNode &rhs);

    // access
    const QString &getName() const;
    void setName(const QString &newname);
    const QString &getPath() const;
    void setPath(const QString &newpath);

    // comparisons
    bool operator==(const FileNode &rhs);
    bool operator!=(const FileNode &rhs);
    bool operator<(const FileNode &rhs);
    bool operator<=(const FileNode &rhs);
    bool operator>(const FileNode &rhs);
    bool operator>=(const FileNode &rhs);
    
private:
    QString name_;
    QString path_;
};

#endif 
