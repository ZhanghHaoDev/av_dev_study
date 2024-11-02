#include "gtest/gtest.h"
#include "glog/logging.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO); 
    google::InstallFailureSignalHandler();  

    int result = RUN_ALL_TESTS();

    google::ShutdownGoogleLogging();
    return result;
}