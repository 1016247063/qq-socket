#ifndef _TASK_H_
#define _TASK_H_
#include <stdio.h>
#include <unistd.h>

class Task
{
    
public:
    Task();
    ~Task();
    virtual void run() = 0;

};


#endif //_TASK_H_