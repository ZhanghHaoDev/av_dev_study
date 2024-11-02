#include "file_info.h"

#include <_types/_uint64_t.h>
#include <fstream>

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

file_info::file_info(){
    av_log_set_level(AV_LOG_QUIET);   // 禁用 FFmpeg 日志输出
}

file_info::~file_info() = default;

int file_info::file_open(const std::string &file_name) {
    if (file_name.empty() || !std::ifstream(file_name).is_open()) {
        LOG(ERROR) << "无法打开文件: " << file_name;
        return -1;
    }

    AVFormatContext* formatContext = avformat_alloc_context();
    if (avformat_open_input(&formatContext, file_name.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << file_name;
        return -1;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        LOG(ERROR) << "无法获取流信息: " << file_name;
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
        LOG(ERROR) << "文件中不存在音频流和视频流: " << file_name;
        return -1;
    }

    this->file_path = file_name;
    return 0;
}

file_info::audio_file_info file_info::get_audio_file_info(){
    if (!this->has_audio_stream) {
        LOG(ERROR) << "文件中不存在音频流: " << this->file_path;
        throw std::runtime_error("文件中不存在音频流");
    }

    audio_file_info info;
    info.file_path = this->file_path;

    AVFormatContext* format_context = nullptr;
    if (avformat_open_input(&format_context, this->file_path.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << this->file_path;
        throw std::runtime_error("无法打开文件");
    }

    if (avformat_find_stream_info(format_context, nullptr) < 0) {
        LOG(ERROR) << "无法找到流信息: " << this->file_path;
        avformat_close_input(&format_context);
        throw std::runtime_error("无法找到流信息");
    }

    AVCodecContext* codec_context = nullptr;
    const AVCodec* codec = nullptr;
    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();

    for (unsigned int i = 0; i < format_context->nb_streams; i++) {
        AVStream* stream = format_context->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            codec = avcodec_find_decoder(stream->codecpar->codec_id);
            if (!codec) {
                LOG(ERROR) << "无法找到解码器: " << info.file_path;
                avformat_close_input(&format_context);
                throw std::runtime_error("无法找到解码器");
            }

            codec_context = avcodec_alloc_context3(codec);
            if (!codec_context) {
                LOG(ERROR) << "无法分配解码器上下文: " << info.file_path;
                avformat_close_input(&format_context);
                throw std::runtime_error("无法分配解码器上下文");
            }

            if (avcodec_parameters_to_context(codec_context, stream->codecpar) < 0) {
                LOG(ERROR) << "无法初始化解码器上下文: " << info.file_path;
                avcodec_free_context(&codec_context);
                avformat_close_input(&format_context);
                throw std::runtime_error("无法初始化解码器上下文");
            }

            if (avcodec_open2(codec_context, codec, nullptr) < 0) {
                LOG(ERROR) << "无法打开解码器: " << info.file_path;
                avcodec_free_context(&codec_context);
                avformat_close_input(&format_context);
                throw std::runtime_error("无法打开解码器");
            }

            info.duration = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
            info.file_size = avio_size(format_context->pb);
            info.sample_rate = stream->codecpar->sample_rate;
            info.channels = stream->codecpar->ch_layout.nb_channels;
            info.bit_rate = stream->codecpar->bit_rate;
            info.bit_depth = av_get_bytes_per_sample(static_cast<AVSampleFormat>(stream->codecpar->format)) * 8;
            info.frame_length = stream->codecpar->frame_size;
        }
    }

    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codec_context);
    avformat_close_input(&format_context);

    return info;
}

template <typename T>
std::vector<T> file_info::get_sample_values() {
    std::vector<T> samples;

    AVFormatContext* formatContext = nullptr;
    if (avformat_open_input(&formatContext, file_path.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << file_path;
        return samples;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        LOG(ERROR) << "无法获取流信息: " << file_path;
        avformat_close_input(&formatContext);
        return samples;
    }

    const AVCodec* codec = nullptr;
    AVCodecContext* codecContext = nullptr;
    int audioStreamIndex = -1;

    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = i;
            codec = avcodec_find_decoder(formatContext->streams[i]->codecpar->codec_id);
            if (!codec) {
                LOG(ERROR) << "无法找到解码器: " << file_path;
                avformat_close_input(&formatContext);
                return samples;
            }
            codecContext = avcodec_alloc_context3(codec);
            if (!codecContext) {
                LOG(ERROR) << "无法分配解码器上下文: " << file_path;
                avformat_close_input(&formatContext);
                return samples;
            }
            if (avcodec_parameters_to_context(codecContext, formatContext->streams[i]->codecpar) < 0) {
                LOG(ERROR) << "无法复制解码器参数: " << file_path;
                avcodec_free_context(&codecContext);
                avformat_close_input(&formatContext);
                return samples;
            }
            if (avcodec_open2(codecContext, codec, nullptr) < 0) {
                LOG(ERROR) << "无法打开解码器: " << file_path;
                avcodec_free_context(&codecContext);
                avformat_close_input(&formatContext);
                return samples;
            }
            break;
        }
    }

    if (audioStreamIndex == -1) {
        LOG(ERROR) << "找不到音频流: " << file_path;
        avformat_close_input(&formatContext);
        return samples;
    }

    AVPacket packet;
    packet.data = nullptr;
    packet.size = 0;

    while (av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == audioStreamIndex) {
            int ret = avcodec_send_packet(codecContext, &packet);
            if (ret < 0) {
                LOG(ERROR) << "Error sending packet for decoding: " << ret;
                break;
            }

            AVFrame* frame = av_frame_alloc();
            if (!frame) {
                LOG(ERROR) << "无法分配帧";
                break;
            }

            ret = avcodec_receive_frame(codecContext, frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                av_frame_free(&frame);
                continue;
            } else if (ret < 0) {
                LOG(ERROR) << "Error during decoding: " << ret;
                av_frame_free(&frame);
                break;
            }

            // 确保 frame->data[0] 是有效的
            if (frame->data[0]) {
                int data_size = av_get_bytes_per_sample(codecContext->sample_fmt);
                if (data_size < 0) {
                    LOG(ERROR) << "Failed to calculate data size";
                    av_frame_free(&frame);
                    break;
                }

                for (int i = 0; i < frame->nb_samples; i++) {
                    T sample;
                    memcpy(&sample, frame->data[0] + data_size * i, data_size);
                    samples.push_back(sample);
                }
            }

            av_frame_free(&frame);
        }

        av_packet_unref(&packet);
    }

    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);

    return samples;
}

template std::vector<uint8_t> file_info::get_sample_values<uint8_t>();
template std::vector<int16_t> file_info::get_sample_values<int16_t>();
template std::vector<int32_t> file_info::get_sample_values<int32_t>();
template std::vector<float> file_info::get_sample_values<float>();
template std::vector<double> file_info::get_sample_values<double>();

file_info::video_file_info file_info::get_video_file_info(){
    if (!this->has_video_stream) {
        LOG(ERROR) << "文件中不存在视频流: " << this->file_path;
        throw std::runtime_error("文件中不存在视频流");
    }

    video_file_info info;
     info.file_path = this->file_path;

    AVFormatContext* formatContext = nullptr;
    if (avformat_open_input(&formatContext, file_path.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << file_path;
        throw std::runtime_error("无法打开文件");
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        LOG(ERROR) << "无法获取流信息: " << file_path;
        avformat_close_input(&formatContext);
        throw std::runtime_error("无法获取流信息");
    }

    AVCodecParameters* codecParameters = nullptr;
    int videoStreamIndex = -1;

    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            codecParameters = formatContext->streams[i]->codecpar;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        LOG(ERROR) << "找不到视频流: " << file_path;
        avformat_close_input(&formatContext);
        throw std::runtime_error("找不到视频流");
    }

    info.file_type = av_guess_format(nullptr, file_path.c_str(), nullptr)->name;
    info.file_size = formatContext->pb->seekable ? avio_size(formatContext->pb) : -1;
    info.duration = formatContext->duration / AV_TIME_BASE;
    info.width = codecParameters->width;
    info.height = codecParameters->height;
    info.frame_rate = av_q2d(formatContext->streams[videoStreamIndex]->avg_frame_rate);
    info.bit_rate = codecParameters->bit_rate;
    info.codec = avcodec_get_name(codecParameters->codec_id);
    
    avformat_close_input(&formatContext);

    return info;
}