#include "examples_model.h"

#include <string>

#include "gtest/gtest.h"

std::string list_dir_file = "/Users/zhanghao/code/cpp/ffmpeg_stu/build";
std::string print_info_file_1 = "/Users/zhanghao/code/cpp/ffmpeg_stu/test/test_file/1227469313-1-16.mp4";
std::string print_info_file_2 = "/Users/zhanghao/code/cpp/ffmpeg_stu/test/test_file/output_audio.mp3";
std::string audio_encode_pcm_file = "/Users/zhanghao/code/cpp/ffmpeg_stu/test/test_file/output_audio.pcm";
std::string file_mp2 = "/Users/zhanghao/code/cpp/ffmpeg_stu/test/test_file/output_audio.mp2";

TEST(examples_model,list_dir_test){
    examples_model examples;
    EXPECT_EQ(examples.list_dir(list_dir_file),0);
}

TEST(examples_model,print_info_test){
    examples_model examples;
    EXPECT_EQ(examples.print_info(print_info_file_1),0);
    EXPECT_EQ(examples.print_info(print_info_file_2),0);
}

TEST(examples_model,audio_encode_pcm_test){
    examples_model examples;
    EXPECT_EQ(examples.audio_encode_pcm(print_info_file_2, audio_encode_pcm_file),0);
}

TEST(examples_model,parse_mp4_file_test){
    examples_model examples;
    EXPECT_EQ(examples.parse_mp4_file(print_info_file_1),0);
}

TEST(examples_model,video_extract_audio_test){
    examples_model model;
    EXPECT_EQ(model.video_extract_audio(print_info_file_1,"./test.aac"),0);
}

TEST(examples_model,avio_list_dir_test){
    examples_model model;
    EXPECT_EQ(model.avio_list_dir("../"),0);
}

// TEST(examples_model,avio_read_callback_test){
//     examples_model model;
//     EXPECT_EQ(model.avio_read_callback(print_info_file_1),0);
// }

TEST(examples_model,my_av_dump_format_test){
    examples_model model;
    EXPECT_EQ(model.my_av_dump_format(print_info_file_1),0);
}

TEST(examples_model,decode_audio_mp2_pcm_test){
    examples_model model;
    std::string output_file = "./audio.pcm";
    EXPECT_EQ(model.decode_audio_mp2_pcm(file_mp2,output_file),0);
}