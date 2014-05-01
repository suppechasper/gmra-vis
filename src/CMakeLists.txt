cmake_minimum_required(VERSION 2.6)

PROJECT(GMRAVis)

FIND_PACKAGE(GLUT)
FIND_PACKAGE(OpenGL)

INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} . lib  config tclap)

ADD_EXECUTABLE(GMRAVis GMRAVisCmdLine.cpp )
TARGET_LINK_LIBRARIES(GMRAVis
   # gfortran
    blas
    lapack 
    ${OPENGL_LIBRARIES}  
    ${GLUT_LIBRARIES}
)

ADD_EXECUTABLE(CreateIPCATree CreateIPCATree.cxx )
TARGET_LINK_LIBRARIES(CreateIPCATree
   # gfortran
    blas
    lapack 
    ${OPENGL_LIBRARIES}  
    ${GLUT_LIBRARIES}
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(CUSTOM_CFLAGS ${CUSTOM_CFLAGS}
 #  -Wall 
 #)

IF(APPLE) # Clang / Mac OS only
  # Required on OSX to compile c++11
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -stdlib=libc++ -mmacosx-version-min=10.7")
ENDIF(APPLE)
