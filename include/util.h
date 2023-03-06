//
// Created by seu-wxy on 2023/3/6.
//

#ifndef OPENGL_FPSDEMO_UTIL_H
#define OPENGL_FPSDEMO_UTIL_H

#endif //OPENGL_FPSDEMO_UTIL_H

#include "string"
#include "vector"

namespace util
{
    // 常规split函数
    inline void spilt(const std::string& input_str, std::vector<std::string>& out_vec, const std::string& token){
        out_vec.clear();
        int start = 0;
        int cur_find = 0;
        while(cur_find != std::string::npos){
            cur_find = (int) input_str.find(token, start);
            out_vec.push_back(input_str.substr(start, cur_find - start));
            start = cur_find + (int)token.size();
        }
    }

    // 获得一串字符的最尾部元素(以空格等作为分隔符)
    inline std::string getTailElement(const std::string& input_str){
        size_t token_start = input_str.find_first_not_of(" \t");
        size_t space_start = input_str.find_first_of(" \t", token_start);
        size_t tail_start = input_str.find_first_not_of(" \t", space_start);
        size_t tail_end = input_str.find_last_not_of(" \t");
        if (tail_start != std::string::npos && tail_end != std::string::npos){
            return input_str.substr(tail_start, tail_end - tail_start + 1);
        }
        else if (tail_start != std::string::npos){
            return input_str.substr(tail_start);
        }
        return "";
    }

    // 获得一串字符的最头部元素(以空格等作为分隔符)
    inline std::string getHeadElement(const std::string& input_str){
        if (!input_str.empty()){
            size_t token_start = input_str.find_first_not_of(" \t");
            size_t token_end = input_str.find_first_of(" \t", token_start);
            if (token_start != std::string::npos && token_end != std::string::npos){
                return input_str.substr(token_start, token_end - token_start);
            }
            else if (token_start != std::string::npos){
                return input_str.substr(token_start);
            }
        }
        return "";
    }

    // 将路径内的反(双)斜杠替换为斜杠
    inline std::string alterBackslash(const std::string& input_str){
        std::string tmp = input_str;
        for(auto &s: tmp)
            if(s == '\\')
                s = '/';
        if(tmp.find("//") != std::string::npos){
            vector<std::string> tmp_vec;
            spilt(tmp, tmp_vec, "//");
            tmp.clear();
            tmp = tmp_vec.front();
            for(int i = 1;i<tmp_vec.size();++i)
                tmp += ("/" + tmp_vec[i]);
        }
        return tmp;
    }

}