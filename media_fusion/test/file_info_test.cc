#include "file_info.h"

#include <iostream>
#include <string>

#include "gtest/gtest.h"

/*
* 测试文件路径 /Users/zhanghao/code/cpp/media_fusion/test/test_file/test.aac
* 文件大小：1840235字节
* 文件类型：AAC
* 音频时长：3041s
* 采样率：48000
* 声道数：2
* 比特率：4841
* 比特深度：32
* 采样值 size : 27086848
* 帧长度：2048
*/
std::string aac_file = "/Users/zhanghao/code/cpp/media_fusion/test/test_file/test_aac.aac";

/*
* 测试文件路径 /Users/zhanghao/code/cpp/media_fusion/test/test_file/test_mp4.mp4
* 文件大小：类型：MP4
* 文件大小：10194885字节
* 视频时长：282s
* 视频宽度：640
* 视频高度：360
* 帧率：24
* 比特率：4841
* 编码格式：h264
* 颜色空间：yuv420p
*/
std::string mp4_file = "/Users/zhanghao/code/cpp/media_fusion/test/test_file/test_mp4.mp4";

/*
* 测试文件路径 /Users/zhanghao/code/cpp/media_fusion/test/test_file/test_wav.wav
* 文件类型：WAV
* 文件大小：1840235字节
* 文件类型：WAV
* 音频时长：3041s
* 采样率：48000
* 声道数：2
* 比特率：4841
* 比特深度：32
* 采样值 size : 12443021
* 帧长度：2048
*/
std::string wav_file = "/Users/zhanghao/code/cpp/media_fusion/test/test_file/test_wav.wav";

TEST(file_info, open_file_aac){
    file_info file;
    EXPECT_EQ(0, file.file_open(aac_file));
}

TEST(file_info, get_audio_file_info_aac_tes) {
    file_info file;
    EXPECT_EQ(0, file.file_open(aac_file));
    auto info = file.get_audio_file_info();
    EXPECT_EQ(info.file_size, 1840235);
    EXPECT_EQ(info.duration, 3041);
    EXPECT_EQ(info.sample_rate, 48000);
    EXPECT_EQ(info.channels, 2);
    EXPECT_EQ(info.bit_rate, 4841);
    EXPECT_EQ(info.bit_depth, 32);
    EXPECT_EQ(info.frame_length, 2048);
}

TEST(file_info, get_sample_values_aac_test) {
    file_info file;
    EXPECT_EQ(0, file.file_open(aac_file));
    auto samples = file.get_sample_values<int16_t>();
    EXPECT_EQ(samples.size(), 6614);
}

TEST(file_info, get_video_file_info_test) {
    file_info file;
    EXPECT_EQ(0, file.file_open(mp4_file));
    auto info = file.get_video_file_info();
    EXPECT_EQ(info.file_path, mp4_file);
    EXPECT_EQ(info.file_size, 10194885);
    EXPECT_EQ(info.duration, 282);
    EXPECT_EQ(info.width, 640);
    EXPECT_EQ(info.height, 360);
    EXPECT_EQ(info.frame_rate, 24);
    EXPECT_EQ(info.bit_rate, 232685);
    EXPECT_EQ(info.codec, "h264");
}

TEST(file_info, open_file_wav){
    file_info file;
    EXPECT_EQ(0, file.file_open(wav_file));
}

TEST(file_info, get_audio_file_info_wav_tes) {
    file_info file;
    EXPECT_EQ(0, file.file_open(wav_file));
    auto info = file.get_audio_file_info();
    EXPECT_EQ(info.file_size, 49772162);
    EXPECT_EQ(info.duration, 282);
    EXPECT_EQ(info.sample_rate, 44100);
    EXPECT_EQ(info.channels, 2);
    EXPECT_EQ(info.bit_rate, 1411200);
    EXPECT_EQ(info.bit_depth, 16);
    EXPECT_EQ(info.frame_length, 0);
}

TEST(file_info, get_sample_values_wav_test) {
    file_info file;
    EXPECT_EQ(0, file.file_open(wav_file));
    auto samples = file.get_sample_values<int16_t>();
    EXPECT_EQ(samples.size(), 12443021);
}