//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_NOTIFY_H
#define __OMBT_NOTIFY_H

// system headers
#include <unistd.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <map>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"
#include "atomic/UCBaseObject.h"
#include "sockets/Address.h"
#include "sockets/Socket.h"
#include "sockets/EndPoint.h"

namespace ombt {

// forward declaration
class EndPoint;

// file notify class
class Notify: public Socket {
public:
    // ctors and dtor
    Notify();
    ~Notify();

    // basic socket operations
    int setReuseAddr(bool onoff);
    int setNonBlocking(bool onoff);
    int bind(const Address &address);
    int listen(int backlog = DefaultQueueSize);
    EndPoint *accept(bool &retry);
    int connect(Address &address);
    //virtual int read(void *inbuffer, size_t count);
    int read(void *inbuffer, size_t count, Address &peer_addr);
    int write(const void *outbuffer, size_t count);
    int write(const void *outbuffer, size_t count, const Address &peer_addr);
    //virtual int readn(void *inbuffer, size_t count);
    int writen(const void *outbuffer, size_t count);
    int open();
    int close();

    // specific to notifying
    void watch(const std::string &path, unsigned long mask);
    void unwatch(const std::string &path, unsigned long mask);
    void unwatch(int id, unsigned long mask);
    void getPath(int id, std::string &path);
    void getMask(int id, unsigned long &mask);
    void getMask(const std::string &path, unsigned long &mask);

private:
    // not allowed
    Notify(const Notify &src);
    Notify &operator=(const Notify &rhs);

protected:
    typedef std::map<std::string, unsigned long> MonitoredFiles;
    typedef std::map<std::string, int> MonitoredFile2ID;
    typedef std::map<int, std::string> ID2MonitoredFile;
    MonitoredFiles files_;
    MonitoredFile2ID file2id_;
    ID2MonitoredFile id2file_;
};

// connection class - socket and address combined.
class NotifyEndPoint: public EndPoint {
public:
    NotifyEndPoint(Socket *sock = NULL);
    virtual ~NotifyEndPoint();

    // socket-base operations 
    int bind();
    int listen(int backlog = Socket::DefaultQueueSize);
    EndPoint *accept(bool &retry);
    int connect(Address &address);
    //virtual int read(void *inbuffer, size_t count);
    int read(void *inbuffer, size_t count, Address &peer_addr);
    int write(void *outbuffer, size_t count);
    int write(void *outbuffer, size_t count, const Address &peer_addr);
    //virtual int readn(void *inbuffer, size_t count);
    int writen(void *outbuffer, size_t count);
    //virtual int close();
    //virtual int open();

    // specific to notify endpoint
    void watch(const std::string &path, unsigned long mask);
    void unwatch(const std::string &path, unsigned long mask);
    void unwatch(int id, unsigned long mask);
    void getPath(int id, std::string &path);
    void getMask(int id, unsigned long &mask);
    void getMask(const std::string &path, unsigned long &mask);

private:
    // leper colony
    NotifyEndPoint(const NotifyEndPoint &src);
    NotifyEndPoint &operator=(const NotifyEndPoint &rhs);
};

}

#endif 

