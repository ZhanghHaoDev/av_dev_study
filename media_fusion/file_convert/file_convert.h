#ifndef FILE_CONVERT_H
#define FILE_CONVERT_H

#include <iostream>
#include <string>

class file_convert{
public:
    file_convert();
    ~file_convert();

    /**
     * @brief 检测文件是否存在
     * 
     * @param input_file 文件路径
     * @return int 错误码
     */
    int file_open(std::string input_file);

    /**
     * @brief 视频提取音频
     * 
     * @param output_audio_path 音频路径
     * @return int 
     */
    int video_to_audio(std::string& output_audio_path);

    /**
     * @brief 音频转换
     * 
     * @param output_audio_path  输出音频路径
     * @return int 错误吗
     */
    int audio_convert(std::string& output_audio_path);

private:
    std::string file_path; // 输入的文件路径
    bool has_audio_stream; // 是否有音频流
    bool has_video_stream; // 是否有视频流
};

#endif // FILE_CONVERT_H