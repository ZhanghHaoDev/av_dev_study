find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_library(opengl_stu STATIC
    opengl_stu.cpp
)

target_link_libraries(opengl_stu PUBLIC 
    OpenGL::GL 
    GLUT::GLUT
)
