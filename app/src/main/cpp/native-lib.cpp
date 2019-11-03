#include <jni.h>
#include <string>
#include <unistd.h>
#include "hook_jvm.h"
#include "process_utils.h"
#include "Hook.h"
#include "hook_app_oncreate.h"
#include "ServiceManager.h"
#include "log_utils.h"
#include "hook_fork_system_server.h"
#include "context_utils.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_pvdnc_world_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = get_process_name();
    JavaVM *vm= nullptr;
    env->GetJavaVM(&vm);
    Hook::GetInstance()->InitJavaHook(vm,env);
    hook_call_app_oncreate(env);
    hook_fork_system_server(env);
    return env->NewStringUTF(hello.c_str());
}

class static_initializer {
public:
    static_initializer() {
        hook_create_jvm();
    }
};
static static_initializer s;

extern "C"
JNIEXPORT void JNICALL
Java_com_pvdnc_world_MainActivity_loadService(JNIEnv *env, jobject thiz, jstring opt_dir) {
    //const char* c_opt_dir=env->GetStringUTFChars(opt_dir, nullptr);
    std::string opt_dir_str;
     if(!get_dir_normal(env,"optDex",opt_dir_str)){
         AKLogErr("fail to get opt_dir in pid:%d\n",getpid());
         return;
     }
     bool result=ServiceManager::LoadService(env,opt_dir_str, false);
     AKLog("LoadService result:%d\n",result);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_pvdnc_world_MainActivity_getNegativeUnsigned(JNIEnv *env, jobject thiz) {
    unsigned int val=0;
    memset(&val,-1, sizeof(unsigned int));
    AKLog("val:%ud",val);
    return val;
}