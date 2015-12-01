//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// notify class

// headers
#include "hdr/Notify.h"
#include "logging/Logging.h"

namespace ombt {

// ctors and dtor
Notify::Notify(): 
    Socket(), files_()
{
    socket_ = inotify_init();
    setOk(socket_ != NOTOK);
}
Notify::~Notify()
{
    if (socket_ != NOTOK)
    {
        ::close(socket_);
        socket_ = NOTOK;
        setOk(false);
    }
}

// basic socket operations
int Notify::setReuseAddr(bool onoff)
{
    return NOTOK;
}
int Notify::setNonBlocking(bool onoff)
{
    if (isNotOk()) return NOTOK;
    int flag = ::fcntl(socket_, F_GETFL, 0);
    if (onoff)
        flag |= O_NONBLOCK;
    else
        flag &= ~O_NONBLOCK;
    return(::fcntl(socket_, F_SETFL, flag));
}
int Notify::bind(const Address &address)
{
    return NOTOK;
}
int Notify::listen(int backlog)
{
    return NOTOK;
}
EndPoint *Notify::accept(bool &retry)
{
    return NULL;
}
int Notify::connect(Address &address)
{
    return NOTOK;
}
int Notify::read(void *inbuffer, size_t count, Address &peer_addr)
{
    return NOTOK;
}
int Notify::write(const void *outbuffer, size_t count)
{
    return NOTOK;
}
int Notify::write(const void *outbuffer, size_t count, const Address &peer_addr)
{
    return NOTOK;
}
int Notify::writen(const void *outbuffer, size_t count)
{
    return NOTOK;
}
int Notify::open()
{
    socket_ = inotify_init();
    setOk(socket_ != NOTOK);
    return (socket_ != NOTOK) ? OK : NOTOK;
}
int Notify::close()
{
    if (socket_ != NOTOK)
    {
        ::close(socket_);
        setOk(false);
    }
    return OK;
}

// specific to notify
void Notify::watch(const std::string &path, unsigned long mask)
{
    files_[path] = mask;
    int fileid = ::inotify_add_watch(socket_, path.c_str(), mask);
    id2file_[fileid] = path;
    file2id_[path] = fileid;
}
void Notify::unwatch(const std::string &path, unsigned long mask)
{
    MonitoredFile2ID::const_iterator mfit = file2id_.find(path);
    if (mfit != file2id_.end())
    {
        inotify_rm_watch(socket_, mfit->second);
        files_.erase(path);
        id2file_.erase(mfit->second);
        file2id_.erase(path);
    }
}
void Notify::unwatch(int id, unsigned long mask)
{
    ID2MonitoredFile::const_iterator mfit = id2file_.find(id);
    if (mfit != id2file_.end())
    {
        inotify_rm_watch(socket_, id);
        files_.erase(mfit->second);
        file2id_.erase(mfit->second);
        id2file_.erase(id);
    }
}
void Notify::getPath(int id, std::string &path)
{
    path = "";
    ID2MonitoredFile::const_iterator mfit = id2file_.find(id);
    if (mfit != id2file_.end()) path = mfit->second;
}
void Notify::getMask(int id, unsigned long &mask)
{
    mask = 0;
    ID2MonitoredFile::const_iterator id2mfit = id2file_.find(id);
    if (id2mfit == id2file_.end()) return;
    MonitoredFiles::const_iterator mfit = files_.find(id2mfit->second);
    if (mfit == files_.end()) return;
    mask = mfit->second;
}
void Notify::getMask(const std::string &path, unsigned long &mask)
{
    mask = 0;
    MonitoredFiles::const_iterator mfit = files_.find(path);
    if (mfit != files_.end()) mask = mfit->second;
}

// connection class - socket and address combined.
NotifyEndPoint::NotifyEndPoint(Socket *sock):
    EndPoint(sock, NULL)
{
}
NotifyEndPoint::~NotifyEndPoint()
{
}

// socket-base operations
int NotifyEndPoint::bind()
{
    return NOTOK;
}
int NotifyEndPoint::listen(int backlog)
{
    return NOTOK;
}
EndPoint *NotifyEndPoint::accept(bool &retry)
{
    return NULL;
}
int NotifyEndPoint::connect(Address &address)
{
    return NOTOK;
}
int NotifyEndPoint::read(void *inbuffer, size_t count, Address &peer_addr)
{
    return NOTOK;
}
int NotifyEndPoint::write(void *outbuffer, size_t count)
{
    return NOTOK;
}
int NotifyEndPoint::write(void *outbuffer, size_t count, const Address &peer_addr)
{
    return NOTOK;
}
int NotifyEndPoint::writen(void *outbuffer, size_t count)
{
    return NOTOK;
}

// notify operations
void NotifyEndPoint::watch(const std::string &path, unsigned long mask)
{
    Notify *pnot = dynamic_cast<Notify *>(socket_.ptr());
    if (pnot) pnot->watch(path, mask);
}
void NotifyEndPoint::unwatch(const std::string &path, unsigned long mask)
{
    Notify *pnot = dynamic_cast<Notify *>(socket_.ptr());
    if (pnot) pnot->unwatch(path, mask);
}
void NotifyEndPoint::unwatch(int id, unsigned long mask)
{
    Notify *pnot = dynamic_cast<Notify *>(socket_.ptr());
    if (pnot) pnot->unwatch(id, mask);
}
void NotifyEndPoint::getPath(int id, std::string &path)
{
    Notify *pnot = dynamic_cast<Notify *>(socket_.ptr());
    if (pnot) pnot->getPath(id, path);
}
void NotifyEndPoint::getMask(int id, unsigned long &mask)
{
    Notify *pnot = dynamic_cast<Notify *>(socket_.ptr());
    if (pnot) pnot->getMask(id, mask);
}
void NotifyEndPoint::getMask(const std::string &path, unsigned long &mask)
{
    Notify *pnot = dynamic_cast<Notify *>(socket_.ptr());
    if (pnot) pnot->getMask(path, mask);
}

}

