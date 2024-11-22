#include "ffmpeg_stu.h"

#include <iostream>

extern "C"{
    #include "libavformat/avformat.h"
}

ffmpeg_stu::ffmpeg_stu(){
    std::string file_path = "/Users/zhanghao/code/cpp/av_dev_study/ffmpeg_stu/test/test_file/1227469313-1-16.mp4";

    int err = 0;
    av_log_set_level(AV_LOG_INFO);
    if (file_path.empty()){
        av_log(0,AV_LOG_ERROR, "文件为空");
        return;
    }

    AVFormatContext *ps = nullptr;
    if ((err = avformat_open_input(&ps, file_path.c_str(), nullptr, nullptr)) != 0){
        av_log(nullptr,AV_LOG_ERROR,"读取文件头部信息错误");
        return;
    }

    if((err = avformat_find_stream_info(ps,nullptr)) != 0){
        av_log(nullptr,AV_LOG_ERROR,"获取文件流信息错误");
        return;
    }

    av_dump_format(ps, 0, file_path.c_str(), 0);

    avformat_close_input(&ps);
}

ffmpeg_stu::~ffmpeg_stu() = default;