// test file data stream

#include "TestDataStream.h"

// ctors and dtor
TestDataStream::TestDataStream(const std::string &filename):
    ok_(false), filename_(filename), keys_(), data_()
{
    ok_ = true;
}

TestDataStream::~TestDataStream()
{
    // do nothing
}

// parser and utility functions
int
TestDataStream::parse(const std::string &buf, KeyType &key, DataType &data,
                      const std::string &delimiters)
{
    std::string::size_type lastpos = buf.find_first_not_of(delimiters, 0);
    std::string::size_type pos = buf.find_first_of(delimiters, lastpos);

    bool iskey = true;

    while (std::string::npos != pos || std::string::npos != lastpos)
    {
        if (iskey)
        {
            key = buf.substr(lastpos, pos - lastpos);
            iskey = false;
        }
        else
            data.push_back(buf.substr(lastpos, pos - lastpos));

        lastpos = buf.find_first_not_of(delimiters, pos);
        pos = buf.find_first_of(delimiters, lastpos);
    }

    if (key.length() == 0)
        return(1);
    else
        return(0);
}

// init/release data
int
TestDataStream::open()
{
    if (!ok_ || (filename_.length() == 0)) return(-1);

    std::ifstream infile;
    infile.open(filename_.c_str());
    if (!infile) return(-1);

    char buf[BUFSIZ+1];
    for (infile.getline(buf, BUFSIZ); 
        !infile.eof() && !infile.fail();
         infile.getline(buf, BUFSIZ))
    {
        // parse the record and store the data
        KeyType key;
        DataType data;
        if (parse(buf, key, data) != 0)
        {
            std::cout << "Warning:: skip record!" << std::endl;
            continue;
        }

        // add key at the end and in order
        keys_.push_back(key);
        data_.insert(std::pair<KeyType, DataType>(key, data));
    }

    infile.close();

    return(0);
}

int
TestDataStream::close()
{
    return(-1);
}

// iterators for list of keys
TestDataStream::KeyTypeListConstIterator
TestDataStream::begin() const
{
    return(keys_.begin());
}

TestDataStream::KeyTypeListConstIterator
TestDataStream::end() const
{
    return(keys_.end());
}

// access data via key
int
TestDataStream::read(const KeyType &key, DataType &data) const
{
    // returns -1=error, 0=not found, 1=found
    if (!isOk()) return(-1);
    DataTypeMapConstIterator iter = data_.find(key);
    if (iter == data_.end()) return(0);
    data = iter->second;
    return(1);
}
