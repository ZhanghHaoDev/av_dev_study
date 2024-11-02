#include "file_convert.h"

#include <iostream>
#include <fstream>
#include <string>

#include "glog/logging.h"

extern "C" {
    #include "libavformat/avformat.h"
    #include "libavcodec/avcodec.h"
    #include "libavutil/log.h"
    #include "libavutil/opt.h"
    #include "libswresample/swresample.h"
    #include "libavutil/channel_layout.h"
    #include "libavutil/pixfmt.h"
}

file_convert::file_convert(){
    av_log_set_level(AV_LOG_QUIET);   // 禁用 FFmpeg 日志输出
}

file_convert::~file_convert() = default;

int file_convert::file_open(std::string input_file){
    if (input_file.empty() || !std::ifstream(input_file).is_open()) {
        LOG(ERROR) << "无法打开文件: " << input_file;
        return -1;
    }

    AVFormatContext* formatContext = avformat_alloc_context();
    if (avformat_open_input(&formatContext, input_file.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << input_file;
        return -1;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        LOG(ERROR) << "无法获取流信息: " << input_file;
        avformat_close_input(&formatContext);
        return -1;
    }

    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            this->has_audio_stream = true;
        }
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            this->has_video_stream = true;
        }
    }

    avformat_close_input(&formatContext);

    if (!has_audio_stream && !has_video_stream) {
        LOG(ERROR) << "文件中不存在音频流和视频流: " << file_path;
        return -1;
    }

    this->file_path = input_file;
    return 0;
}

int file_convert::video_to_audio(std::string& output_audio_path) {
    int error_code = 0;
    if (has_video_stream == false) {
        LOG(ERROR) << "文件中不存在视频流: " << file_path;
        return error_code = -1;
    }else if (has_audio_stream == false) {
        LOG(ERROR) << "文件中不存在音频流: " << file_path;
        return error_code = -1;
    }
    
    // 1. 初始化 FFmpeg,初始化解码器
    avformat_network_deinit(); // 初始化网络流

    // 2. 打开输入文件
    AVFormatContext* formatContext = avformat_alloc_context();
    if ((error_code = avformat_open_input(&formatContext, file_path.c_str(), nullptr, nullptr)) != 0) {
        LOG(ERROR) << "无法打开文件: " << file_path << " 错误码: " << error_code;
        return error_code;
    }
    if ((error_code = avformat_open_input(&formatContext, file_path.c_str(), nullptr, nullptr)) != 0) {
        LOG(ERROR) << "无法打开文件: " << file_path << " 错误码: " << error_code;
        return error_code;
    }

    // 3. 获取流信息
    if ((error_code = avformat_find_stream_info(formatContext, nullptr)) < 0) {
        LOG(ERROR) << "无法获取流信息: " << file_path << " 错误码: " << error_code;
        avformat_close_input(&formatContext);
        return error_code;
    }

    // 4. 初始化解码器上下文
    AVCodecContext* codecContext = avcodec_alloc_context3(nullptr);
    if (codecContext == nullptr) {
        LOG(ERROR) << "无法分配解码器上下文: " << file_path;
        avformat_close_input(&formatContext);
        return error_code;
    }
    if ((error_code = avcodec_parameters_to_context(codecContext, formatContext->streams[0]->codecpar)) < 0) {
        LOG(ERROR) << "无法复制编解码器参数: " << file_path << " 错误码: " << error_code;
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return error_code;
    }
    const AVCodec* codec = avcodec_find_decoder(codecContext->codec_id);
    avcodec_open2(codecContext, codec, nullptr);

    // 5. 创建输出文件
    AVFormatContext* outputFormatContext = avformat_alloc_context(); // 创建输出格式上下文
    if ((error_code = avformat_alloc_output_context2(&outputFormatContext, nullptr, nullptr, output_audio_path.c_str())) < 0) {
        LOG(ERROR) << "无法创建输出文件: " << output_audio_path << " 错误码: " << error_code;
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return error_code;
    }
    AVStream* outputStream = avformat_new_stream(outputFormatContext, nullptr);
    if (outputStream == nullptr) {
        LOG(ERROR) << "无法创建输出流: " << output_audio_path;
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        avformat_free_context(outputFormatContext);
        return error_code;
    }
    if (avcodec_parameters_from_context(outputStream->codecpar, codecContext) < 0) {
        LOG(ERROR) << "无法复制编解码器参数: " << output_audio_path;
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        avformat_free_context(outputFormatContext);
        return error_code;
    }
    
    // 我觉得错误有可能是需要指定输出文件的参数
    outputFormatContext->streams[0]->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
    outputFormatContext->streams[0]->codecpar->codec_id = AV_CODEC_ID_AAC;
    outputFormatContext->streams[0]->codecpar->format = AV_SAMPLE_FMT_FLTP;
    outputFormatContext->streams[0]->codecpar->sample_rate = 44100;
    outputFormatContext->streams[0]->codecpar->bit_rate = 128000;
    outputFormatContext->streams[0]->codecpar->frame_size = 2048;
    outputFormatContext->streams[0]->codecpar->ch_layout.nb_channels = 2;
    outputFormatContext->streams[0]->codecpar->codec_tag = 0;

    outputStream->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
    outputStream->codecpar->codec_id = AV_CODEC_ID_AAC;
    outputStream->codecpar->format = AV_SAMPLE_FMT_FLTP;
    outputStream->codecpar->sample_rate = 44100;
    outputStream->codecpar->bit_rate = 128000;
    outputStream->codecpar->frame_size = 2048;
    outputStream->codecpar->ch_layout.nb_channels = 2;
    outputStream->codecpar->codec_tag = 0;
    //outputStream->codecpar->chroma_location = AV_CH_LAYOUT_STEREO;

    if ((error_code = avio_open(&outputFormatContext->pb, output_audio_path.c_str(), AVIO_FLAG_WRITE)) < 0) {
        LOG(ERROR) << "无法打开输出文件: " << output_audio_path << " 错误码: " << error_code;
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        avformat_free_context(outputFormatContext);
        return error_code;
    }   

    // TODO 报错，无法写入文件头 错误码: -1094995529
    if ((error_code = avformat_write_header(outputFormatContext, nullptr)) < 0) {
        LOG(ERROR) << "无法写入文件头: " << output_audio_path << " 错误码: " << error_code;
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        avformat_free_context(outputFormatContext);
        return error_code;
    }

    // 6. 读取和解码音频数据
    AVPacket packet;
    while (av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == 0) {
            LOG(INFO) << "解码音频数据: " << packet.stream_index;
            AVFrame* frame = av_frame_alloc();
            if (avcodec_send_packet(codecContext, &packet) < 0) {
                LOG(ERROR) << "无法发送数据包到解码器: " << file_path ;
                av_frame_free(&frame);
                break;
            }
            if (avcodec_receive_frame(codecContext, frame) < 0) {
                LOG(ERROR) << "无法从解码器接收帧: " << file_path;
                av_frame_free(&frame);
                break;
            }
            if (avcodec_send_frame(codecContext, frame) < 0) {
                LOG(ERROR) << "无法发送帧到解码器: " << file_path;
                av_frame_free(&frame);
                break;
            }
            if (avcodec_receive_packet(codecContext, &packet) < 0) {
                LOG(ERROR) << "无法从解码器接收数据包: " << file_path;
                av_frame_free(&frame);
                break;
            }
            if (av_write_frame(outputFormatContext, &packet) < 0) {
                LOG(ERROR) << "无法写入数据包: " << output_audio_path;
                av_frame_free(&frame);
                break;
            }
            av_frame_free(&frame);
        }
        av_packet_unref(&packet);
    }








    return error_code;
}

int file_convert::audio_convert(std::string& output_audio_path) {
    
    return 0;
}