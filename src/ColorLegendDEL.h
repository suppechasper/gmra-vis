#ifndef COLORLEGENDDEL_H
#define COLORLEGENDDEL_H

#include "Display.h"
#include "GMRATree.h"

#include <algorithm>

#include "colormapper.h"

template<typename TPrecision>
class ColorLegendDEL : public DisplayElement{

 private:
  
  
  float yMid;
  int xM, yM;
  int mod, cur_button; 
  int lw;
  
  ColorMapper<float> &cmap;

  bool isVertical;

 public:
  
  //--- Constructor ---//
 ColorLegendDEL( ColorMapper<float> &cm, bool vertical = true ) : cmap(cm), isVertical(vertical){ 
  };
    
  //--- Set the location of the color legend ---///
  void location(int xPos, int yPos, int w, int h){
    width = w;
    height = h;
      
    xLeft = xPos;
    yTop = yPos;
    
    yMid = height*(cmap.rangeMax - cmap.rangeMid) / (cmap.rangeMax - cmap.rangeMin);
  };
  
  //--- Initialization ---//
  void init(){};
  
  //--- Display the color legend ---//
  void display(void){
    
    glMatrixMode(GL_MODELVIEW); 	
    glLoadIdentity();
    
    glBegin(GL_QUADS); 
    
    glColor3f(cmap.rmax, cmap.gmax, cmap.bmax);
    glVertex2f(xLeft, yTop ) ;
    glVertex2f(xLeft + width, yTop );

    glColor3f(cmap.rmid, cmap.gmid, cmap.bmid);
    glVertex2f(xLeft + width, yTop + yMid );
    glVertex2f(xLeft, yTop + yMid);    

    glVertex2f(xLeft, yTop + yMid );
    glVertex2f(xLeft+width, yTop + yMid);

    glColor3f(cmap.rmin, cmap.gmin, cmap.bmin);
    glVertex2f(xLeft + width, yTop + height );
    glVertex2f(xLeft, yTop + height);
    
    glEnd();			
    
    glLineWidth(1.0);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES); 
    
    glVertex2f(xLeft + width, yTop + yMid );
    glVertex2f(xLeft, yTop + yMid);
    
    glEnd();
  };
  
  void reshape(int w, int h){};
  void idle(){};
  void keyboard(unsigned char key, int x, int y){};
  void special(int key, int x, int y){};
  void mouse(int button, int state, int x, int y){};
  // catch mouse move events
  void motion(int x, int y){};
  void passive(int x, int y){};
};

#endif
