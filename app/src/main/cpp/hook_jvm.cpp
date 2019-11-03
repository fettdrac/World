//
// Created by Administrator on 2019/11/2.
//

#include <dlfcn.h>
#include <jni.h>
#include "hook_jvm.h"
#include "Hook.h"
#include "log_utils.h"
#include "AndHook.h"
#include "UserUtils.h"
#include "hook_app_oncreate.h"
#include "ServiceManager.h"
#include "process_utils.h"
#include "hook_fork_system_server.h"

#define FUNC_NAME "JNI_CreateJavaVM"

typedef jint (*func_create_vm)(JavaVM**, JNIEnv**, void*);
jint my_create_jvm(JavaVM** p_vm,JNIEnv** p_env,void* reserved){
    void* old_function= Hook::GetInstance()->GetOldFunction(FUNC_NAME);
    if(!old_function){
        AKLogErr("fail to get old_function\n");
        return JNI_OK;
    }
    jint ret= reinterpret_cast<func_create_vm>(old_function)(p_vm,p_env,reserved);
    AKLog("ret:%d\n",ret);
    //初始化AndHook
    JavaVM *vm=*p_vm;
    JNIEnv *env=*p_env;
    AKLog("vm:%p env=%p\n",vm,env);
    Hook::GetInstance()->InitJavaHook(vm,env);
    //进行SystemServer的hook
    hook_fork_system_server(env);
    //进行每一个APP的hook
    hook_call_app_oncreate(env);
    return ret;
}

void hook_create_jvm(){
    void* symbol= dlsym(RTLD_DEFAULT,FUNC_NAME);
    if(!symbol){
        AKLogErr("symbol is nullptr\n");
        return;
    }
    AKLog("symbol:%p\n",symbol);
    Hook *hook=Hook::GetInstance();
    hook->NativeHook(FUNC_NAME,symbol,reinterpret_cast<void*>(my_create_jvm));
    //DEMO
    void* old_function= Hook::GetInstance()->GetOldFunction(FUNC_NAME);
    AKLog("old_function from map:%p\n",old_function);
}


