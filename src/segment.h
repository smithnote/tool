// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-19

#ifndef TOOL_SEGMENT_H_
#define TOOL_SEGMENT_H_

#include <cctype>
#include <iostream>
#include <string>
#include <vector>


namespace tool {

class Segment {
  public:
    Segment() {};
    ~Segment() {};
    
    bool segUnigram(const std::string &sentence, std::vector<std::string> &word_vec);

    bool segBigram(const std::string &sentence, std::vector<std::string> &word_vec);
    
    bool segBigram(const std::vector<std::string> &unigram_vec,
                   std::vector<std::string> &word_vec);

};

}

#endif 
