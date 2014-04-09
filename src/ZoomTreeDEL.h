#ifndef ZOOMTREEDEL_H
#define ZOOMTREEDEL_H

#include "Display.h"
#include "GMRATree.h"

#include <algorithm>

#include "colormapper.h"

template<typename TPrecision>
class ZoomTreeDEL : public DisplayElement{

  private:


#define BUFSIZE 512
    


    int nTop;
    int xM, yM;
    int mod, cur_button; 
    int lw;
    int nScales;
    
    int pickW, pickH; 
    int selected;
   
    Data<TPrecision> &data;

  public:

    ZoomTreeDEL( Data<TPrecision> &d )   :  data(d){ 
      pickW = 2;
      pickH = 2;
      lw = 1;
      selected = -1;
      nScales = data.maxScale + 1;
    };



    void location(int xPos, int yPos, int w, int h){
      width = w;
      height = h;
      xLeft = xPos;
      yTop = yPos;
    };




    void init(){  

    };


    void display(void){
     
     if(data.selectedNode != -1){      

      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();
     
      
      glLineWidth(lw);

        VisGMRANode<TPrecision> *node = data.nodeMap[data.selectedNode];
        double maxWidth = node->getPoints().size();      

        std::list<VisGMRANode<TPrecision> *> nodes;
        nodes.push_back( node );

        int startScale = node->scale;
        int startX = node->xPos;

        while( !nodes.empty()){           
          
          VisGMRANode<TPrecision> *node = (VisGMRANode<TPrecision> *) nodes.front();
          nodes.pop_front();

          int scale = node->scale-startScale;
          
          if(scale >= nScales ){
            break;
          }

          int xStart = xLeft + (node->xPos-startX) / maxWidth * width;
          double w = node->nPoints  / maxWidth;

          float col[3];
          data.treeColor.getColor(node->ratio, col);
                   
          float alpha = 0.75;
          if( node->ID == data.selectedNode ){
            alpha = 1;
          }

          glColor3f(col[0], col[1], col[2]);
          
      
          glPushName( node->ID );         
           double wnode = w*width;
          if( wnode < 1){
            wnode = 1;
          }
          glBegin(GL_QUADS);           
           glVertex2f(xStart, yTop + height * (1.0 - scale/((double)nScales)) ) ;
           glVertex2f(xStart, yTop + height * (1.0 - (scale+0.9)/(nScales)) );
           glVertex2f(xStart + wnode, yTop + height * (1.0 - (scale+0.9)/(nScales)) );
           glVertex2f(xStart + wnode, yTop + height * (1.0 - scale/((double)nScales)) );
          glEnd();
          glPopName();    
                  
          std::vector< GMRANode<TPrecision>* > children = node->getChildren();
          int offset = 0;
          for(typename std::vector< GMRANode<TPrecision> * >::iterator it =
            children.begin(); it != children.end(); ++it){
            nodes.push_back(*it);
          }


        }    
      }   


    };

    void reshape(int w, int h){

    };
  
    void idle(){

    };


    void keyboard(unsigned char key, int x, int y){

    };



    void special(int key, int x, int y){
    };




    void mouse(int button, int state, int x, int y){
      xM = x;
      yM = y;
      if(!isInside(x, y)){ return; };
      
       if ( state == GLUT_DOWN ){

      mod = glutGetModifiers();

      GLint vp[4];
      glGetIntegerv(GL_VIEWPORT, vp);
      GLuint selectBuf[BUFSIZE];
      glSelectBuffer(BUFSIZE, selectBuf);
      glRenderMode(GL_SELECT);
      glInitNames();

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluPickMatrix(x, vp[3]-y, pickW, pickH, vp);
      glOrtho (0, vp[2], vp[3], 0, 0, 1);

      display();
      GLint hits = glRenderMode(GL_RENDER);
      selected = -1;
      for(int i=0; i<hits; i++){
        int tmp = selectBuf[i*4 + 3];
        if(tmp != -1){
          selected = tmp;
        }
      }
      
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      
       if ( button == GLUT_LEFT_BUTTON){
         data.setSelected(selected);
       }
       else if( button == GLUT_RIGHT_BUTTON){
         data.setSelected(selected);
       }
      
       glutPostRedisplay();      
      }
  
    };



    // catch mouse move events
    void motion(int x, int y){

    };




    void passive(int x, int y){
      
    };



};

#endif
