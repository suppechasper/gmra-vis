#ifndef GMRAVIS_H
#define GMRAVIS_H

#include "Animator.h"
#include "Display.h"
#include "Data.h"
#include "DenseVector.h"
#include "DenseMatrix.h"

#include "DisplayElement.h"
#include "TreeDEL.h"
#include "ZoomTreeDEL.h"
#include "PatchDEL.h"
#include "SinglePatchDEL.h"
#include "SinglePatchReconstructDEL.h"
#include "ProjectionDEL.h"
#include "MultiscaleProjectionDEL.h"
#include "ColorLegendDEL.h"
#include "ParallelDEL.h"

#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <list>

#include "Font.h"

bool whiteBackground = false;

template<typename TPrecision>
class GMRAVis : public Display{

  private:


    int width, height;
    int xM, yM;
    int pickH, pickW; 
    Data<TPrecision> &data; 


    std::list<DisplayElement *> elements;
    Animator animator;

public:
  
  //--- Constructor ---//
  GMRAVis(Font &f, Data<TPrecision> &d) : Display(f), data(d){ 
    pickH = 5;
    pickW = 5;
    };
  
  
  //--- Return the display title ---//
    std::string title(){
      return "IPCA GMRA Visualization";
    };


  //--- Reshape the display ---//
    void reshape(int w, int h){
      width = w;
      height = h;
      
      int size = std::min(w, h)/2 - 5;
      glViewport(0, 0, w, h);       
      glMatrixMode(GL_PROJECTION);  
      glLoadIdentity();
      glOrtho (0, width, height, 0, 0, 1);
      
      // for each element in display (ignoring PCA graph), resize to the window size
      // please note that this is manually using pixel values defined in each class
      // not the best, but do-able until more elegant variables exist to store the values
      for(std::list<DisplayElement *>::iterator it=elements.begin(); 
           it != elements.end(); ++it){
           (*it)->reshape(w, h);
      }
    };


  //--- Set up the display ---//
  void setupDisplay(){
    
      
    // Set the background color
    setBackgroundColor(0.15, 0.15, 0.15);
    
    // Clear display elements
    for(std::list<DisplayElement *>::iterator it = elements.begin(); it !=
          elements.end(); ++it){
      delete (*it);
    }
    elements.clear();

    // Add the icicle tree
    TreeDEL<TPrecision> *pd = new TreeDEL<TPrecision>(data);
    pd->location(10, 10, 800, 200);
    elements.push_back(pd);
     
    // Add the color legend for the tree
    ColorLegendDEL<TPrecision> *cld =  new ColorLegendDEL<TPrecision>(data.treeColor);
    cld->location(820, 10, 20, 200);
    elements.push_back(cld);
    
    // The scatter plot
    ProjectionDEL<TPrecision> *pDel = new ProjectionDEL<TPrecision>(data);
    pDel->location(450, 320, 350, 350);
    elements.push_back(pDel);
      
    // The parallel coordinates plot
    ParallelDEL<TPrecision> * pc = new ParallelDEL<TPrecision>(data);
    pc->location(250, 500, 500, 200);
    elements.push_back(pc);

    // The ellipses
    MultiscaleProjectionDEL<TPrecision> *projectionDel = new
      MultiscaleProjectionDEL<TPrecision>(data, animator);
    projectionDel->location(10, 220, 400, 400);
    elements.push_back(projectionDel);

    /*
      ZoomTreeDEL<TPrecision> *ztd = new ZoomTreeDEL<TPrecision>(data);
      ztd->location(10, 320, 350, 350);
      elements.push_back(ztd);
    */
    
    /* PatchDEL<TPrecision> *patchDel = new PatchDEL<TPrecision>(data);
       patchDel->location(550, 420, 400, 400);
       elements.push_back(patchDel);
    */
 
    /*
      SinglePatchDEL<TPrecision> *spatchDel = new SinglePatchDEL<TPrecision>(data);
      spatchDel->location(750, 320, 100, 100);
      elements.push_back(spatchDel);
     
      SinglePatchReconstructDEL<TPrecision> *srpatchDel = new SinglePatchReconstructDEL<TPrecision>(data);
      srpatchDel->location(750, 430, 100, 100);
      elements.push_back(srpatchDel);
    */
      
    }
  
  //--- Initialize the display ---//
  void init(){  
    glClearColor(0.15, 0.15, 0.15, 0);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable( GL_POINT_SMOOTH );
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    //glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    setupDisplay(); 
  };
  
  //--- Actions to take on display ---//
  void display(void){
      
    // Set background color
    if(whiteBackground)
      glClearColor(1.00, 1.00, 1.00, 0);
    else
      glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 0);
    
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Set the modelview matrix
    glMatrixMode(GL_MODELVIEW); 	
    glLoadIdentity();
    
    // Show all display elements
    for(std::list<DisplayElement *>::iterator it=elements.begin(); 
	it != elements.end(); ++it){
      (*it)->display();
    }
    
    // Swap buffers
    glutSwapBuffers();
  };

  //--- On idle, animate ---//
  void idle(){
    animator.step();
  };

  //--- Actions to take on keystrokes ---//
  void keyboard(unsigned char key, int x, int y){
    Display::keyboard(key, x, y);
    
    for(std::list<DisplayElement *>::iterator it=elements.begin(); 
	it != elements.end(); ++it){
      (*it)->keyboard(key, x, y);
    }
    glutPostRedisplay();
  } 
  
  //--- Special actions ---//
  void special(int key, int x, int y){
    for(std::list<DisplayElement *>::iterator it=elements.begin(); 
	it != elements.end(); ++it){
      (*it)->special(key, x, y);
    }
    glutPostRedisplay();
  };
  
  void mouse(int button, int state, int x, int y){
    xM = x;
    yM = y;
    for(std::list<DisplayElement *>::iterator it=elements.begin(); 
	it != elements.end(); ++it){
      (*it)->mouse(button, state, x, y);
    }
    glutPostRedisplay();
  };

  void motion(int x, int y){
    for(std::list<DisplayElement *>::iterator it=elements.begin(); 
	it != elements.end(); ++it){
      (*it)->motion(x, y);
    }
    glutPostRedisplay();
  };

  void passive(int x, int y){
    xM = x;
    yM = y;
    
    for(std::list<DisplayElement *>::iterator it=elements.begin(); 
	it != elements.end(); ++it){
      (*it)->passive(x, y);
    }
    glutPostRedisplay();
  };

  







};

#endif
