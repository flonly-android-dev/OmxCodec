#ifndef _MIMIC_ANDROID_PSEM_H__
#define _MIMIC_ANDROID_PSEM_H__

#include "pthread.h"

class CPSem
{
public:
    CPSem();
    ~CPSem();

public:
    void Post();
    void Pend();
    void setDebug(bool x){
        _print_count = x;
    }

private:
    int _count;
    bool _print_count;
    pthread_cond_t _cond;
    pthread_mutex_t _mutex;
};

#endif  //_MIMIC_ANDROID_PSEM_H__