cmake_minimum_required(VERSION 2.6)

PROJECT(GMRAVis)

FIND_PACKAGE(GLUT)
FIND_PACKAGE(OpenGL)




INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} . lib  config tclap)


ADD_EXECUTABLE(GMRAVis GMRAVisCmdLine.cpp )
TARGET_LINK_LIBRARIES(GMRAVis
    gfortran
    blas
    lapack 
    ${OPENGL_LIBRARIES}  
    ${GLUT_LIBRARIES}
)


