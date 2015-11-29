#ifndef _ABSTRACTITEMMODELADAPTOR_H
#define _ABSTRACTITEMMODELADAPTOR_H

// adaptor to Qt's abstract item model 
#include <string>
#include <vector>
#include <QAbstractItemModel>

class AbstractItemModelAdaptor: public QAbstractItemModel
{
public:
    AbstractItemModelAdaptor(QObject *parent = NULL):
        QAbstractItemModel(parent), ok_(false), readonly_(false) { }
    virtual ~AbstractItemModelAdaptor() { }

    inline bool isOk() const { return(ok_); }

    inline bool getReadOnly() const { return(readonly_); }
    inline void setReadOnly(bool rd) { readonly_ = rd; }

    void tokenize(const std::string &str, 
                  std::vector<std::string> &tokens,
                  int &keylevel, 
                  const std::string delimiters = "\t\n")
    {
        std::string::size_type lastpos = 
            str.find_first_not_of(delimiters, 0);
        std::string::size_type pos = 
            str.find_first_of(delimiters, lastpos);

        keylevel = lastpos;

        for (tokens.clear(); 
             std::string::npos != pos || std::string::npos != lastpos; )
        {
            tokens.push_back(str.substr(lastpos, pos - lastpos));
            lastpos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastpos);
        }
    }

protected:
    mutable bool ok_;
    bool readonly_;
};

#endif
