//
// Created by Administrator on 2019/11/2.
//

#include "jni_utils.h"

JavaVM *get_java_vm(JNIEnv *env) {
    JavaVM *vm= nullptr;
    env->GetJavaVM(&vm);
    return vm;
}

bool try_catch(JNIEnv *env, bool describe) {
    if(env->ExceptionCheck()){
        if(describe)
            env->ExceptionDescribe();
        env->ExceptionClear();
        return true;
    }
    return false;
}
