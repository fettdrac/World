//
// Created by Administrator on 2019/11/2.
//

#include "Hook.h"
#include "log_utils.h"
#include "AndHook.h"
#include "UserUtils.h"
#include "jni_utils.h"

void Hook::NativeHook(const std::string& name,void* symbol,void* override_func) {
    if(GetOldFunction(name)){
        AKLogErr("hook can be applied to %s for only once\n",name.c_str());
        return;
    }
    void *old_func = nullptr;
    AKHookFunction(symbol, override_func, &old_func);
    AKLog("%s old_func:%p",name.c_str(),old_func);
    native_func_map_.insert(std::make_pair(name,old_func));
}

void *Hook::GetOldFunction(const std::string &name) {
    void* old_function= nullptr;
    for (auto itor = native_func_map_.begin();  itor!=native_func_map_.end(); itor++) {
        if(name!=itor->first)
            continue;
        old_function=itor->second;
        break;
    }
    AKLog("%s old_func:%p",name.c_str(),old_function);
    return old_function;
}

Hook::Hook() {
    AKLog("pre-init\n");
    java_hookable_=false;
}

bool Hook::InitJavaHook(JavaVM *vm, JNIEnv *env) {
    std::string cache_path_str;
    if(UserUtils::GetCacheDir(env,cache_path_str)) {
        const char* cache_path=cache_path_str.c_str();
        AKLog("cache_path:%s\n",cache_path);
        AKSetCacheDirectory(cache_path);
    }
    jint java_hook_init_ret= AKInitializeOnce(env,vm);
    AKLog("java_hook_init_ret:%d\n",java_hook_init_ret);
    java_hookable_= java_hook_init_ret>=JNI_OK;
    return java_hookable_;
}

void Hook::JavaHook(JNIEnv *env,const std::string &name, jmethodID target,void* override_func) {
    if(!java_hookable_){
        JavaVM *vm=get_java_vm(env);
        if(!InitJavaHook(vm,env)) {
            AKLogErr("java hook has not been prepared\n");
            return;
        }
    }
    if(GetOldMethod(name)){
        AKLogErr("hook can be applied to %s for only once\n",name.c_str());
        return;
    }
    jmethodID old_method= nullptr;
    AKJavaHookMethodV(env,target,override_func,&old_method);
    AKLog("%s old_method:%p",name.c_str(),old_method);
    java_func_map_.insert(std::make_pair(name,old_method));
}

jmethodID Hook::GetOldMethod(const std::string &name) {
    jmethodID old_method= nullptr;
    for (auto itor = java_func_map_.begin();  itor!=java_func_map_.end(); itor++) {
        if(name!=itor->first)
            continue;
        old_method=itor->second;
        break;
    }
    AKLog("%s old_method:%p",name.c_str(),old_method);
    return old_method;
}
