#include "JniManager.h"
#include <iostream>

JniManager::JniManager(JavaVM* vm)
{
    assert(vm);
    jvm = vm;
    init();
}

void JniManager::init()
{
    JNIEnv* env = getEnv();

    assert(env);

    // Exception class
    jclass exceptionClazzTemp = env->FindClass("java/lang/Exception");

    if (exceptionClazzTemp == NULL)
    {
            checkJniException();
    }

    exceptionClazz = (jclass)env->NewGlobalRef(exceptionClazzTemp);

    env->DeleteLocalRef(exceptionClazzTemp);
    
    // org/mule/api/jniRouter
    jclass routerClazzTmp = env->FindClass("org/mule/api/jni/Bridge");

    checkJniException();

    logMethod = env->GetMethodID(routerClazzTmp, "log", "(Ljava/lang/String;)V");

    checkJniException();

    instrumentMethod = env->GetMethodID(routerClazzTmp, "instrument", "(Ljava/lang/String;I)V");

    checkJniException();

    env->DeleteLocalRef(routerClazzTmp);
    
    // org/mule/api/jni/Response
    jclass responseClazzTmp = env->FindClass("org/mule/api/jni/Response");

    checkJniException();

    responseClazz = (jclass)env->NewGlobalRef(responseClazzTmp);
    env->DeleteLocalRef(responseClazzTmp);

    responseCtor = env->GetMethodID(responseClazz, "<init>", "()V");

    checkJniException();

    setPayloadMethod = env->GetMethodID(responseClazz, "setPayload", "(Ljava/lang/Object;)V");

    checkJniException();

    setInvocationProperties = env->GetMethodID(responseClazz, "setInvocationProperties", "(Ljava/util/Map;)V");

    checkJniException();

    setSessionProperties = env->GetMethodID(responseClazz, "setSessionProperties", "(Ljava/util/Map;)V");

    checkJniException();

    setOutboundProperties = env->GetMethodID(responseClazz, "setOutboundProperties", "(Ljava/util/Map;)V");

    checkJniException();

    // org/mule/api/jni/Request

    jclass processRequestClazz = env->FindClass("org/mule/api/jni/Request");

    checkJniException();

    getAssemblyName = env->GetMethodID(processRequestClazz, "getAssemblyName", "()Ljava/lang/String;");

    checkJniException();

    getAssemblyPath = env->GetMethodID(processRequestClazz, "getAssemblyPath", "()Ljava/lang/String;");

    checkJniException();

    getMethodName = env->GetMethodID(processRequestClazz, "getMethodName", "()Ljava/lang/String;");

    checkJniException();

    getLog = env->GetMethodID(processRequestClazz, "getLog", "()Z");

    checkJniException();

    getNotifyEvents = env->GetMethodID(processRequestClazz, "getNotifyEvents", "()Z");

    checkJniException();

    getFullTrust = env->GetMethodID(processRequestClazz, "getFullTrust", "()Z");

    checkJniException();

    getIsSingleton = env->GetMethodID(processRequestClazz, "getIsSingleton", "()Z");

    checkJniException();

    getInboundProperties = env->GetMethodID(processRequestClazz, "getInboundProperties", "()Ljava/util/Map;");

    checkJniException();

    getMethodArguments = env->GetMethodID(processRequestClazz, "getMethodArguments", "()Ljava/util/Map;");

    checkJniException();

    getInvocationProperties = env->GetMethodID(processRequestClazz, "getInvocationProperties", "()Ljava/util/Map;");

    checkJniException();

    getSessionProperties = env->GetMethodID(processRequestClazz, "getSessionProperties", "()Ljava/util/Map;");

    checkJniException();

    getOutboundProperties = env->GetMethodID(processRequestClazz, "getOutboundProperties", "()Ljava/util/Map;");

    checkJniException();
    
    typeConverter = new TypeConverter();
    typeConverter->init(getEnv());
}

void JniManager::setRouter(jobject obj)
{
    JNIEnv* env = getEnv();

    assert(env);

    routerInstance = env->NewGlobalRef(obj);
}

JNIEnv* JniManager::getEnv()
{
    JNIEnv *env;
    assert(jvm);
    int envStatus = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (envStatus == JNI_OK)
    {
        jvm->AttachCurrentThread((void**)&env, NULL);
        return env;
    }

    return NULL;
}

void JniManager::cleanup()
{
    JNIEnv* env = getEnv();

    assert(env);


    env->DeleteGlobalRef(responseClazz);
    env->DeleteGlobalRef(routerInstance);

    //typeConverter->cleanup(env);

    checkJniException();
}

void JniManager::checkJniException()
{
    JNIEnv* env = getEnv();
    assert(env);

    jthrowable error = env->ExceptionOccurred();

    if (error)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

void JniManager::throwException(const char* message)
{
    JNIEnv* env = getEnv();
    assert(env);
    env->ThrowNew(exceptionClazz, message);
}

void JniManager::log(char* message)
{
    JNIEnv* env = getEnv();

    assert(env);

    jstring j_message = env->NewStringUTF(message);

    env->CallVoidMethod(routerInstance, logMethod, j_message);

    env->DeleteLocalRef(j_message);
}

void JniManager::instrument(char* message, int id)
{
    JNIEnv* env = getEnv();

    assert(env);

    jstring jMessage = env->NewStringUTF(message);
    jint jId = (int)id;

    env->CallVoidMethod(routerInstance, instrumentMethod, jMessage, jId);

    env->DeleteLocalRef(jMessage);
}

jobject JniManager::toResponse(string result)
{
    rapidjson::Document d;
    d.Parse<0>(result.c_str());
    
    if(d.HasParseError())
    {
        string error = d.GetParseError();
        throwException(error.c_str());
        return NULL;
    }
    
    JNIEnv* env = getEnv();

    assert(env);
    
    if(d.HasMember("exception"))
    {
        throwException(d["exception"].GetString());
        return NULL;
    }

    jobject response = env->NewObject(responseClazz, responseCtor);
    jobject jPayload = NULL;

    if(d.HasMember("payload"))
    {
        jPayload = typeConverter->toJavaObject(env, d["payload"]);
    }
    
    env->CallVoidMethod(response, setPayloadMethod, jPayload);
    env->DeleteLocalRef(jPayload);
    
    // set Mule Message properties
    jobject jInvocationProperties = typeConverter->convertToJavaMap(env, d["invocationProperties"]);
    jobject jSessionProperties = typeConverter->convertToJavaMap(env, d["sessionProperties"]);
    jobject jOutboundProperties = typeConverter->convertToJavaMap(env, d["outboundProperties"]);

    env->CallVoidMethod(response, setInvocationProperties, jInvocationProperties);
    env->CallVoidMethod(response, setOutboundProperties, jOutboundProperties);
    env->CallVoidMethod(response, setSessionProperties, jSessionProperties);

    env->DeleteLocalRef(jInvocationProperties);
    env->DeleteLocalRef(jOutboundProperties);
    env->DeleteLocalRef(jSessionProperties);

    return response;
}

string JniManager::toRequestJSON(jobject obj)
{
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    
    JNIEnv* env = getEnv();

    assert(env);
    
    string c_assemblyName = typeConverter->convertToC<string>(env, env->CallObjectMethod(obj, getAssemblyName, "()Ljava/lang/String;"));
    string c_assemblyPath = typeConverter->convertToC<string>(env, env->CallObjectMethod(obj, getAssemblyPath, "()Ljava/lang/String;"));
    string c_methodName = typeConverter->convertToC<string>(env, env->CallObjectMethod(obj, getMethodName, "()Ljava/lang/String;"));

    bool fullTrust = env->CallBooleanMethod(obj, getFullTrust);
    bool isSingleton = env->CallBooleanMethod(obj, getIsSingleton);
    bool log = env->CallBooleanMethod(obj, getLog);
    bool notifyEvents = env->CallBooleanMethod(obj, getNotifyEvents);
    
    jobject javaMethodArguments = env->CallObjectMethod(obj, getMethodArguments);
    jobject javaInboundProperties = env->CallObjectMethod(obj, getInboundProperties);    
    jobject javaInvocationProperties = env->CallObjectMethod(obj, getInvocationProperties);
    jobject javaOutboundProperties = env->CallObjectMethod(obj, getOutboundProperties);
    jobject javaSessionProperties = env->CallObjectMethod(obj, getSessionProperties);
    
    writer.StartObject();
    
    writer.String("assemblyName");
    writer.String(c_assemblyName.c_str());
    writer.String("assemblyPath");
    writer.String(c_assemblyPath.c_str());
    writer.String("methodName");
    writer.String(c_methodName.c_str());
    writer.String("log");
    writer.Bool(log);
    writer.String("instrument");
    writer.Bool(notifyEvents);
    writer.String("isSingleton");
    writer.Bool(isSingleton);

    if(javaMethodArguments != NULL)
    {
        writer.String("methodArguments");
        typeConverter->toObjectJson(env, javaMethodArguments, writer);
    }
    
    if(javaInvocationProperties != NULL)
    {
        writer.String("invocationProperties");
        typeConverter->toObjectJson(env, javaInvocationProperties, writer);
    }
    
    if(javaOutboundProperties != NULL)
    {
        writer.String("outboundProperties");
        typeConverter->toObjectJson(env, javaOutboundProperties, writer);
    }

    if(javaSessionProperties != NULL)
    {
        writer.String("sessionProperties");
        typeConverter->toObjectJson(env, javaSessionProperties, writer);
    }
    
    writer.EndObject();

    return sb.GetString();
}