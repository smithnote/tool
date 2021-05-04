// Copyright (c) 2021 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2021-01-26


#include <iostream>
#include <fstream>
#include <thread>
#include <memory>
#include <gtest/gtest.h>

#include "inotify.h"

using namespace tool;

class InotifyTest : public testing::Test {
  public:
    virtual void SetUp() {}
    virtual void TearDown() {}
};


TEST_F(InotifyTest, add) {
    FileWatcher file_watcher;
    ASSERT_FALSE(file_watcher.isInit());
    ASSERT_TRUE(file_watcher.start());
    const std::string path1{"/tmp/datachannel.txt"};
    system("rm -f /tmp/datachannel.txt");
    int triger_count = 0;
    ASSERT_FALSE(file_watcher.add(path1, IN_DELETE_SELF|IN_DONT_FOLLOW,
                                  std::make_shared<std::function<void()>>([&](){
                                      ++triger_count;
                                  })));
    system("touch /tmp/datachannel.txt");
    ASSERT_TRUE(file_watcher.add(path1, IN_DELETE_SELF|IN_DONT_FOLLOW,
                                 std::make_shared<std::function<void()>>([&](){
                                      ++triger_count;
                                 })));
    ASSERT_TRUE(file_watcher.add(path1, IN_DELETE_SELF|IN_DONT_FOLLOW,
                                 std::make_shared<std::function<void()>>([&](){
                                      ++triger_count;
                                 })));
    ASSERT_EQ(file_watcher.count(), 1);
    system("rm -f /tmp/datachannel.txt");
    sleep(1);
    ASSERT_EQ(triger_count, 1);
    ASSERT_EQ(file_watcher.count(), 0);
    system("touch /tmp/datachannel.txt");
    ASSERT_TRUE(file_watcher.add(path1, IN_MODIFY|IN_DELETE_SELF,
                                 std::make_shared<std::function<void()>>([&](){
                                      ++triger_count;
                                 })));
    system("echo '123' >> /tmp/datachannel.txt");
    sleep(1);
    ASSERT_EQ(triger_count, 2);
    system("echo '123' >> /tmp/datachannel.txt");
    sleep(1);
    ASSERT_EQ(triger_count, 2);
    system("rm -f /tmp/datachannel.txt");
}


TEST_F(InotifyTest, remove) {
    FileWatcher file_watcher;
    ASSERT_FALSE(file_watcher.isInit());
    ASSERT_TRUE(file_watcher.start());
    const std::string path1{"/tmp/datachannel.txt"};
    system("rm -f /tmp/datachannel.txt");
    int triger_count = 0;
    ASSERT_EQ(file_watcher.count(), 0);
    ASSERT_TRUE(file_watcher.remove(path1));
    system("touch /tmp/datachannel.txt");
    ASSERT_TRUE(file_watcher.add(path1, IN_DELETE_SELF|IN_DONT_FOLLOW,
                                 std::make_shared<std::function<void()>>([&](){
                                      ++triger_count;
                                 })));
    ASSERT_EQ(file_watcher.count(), 1);
    ASSERT_TRUE(file_watcher.remove(path1));
    ASSERT_EQ(file_watcher.count(), 0);
    system("rm -f /tmp/datachannel.txt");
    sleep(1);
    ASSERT_EQ(triger_count, 0);
    ASSERT_EQ(file_watcher.count(), 0);
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
