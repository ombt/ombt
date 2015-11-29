#ifndef __OMBT_THREAD_SAFE_PTR_H
#define __OMBT_THREAD_SAFE_PTR_H

// thread safe pointers

// headers
#include <Debug.h>
#include <ThreadMutex.h>
#include <logging/Logging.h>

namespace ombt {

// thread safe pointer
template <class DT> 
class ThreadSafePtr
{
public:
    // ctor and dtor
    ThreadSafePtr(DT *pd, bool delete_on_exit = false):
        pdata_(pd), delete_on_exit_(delete_on_exit), mutex_() {
        MustBeTrue(pd != NULL);
        DBGTRACE();
    }
    ~ThreadSafePtr() {
        DBGTRACE();
        if (delete_on_exit_ && pdata_ != NULL) 
        {
            delete pdata_;
        }
        pdata_ = NULL;
        delete_on_exit_ = false;
    }

    // explicit clean up
    void remove() {
        if (pdata_ != NULL) delete pdata_;
        delete_on_exit_ = false;
        pdata_ = NULL;
    }

    // lock/unlock class
    template <class DT2>
    class Lock
    {
    public:
        // ctor and dtor
        Lock(DT2 *pd, ThreadMutex &mutex):
            pdata_(pd), mutex_(mutex) {
DBGPRINTF("%s", "LOCKING MUTEX !!!");
            mutex_.lock();
        }
        ~Lock() {
DBGPRINTF("%s", "UN-LOCKING MUTEX !!!");
            mutex_.unlock();
        }

        // return locked pointer
        DT2 *operator->() {
DBGPRINTF("%s", "RETURNING LOCKED POINTER!!!");
            return(pdata_);
        }

    private:
        // data
        DT *pdata_;
        ThreadMutex &mutex_;
    };

    // returned lock pointer
    Lock<DT> operator->() const {
        return(Lock<DT>(pdata_, mutex_));
    }
    Lock<DT> operator->() {
        return(Lock<DT>(pdata_, mutex_));
    }

protected:
    // internal data
    DT *pdata_;
    bool delete_on_exit_;
    ThreadMutex mutex_;
};

}

#endif
