#ifndef _MIMIC_ANDROID_PLOCK_H__
#define _MIMIC_ANDROID_PLOCK_H__

#include "pthread.h"

class CPLock
{
public:
    CPLock();
    ~CPLock();

public:

    int Lock();
    int UnLock();

private:
    pthread_mutex_t _mutex;
};

#endif  //_MIMIC_ANDROID_PLOCK_H__