//
// Created by Administrator on 2019/11/2.
//

#include "string_utils.h"
#include <sstream>

std::string itoa(int i) {
    std::string result;
    std::stringstream ss;
    ss<<i;
    ss>>result;
    return result;
}
