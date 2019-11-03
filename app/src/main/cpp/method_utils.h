//
// Created by Administrator on 2019/11/2.
//

#ifndef WORLD_METHOD_UTILS_H
#define WORLD_METHOD_UTILS_H

#include <jni.h>
#include <string>

jmethodID get_method_by_name(JNIEnv *env,jclass def_class,const std::string &name);
#endif //WORLD_METHOD_UTILS_H
