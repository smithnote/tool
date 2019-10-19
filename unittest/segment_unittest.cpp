// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-19


#include <iostream>
#include <gtest/gtest.h>

#include "segment.h"


class SegmentTest: public testing::Test {
  public:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    tool::Segment segment;
};


TEST_F(SegmentTest, seg_unigram_test) {
    std::string cn_sentence = "这是一个美好的世界";
    std::vector<std::string> unigram_vec;
    ASSERT_EQ(segment.segUnigram(cn_sentence, unigram_vec), true);
    ASSERT_EQ(unigram_vec.size(), 9);
    std::string en_sentence = "hello china, hello beijing";
    ASSERT_EQ(segment.segUnigram(en_sentence, unigram_vec), true);
    ASSERT_EQ(unigram_vec.size(), 5);
    std::string encn_sentence = "这个很fashion嘛484?";
    ASSERT_EQ(segment.segUnigram(encn_sentence, unigram_vec), true);
    ASSERT_EQ(unigram_vec.size(), 7);
}

TEST_F(SegmentTest, seg_bigram_test) {
    std::string cn_sentence = "这是一个美好的世界";
    std::vector<std::string> bigram_vec;
    ASSERT_EQ(segment.segBigram(cn_sentence, bigram_vec), true);
    ASSERT_EQ(bigram_vec.size(), 8);
    std::string en_sentence = "hello china, hello beijing";
    ASSERT_EQ(segment.segBigram(en_sentence, bigram_vec), true);
    ASSERT_EQ(bigram_vec.size(), 4);
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
