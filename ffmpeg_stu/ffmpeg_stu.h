#ifndef FFMPEG_STU_H
#define FFMPEG_STU_H

#include <string>
#include <map>

class ffmpeg_stu{
public:
    ffmpeg_stu();
    ~ffmpeg_stu();

    void my_av_dump_format(std::string file_path);

    void av_info_map(const std::string& file_path, std::map<std::string, std::string>& info_map);

    void list_av_codecs();

    void analyze_h264_file(std::string file_path);

    // 输出aac的一些参数
    void aac_file(std::string file_path);

    // pcm转换为aac
    void wav_to_aac(std::string input_file,std::string output_file);

};

#endif // FFMPEG_STU_H