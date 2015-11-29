#ifndef __TESTDATASTREAM_H
#define __TESTDATASTREAM_H

// test file data stream:
//
// key data data ...
// key data data ...
// key data data ...

// system headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <map>

// test data stream read in from a file
class TestDataStream
{
public:
    // test types
    typedef std::string KeyType;
    typedef std::list<KeyType> KeyTypeList;
    typedef KeyTypeList::const_iterator KeyTypeListConstIterator;

    typedef std::list<std::string> DataType;
    typedef DataType::const_iterator DataTypeConstIterator;
    typedef std::map<KeyType, DataType> DataTypeMap;
    typedef DataTypeMap::const_iterator DataTypeMapConstIterator;

public:
    // ctors and dtor
    TestDataStream(const std::string &filename);
    ~TestDataStream();

    // check internal status
    inline bool isOk() const { return(ok_); }

    // init and release of data
    int open();
    int close();

    // provide these to access data
    int read(const KeyType &key, DataType &data) const;

    // iterators
    KeyTypeListConstIterator begin() const;
    KeyTypeListConstIterator end() const;

protected:
    // utilities
    int parse(const std::string &buf, KeyType &key, DataType &data,
              const std::string &delimiters = " ");

private:
    // do not allow
    TestDataStream();
    TestDataStream(const TestDataStream &src);
    TestDataStream &operator=(const TestDataStream &rhs);

protected:
    mutable bool ok_;
    std::string filename_;
    KeyTypeList keys_;
    DataTypeMap data_;
};

#endif
