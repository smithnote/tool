// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-16

#include <iostream>
#include <gtest/gtest.h>

#include "connection_pool.h"

class ConnectionPoolTest: public testing::Test {
  public:
    virtual void SetUp() {}
    virtual void TearDown() {}

};


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
