// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-11-02

#include <iostream>
#include <gtest/gtest.h>

#include "hz2py.h"


class TransformTest: public testing::Test {
  public:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    tool::HZ2PY hz2py;
};


TEST_F(TransformTest, base64_test) {
    std::string src_string = "我爱我的祖国";
    std::vector<std::string> py_vec, jp_vec;
    ASSERT_EQ(hz2py.toPinYin(src_string, py_vec), true);
    ASSERT_EQ(py_vec.empty(), false);
    ASSERT_EQ(hz2py.toJianPin(src_string, jp_vec), true);
    ASSERT_EQ(jp_vec.empty(), false);
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
