//
// Created by Administrator on 2019/11/2.
//
#include <unistd.h>
#include "process_utils.h"
#include "file_utils.h"
#include "string_utils.h"
#include "log_utils.h"

std::string get_process_name(){
    std::string result;
    std::string cmdline_path=std::string("/proc/")
            .append(itoa(getpid()))
            .append("/cmdline");
    AKLog("cmdline path:%s\n",cmdline_path.c_str());
    read_text_file(cmdline_path,result);
    return result;
}
