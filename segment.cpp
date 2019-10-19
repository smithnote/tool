// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-19

#include "segment.h"

namespace tool {

bool Segment::segUnigram(const std::string &sentence,
                         std::vector<std::string> &unigram_vec) {
    unigram_vec.clear();
    for (size_t i = 0; i < sentence.size();) {
        unsigned char c = sentence[i];
        if ((c & 0xFC) == 0xFC) {// 6 bytes
            unigram_vec.push_back(sentence.substr(i, 6));
            i += 6;
        } else if ((c & 0xF8) == 0xF8) {// 5 bytes
            unigram_vec.push_back(sentence.substr(i, 5));
            i += 5;
        } else if ((c & 0xF0) == 0xF0) {// 4 bytes
            unigram_vec.push_back(sentence.substr(i, 4));
            i += 4;
        } else if ((c & 0xE0) == 0xE0) {// 3 bytes
            unigram_vec.push_back(sentence.substr(i, 3));
            i += 3;
        } else if ((c & 0xC0) == 0xC0) {// 2 bytes
            unigram_vec.push_back(sentence.substr(i, 2));
            i += 2;
        } else{
            size_t end = i;
            while (isalnum(sentence[end])) {
                ++end;
            }
            if (end != i) {
                unigram_vec.push_back(sentence.substr(i, end-i));
                i = end;
            } else {
                if (!isspace(c)) {
                    unigram_vec.push_back(sentence.substr(i, 1));
                }
                i++;
            }
        }
    }
    return true;
}

bool Segment::segBigram(const std::string &sentence,
                        std::vector<std::string>& bigram_vec) {
    std::vector<std::string> unigram_vec;
    if (!segUnigram(sentence, unigram_vec)) {
        return false;
    }
    return segBigram(unigram_vec, bigram_vec);
}

bool Segment::segBigram(const std::vector<std::string>& unigram_vec,
                        std::vector<std::string>& bigram_vec) {
    bigram_vec.clear();
    if (unigram_vec.empty()) {
        return true;
    }
    if (unigram_vec.size() == 1) {
        bigram_vec.push_back(unigram_vec[0]);
    }
    for (size_t i = 0; i < unigram_vec.size() - 1; ++i) {
        bigram_vec.push_back(unigram_vec[i] + unigram_vec[i+1]);
    }
    return true;
}

}
