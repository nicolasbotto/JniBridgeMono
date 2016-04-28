#include <jni.h>
#include <map>
#include "TypeConverter.h"

class JniManager
{
private:
    TypeConverter* typeConverter;
    JavaVM* jvm;
    jmethodID getAssemblyName;
    jmethodID getAssemblyPath;
    jmethodID getMethodName;
    jmethodID getLog;
    jmethodID getNotifyEvents;
    jmethodID getFullTrust;
    jmethodID getIsSingleton;
    jmethodID getMethodArguments;
    jmethodID getInvocationProperties;
    jmethodID getSessionProperties;
    jmethodID getOutboundProperties;
    jmethodID getInboundProperties;
    jmethodID responseCtor;
    jmethodID setPayloadMethod;
    jmethodID logMethod;
    jmethodID instrumentMethod;
    jmethodID setInvocationProperties;
    jmethodID setSessionProperties;
    jmethodID setOutboundProperties;
    jclass responseClazz;
    jobject routerInstance;
    jclass exceptionClazz;

public:
    JniManager(JavaVM*);
    void init();
    JNIEnv* getEnv();
    jobject toResponseObject(jobject);
    void cleanup();
    void setRouter(jobject);
    void checkJniException();
    void throwException(const char*);
    void log(char*);
    void instrument(char*, int);
    string toRequestJSON(jobject);
    jobject toResponse(string);
};

