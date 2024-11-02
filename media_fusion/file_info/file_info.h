#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <iostream>
#include <string>

class file_info {
struct audio_file_info{
        std::string file_path; // 文件路径
        int file_size; // 文件大小, 单位：字节
        std::string file_type; // 文件类型
        int duration;   // 音频时长 单位：秒
        int sample_rate;   // 采样率
        uint64_t channels;      // 声道数
        int bit_rate;      // 比特率
        int bit_depth;  // 比特深度
        int frame_length;   // 帧长度
    };

struct video_file_info {
        std::string file_path; // 文件路径
        std::string file_type; // 文件类型
        int file_size;         // 文件大小, 单位：字节
        int duration;          // 视频时长 单位：秒
        int width;             // 视频宽度
        int height;            // 视频高度
        int frame_rate;        // 帧率
        int bit_rate;          // 比特率
        std::string codec;     // 编码格式
    };

public:
    file_info();
    ~file_info();

    /**
     * @brief 打开文件
     * 
     * @param file_name 文件名
     * @return int 错误码
     */
    int file_open(const std::string &file_name);

    /**
     * @brief 获取文件参数
     * 
     * @return audio_file_info 文件参数
     */
    audio_file_info get_audio_file_info();

    /**
     * @brief 获取音频文件的采样值
     * 
     * @tparam T 
     * @return std::vector<T> 采样值
     */
    template <typename T>
    std::vector<T> get_sample_values();

    /**
     * @brief 获取视频文件参数
     * 
     * @return video_file_info 文件参数
     */
    video_file_info get_video_file_info();

private:
    std::string file_path;  // 文件路径
    bool has_video_stream;  // 是否有视频流
    bool has_audio_stream;  // 是否有音频流
    audio_file_info audio_info; // 音频文件信息
    video_file_info video_info; // 视频文件信息
};

#endif // FILE_INFO_H