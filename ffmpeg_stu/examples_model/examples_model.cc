#include "examples_model.h"

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <sys/stat.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <inttypes.h>
#include <stdio.h>

#include "glog/logging.h"
#include <system_error>

extern "C"{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavformat/avio.h"
    #include "libavutil/file.h"
    #include "libavutil/mem.h"
    #include "libavutil/opt.h"
    #include "libavutil/channel_layout.h"
    #include "libavutil/samplefmt.h"
    #include "libavutil/timestamp.h"
    #include "libswresample/swresample.h"
}

examples_model::examples_model(){
    google::InitGoogleLogging("examples_model");
    FLAGS_logtostderr = 1;
    FLAGS_alsologtostderr = 1;
    google::SetStderrLogging(google::INFO);
    google::InstallFailureSignalHandler();
}

examples_model::~examples_model(){
    google::ShutdownGoogleLogging();
}

int examples_model::avio_list_dir(std::string file_path){
    int ret,cnt;
    av_log_set_level(AV_LOG_DEBUG);
    if (file_path.empty()){
        std::cerr << "文件路径为空" << std::endl;
        return 1;
    }

    avformat_network_init();

    AVIODirEntry *entry = NULL;
    AVIODirContext *ctx = NULL;
    char filemode[4], uid_and_gid[20];

    if ((ret = avio_open_dir(&ctx, file_path.c_str(), NULL)) < 0) {
        av_log(NULL,AV_LOG_ERROR,"无法打开目录",av_err2str(ret));
        avformat_network_deinit();
        avio_close_dir(&ctx);
        return ret;
    }

    for (;;) {
        if ((ret = avio_read_dir(ctx, &entry)) < 0) {
            av_log(NULL,AV_LOG_ERROR,"无法列出目录",av_err2str(ret));
            avformat_network_deinit();
            avio_close_dir(&ctx);
            return ret;
        }
        if (!entry)
            break;
        if (entry->filemode == -1) {
            snprintf(filemode, 4, "???");
        } else {
            snprintf(filemode, 4, "%3" PRIo64, entry->filemode);
        }
        snprintf(uid_and_gid, 20, "%" PRId64"(%" PRId64")", entry->user_id, entry->group_id);
        if (cnt == 0)
            av_log(NULL, AV_LOG_INFO, "%-9s %12s %30s %10s %s %16s %16s %16s\n",
                   "TYPE", "SIZE", "NAME", "UID(GID)", "UGO", "MODIFIED",
                   "ACCESSED", "STATUS_CHANGED");
        av_log(NULL, AV_LOG_INFO, "%-9s %12" PRId64" %30s %10s %s %16" PRId64" %16" PRId64" %16" PRId64"\n",
               this->type_string(entry->type),
               entry->size,
               entry->name,
               uid_and_gid,
               filemode,
               entry->modification_timestamp,
               entry->access_timestamp,
               entry->status_change_timestamp);
        avio_free_directory_entry(&entry);
        cnt++;
    };
    avformat_network_deinit();
    return ret;
}

const char* examples_model::type_string(int type){
    switch (type) {
        case AVIO_ENTRY_DIRECTORY:
            return "<DIR>";
        case AVIO_ENTRY_FILE:
            return "<FILE>";
        case AVIO_ENTRY_BLOCK_DEVICE:
            return "<BLOCK DEVICE>";
        case AVIO_ENTRY_CHARACTER_DEVICE:
            return "<CHARACTER DEVICE>";
        case AVIO_ENTRY_NAMED_PIPE:
            return "<PIPE>";
        case AVIO_ENTRY_SYMBOLIC_LINK:
            return "<LINK>";
        case AVIO_ENTRY_SOCKET:
            return "<SOCKET>";
        case AVIO_ENTRY_SERVER:
            return "<SERVER>";
        case AVIO_ENTRY_SHARE:
            return "<SHARE>";
        case AVIO_ENTRY_WORKGROUP:
            return "<WORKGROUP>";
        case AVIO_ENTRY_UNKNOWN:
        default:
            break;
        }
        return "<UNKNOWN>";
}

int examples_model::avio_read_callback(std::string file_path){
    int error_code = 0;
    
    if(file_path.empty()){
        std::cerr << "输入参数为空" << std::endl;
        return -1;
    }
    
    AVFormatContext *av_fc = nullptr;
    uint8_t *buffer = NULL;
    size_t buffer_size = 4096; size_t avio_ctx_buffer_size = 4096;
    
    error_code = av_file_map(file_path.c_str(),&buffer,&buffer_size,0,NULL);
    if (error_code !=0 ){
        std::cerr << "打开文件错误" << std::endl;
        return error_code;
    }
    struct buffer_data bd = {0};
    bd.ptr = buffer;
    bd.size = buffer_size;
    if (!(av_fc = avformat_alloc_context())){
        error_code = AVERROR(ENOMEM);
        avformat_close_input(&av_fc);
        return error_code;
    }
    uint8_t *avio_ctx_buffer =(uint8_t *) av_malloc(avio_ctx_buffer_size);
    if(!avio_ctx_buffer){
        std::cerr << "av_malloc 错误" << std::endl;
        error_code = AVERROR(ENOMEM);
        avformat_close_input(&av_fc);
        av_free(&avio_ctx_buffer);
    }
    AVIOContext *avio_ctx = avio_alloc_context(avio_ctx_buffer,avio_ctx_buffer_size,0,&bd,&read_packet,NULL,NULL);
    if (!avio_ctx){
        std::cerr << "avio_alloc_contect 函数错误" << std::endl;
        return 0;
    }
    av_fc->pb = avio_ctx;
    error_code = avformat_open_input(&av_fc,NULL,NULL,NULL);
    if(error_code < 0){
        std::cerr << "avformat_open_input 错误" << std::endl;
        return error_code;
    }
    error_code = avformat_find_stream_info(av_fc,NULL);
    if(error_code < 0){
        std::cerr << "avformat_find_stream_info 错误" << std::endl;
        return error_code;
    }
    av_dump_format(av_fc,0,file_path.c_str(),0);
    
    avformat_close_input(&av_fc);
    av_free(avio_ctx);
    avio_context_free(&avio_ctx);
    av_file_unmap(buffer,buffer_size);
    
    return error_code;
}

int examples_model::read_packet(void *opaque,uint8_t *buf,int buf_size){
    struct buffer_data *bd = (struct buffer_data*)opaque;
    buf_size = FFMIN(buf_size,bd->size);
    if(!buf_size){
        return AVERROR_EOF;
    }
    std::cout << "ptr: " << bd->ptr << "size: " << bd->size << std::endl;

    memcpy(buf,bd->ptr,buf_size);
    bd->ptr += buf_size;
    bd->size -= buf_size;
    
    return buf_size;
}


int examples_model::list_dir(std::string dir){
    int error_code = 0;
    if(dir.empty() != 0){
        LOG(ERROR) << "输入路径为空";
        return error_code = -1;
    }
    std::cout << std::left << std::setw(9) << "文件类型" << std::setw(12) << "文件大小"
              << std::setw(30) << "文件名称" << std::setw(10) << "用户和组"
              << std::setw(10) << "文件模式" << std::setw(20) << "修改时间"
              << std::setw(20) << "访问时间"<< std::setw(20) << "状态更改时间"
              << std::endl;

    for (const auto entry : std::filesystem::directory_iterator(dir)){
        std::string type = entry.is_directory() ? "<DIR>" : "<FILE>";
        auto size = entry.is_regular_file() ? entry.file_size() : 0;
        auto name = entry.path().filename().string();
        struct stat file_stat;
        if (stat(entry.path().c_str(),&file_stat) != 0){
            perror("stat");
            continue;
        }
        struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group *gr = getgrgid(file_stat.st_gid);
        std::string uid_and_gid = pw ? pw->pw_name : "N/A";
        uid_and_gid += "(";
        uid_and_gid += gr ? gr->gr_name : "N/A";
        uid_and_gid += ")";

        std::string filemode = std::to_string(file_stat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
        auto mod_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(file_stat.st_mtime));
        std::string mod_time_str = std::asctime(std::localtime(&mod_time));
        mod_time_str.pop_back(); 
        auto acc_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(file_stat.st_atime));
        std::string acc_time_str = std::asctime(std::localtime(&acc_time));
        acc_time_str.pop_back(); 
        auto status_change_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(file_stat.st_ctime));
        std::string status_change_time_str = std::asctime(std::localtime(&status_change_time));
        status_change_time_str.pop_back(); 
        
        std::cout << std::left << std::setw(9) << type << std::setw(12) << size
                  << std::setw(30) << name << std::setw(10) << uid_and_gid
                  << std::setw(10) << filemode << std::setw(20) << mod_time_str
                  << std::setw(20) << acc_time_str << std::setw(20) << status_change_time_str
                  << std::endl;
    }

    return error_code;
}

int examples_model::print_info(std::string file_path){
    int error_code = 0;
    AVFormatContext *fmt_ctx = nullptr;
    if ((error_code =avformat_open_input(&fmt_ctx, file_path.c_str(), nullptr, nullptr)) < 0){
        LOG(ERROR) << "获取文件头部信息错误" << file_path;
        return error_code;
    }

    if ((error_code = avformat_find_stream_info(fmt_ctx, nullptr)) < 0){
        LOG(ERROR) << "获取文件流信息错误" << file_path;
        return error_code;
    }

    LOG(INFO) << "文件名称：" << file_path;
    LOG(INFO) << "文件格式信息：" << fmt_ctx->iformat->name;

    AVDictionaryEntry *tag = nullptr;
    while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        LOG(INFO) << "元数据: " << tag->key << " = " << tag->value << std::endl;
    }
    LOG(INFO) << "文件时长：" << fmt_ctx->duration / AV_TIME_BASE << " 秒";
    LOG(INFO) << "文件比特率：" << fmt_ctx->bit_rate << " bps";

    for(int i = 0; i < fmt_ctx->nb_streams; i++){
        AVStream *stream = fmt_ctx->streams[i];
        AVCodecParameters *codecper = stream->codecpar;

        std::string codec_type;
        if (codecper->codec_type == AVMEDIA_TYPE_VIDEO){
            codec_type = "Video";
        }else if (codecper->codec_type == AVMEDIA_TYPE_AUDIO){
            codec_type = "Audio";
        } else {
            codec_type = "Other";
        }
        LOG(INFO) << "stream " << i << " type : " << codec_type;

        const AVCodec *codec = avcodec_find_decoder(codecper->codec_id);
        if (codec){
            LOG(INFO) << "codec " << codec->name << "\t" << codec->long_name;
        }

        if (codecper->codec_type == AVMEDIA_TYPE_VIDEO){
            LOG(INFO) << "分辨率: " << codecper->width << "x " << codecper->height;
            LOG(INFO) << "通道数: " << codecper->ch_layout.nb_channels;
        }

        if(codecper->codec_type == AVMEDIA_TYPE_AUDIO){
            LOG(INFO) << "采样率: " << codecper->sample_rate << "Hz";
            LOG(INFO) << "通道数: " << codecper->ch_layout.nb_channels;
        }
    }
    avformat_close_input(&fmt_ctx);

    return error_code;  
}

template<typename T> 
std::vector<T> examples_model::get_audio_sample_rate(std::string input_file){
    AVFormatContext *fmt_ctx = nullptr;
    AVCodecContext *codec_ctx = nullptr;
    const AVCodec *codec = nullptr;
    AVPacket *pkt = nullptr;
    AVFrame *frame = nullptr;
    std::vector<T> all_samples;
    int ret;

    if ((ret = avformat_open_input(&fmt_ctx, input_file.c_str(), nullptr, nullptr)) < 0) {
        LOG(ERROR) << "Could not open input file " << input_file;
        return all_samples;
    }

    if ((ret = avformat_find_stream_info(fmt_ctx, nullptr)) < 0) {
        LOG(ERROR) << "Could not find stream information";
        avformat_close_input(&fmt_ctx);
        return all_samples;
    }

    AVStream *audio_stream = nullptr;
    int audio_stream_index = -1;
    for (int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            audio_stream = fmt_ctx->streams[i];
            break;
        }
    }

    if (audio_stream_index == -1) {
        LOG(ERROR) << "Could not find audio stream";
        avformat_close_input(&fmt_ctx);
        return all_samples;
    }

    // 查找解码器
    codec = avcodec_find_decoder(audio_stream->codecpar->codec_id);
    if (!codec) {
        LOG(ERROR) << "Could not find decoder" << std::endl;
        avformat_close_input(&fmt_ctx);
        return all_samples;
    }

    // 打开解码器
    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        LOG(ERROR) << "Could not allocate codec context";
        avformat_close_input(&fmt_ctx);
        return all_samples;
    }

    if (avcodec_parameters_to_context(codec_ctx, audio_stream->codecpar) < 0) {
        LOG(ERROR) << "Could not copy codec parameters to context";
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return all_samples;
    }

    if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
        LOG(ERROR) << "Could not open codec";
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return all_samples;
    }

    pkt = av_packet_alloc();
    if (!pkt) {
        LOG(ERROR) <<"Could not allocate packet";
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return all_samples;
    }

    frame = av_frame_alloc();
    if (!frame) {
        LOG(ERROR) << "Could not allocate frame";
        av_packet_free(&pkt);
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return all_samples;
    }

    while (av_read_frame(fmt_ctx, pkt) >= 0) {
        if (pkt->stream_index == audio_stream_index) {
            if (avcodec_send_packet(codec_ctx, pkt) < 0) {
                LOG(ERROR) << "Error sending packet for decoding"; 
                break;
            }

            while (avcodec_receive_frame(codec_ctx, frame) >= 0) {
                // 处理解码后的音频帧
                int nb_samples = frame->nb_samples;
                int nb_channels = frame->ch_layout.nb_channels;
                std::vector<T> samples(nb_samples * nb_channels);
                if (frame->format == AV_SAMPLE_FMT_FLTP) {
                    for (int ch = 0; ch < nb_channels; ch++) {
                        float *data = (float *)frame->data[ch];
                        for (int i = 0; i < nb_samples; i++) {
                            samples[i * nb_channels + ch] = static_cast<T>(data[i]);
                        }
                    }
                } else if (frame->format == AV_SAMPLE_FMT_S16) {
                    int16_t *data = (int16_t *)frame->data[0];
                    for (int i = 0; i < nb_samples * nb_channels; i++) {
                        samples[i] = static_cast<T>(data[i]) / 32768.0f;
                    }
                } else {
                    LOG(ERROR) << "Unsupported sample format";
                    break;
                }
                all_samples.insert(all_samples.end(), samples.begin(), samples.end());
            }
        }
        av_packet_unref(pkt);
    }

    av_frame_free(&frame);
    av_packet_free(&pkt);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);

    return all_samples;
}

int examples_model::audio_encode_pcm(std::string audio_file,std::string pcm_file){
    int error_code = 0;
    if(audio_file.empty() != 0 || pcm_file.empty() != 0){
        LOG(ERROR) << "文件为空";
        return error_code = -1;
    }

    AVFormatContext *fmt_ctx = nullptr;
    if ((error_code = avformat_open_input(&fmt_ctx,audio_file.c_str(),nullptr,nullptr)) < 0){
        LOG(ERROR) << "打开文件头部错误";
        return error_code;
    }

    if ((error_code = avformat_find_stream_info(fmt_ctx, nullptr)) < 0){
        LOG(ERROR) << "获取文件信息错误";
        return error_code;
    }

    const AVCodec *codec = nullptr;
    AVCodecContext *codec_ctx = nullptr;
    int audio_stream_index = -1;
    for (unsigned int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            codec = avcodec_find_decoder(fmt_ctx->streams[i]->codecpar->codec_id);
            if (!codec) {
                LOG(ERROR) << "找不到解码器";
                avformat_close_input(&fmt_ctx);
                return error_code = -1;
            }
            codec_ctx = avcodec_alloc_context3(codec);
            if (!codec_ctx) {
                LOG(ERROR) << "无法分配解码器上下文";
                avformat_close_input(&fmt_ctx);
                return error_code = -1;
            }
            if ((error_code = avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[i]->codecpar)) < 0) {
                LOG(ERROR) << "无法复制解码器参数到解码器上下文";
                avcodec_free_context(&codec_ctx);
                avformat_close_input(&fmt_ctx);
                return error_code;
            }
            if ((error_code = avcodec_open2(codec_ctx, codec, nullptr)) < 0) {
                LOG(ERROR) << "无法打开解码器";
                avcodec_free_context(&codec_ctx);
                avformat_close_input(&fmt_ctx);
                return error_code;
            }
            break;
        }
    }

    if (audio_stream_index == -1) {
        LOG(ERROR) << "找不到音频流";
        avformat_close_input(&fmt_ctx);
        return error_code = -1;
    }

    std::ofstream pcm_out(pcm_file, std::ios::binary);
    if (!pcm_out.is_open()) {
        LOG(ERROR) << "无法打开输出文件";
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return error_code = -1;
    }
    SwrContext *swr_ctx = nullptr;
    AVChannelLayout out_ch_layout = codec_ctx->ch_layout;
    AVChannelLayout in_ch_layout = codec_ctx->ch_layout;
    if ((error_code = swr_alloc_set_opts2(
            &swr_ctx,
            &out_ch_layout,       // 输出通道布局
            AV_SAMPLE_FMT_FLTP,    // 输出采样格式
            codec_ctx->sample_rate, // 输出采样率
            &in_ch_layout,        // 输入通道布局
            AV_SAMPLE_FMT_FLTP,   // 输入采样格式
            codec_ctx->sample_rate, // 输入采样率
            0,                    // 日志偏移量
            nullptr               // 日志上下文
        )) < 0) {
        LOG(ERROR) << "无法初始化重采样上下文";
        return error_code;
    }

    if ((error_code = swr_init(swr_ctx)) > 0){
        LOG(INFO) << "swr_init 初始化错误";
        return error_code;
    }

    AVPacket *out_pack = av_packet_alloc();
    AVFrame *frname = av_frame_alloc();
    AVFrame *swr_frname = av_frame_alloc();
    swr_frname->format = AV_SAMPLE_FMT_S16;
    swr_frname->ch_layout.nb_channels = codec_ctx->ch_layout.nb_channels;
    swr_frname->sample_rate = codec_ctx->sample_rate;
    
    

    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);
    pcm_out.close();

    return error_code;
}

int examples_model::parse_mp4_file(std::string file_path){
    int error_code = 0;
    if (file_path.empty() != 0){
        LOG(ERROR) << "输入文件为空";
        return error_code = -1;
    }

    AVFormatContext *mp4_file = avformat_alloc_context();
    if((error_code = avformat_open_input(&mp4_file,file_path.c_str(),nullptr,nullptr)) < 0){
        LOG(ERROR) << "文件头部打开错误";
        return error_code;
    }
    if((error_code = avformat_find_stream_info(mp4_file,nullptr)) < 0){
        LOG(ERROR) << "获取文件流信息错误";
        return error_code;
    }
    
    int vidoe_index = -1;
    for(int i = 0; i < mp4_file->nb_streams; i++){
        if(mp4_file->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            vidoe_index = i;
            break;
        }
        if (mp4_file->streams[i]->codecpar->codec_id != AV_CODEC_ID_MPEG4){
            LOG(ERROR) << "不是mp4的视频";
            return error_code = -1;
        }
    }

    print_info(file_path);

    avformat_close_input(&mp4_file);
    return error_code;
}

int examples_model::decode_audio(std::string file_path){
    int error_code = 0;

    return error_code;
}

int examples_model::decode_video(std::string file_path){
    int error_code = 0;

    return error_code;
}

int examples_model::encode_audio(std::string input_file, std::string output_file){
    int error_code = 0;

    return error_code;
}

int examples_model::encode_video(std::string input_file, std::string output_file){
    int error_code = 0;

    return error_code;
}

int examples_model::video_extract_audio(std::string input_file,std::string output_file){
    int error_code = 0;
    AVFormatContext *fmt_ctx = nullptr;

    // 1. 读取文件头部信息
    if ((error_code = avformat_open_input(&fmt_ctx,input_file.c_str(),nullptr,nullptr)) != 0){
        std::cerr << "读取其头部信息,错误码：" << error_code << std::endl;
        return error_code;
    }

    // 2. 读取文件流信息
    if ((error_code = avformat_find_stream_info(fmt_ctx,nullptr) != 0)){
        std::cerr << "获取流信息错误,错误码：" << error_code << std::endl;
        return error_code;
    }

    // 3. 查找音频流
    AVStream *audio_stream = nullptr;
    int audio_stream_index = -1;
    for (int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            audio_stream = fmt_ctx->streams[i];
            break;
        }
    }

    if (audio_stream_index < 0){
        std::cerr << "文件不包含音频流" << std::endl;
        avformat_close_input(&fmt_ctx);
        return -1;
    }

    // 4. 查找解码器
    const AVCodec *codec = avcodec_find_decoder(audio_stream->codecpar->codec_id);
    if (!codec){
        std::cerr << "没有查找到解码器" << std::endl;
        avformat_close_input(&fmt_ctx);
        return -1;
    }

    // 5. 初始化解码器上下文
    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        std::cerr << "无法分配解码器上下文" << std::endl;
        avformat_close_input(&fmt_ctx);
        return -1;
    }

    if ((error_code = avcodec_parameters_to_context(codec_ctx, audio_stream->codecpar)) < 0) {
        std::cerr << "无法复制解码器参数到上下文, 错误码：" << error_code << std::endl;
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return error_code;
    }

    // 6. 打开解码器
    if ((error_code = avcodec_open2(codec_ctx, codec, nullptr)) < 0) {
        std::cerr << "无法打开解码器, 错误码：" << error_code << std::endl;
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return error_code;
    }

     // 7. 打开输出文件
    std::ofstream output(output_file, std::ios::binary);
    if (!output.is_open()) {
        std::cerr << "无法打开输出文件: " << output_file << std::endl;
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return -1;
    }

    // 8. 分配 AVPacket 和 AVFrame
    AVPacket *pkt = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    if (!pkt || !frame) {
        std::cerr << "无法分配 AVPacket 或 AVFrame" << std::endl;
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return -1;
    }

    // 9. 读取并解码音频帧
    while (av_read_frame(fmt_ctx, pkt) >= 0) {
       

        
        

        av_packet_unref(pkt);
    }

    // 10. 释放资源
    av_packet_free(&pkt);
    av_frame_free(&frame);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);
    output.close();

    return error_code;
}

int examples_model::my_av_dump_format(std::string file_path){
    int err = 0;
    av_log_set_level(AV_LOG_INFO);
    if (file_path.empty()){
        av_log(0,AV_LOG_ERROR, "文件为空");
        return -1;
    }

    AVFormatContext *ps = nullptr;
    if ((err = avformat_open_input(&ps, file_path.c_str(), nullptr, nullptr)) != 0){
        av_log(nullptr,AV_LOG_ERROR,"读取文件头部信息错误");
        return err;
    }

    if((err = avformat_find_stream_info(ps,nullptr)) != 0){
        av_log(nullptr,AV_LOG_ERROR,"获取文件流信息错误");
        return err;
    }

    av_dump_format(ps, 0, file_path.c_str(), 0);

    avformat_close_input(&ps);
    return err;
}

// decode 函数的作用是将压缩的音频数据包解码为原始音频数据，并将解码后的数据写入输出文件
void decode(AVCodecContext *dec_ctx, AVPacket *pkt, AVFrame *frame,FILE *outfile){
    int i, ch,ret, data_size;
    char errbuf[AV_ERROR_MAX_STRING_SIZE];
    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        av_strerror(ret, errbuf, sizeof(errbuf));
        std::cerr << "Error submitting the packet to the decoder error code is " << ret
        << " 错误码具体含义 " << errbuf << std::endl;
        return;
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            std::cerr << "Error during decoding" << std::endl;
            return;
        }
        data_size = av_get_bytes_per_sample(dec_ctx->sample_fmt);
        if (data_size < 0) {
            std::cerr << "Failed to calculate data size" << std::endl;
            return;
        }
        for (i = 0; i < frame->nb_samples; i++)
            for (ch = 0; ch < dec_ctx->ch_layout.nb_channels; ch++)
                fwrite(frame->data[ch] + data_size*i, 1, data_size, outfile);
    }
}

// get_format_from_sample_fmt 函数的作用是根据给定的音频样本格式（AVSampleFormat），
// 返回对应的字符串格式（例如 "s16le"、"s32be" 等）。这个函数在音频处理和转换过程中非常有用，
// 因为它可以将内部使用的样本格式转换为人类可读的字符串格式，便于输出和调试。
int get_format_from_sample_fmt(const char **fmt,enum AVSampleFormat sample_fmt){
    int i;
    struct sample_fmt_entry {
        enum AVSampleFormat sample_fmt; const char *fmt_be, *fmt_le;
    } sample_fmt_entries[] = {
        { AV_SAMPLE_FMT_U8,  "u8",    "u8"    },
        { AV_SAMPLE_FMT_S16, "s16be", "s16le" },
        { AV_SAMPLE_FMT_S32, "s32be", "s32le" },
        { AV_SAMPLE_FMT_FLT, "f32be", "f32le" },
        { AV_SAMPLE_FMT_DBL, "f64be", "f64le" },
    };
    *fmt = NULL;
 
    for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++) {
        struct sample_fmt_entry *entry = &sample_fmt_entries[i];
        if (sample_fmt == entry->sample_fmt) {
            *fmt = AV_NE(entry->fmt_be, entry->fmt_le);
            return 0;
        }
    }
 
    fprintf(stderr,
            "sample format %s is not supported as output format\n",
            av_get_sample_fmt_name(sample_fmt));
    return -1;
}

int examples_model::decode_audio_mp2_pcm(std::string file_path,std::string output_file){
    int err = 0;

    if(file_path.empty()){
        av_log(0,AV_LOG_ERROR,"文件为空");
        return -1;
    }

    // 1. 查找编码器
    const AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_MP2);
    if (!codec){
        av_log(0,AV_LOG_ERROR,"没有找到mp2的解码器");
        return -1;
    }

    // 2. 初始化编码器
    AVCodecParserContext *parser = av_parser_init(codec->id);
    if (!parser){
        av_log(0,AV_LOG_ERROR,"初始化编码器错误");
        return -1;
    }

    // 3. 初始化解析器上下文
    AVCodecContext *c = avcodec_alloc_context3(codec);
    if (!c){
        av_log(0,AV_LOG_ERROR,"初始化解析器上下文错误");
        return -1;
    }

    // 4. 初始化编码器上下文
    if(avcodec_open2(c,codec,nullptr) < 0){
        av_log(0,AV_LOG_ERROR,"初始化编码器上下文错误");
        return -1;
    }
    
    //  5. 打开输入文件
    FILE *f = fopen(file_path.c_str(),"rb");
    if (!f){
        av_log(0,AV_LOG_ERROR,"打开输入文件错误");
        return -1;
    }
    // 打开输出文件
    FILE *output = fopen(output_file.c_str(), "wb");
    if (!output){
        av_log(0,AV_LOG_ERROR,"打开输出文件错误");
        return -1;
    }

    const int AUDIO_INBUF_SIZE = 20480;
    uint8_t inbuf[AUDIO_INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t *data      = inbuf;
    size_t data_size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f);

    AVFrame *decoded_frame = nullptr;
    int ret,len;
    AVPacket *pkt = av_packet_alloc();
    const int AUDIO_REFILL_THRESH = 4096;
    enum AVSampleFormat sfmt;
    int n_channels = 0;
    const char *fmt;
    while(data_size > 0){
        if(!decoded_frame){
            if(!(decoded_frame = av_frame_alloc())){
                av_log(0,AV_LOG_ERROR,"申请avframe内存错误");
                return -1;
            }
        }

        // 6. 读取帧数据
        ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,
                               data, data_size,
                               AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        if (ret < 0) {
            av_log(0,AV_LOG_ERROR,"Error while parsing");
            return -1;
        }
        data += ret;
        data_size -= ret;
        if (pkt->size)
            decode(c, pkt, decoded_frame, output);

        if (data_size < AUDIO_REFILL_THRESH) {
            memmove(inbuf, data, data_size);
            data = inbuf;
            len = fread(data + data_size, 1,
                        AUDIO_INBUF_SIZE - data_size, f);
            if (len > 0)
                data_size += len;
        }
    }

    pkt->data = NULL;
    pkt->size = 0;
    decode(c, pkt, decoded_frame, output);
    sfmt = c->sample_fmt;

    if (av_sample_fmt_is_planar(sfmt)) {
        const char *packed = av_get_sample_fmt_name(sfmt);
        printf("Warning: the sample format the decoder produced is planar "
            "(%s). This example will output the first channel only.\n",
            packed ? packed : "?");
        sfmt = av_get_packed_sample_fmt(sfmt);
    }

    n_channels = c->ch_layout.nb_channels;
    if ((ret = get_format_from_sample_fmt(&fmt, sfmt)) < 0)
        return 0;

    std::cout << "Play the output audio file with the command:\n"
              << "ffplay -f %s -ac %d -ar %d %s\n" << fmt << n_channels << c->sample_rate << output
              << std::endl;

    fclose(output);
    fclose(f);
 
    avcodec_free_context(&c);
    av_parser_close(parser);
    av_frame_free(&decoded_frame);
    av_packet_free(&pkt);
    return err;
}