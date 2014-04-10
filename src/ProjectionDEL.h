#ifndef PROJECTIONDEL_H
#define PROJECTIONDEL_H

#include "Display.h"
#include "DenseVector.h"
#include "DenseMatrix.h"

#include <math.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#include "Font.h"

// + + + + + + + + + + + + + + //
// The scatter plot
// + + + + + + + + + + + + + + //
template<typename TPrecision>
class ProjectionDEL : public DisplayElement{
  
private:
  
#define BUFSIZE 512
  
  
  int size; 
  
  int xM, yM;
  int mod, cur_button; 
  int pickH, pickW;  
  float alpha;

  Data<TPrecision> &data;
  
public:

  //--- Constructor ---//
  ProjectionDEL(Data<TPrecision> &d) 
    : DisplayElement(), data(d){ 
    pickW = 5;
    pickH = 5;
    alpha = 0.1;
  };


  //--- Set the location of the scatterplot ---//
  void location(int xPos, int yPos, int w, int h){
    width = w;
    height = h;
    xLeft = xPos;
    yTop = yPos;
    size = std::min(w, h)/2 ;
  };

  void init(){ };

  //--- Display the scatterplot ---//
  void display(void){
    glMatrixMode(GL_MODELVIEW); 	
    glLoadIdentity();
    
    //glPushName(-1); 
    glPointSize(6);
    int x1;
    int x2;
    float col[4];
    col[3] = alpha;

    // Draw the axes
    glLineWidth(1);
    glColor3f(0.75, 0.75, 0.75);
    glBegin(GL_LINES);
    glVertex2f( toScreenX( - 1.25), toScreenY( 0 ) );
    glVertex2f( toScreenX( + 1.25), toScreenY( 0 ) );
    glVertex2f( toScreenX( 0 ), toScreenY( - 1.25) );
    glVertex2f( toScreenX( 0 ), toScreenY( + 1.25) );
    glEnd();
      
    if(data.selectedNode != -1){

      std::vector<int> pts = data.nodeMap[data.selectedNode]->getPoints();
      
      glBegin(GL_POINTS);
      for(int i=0; i<data.P.N(); i++){
	
	// Convert the points to screen coordinates
	x1 = toScreenX(data.P(0,i));
	x2 = toScreenY(data.P(1,i));
	
	// Color the point based on it's label
	/*	if(data.labels(pts[i]) == 0){
	  glColor4f(0, 0.25, 1, alpha);
	}
	else{	  
	  glColor4f(1, 0.45, 0, alpha);
	  }*/
	Tuple labelColor = data.labelsColor.getColor(data.labels(pts[i]));
	glColor4f(labelColor.r, labelColor.g, labelColor.b, alpha);
	
	// Draw the point
	glVertex2f(x1, x2);
      }
      glEnd();

      if(isInside(xM, yM)){
	//draw pick rectangle
	glColor4f(0.75, 0.75, 0.75, 0.75);
	glLineWidth(2);
	double pw = pickW / 2.0;
	double ph = pickH / 2.0;
	glBegin(GL_LINE_LOOP);
	glVertex2f(xM-pw, yM-ph);
	glVertex2f(xM+pw, yM-ph);
	glVertex2f(xM+pw, yM+ph);
	glVertex2f(xM-pw, yM+ph);
	glEnd();
      }
    }
  };
  
  void reshape(int w, int h){};    

  void idle(){};


    void keyboard(unsigned char key, int x, int y){
      //std::vector<int> keep;
      if(!isInside(x, y)){ return; };
      switch(key)
      {

        case '+':
          pickW+=1;
          pickH+=1;
          break;
        case '-':
          pickW-=1;
          pickH-=1;
          if(pickW < 5){
            pickW=5;
          }
          if(pickH < 5){
            pickH=5;
          }
          break;

      }
      glutPostRedisplay();
    };



    void special(int key, int x, int y){
      switch(key)
      {
        case GLUT_KEY_LEFT:
          alpha -= 0.01;
          break;
        case GLUT_KEY_RIGHT:
          alpha += 0.01;
          break;
      }
      if(alpha < 0){
        alpha=0;
      }
      else if(alpha > 1){
        alpha=1;
      }

      glutPostRedisplay();
    };




    void mouse(int button, int state, int x, int y){
      xM = x;
      yM = y;
      if(!isInside(x, y)){ return; };
    };





    // catch mouse move events
    void motion(int x, int y){};




    void passive(int x, int y){
      xM = x;
      yM = y;

      if(data.selectedNode != -1){
        std::vector<int> pts = data.nodeMap[data.selectedNode]->getPoints();
        if(!isInside(x, y) ){ 
          return; 
        };


        double xd = toDataX(x);
        double yd = toDataY(y);
        double wd = pickW/(double)size;
        double hd = pickH/(double)size;
        int selected = -1;
        double minD = std::numeric_limits<double>::max();
        for(int i=0; i< data.P.N(); i++){
          double dx = fabs(data.P(0, i) - xd);
          double dy = fabs(data.P(1, i) - yd );
          if(fabs(data.P(0, i) - xd) < wd && fabs(data.P(1, i) - yd ) < hd){
            double d = dx*dx + dy*dy;
            if(d < minD){ 
              selected = pts[i];
              minD = d;
            }
          }
        }

        data.selectedIndex = selected;

        glutPostRedisplay();
      }

    };


  private:

    double toScreenX(TPrecision x){
      return xLeft + (x+1.0) * size; 
    };
    double toScreenY(TPrecision y){
      return yTop + (y+1.0) * size; 
    };

    double toDataX(int x){
      return (x - xLeft ) / (double) size -1.0;
    };

    double toDataY(int y){
      return (y - yTop) / (double) size - 1.0;
    }; 

};

#endif
