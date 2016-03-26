#ifndef _MIMIC_ANDROID_PTHREAD_H__
#define _MIMIC_ANDROID_PTHREAD_H__

#include "sync/psem.h"
#include "pthread.h"

typedef void* (*CPThreadWorkProc)(void* arg);

class CPThread
{
public:
    CPThread();
    ~CPThread();

public:
    int  Start(const char* thread_name = NULL);
    void Stop();
    void AddTask(CPThreadWorkProc pWorker, void* pJob);

private:
    void Process();
    static void* Run(void* param);

private:
    bool _run;
    CPThreadWorkProc _worker;
    void* _user;
    pthread_t _thread;
    CPSem _sem;
    char* _name;
};

#endif  //_MIMIC_ANDROID_PTHREAD_H__
