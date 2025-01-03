#ifndef H264_INFO_H
#define H264_INFO_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QLineEdit>
#include <QMenuBar>
#include <QToolBar>

class h264_info : public QWidget {
    Q_OBJECT
public:
    explicit h264_info(QWidget *parent = nullptr);
    ~h264_info() override;

private:
    QWidget *videoWidget;
    QFrame *separator;
    QWidget *infoWidget;
    QPushButton *button;
    QLineEdit *lineEdit;
    QMenuBar *menuBar;
    QToolBar *toolBar;
};

#endif // H264_INFO_H