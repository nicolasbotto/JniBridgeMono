#include <jni.h>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;

class JniManager
{
private:
    JavaVM* jvm;
    jclass exceptionClazz;

public:
    JniManager(JavaVM*);
    void init();
    JNIEnv* getEnv();
    void cleanup();
    void checkJniException();
    void throwException(const char*);
    void log(char*);
    void instrument(char*, int);
    string toRequestJSON(jobject);
    jobject toResponse(string);
};

