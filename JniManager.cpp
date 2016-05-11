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

    checkJniException();
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
    env->DeleteGlobalRef(exceptionClazz);

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
    /*JNIEnv* env = getEnv();

    assert(env);

    jstring j_message = env->NewStringUTF(message);

    env->CallVoidMethod(routerInstance, logMethod, j_message);

    env->DeleteLocalRef(j_message);*/
}

void JniManager::instrument(char* message, int id)
{
   /* JNIEnv* env = getEnv();

    assert(env);

    jstring jMessage = env->NewStringUTF(message);
    jint jId = (int)id;

    env->CallVoidMethod(routerInstance, instrumentMethod, jMessage, jId);

    env->DeleteLocalRef(jMessage);*/
}

jobject JniManager::toResponse(string result)
{
    JNIEnv* env = getEnv();

    assert(env);

    return env->NewStringUTF(result.c_str());
}

string JniManager::toRequestJSON(jobject obj)
{
    JNIEnv* env = getEnv();

    assert(env);
    
    jbyteArray input = (jbyteArray)obj;
    const int intsSize = env->GetArrayLength(input);
    vector<char> data(intsSize);

    jbyte *body = env->GetByteArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
        data[i] = (char)body[i];
    }

    env->ReleaseByteArrayElements(input, body, 0);
    env->DeleteLocalRef(input);
    
    return string(&data[0], intsSize);
}