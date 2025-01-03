#include <QApplication>
#include "h264_info.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    h264_info info;
    info.show();

    return app.exec();
}