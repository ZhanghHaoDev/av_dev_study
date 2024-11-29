add_library(opengl_stu OBJECT
    opengl_stu.cpp
)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(opengl_stu PUBLIC 
    OpenGL::GL 
    GLUT::GLUT
)
