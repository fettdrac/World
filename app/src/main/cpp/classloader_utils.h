//
// Created by Administrator on 2019/11/2.
//

#ifndef WORLD_CLASSLOADER_UTILS_H
#define WORLD_CLASSLOADER_UTILS_H

#include <jni.h>
#include <string>

jobject get_system_classloader(JNIEnv *env);

jclass load_by_specific(JNIEnv *env,jobject cl,const std::string &name);
#endif //WORLD_CLASSLOADER_UTILS_H
