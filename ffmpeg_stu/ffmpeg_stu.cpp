#include "ffmpeg_stu.h"

#include <iostream>

extern "C"{
    #include "libavformat/avformat.h"
}

ffmpeg_stu::ffmpeg_stu() = default;

ffmpeg_stu::~ffmpeg_stu() = default;

void ffmpeg_stu::my_av_dump_format(std::string file_path){
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

void ffmpeg_stu::list_av_codecs(){
    const AVCodec* codec = nullptr; // 列举所有编码器
    void* iter = nullptr;
    printf("Encoders:\n");
    while ((codec = av_codec_iterate(&iter))) {
        if (av_codec_is_encoder(codec)) {
            printf("Encoder: %s", codec->name);
        }
    }

    std::cout << "\n";

    iter = nullptr;             // 列举所有解码器
    printf("Decoders:\n");
    while ((codec = av_codec_iterate(&iter))) {
        if (av_codec_is_decoder(codec)) {
            printf("Decoder: %s", codec->name);
        }
    }
    std::cout << "\n";
}