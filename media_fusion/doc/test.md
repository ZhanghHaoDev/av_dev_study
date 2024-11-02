# 测试

## 1. 介绍

测试是一个软件开发过程中不可或缺的环节，测试的目的是为了保证软件的质量，保证软件的正确性。

## 2. 测试使用的技术
+ 使用gtest进行单元测试
+ main_test.cc 是测试的入口文件，glog初始化

## 3. 测试用例

### aac文件，文件参数
+ 测试文件：test/file_info_test
* 测试文件路径 /Users/zhanghao/code/cpp/media_fusion/test/test_file/test.aac
* 文件大小：1840235字节
* 文件类型：AAC
* 音频时长：3041s
* 采样率：48000
* 声道数：2
* 比特率：4841
* 比特深度：32
* 采样值 size : 27086848
* 帧长度：2048

### wav文件，文件参数
+ 测试文件：test/file_info_test
* 测试文件路径 /Users/zhanghao/code/cpp/media_fusion/test/test_file/test_wav.wav
* 文件类型：WAV
* 文件大小：1840235字节
* 文件类型：WAV
* 音频时长：3041s
* 采样率：48000
* 声道数：2
* 比特率：4841
* 比特深度：32
* 采样值 size : 12443021
* 帧长度：2048

### mp4文件，文件参数
+ 测试文件：test/file_info_test
* 测试文件路径 /Users/zhanghao/code/cpp/media_fusion/test/test_file/test_mp4.mp4
* 文件大小：类型：MP4
* 文件大小：10194885字节
* 视频时长：282s
* 视频宽度：640
* 视频高度：360
* 帧率：24
* 比特率：4841
* 编码格式：h264
* 颜色空间：yuv420p
