#pragma once

#include <pthread.h>
#include <unistd.h>
#include <deque>
#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>
#include "Router.h"
#include "Global.h"

using namespace std;

class Task
{
public:
    typedef char* (Router::*callback_func_ptr)();
    char* run();
    void setCallBack(callback_func_ptr);
    void setServerInstance(Router*);
private:
  callback_func_ptr cb_func;
  Router* instance;
};
