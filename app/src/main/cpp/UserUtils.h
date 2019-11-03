//
// Created by Administrator on 2019/11/2.
//

#ifndef WORLD_USERUTILS_H
#define WORLD_USERUTILS_H


#include <jni.h>
#include <string>

class UserUtils {
public:
   static bool GetCacheDir(JNIEnv *env,std::string &path);
};


#endif //WORLD_USERUTILS_H
