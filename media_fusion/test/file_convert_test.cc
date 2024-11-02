#include "file_convert.h"

#include <iostream>
#include <string>

#include "gtest/gtest.h"

std::string input_mp4 = "/Users/zhanghao/code/cpp/media_fusion/test/test_file/test_mp4.mp4";
std::string input_wav = "/Users/zhanghao/code/cpp/media_fusion/test/test_file/test_wav.wav";

std::string output_aac = "./output.aac";
std::string output_mp3 = "/Users/zhanghao/code/cpp/media_fusion/test/test_file/output.mp3";

// 测试文件是否能够打开
TEST(file_convert, open_file_test){
    file_convert file;
    EXPECT_EQ(file.file_open(input_mp4),0);
    EXPECT_EQ(file.file_open(input_wav),0);
}

// 测试视频提取音频
TEST(file_convert, video_to_audio_test){
    file_convert file;
    EXPECT_EQ(file.file_open(input_mp4),0);
    EXPECT_EQ(file.video_to_audio(output_aac), 0);
}

// 测试音频转换
TEST(file_convert, audio_convert_test){
    file_convert file;
    EXPECT_EQ(file.file_open(input_wav), 0);
}