#include "plock.h"

CPLock::CPLock()
{
    pthread_mutex_init(&_mutex, NULL);
}

CPLock::~CPLock()
{
    pthread_mutex_destroy(&_mutex);
}

int CPLock::Lock()
{
    return pthread_mutex_lock(&_mutex);
}

int CPLock::UnLock()
{
    return pthread_mutex_unlock(&_mutex);
}