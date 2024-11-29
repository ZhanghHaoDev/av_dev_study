#include "ffmpeg_stu.h"

int main(){
    ffmpeg_stu test;
    std::string file_path = "/Users/zhanghao/code/cpp/av_dev_study/test/test_file/test.mp4";

    test.my_av_dump_format(file_path);
    //test.list_av_codecs();

    return 0;
}
