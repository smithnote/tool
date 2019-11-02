// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-11-02

#include <iostream>
#include <gtest/gtest.h>

#include "transform.h"


class TransformTest: public testing::Test {
  public:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    tool::Base64 base64;
};


TEST_F(TransformTest, base64_test) {
    std::string src_string = "我爱我的祖国, I love my homeland";
    std::string encode_string, decode_string;
    ASSERT_EQ(base64.encode(src_string, encode_string), true);
    ASSERT_EQ(encode_string.empty(), false);
    ASSERT_EQ(base64.decode(encode_string, decode_string), true);
    ASSERT_EQ(decode_string.empty(), false);
    ASSERT_EQ(decode_string, src_string);
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
