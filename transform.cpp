// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-11-02

#include "transform.h"


namespace tool {

const std::string Base64::base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

bool Base64::encode(const std::string &src, std::string &result) const {
    if (src.empty()) {
        return false;
    }
    auto bytes_to_encode = src.c_str();
    unsigned int in_len = src.length();
    int i = 0, j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4)
                              + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2)
                              + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            for (i = 0; i <4; i++) {
                result += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }
    if (i) {
        for (j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }
        char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4)
                          + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2)
                          + ((char_array_3[2] & 0xc0) >> 6);
        for (j = 0; j < i + 1; j++) {
            result += base64_chars[char_array_4[j]];
        }
        while((i++ < 3)) {
            result += '=';
        }
    }
    return true;
}

bool Base64::decode(const std::string &src, std::string &result) const {
    const auto &encoded_string = src;
    size_t in_len = encoded_string.size();
    int i = 0, j = 0, in = 0;
    unsigned char char_array_4[4], char_array_3[3];

    while (in_len-- && (encoded_string[in] != '=') && is_base64(encoded_string[in])) {
        char_array_4[i++] = encoded_string[in];
        in++;
        if (i ==4) {
            for (i = 0; i <4; i++) {
                char_array_4[i] = base64_chars.find(char_array_4[i]) & 0xff;
            }
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4)
                              + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; i < 3; i++) {
                result += char_array_3[i];
            }
            i = 0;
        }
    }
    if (i) {
        for (j = 0; j < i; j++) {
            char_array_4[j] = base64_chars.find(char_array_4[j]) & 0xff;
        }
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4)
                          + ((char_array_4[2] & 0x3c) >> 2);
        for (j = 0; j < i - 1; j++) {
            result += char_array_3[j];
        }
    }
    return true;
}

}
