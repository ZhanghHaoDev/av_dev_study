#include "audio_module.h"

#include "gtest/gtest.h"

TEST(audio_module,sdl_audio_play_test){
    audio_module module;
    module.sdl_audio_play();
}