#include "core_module.h"

#include "gtest/gtest.h"

TEST(core_model,get_sdl_version_test){
    core_model core;
    testing::internal::CaptureStdout();
    core.get_sdl_version();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("SDL 的编译时版本为："), std::string::npos);
    EXPECT_NE(output.find("SDL 的运行时版本为："), std::string::npos);
}

TEST(core_model, sdl_log_test) {
    core_model core;
    testing::internal::CaptureStdout();
    core.sdl_log();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(!output.empty());  // 检查是否有输出
}

TEST(core_model, sdl_error_test) {
    core_model core;
    testing::internal::CaptureStdout();
    core.sdl_error();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(!output.empty());
}

TEST(core_model, sdl_platform_test) {
    core_model core;
    testing::internal::CaptureStdout();
    core.sdl_platform();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(!output.empty());
}

TEST(core_model,sdl_memory_test){
    core_model core;
    testing::internal::CaptureStdout();
    core.sdl_memory();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(output.empty());
}

TEST(core_model,sdl_atomic_test){
    core_model core;
    testing::internal::CaptureStdout();
    core.sdl_atomic();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(output.empty());
}