//
// Created by Administrator on 2019/11/2.
//

#include <cstdio>
#include <cstring>
#include <jni.h>
#include "shell_utils.h"

#define BUFFER_SIZE 1024
int shell_with_return(const std::string &cmd, std::string &result)
{
    FILE* fp=popen(cmd.c_str(), "r");
    if(!fp)
        return JNI_ERR;
    char temp[BUFFER_SIZE];
    while(fgets(temp, BUFFER_SIZE, fp)) {
        int len = strlen(temp);
        result.append(temp);
    }
    return pclose(fp);
}

