#include "h264_info.h"

#include <iostream>
#include <QFile>
#include <QApplication>
#include <QMenu>
#include <QAction>

#include "ffmpeg_stu.h"

h264_info::h264_info(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建菜单栏
    menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("File", this);
    QAction *openAction = new QAction("Open", this);
    QAction *exitAction = new QAction("Exit", this);
    fileMenu->addAction(openAction);
    fileMenu->addAction(exitAction);
    menuBar->addMenu(fileMenu);

    QMenu *editMenu = new QMenu("Edit", this);
    QAction *copyAction = new QAction("Copy", this);
    QAction *pasteAction = new QAction("Paste", this);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    menuBar->addMenu(editMenu);

    // 创建工具栏
    toolBar = new QToolBar(this);
    toolBar->addAction(openAction);
    toolBar->addAction(copyAction);
    toolBar->addAction(pasteAction);

    videoWidget = new QWidget(this);
    videoWidget->setMinimumSize(1200, 600);

    separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);

    infoWidget = new QWidget(this);
    infoWidget->setMinimumSize(1200, 200);

    // 添加按钮和文本框
    button = new QPushButton("Click Me", this);
    lineEdit = new QLineEdit(this);

    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->addWidget(button);
    infoLayout->addWidget(lineEdit);

    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(videoWidget);
    mainLayout->addWidget(separator);
    mainLayout->addWidget(infoWidget);

    setLayout(mainLayout);
}

h264_info::~h264_info() = default;