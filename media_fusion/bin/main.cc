#include <QApplication>
#include "glog/logging.h"

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    google::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_minloglevel = 0; 
    FLAGS_logtostderr = 1; 

    google::ShutdownGoogleLogging();
    return 0;
}