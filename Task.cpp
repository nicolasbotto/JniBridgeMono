#include "Task.h"

char* Task::run() {
    return (instance->*cb_func)();
}

void Task::setServerInstance(Router* serverInstance)
{
    instance = serverInstance;
}

void Task::setCallBack(callback_func_ptr callback)
{
    cb_func = callback;
}
