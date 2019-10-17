// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-16

#include <iostream>
#include <memory>
#include <gtest/gtest.h>

#include "thread_pool.h"

class ThreadPoolTest: public testing::Test {
  public:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    std::shared_ptr<tool::ThreadPool> pool;
};


TEST_F(ThreadPoolTest, pool_start_test) {
    pool = std::make_shared<tool::ThreadPool>();
    pool->start(10);
    size_t pool_size;
    ASSERT_EQ(pool->getPoolSize(pool_size), true);
    ASSERT_EQ(pool_size, 10);
    pool->stop();
}

TEST_F(ThreadPoolTest, pool_work_test) {
    pool = std::make_shared<tool::ThreadPool>();
    pool->start(10);
    for (size_t i = 0; i < 100; ++i) {
        pool->pushTask(std::make_shared<tool::PoolTask>());
    }
    pool->stop();
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
