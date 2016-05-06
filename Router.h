#pragma once
#include "org_mule_api_jni_Bridge.h"
#include "JniManager.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>

class Router
{
public:
    Router(){}
    char* HandleRequest();
    
private:
};

