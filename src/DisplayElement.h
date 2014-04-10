#ifndef DISPLAYELEMENT_H
#define DISPLAYELEMENT_H

//include opengl stuff
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <glut.h>
//#include <FTGL/FTGL.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//#include <FTGL/ftgl.h>
#endif


#include <string>

class  DisplayElement{

 protected:
  float backgroundColor[3];
  
 public:

  int xLeft, yTop, width, height;
  int windowWidth, windowHeight;
  int pickH, pickW;
  
 //--- Constructor ---//
  DisplayElement() {
    windowWidth = -1;
    windowHeight = -1;   
    init();
  }

  virtual ~DisplayElement(){};

 virtual void init(){};

  virtual void location(int x, int y, int w, int h) = 0;

  virtual void display() = 0;
 
  virtual void idle(){};
  
  virtual void reshape(int w, int h){};

  virtual void keyboard(unsigned char key, int x, int y) =0;

  virtual void special(int key, int x, int y) =0;

  virtual void mouse(int button, int state, int x, int y) =0;

  virtual void motion(int x, int y) = 0;
  
  virtual void passive(int x, int y) = 0;


  bool isInside(int x, int y){
    return x>xLeft && x < (xLeft+width) &&
           y>yTop && y < (yTop+height) ;
  };

 void setBackgroundColor(float color[3]){
    backgroundColor[0] = color[0];
    backgroundColor[1] = color[1];
    backgroundColor[2] = color[2];
  }

};

#endif
