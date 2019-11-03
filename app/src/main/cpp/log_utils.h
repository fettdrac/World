//
// Created by Administrator on 2019/11/2.
//

#ifndef WORLD_LOG_UTILS_H
#define WORLD_LOG_UTILS_H

#include <android/log.h>

#define AKLog(...) __android_log_print(ANDROID_LOG_INFO, __FUNCTION__, __VA_ARGS__)
#define AKLogErr(...) __android_log_print(ANDROID_LOG_ERROR, __FUNCTION__, __VA_ARGS__)
#endif //WORLD_LOG_UTILS_H
