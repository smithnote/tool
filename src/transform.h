// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-11-02

#ifndef TOOL_TRANSFORM_H_
#define TOOL_TRANSFORM_H_

#include <iostream>


namespace tool {

class Base64 {
  public:
    inline bool is_base64(unsigned char c) const {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
    
    bool encode(const std::string &src, std::string &result) const;
    
    bool decode(const std::string &src, std::string &result) const;

  private:
    const static std::string base64_chars;
};

}

#endif 
