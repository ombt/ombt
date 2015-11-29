//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// simple test for notify class

// system headers
#include <string>

// local headers
#include <Debug.h>
#include <logging/Logging.h>
#include <atomic/UseCntPtr.h>
#include <files/Notify.h>

using namespace ombt;

static const size_t BufSize = 10*1024;
// main entry point
int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc < 2)
    {
        dbgprintf("usage: %s dir1 [dir2 ...]\n", argv[0]);
        return(2);
    }

    dbgprintf("Create a Notifier ...\n");
    UseCntPtr<Notify> pnot(new Notify);
    MustBeTrue(pnot != NULL);

    dbgprintf("Create a Notify EndPoint ...\n");
    UseCntPtr<EndPoint> pep(new NotifyEndPoint(pnot));
    MustBeTrue(pep != NULL);

    for (int arg=1; arg < argc; ++arg)
    {
        pnot->watch(argv[arg], IN_CLOSE_WRITE|IN_MODIFY|IN_CREATE|IN_DELETE);
    }

    unsigned char buffer[BufSize];
    for (int nread; (nread = pep->read((void*)buffer, BufSize)) > 0; )
    {
        inotify_event *pevent = (inotify_event *)buffer;
        for (int offset = 0; offset<nread; 
             offset += offset + sizeof(inotify_event) + pevent->len)
        {
            std::string path;
            pnot->getPath(pevent->wd, path);
            dbgprintf("Watch descriptor: %d\n", pevent->wd);
            dbgprintf("Event Path      : %s\n", path.c_str());
            dbgprintf("Event Mask      : %lu\n", pevent->mask);
            dbgprintf("Event Cookie    : %lu\n", pevent->cookie);
            dbgprintf("Event Length    : %lu\n", pevent->len);
            if (pevent->len > 0)
                dbgprintf("File Name       : %s\n", pevent->name);
        }
    }

    return(0);
}

