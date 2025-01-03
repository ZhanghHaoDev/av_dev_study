#include "ffmpeg_stu.h"

#include <iostream>

int main(){
    ffmpeg_stu test;
    std::string file_path = "/Users/zhanghao/code/cpp/av_dev_study/test/test_file/test.mp4";
    std::string aac_file = "/Users/zhanghao/code/cpp/av_dev_study/test/test_file/output.m4a";
    std::string pcm_file = "/Users/zhanghao/code/cpp/av_dev_study/test/test_file/test.wav";

    //test.my_av_dump_format(file_path);
    //test.list_av_codecs();
    test.aac_file(aac_file);
    std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << "\n";

    std::string output_file = "output_file.aac";
    //test.wav_to_aac(pcm_file, output_file);
    std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << "\n";
    test.analyze_h264_file(file_path);

    return 0;
}
