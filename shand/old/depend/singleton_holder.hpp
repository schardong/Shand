// Copyright Akira Takahashi 2006
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef SINGLETON_HOLDER_INCLUDE
#define SINGLETON_HOLDER_INCLUDE

#include <atlbase.h>

template<class Type, class ThreadModel = CComSingleThreadModel>
class SingletonHolder {
private:
    static Type* p_;
    static typename ThreadModel::AutoCriticalSection cs_;
 
protected:
    SingletonHolder() { }
 
public:
    static Type& Instance()
    {
        if (p_ == NULL)
        {
            cs_.Lock();
            if (p_ == NULL)
            {
                try
                {
                    p_ = new Type();
                }
                catch (...)
                {
                    cs_.Unlock();
                    throw;
                }
            }
            cs_.Unlock();
        }
        return *p_;
    }
};
 
template<class Type, class ThreadModel>
    Type* SingletonHolder<Type, ThreadModel>::p_ = NULL;
template<class Type, class ThreadModel>
    typename ThreadModel::AutoCriticalSection SingletonHolder<Type, ThreadModel>::cs_;

#endif // SINGLETON_HOLDER_INCLUDE

