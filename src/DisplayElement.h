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
  ColorF backgroundColor;
  int xLeft, yTop, width, height;
  int windowWidth, windowHeight;
  int pickH, pickW;
  float wRatio, hRatio;      
  
 public:
 //--- Constructor ---//
  DisplayElement() {
    windowWidth = -1;
    windowHeight = -1;   
    wRatio = hRatio = 1.0;
    init();
  }

  virtual ~DisplayElement(){};

  virtual void init(){};

  virtual void location(int x, int y, int w, int h) = 0;

  virtual void display() = 0;
 
  virtual void idle(){};
  
  // Reshape the element, save the new aspect ratios
  virtual void reshape(int w, int h){
    // If we don't have the window dims yet, set them
    if((windowWidth == -1) && (windowHeight == -1)){
      windowWidth = w;
      windowHeight = h;
    }
    else{
      
      // Get the ratio of the old window size to the new
      wRatio = (float)w/(float)windowWidth;
      hRatio = (float)h/(float)windowHeight;
  
      // Set the new window dimensions
      windowWidth = w;
      windowHeight = h;
    }
  };

  virtual void keyboard(unsigned char key, int x, int y) =0;

  virtual void special(int key, int x, int y) =0;

  virtual void mouse(int button, int state, int x, int y) =0;

  virtual void motion(int x, int y) = 0;
  
  virtual void passive(int x, int y) = 0;


  bool isInside(int x, int y){
    return x>xLeft && x < (xLeft+width) &&
           y>yTop && y < (yTop+height) ;
  };

 void setBackgroundColor(ColorF color){
    backgroundColor[0] = color[0];
    backgroundColor[1] = color[1];
    backgroundColor[2] = color[2];
  }

};

#endif
