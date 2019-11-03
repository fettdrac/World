//
// Created by Administrator on 2019/11/2.
//

#ifndef WORLD_JNI_UTILS_H
#define WORLD_JNI_UTILS_H

#include <jni.h>

JavaVM* get_java_vm(JNIEnv *env);

bool try_catch(JNIEnv *env,bool describe);
#endif //WORLD_JNI_UTILS_H
