#ifndef DISPLAY_H
#define DISPLAY_H

//include opengl stuff
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "Font.h"
#include <string>

class Display{

protected:
  
  Font &font;
  bool whiteBackground;
  float backgroundColor[3];

public:
  
  Display(Font &f)
    : font(f), whiteBackground(false){};
  virtual ~Display(){};
  
  virtual std::string title() = 0;
  
  virtual void printHelp(){
    
    std::cout << "Usage" << std::endl;
    std::cout << "------------------" << std::endl;
    
  };
  
  virtual void init() = 0;
  
  virtual void reshape(int w, int h) = 0;
  
  virtual void display() = 0;
  
  virtual void idle(){};
  
  virtual void keyboard(unsigned char key, int x, int y){
     switch(key){

     case 'h':
     case 'H':
       printHelp();
       break;
       
       // toggle background color between white and black
     case 'b':
     case 'B':
       if(whiteBackground)
	 whiteBackground = false;
       else
	 whiteBackground = true;
       break;
     }    
  }

  virtual void special(int key, int x, int y) =0;

  virtual void mouse(int button, int state, int x, int y) =0;

  virtual void motion(int x, int y) = 0;
  
  virtual void passive(int x, int y) = 0;

  void setBackgroundColor(float r, float g, float b){
    backgroundColor[0] = r;
    backgroundColor[1] = g;
    backgroundColor[2] = b;
  }

};

#endif
