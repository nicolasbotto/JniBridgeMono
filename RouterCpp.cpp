#include "org_mule_api_jni_Bridge.h"
#include "JniManager.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#define BUFFER_SIZE 4096
#define SOCKET_NAME "/tmp/dotnetsocket"

using namespace std;

static JniManager* jniManager;
static jobject javaRouter;
struct sockaddr_un remote;
int serverSocket;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    jniManager = new JniManager(vm);
    JNIEnv* env = jniManager->getEnv();

    if (!env)
    {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

// The VM calls JNI_OnUnload when the class loader containing the native library is garbage collected.
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
    //remove global references
//	jniManager->cleanup();
    
    // stop server
    /* Close socket. */
    close(serverSocket);
}

JNIEXPORT void JNICALL Java_org_mule_api_jni_Bridge_init
(JNIEnv *env, jobject obj, jobject path)
{
    jniManager->setRouter(obj);
    
    // Start server
}

JNIEXPORT jobject JNICALL Java_org_mule_api_jni_Bridge_invokeNetMethod
(JNIEnv *env, jobject obj, jobject request)
{
    pthread_mutex_lock(&lock);
    string requestJSON = jniManager->toRequestJSON(request);
    
    if ((serverSocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        jniManager->throwException("Cannot create socket.");
    }

    memset(&remote, 0, sizeof(struct sockaddr_un));

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCKET_NAME);
    int len = strlen(remote.sun_path) + sizeof(remote.sun_family);

    if (connect(serverSocket, (struct sockaddr *)&remote, len) == -1) {
        string errorMessage = "The server is down: ";
        errorMessage.append(strerror(errno));
        jniManager->throwException(errorMessage.c_str());
        return NULL;
    }
    
    /* Send arguments. */
    
    if (write(serverSocket, requestJSON.c_str(), strlen(requestJSON.c_str())) == -1) {
        string errorMessage = "Error sending message: ";
        errorMessage.append(strerror(errno));
        jniManager->throwException(errorMessage.c_str());
        return NULL;
    }

    /* Receive result. */
    char response[7000] = {};
    if (read(serverSocket, &response, 7000) == -1) {
        string errorMessage = "Error reading client socket: ";
        errorMessage.append(strerror(errno));
        jniManager->throwException(errorMessage.c_str());
        return NULL;
    }
    
    close(serverSocket);
    
    pthread_mutex_unlock(&lock);
    return jniManager->toResponse(response);
}

