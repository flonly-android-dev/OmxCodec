#include "psem.h"
#include "utils/mlog.h"
CPSem::CPSem()
{
    _print_count = false;
    _count = 0;
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);
}

CPSem::~CPSem()
{
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_cond);
}

void CPSem::Post()
{
    pthread_mutex_lock(&_mutex);
    _count++;
    pthread_cond_signal(&_cond);
    pthread_mutex_unlock(&_mutex);
}

void CPSem::Pend()
{
    pthread_mutex_lock(&_mutex);
    while(_count == 0)
    {
        pthread_cond_wait(&_cond, &_mutex);
    }
    if(_print_count){
        LOGE("sem count : %d",_count);
    }

    _count--;
    pthread_mutex_unlock(&_mutex);
}
