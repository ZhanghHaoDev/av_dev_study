#ifndef FFMPEG_STU_H
#define FFMPEG_STU_H

#include <string>

class ffmpeg_stu{
public:
    ffmpeg_stu();
    ~ffmpeg_stu();

    void my_av_dump_format(std::string file_path);

};

#endif // FFMPEG_STU_H