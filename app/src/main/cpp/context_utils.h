//
// Created by Administrator on 2019/11/2.
//

#ifndef WORLD_CONTEXT_UTILS_H
#define WORLD_CONTEXT_UTILS_H

#include <jni.h>
#include <string>

jobject get_app_context(JNIEnv *env);

bool get_dir_normal(JNIEnv *env,const std::string &name_str,std::string &dir_path);
#endif //WORLD_CONTEXT_UTILS_H
