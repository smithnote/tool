// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-11-02

#ifndef TOOL_HZ2PY_H_
#define TOOL_HZ2PY_H_

#include <sstream>
#include <string>
#include <vector>


namespace tool {


class HZ2PY {
  public:
    HZ2PY() {}
    ~HZ2PY() {}

    typedef unsigned short Unicode;

    bool toPinYin(const std::string &src, std::vector<std::string> &result_vec) const;

    bool toJianPin(const std::string &src, std::vector<std::string> &result_vec) const;
  
  private:
    inline bool isHanUnicode(const Unicode unicode) const {
        return begin_index_ <= unicode && end_index_ >= unicode;
    }

    bool toUnicode(const std::string &src, size_t &index, Unicode &result) const;

    bool getPinYinVec(const Unicode unicode,
                      std::vector<std::string> &result_vec,
                      bool width_tone = false) const;

  private:
    // @brief: 硬编码拼音数据
    // 1. pinyin_with_tone_vec_是基于unicode的顺序
    // 2. 即便unicode位无汉字对应拼音，也保留位置
    // 3. 多音字用空格分割
    // 4. 声调用数字表示
    static const Unicode begin_index_, end_index_;
    static const char* pinyin_with_tone_vec_[];

};

}

#endif 
