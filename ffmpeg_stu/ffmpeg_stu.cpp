#include "ffmpeg_stu.h"

#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <sstream>
#include <iostream>

extern "C"{
    #include "libavformat/avformat.h"
    #include "libavcodec/avcodec.h"
    #include "libswresample/swresample.h"
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

    AVFormatContext *ps = avformat_alloc_context();
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

void ffmpeg_stu::av_info_map(const std::string& file_path, std::map<std::string, std::string>& info_map) {
    int err = 0;

    if (file_path.empty()) {
        std::cerr << "文件为空" << std::endl;
        return;
    }

    AVFormatContext *ps = nullptr;
    if ((err = avformat_open_input(&ps, file_path.c_str(), nullptr, nullptr)) != 0) {
        std::cerr << "读取文件头部信息错误" << std::endl;
        return;
    }

    if ((err = avformat_find_stream_info(ps, nullptr)) != 0) {
        std::cerr << "获取文件流信息错误" << std::endl;
        avformat_close_input(&ps);
        return;
    }

    // 获取格式信息
    info_map["格式"] = ps->iformat->name;
    info_map["持续时间"] = std::to_string(ps->duration / AV_TIME_BASE) + " 秒";
    info_map["比特率"] = std::to_string(ps->bit_rate) + " bps";
    info_map["流数量"] = std::to_string(ps->nb_streams);

    // 获取每个流的信息
    for (unsigned int i = 0; i < ps->nb_streams; i++) {
        AVStream *stream = ps->streams[i];
        AVCodecParameters *codecpar = stream->codecpar;
        std::ostringstream stream_info;

        stream_info << "编码类型: " << av_get_media_type_string(codecpar->codec_type) << "\n";
        stream_info << "编码器ID: " << codecpar->codec_id << "\n";
        stream_info << "比特率: " << codecpar->bit_rate << "\n";
        stream_info << "宽度: " << codecpar->width << "\n";
        stream_info << "高度: " << codecpar->height << "\n";
        stream_info << "采样率: " << codecpar->sample_rate << "\n";
        stream_info << "通道数: " << codecpar->ch_layout.nb_channels << "\n";

        info_map["流 #" + std::to_string(i)] = stream_info.str();
    }

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

void ffmpeg_stu::aac_file(std::string file_path){
    /**
     * aac 文件分析
     * 1. 是否是aac文件
     * 2. aac文件编码规格
     * 3. aac文件封住格式
     * 4. 头部bit的解析
     */
    int error = 0;
    AVFormatContext *ps = nullptr;
    error = avformat_open_input(&ps, file_path.c_str(), nullptr,nullptr);
    if(error != 0){
        std::cerr << "解析文件的头部错误" << "\n";
        avformat_close_input(&ps);
        return;
    }
    error = avformat_find_stream_info(ps, nullptr);
    if(error != 0){
        std::cerr << "解析文件的流信息错误" << "\n";
        avformat_close_input(&ps);
        return;
    }
    
    // 如果输入的是一个aac的裸流，没有更近一步的封装，这个时候可以输出有关aac的信息
    // 但是这里aac文件如果被封装到容器里面，就需要进一步的查找
    // 查找音频流索引
    int audio_index = -1;
    for(auto i = 0; ps->nb_streams;i++){
        if(ps->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){
            audio_index = i;
            break;
        }
    }
    if(audio_index == -1){
        std::cerr << "没有找到音频流" << "\n";
        avformat_close_input(&ps);
        return ;
    }

    // 这里如果查找到了音频的索引，就不应该再使用AVFormatContext这个结构体
    // 如果想要输出音频的信息，就应该使用AVStream ，这个结构体表示AVFormatContext 里面的音频流
    // AVCodecParameters 表示 音频流的编解码参数
    AVStream *audio_stream = ps->streams[audio_index];
    AVCodecParameters *codecpar = audio_stream->codecpar;

    std::cout << "音频的编码器：" << avcodec_get_name(codecpar->codec_id) << "\n";
    std::cout << "AAC 编码规格: ";
    switch (codecpar->profile) {
        case FF_PROFILE_AAC_MAIN:
            std::cout << "MPEG-2 AAC Main" << std::endl;
            break;
        case FF_PROFILE_AAC_LOW:
            std::cout << "MPEG-2 AAC LC (Low Complexity)" << std::endl;
            break;
        case FF_PROFILE_AAC_SSR:
            std::cout << "MPEG-2 AAC SSR (Scalable Sample Rate)" << std::endl;
            break;
        case FF_PROFILE_AAC_LTP:
            std::cout << "MPEG-2 AAC LTP (Long Term Prediction)" << std::endl;
            break;
        case FF_PROFILE_AAC_HE:
            std::cout << "MPEG-2 AAC HE (High Efficiency)" << std::endl;
            break;
        case FF_PROFILE_AAC_HE_V2:
            std::cout << "MPEG-2 AAC HE v2" << std::endl;
            break;
        case FF_PROFILE_AAC_LD:
            std::cout << "MPEG-2 AAC LD (Low Delay)" << std::endl;
            break;
        case FF_PROFILE_AAC_ELD:
            std::cout << "MPEG-2 AAC ELD (Enhanced Low Delay)" << std::endl;
            break;
        default:
            std::cout << "未知规格" << std::endl;
            break;
    }

    std::cout << "采样率: " << codecpar->sample_rate << " Hz" << "\n";
    std::cout << "声道数: " << codecpar->ch_layout.nb_channels << "\n";
    std::cout << "采样格式: " << av_get_sample_fmt_name(static_cast<AVSampleFormat>(codecpar->format)) << "\n";
    std::cout << "比特率: " << codecpar->bit_rate / 1000 << " kbps" << "\n";

    avformat_close_input(&ps);
}

void ffmpeg_stu::wav_to_aac(std::string input_file, std::string output_file) {
    int64_t frame_index = 0; // 初始化帧计数器
    int error = 0;
    AVFormatContext *ps = nullptr;
    error = avformat_open_input(&ps, input_file.c_str(), nullptr, nullptr);
    if (error < 0) {
        std::cout << "读取文件头错误" << "\n";
        avformat_close_input(&ps);
        return;
    }
    error = avformat_find_stream_info(ps, nullptr);
    if (error < 0) {
        std::cout << "读取文件流错误" << "\n";
        avformat_close_input(&ps);
        return;
    }

    int audio_index = -1;
    for (auto i = 0; i < ps->nb_streams; i++) {
        if (ps->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_index = i;
            break;
        }
    }

    if (audio_index == -1) {
        std::cerr << "没有找到音频流" << "\n";
        avformat_close_input(&ps);
        return;
    }

    const AVCodec *dec_codec = avcodec_find_decoder(ps->streams[audio_index]->codecpar->codec_id);
    if (!dec_codec) {
        std::cerr << "无法找到输入文件的解码器" << "\n";
        avformat_close_input(&ps);
        return;
    }

    AVCodecContext *dec_ctx = avcodec_alloc_context3(dec_codec);
    if (!dec_ctx) {
        std::cerr << "无法分配解码器上下文" << "\n";
        avformat_close_input(&ps);
        return;
    }

    if (avcodec_parameters_to_context(dec_ctx, ps->streams[audio_index]->codecpar) < 0) {
        std::cerr << "无法将输入流参数复制到解码器上下文" << "\n";
        avcodec_free_context(&dec_ctx);
        avformat_close_input(&ps);
        return;
    }

    if (avcodec_open2(dec_ctx, dec_codec, nullptr) < 0) {
        std::cerr << "无法打开解码器" << "\n";
        avcodec_free_context(&dec_ctx);
        avformat_close_input(&ps);
        return;
    }

    // 打开输出文件
    AVFormatContext *out_ps = nullptr;
    error = avformat_alloc_output_context2(&out_ps, nullptr, nullptr, output_file.c_str());
    if (error < 0) {
        std::cerr << "无法创建输出的上下文" << "\n";
        avformat_close_input(&ps);
        return;
    }

    // 查找 aac 编码器
    const AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    if (!codec) {
        std::cerr << "没有找到aac 编码器" << "\n";
        avformat_close_input(&ps);
        avformat_free_context(out_ps);
        return;
    }

    // 分配编码器上下文
    AVCodecContext *codec_context = avcodec_alloc_context3(codec);
    if (!codec_context) {
        std::cerr << "无法分配编码器上下文" << "\n";
        avformat_close_input(&ps);
        avformat_free_context(out_ps);
        return;
    }
    // 设置编码器参数
    codec_context->codec_type = AVMEDIA_TYPE_AUDIO;
    codec_context->sample_rate = 44100;
    av_channel_layout_default(&codec_context->ch_layout, 2); // 设置立体声声道布局
    codec_context->sample_fmt = AV_SAMPLE_FMT_FLTP;
    codec_context->bit_rate = 192000;
    codec_context->time_base = (AVRational){1, codec_context->sample_rate};
    // 创建输出流
    AVStream *out_stream = avformat_new_stream(out_ps, nullptr);
    if (!out_stream) {
        std::cerr << "无法创建输出流" << "\n";
        avcodec_free_context(&codec_context);
        avformat_close_input(&ps);
        avformat_free_context(out_ps);
        return;
    }

    // 将编码器参数复制到输出流
    if (avcodec_parameters_from_context(out_stream->codecpar, codec_context) < 0) {
        std::cerr << "无法将编码器参数复制到输出流" << "\n";
        avcodec_free_context(&codec_context);
        avformat_close_input(&ps);
        avformat_free_context(out_ps);
        return;
    }

    // 打开编码器
    if (avcodec_open2(codec_context, codec, nullptr) < 0) {
        std::cerr << "无法打开编码器" << "\n";
        avcodec_free_context(&codec_context);
        avformat_close_input(&ps);
        avformat_free_context(out_ps);
        return;
    }

    // **在这里初始化重采样上下文**
    SwrContext *swr_ctx = nullptr;
    if (swr_alloc_set_opts2(&swr_ctx,
        &codec_context->ch_layout, codec_context->sample_fmt, codec_context->sample_rate,
        &dec_ctx->ch_layout, dec_ctx->sample_fmt, dec_ctx->sample_rate,
        0, nullptr) < 0) {
        std::cerr << "无法分配重采样上下文" << "\n";
        avcodec_free_context(&codec_context);
        avcodec_free_context(&dec_ctx); // 释放解码器上下文
        avformat_close_input(&ps);
        avformat_free_context(out_ps);
    }
    if (!swr_ctx || swr_init(swr_ctx) < 0) {
        std::cerr << "无法初始化重采样上下文" << "\n";
        avcodec_free_context(&codec_context);
        avcodec_free_context(&dec_ctx); // 释放解码器上下文
        avformat_close_input(&ps);
        avformat_free_context(out_ps);
        return;
    }

    // 将编码器参数复制到输出流
    if (avcodec_parameters_from_context(out_stream->codecpar, codec_context) < 0) {
        std::cerr << "无法将编码器参数复制到输出流" << "\n";
        avcodec_free_context(&codec_context);
        avformat_close_input(&ps);
        avformat_free_context(out_ps);
        return;
    }

    // 打开输出文件
    if (!(out_ps->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&out_ps->pb, output_file.c_str(), AVIO_FLAG_WRITE) < 0) {
            std::cerr << "无法打开输出文件" << "\n";
            avcodec_free_context(&codec_context);
            avformat_close_input(&ps);
            avformat_free_context(out_ps);
            return;
        }
    }

    // 写文件头
    if (avformat_write_header(out_ps, nullptr) < 0) {
        std::cerr << "无法写文件头" << "\n";
        avcodec_free_context(&codec_context);
        avformat_close_input(&ps);
        avformat_free_context(out_ps);
        return;
    }

    AVPacket *in_packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    AVPacket *out_packet = av_packet_alloc();

    while (av_read_frame(ps, in_packet) >= 0) {
        if (in_packet->stream_index == audio_index) {
            // 发送数据包到解码器
            error = avcodec_send_packet(dec_ctx, in_packet);
            if (error < 0) {
                std::cerr << "发送数据包到解码器失败" << "\n";
                break;
            }

            while (error >= 0) {
                // 接收解码后的帧
                error = avcodec_receive_frame(dec_ctx, frame);
                if (error == AVERROR(EAGAIN) || error == AVERROR_EOF)
                    break;
                else if (error < 0) {
                    std::cerr << "接收解码帧失败" << "\n";
                    break;
                }

                // 重采样
                AVFrame *resampled_frame = av_frame_alloc();
                resampled_frame->ch_layout = codec_context->ch_layout;
                resampled_frame->format = codec_context->sample_fmt;
                resampled_frame->sample_rate = codec_context->sample_rate;
                resampled_frame->nb_samples = av_rescale_rnd(
                    swr_get_delay(swr_ctx, dec_ctx->sample_rate) + frame->nb_samples,
                    codec_context->sample_rate, dec_ctx->sample_rate, AV_ROUND_UP);

                if (av_frame_get_buffer(resampled_frame, 0) < 0) {
                    std::cerr << "无法分配重采样帧的缓冲区" << "\n";
                    av_frame_free(&resampled_frame);
                    break;
                }

                error = swr_convert(
                    swr_ctx,
                    resampled_frame->data, resampled_frame->nb_samples,
                    (const uint8_t **)frame->data, frame->nb_samples
                );
                if (error < 0) {
                    std::cerr << "重采样失败" << "\n";
                    av_frame_free(&resampled_frame);
                    break;
                }

                // 拆分帧并发送到编码器
                int total_samples = resampled_frame->nb_samples;
                int offset = 0;
                while (total_samples > 0) {
                    int send_samples = FFMIN(codec_context->frame_size, total_samples);

                    AVFrame *enc_frame = av_frame_alloc();
                    enc_frame->nb_samples = send_samples;
                    enc_frame->ch_layout = resampled_frame->ch_layout;
                    enc_frame->format = resampled_frame->format;
                    enc_frame->sample_rate = resampled_frame->sample_rate;

                    if (av_frame_get_buffer(enc_frame, 0) < 0) {
                        std::cerr << "无法分配编码帧的缓冲区" << "\n";
                        av_frame_free(&enc_frame);
                        break;
                    }

                    // 复制数据
                    for (int i = 0; i < codec_context->ch_layout.nb_channels; i++) {
                        memcpy(enc_frame->data[i],
                               resampled_frame->data[i] + offset * av_get_bytes_per_sample(codec_context->sample_fmt),
                               send_samples * av_get_bytes_per_sample(codec_context->sample_fmt));
                    }

                    // **设置 pts**
                    enc_frame->pts = frame_index;
                    frame_index += send_samples; // 更新帧计数器

                    // 发送帧到编码器
                    error = avcodec_send_frame(codec_context, enc_frame);
                    av_frame_free(&enc_frame);
                    if (error < 0) {
                        std::cerr << "发送帧到编码器失败" << "\n";
                        break;
                    }

                    while (error >= 0) {
                        error = avcodec_receive_packet(codec_context, out_packet);
                        if (error == AVERROR(EAGAIN) || error == AVERROR_EOF)
                            break;
                        else if (error < 0) {
                            std::cerr << "接收编码包失败" << "\n";
                            break;
                        }

                        // 设置数据包的流索引和时基
                        out_packet->stream_index = out_stream->index;
                        av_packet_rescale_ts(out_packet, codec_context->time_base, out_stream->time_base);

                        // 写入数据包到输出文件
                        av_interleaved_write_frame(out_ps, out_packet);
                        av_packet_unref(out_packet);
                    }

                    total_samples -= send_samples;
                    offset += send_samples;
                }

                av_frame_free(&resampled_frame);
                av_frame_unref(frame);
            }
        }
        av_packet_unref(in_packet);
    }

    // **冲刷编码器**
    error = avcodec_send_frame(codec_context, nullptr); // 发送空帧
    if (error < 0) {
        std::cerr << "发送空帧到编码器失败：" << av_err2str(error) << "\n";
    }

    while (true) {
        error = avcodec_receive_packet(codec_context, out_packet);
        if (error == AVERROR_EOF) {
            break;
        } else if (error < 0) {
            std::cerr << "接收编码包失败：" << av_err2str(error) << "\n";
            break;
        }

        // 设置数据包的流索引和时基
        out_packet->stream_index = out_stream->index;
        av_packet_rescale_ts(out_packet, codec_context->time_base, out_stream->time_base);

        // 写入数据包到输出文件
        av_interleaved_write_frame(out_ps, out_packet);
        av_packet_unref(out_packet);
    }

    // 写文件尾
    av_write_trailer(out_ps);

    std::cout << "转换成功，文件：" << output_file << "\n";

    // 释放资源
    av_packet_free(&in_packet);
    av_packet_free(&out_packet);
    av_frame_free(&frame);
    swr_free(&swr_ctx); // 释放重采样上下文
    avcodec_free_context(&codec_context);
    avcodec_free_context(&dec_ctx); // 释放解码器上下文
    avformat_close_input(&ps);
    avformat_free_context(out_ps);
}

void ffmpeg_stu::analyze_h264_file(std::string file_path) {
    int error = 0;
    AVFormatContext *ps = nullptr;
    error = avformat_open_input(&ps, file_path.c_str(), nullptr, nullptr);
    if (error != 0) {
        std::cerr << "解析文件的头部错误" << "\n";
        avformat_close_input(&ps);
        return;
    }
    error = avformat_find_stream_info(ps, nullptr);
    if (error != 0) {
        std::cerr << "解析文件的流信息错误" << "\n";
        avformat_close_input(&ps);
        return;
    }

    // 查找视频流
    int video_index = -1;
    for (unsigned int i = 0; i < ps->nb_streams; i++) {
        if (ps->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = i;
            break;
        }
    }

    if (video_index == -1) {
        std::cerr << "没有找到视频流" << "\n";
        avformat_close_input(&ps);
        return;
    }

    AVPacket *packet = av_packet_alloc();
    if (!packet) {
        std::cerr << "无法分配数据包" << "\n";
        avformat_close_input(&ps);
        return;
    }

    std::vector<std::vector<uint8_t>> nalu_units; // 用于存放 NALU 单元的数组
    std::vector<std::vector<std::vector<uint8_t>>> gops; // 用于存放 GOP 的数组
    std::vector<std::vector<uint8_t>> current_gop; // 当前 GOP

    // 读取数据包并解析 NALU
    while (av_read_frame(ps, packet) >= 0) {
        if (packet->stream_index == video_index) {
            // 解析 NALU
            uint8_t *data = packet->data;
            int size = packet->size;

            while (size > 0) {
                // 查找 NALU 起始码
                if (size >= 4 && data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x00 && data[3] == 0x01) {
                    data += 4;
                    size -= 4;
                } else if (size >= 3 && data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x01) {
                    data += 3;
                    size -= 3;
                } else {
                    data++;
                    size--;
                    continue;
                }

                // 获取 NALU 类型
                uint8_t nalu_type = data[0] & 0x1F;
                std::cout << "NALU 类型: " << (int)nalu_type << "\n";

                // 查找下一个 NALU 起始码
                uint8_t *next_nalu = data;
                int next_size = size;
                while (next_size > 0) {
                    if (next_size >= 4 && next_nalu[0] == 0x00 && next_nalu[1] == 0x00 && next_nalu[2] == 0x00 && next_nalu[3] == 0x01) {
                        break;
                    } else if (next_size >= 3 && next_nalu[0] == 0x00 && next_nalu[1] == 0x00 && next_nalu[2] == 0x01) {
                        break;
                    }
                    next_nalu++;
                    next_size--;
                }

                int nalu_size = next_nalu - data;
                std::cout << "NALU 大小: " << nalu_size << "\n";

                // 存储 NALU 单元
                std::vector<uint8_t> nalu(data, data + nalu_size);
                nalu_units.push_back(nalu);

                // 如果是 IDR 图像片，表示一个新的 GOP 的开始
                if (nalu_type == 5) { // IDR 图像片
                    if (!current_gop.empty()) {
                        gops.push_back(current_gop); // 将当前 GOP 存储到 GOP 数组中
                        current_gop.clear(); // 清空当前 GOP
                    }
                }

                current_gop.push_back(nalu); // 将 NALU 添加到当前 GOP

                data = next_nalu;
                size = next_size;
            }
        }
        av_packet_unref(packet);
    }

    // 将最后一个 GOP 存储到 GOP 数组中
    if (!current_gop.empty()) {
        gops.push_back(current_gop);
    }

    std::cout << "总共找到 " << nalu_units.size() << " 个 NALU 单元\n";
    std::cout << "总共找到 " << gops.size() << " 个 GOP\n";

    // 输出每个 GOP 的第一个 NALU 单元（即 IDR 帧）的信息
    for (size_t i = 0; i < gops.size(); ++i) {
        const std::vector<uint8_t>& idr_nalu = gops[i][0];
        std::cout << "GOP " << i + 1 << " 的第一个 NALU 单元（IDR 帧）:\n";
        for (size_t j = 0; j < idr_nalu.size(); ++j) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)idr_nalu[j] << " ";
            if ((j + 1) % 16 == 0) {
                std::cout << "\n";
            }
        }
        std::cout << std::dec << "\n"; // 恢复为十进制输出
    }

    
    // 分析每个 GOP 中的帧
    for (size_t i = 0; i < gops.size(); ++i) {
        std::cout << "分析 GOP " << i + 1 << " 中的帧:\n";
        for (size_t j = 0; j < gops[i].size(); ++j) {
            const std::vector<uint8_t>& nalu = gops[i][j];
            if (nalu.empty()) {
                continue;
            }

            uint8_t nal_unit_type = nalu[0] & 0x1F;
            std::cout << "  帧 " << j + 1 << " 的 NALU 类型: " << (int)nal_unit_type << "\n";

            // 根据 NALU 类型进行进一步分析
            switch (nal_unit_type) {
                case 1:
                    std::cout << "    NALU 类型: 非 IDR 图像片\n";
                    break;
                case 5:
                    std::cout << "    NALU 类型: IDR 图像片\n";
                    break;
                case 7:
                    std::cout << "    NALU 类型: 序列参数集 (SPS)\n";
                    break;
                case 8:
                    std::cout << "    NALU 类型: 图像参数集 (PPS)\n";
                    break;
                default:
                    std::cout << "    NALU 类型: 其他 (" << (int)nal_unit_type << ")\n";
                    break;
            }

            // 分析片、宏块和子块
            size_t slice_count = 0;
            size_t macroblock_count = 0;
            size_t sub_macroblock_count = 0;

            for (size_t k = 0; k < nalu.size(); ++k) {
                slice_count++;
                macroblock_count++;
                sub_macroblock_count++;
            }

            std::cout << "    该帧包含 " << slice_count << " 个片\n";
            std::cout << "    该片包含 " << macroblock_count << " 个宏块\n";
            std::cout << "    该宏块包含 " << sub_macroblock_count << " 个子块\n";
        }
    }

    av_packet_free(&packet);
    avformat_close_input(&ps);
}