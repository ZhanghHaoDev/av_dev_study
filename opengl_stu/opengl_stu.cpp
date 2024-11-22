#include "opengl_stu.h"

#include <iostream>

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

opengl_stu::opengl_stu(){
     // 初始化 GLUT
    int argc = 1;
    char *argv[1] = {(char*)"Something"};
    glutInit(&argc, argv);

    // 创建一个隐藏的窗口来初始化 OpenGL 上下文
    glutCreateWindow("Hidden Window");

    // 获取并输出 OpenGL 版本信息
    const GLubyte* version = glGetString(GL_VERSION);
    if (version) {
        std::cout << "OpenGL Version: " << version << "\n";
    } else {
        std::cout << "Failed to get OpenGL version" << "\n";
    }

    // 销毁隐藏的窗口
    glutDestroyWindow(glutGetWindow());
}

opengl_stu::~opengl_stu() = default;