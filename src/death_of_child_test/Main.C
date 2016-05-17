//**************************************************************************
//      COPYRIGHT 2012-2013 PANASONIC FACTORY SOLUTIONS COMPANY OF AMERICA
//            DIVISION OF PANASONIC CORPORATION OF NORTH AMERICA
//       5201 TOLLVIEW DRIVE, ROLLING MEADOWS, IL, 60008. ALL RIGHTS RESERVED
//*****************************************************************************
//
// Name Of File  : Main.C
//
// Version       : 8.5.0.3
//
// Last Modified : 07/25/2013
//
// Type Of File  : C++ Source File
//
// Author        : Mike Rumore
//
//*****************************************************************************

//
// OS includes
//
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>

//
// hdr includes
//
#include "PanaCim.H"
#include "SysLogger.H"
#include "Mailbox.H"

//
// db classes includes
//
#include "DataBase.H"

#if 0
#include "MESStrackMsg.H"
#endif

SYSLOG_INIT;

class ChildProcess
{
public:
    // ctors and dtor
    ChildProcess(
        const int argc = 0,
        const char **argv = NULL):
            _pid(-1),
            _argc(0),
            _argv(NULL)
    {
        if ((argc > 0) && (argv != NULL))
        {
            _argc = argc;
            _argv = new char * [argc];

            for (int arg=0; arg<argc; ++arg)
            {
                _argv[arg] = NULL;
                int arglen = ::strlen(argv[arg]);
                if (arglen > 0)
                {
                    _argv[arg] = new char [arglen+1];
                    strcpy(_argv[arg], argv[arg]);
                }
            }
        }
    };

    ChildProcess(
        const ChildProcess &src):
            _pid(-1),
            _argc(0),
            _argv(NULL)
    {
        if ((src._argc > 0) && (src._argv != NULL))
        {
            _argc = src._argc;
            _argv = new char * [src._argc];

            for (int arg=0; arg<src._argc; ++arg)
            {
                _argv[arg] = NULL;
                int arglen = ::strlen(src._argv[arg]);
                if (arglen > 0)
                {
                    _argv[arg] = new char [arglen+1];
                    strcpy(_argv[arg], src._argv[arg]);
                }
            }
        }
    };

    virtual ~ChildProcess()
    {
        for (int arg=0; arg<_argc; ++arg)
        {
            if (_argv[arg] != NULL)
            {
                delete [] _argv[arg];
            }
        }

        if (_argv != NULL)
        {
            delete [] _argv;
        }
    };

    // assignment
    ChildProcess &operator=(const ChildProcess &rhs)
    {
        if (this != &rhs)
        {
            _pid = -1;

            for (int arg=0; arg<_argc; ++arg)
            {
                if (_argv[arg] != NULL)
                {
                    delete [] _argv[arg];
                }
            }
            _argc = 0;

            if (_argv != NULL)
            {
                delete [] _argv;
            }
            _argv = NULL;

            if ((rhs._argc > 0) && (rhs._argv != NULL))
            {
                _argc = rhs._argc;
                _argv = new char * [rhs._argc];

                for (int arg=0; arg<rhs._argc; ++arg)
                {
                    _argv[arg] = NULL;
                    int arglen = ::strlen(rhs._argv[arg]);
                    if (arglen > 0)
                    {
                        _argv[arg] = new char [arglen+1];
                        strcpy(_argv[arg], rhs._argv[arg]);
                    }
                }
            }
        }

        return *this;
    };

    // main entry point
    virtual int main()
    {
#if 0
        sleep(10);
        cout << endl << "writing 'hello world' ..." << endl;
        ::write(_pipefd[1], "hello world\n", 13);
        sleep(10);
        ::close(_pipefd[0]);
        ::close(_pipefd[1]);
#else
        sleep(20);
#endif
        cout << endl << "child is exiting ..." << endl;
        return 0;
    };

    virtual int fork()
    {
        // fork a new process
        if (::pipe(_pipefd) != 0)
        {
            _pid = -1;
            return -1;
        }
    
        switch (_pid = ::fork())
        {
        case 0:
            // child proc
            _pid = getpid();
            ::exit(main());

        case -1:
            // parent proc, but with some type of error.
            cout << "fork failed!" << endl;
            return -1;

        default:
            // parent process and with child pid.
            cout << "fork succeeded!" << endl;
            return _pid;
        }
    }

    // ping
    virtual bool ping() const
    {
        if ((_pid > 0) && (::kill(_pid, 0) == 0))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // file descriptor
    virtual int read_fd()
    {
        return _pipefd[0];
    }
    virtual int write_fd()
    {
        return _pipefd[1];
    }
    virtual void close_read_fd()
    {
        ::close(_pipefd[0]);
        _pipefd[0] = -1;
    }
    virtual void close_write_fd()
    {
        ::close(_pipefd[1]);
        _pipefd[1] = -1;
    }
    virtual int max_fd() const
    {
        if ((_pipefd[0] >= 0) && (_pipefd[1] >= 0))
        {
            if (_pipefd[0] > _pipefd[1])
                return (_pipefd[0] + 1);
            else
                return (_pipefd[1] + 1);
        }
        else if (_pipefd[0] >= 0)
            return (_pipefd[0] + 1);
        else if (_pipefd[1] >= 0)
            return (_pipefd[1] + 1);
        else
            return -1;
    }

    // no zombies
    static bool set_no_zombies()
    {
        return (::signal( SIGCHLD, SIG_IGN ) != SIG_ERR);
    }

protected:
    pid_t  _pid;
    int    _pipefd[2];
    int    _argc;
    char **_argv;
};

//-----------------------------------------------------------------------------
// Procedure:   main
//
// Description: main entry to PPD proxy test applicaiton.
//
// Parameters:  argc - argument count
//              argv - argument vector. see usage for detais.
//
// Returns:     0 - success, anything else - failure.
//-----------------------------------------------------------------------------
int
main(int argc, char **argv)
{
    PanaCIM_DB.app_name("test_db_prod");
    PanaCIM_DB.open();
    
    PMailbox mbx;
    assert(mbx.open(AT_SYSTEM_CHECKER_SLAVE,
                    getpid(),
                    ST_SYSTEM_CHECKER_SLAVE,
                    FALSE) != FALSE);

    ChildProcess::set_no_zombies();

    ChildProcess test;

    if (test.fork() > 0)
    {
        cout << "fork is OK" << endl;

        for (bool done=false; (!done) && test.ping(); )
        {
            cout << "ping is OK ..." << endl;

            done = true;

            fd_set *prfds, rfds;
            fd_set *pwfds, wfds;
            fd_set *pefds, efds;

            FD_ZERO(&rfds);
            FD_ZERO(&wfds);
            FD_ZERO(&efds);

            prfds = NULL;
            pwfds = NULL;
            pefds = &efds;

            if (test.read_fd() >= 0)
            {
                FD_SET(test.read_fd(), &rfds);
                FD_SET(test.read_fd(), &efds);
                cout << "select on read fd ... " << test.read_fd() << endl;
                prfds = &rfds;
                done = false;
            }
            if (test.write_fd() >= 0)
            {
                FD_SET(test.write_fd(), &wfds);
                FD_SET(test.write_fd(), &efds);
                cout << "select on write fd ... " << test.write_fd() << endl;
                pwfds = &wfds;
                done = false;
            }

            if ((done) || (test.max_fd() < 0))
            {
                cout << "no fd to read or write ... done." << endl;
                continue;
            }
            cout << "max fd + 1 is ..." << test.max_fd() << endl;

            timeval tv;
            tv.tv_sec = 15;
            tv.tv_usec = 0;

            int retval = ::select(test.max_fd(), prfds, pwfds, pefds, &tv);
            if (retval < 0)
            {
                cout << "some type of error ..." << endl;
                done = false;
            }
            else if (retval > 0)
            {
                if ((pefds != NULL) && FD_ISSET(test.read_fd(), pefds))
                {
                    cout << "read error !!!" << endl;
                    test.close_read_fd();
                }
                else if ((prfds != NULL) && FD_ISSET(test.read_fd(), prfds))
                {
                    char buf[BUFSIZ+1];
                    buf[BUFSIZ] = '\0';
                    int nr = ::read(test.read_fd(), buf, BUFSIZ);
                    if (nr == 0)
                    {
                        // let ping find the dead process
                        cout << "EOF detected ..." << endl;
                        test.close_read_fd();
                    }
                    else if (nr > 0)
                    {
                        cout << "something was read ... " << nr << endl;
                    }
                    else
                    {
                        cout << "some type of error ..." << endl;
                        done = false;
                    }
                }

                if ((pefds != NULL) && FD_ISSET(test.write_fd(), pefds))
                {
                    cout << "write error !!!" << endl;
                    test.close_write_fd();
                }
                else if ((pwfds != NULL) && FD_ISSET(test.write_fd(), pwfds))
                {
                    char buf[BUFSIZ+1];
                    int nw = ::write(test.write_fd(), buf, BUFSIZ);
                    if (nw == 0)
                    {
                        // let ping find the dead process
                        cout << "EOF detected ..." << endl;
                        test.close_write_fd();
                    }
                    else if (nw > 0)
                    {
                        cout << "something was written ... " << nw << endl;
                    }
                    else
                    {
                        cout << "some type of error ..." << endl;
                        done = false;
                    }
                }
 
            }
            else
            {
                cout << "select time out ..." << endl;
            }
        }

        cout << "ping is NOT OK ..." << endl;
    }
    else
    {
        cout << "fork is NOT OK" << endl;
    }

    return 0;
}
