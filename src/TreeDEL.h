#ifndef TREEDEL_H
#define TREEDEL_H

#include "Display.h"
#include "GMRATree.h"

#include <algorithm>

#include "colormapper.h"

template<typename TPrecision>
class TreeDEL : public DisplayElement{

  private:


#define BUFSIZE 512
    


    int nTop;
    int xM, yM;
    int mod, cur_button; 
    int lw;
    
    int pickW, pickH; 
    int selected;
    int nScales;
 
    Data<TPrecision> &data;

  public:

    TreeDEL( Data<TPrecision> &d )   :  data(d){ 
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
     
      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();
     
      
      glLineWidth(lw);
        VisGMRANode<TPrecision> *node = (VisGMRANode<TPrecision> *) data.tree.getRoot();
        double maxWidth = node->getPoints().size();      

        std::list<GMRANode<TPrecision> *> nodes;
        nodes.push_back( node );

        int selectedScale = -1;
        double selectedXstart = -1;
        double selectedWidth = -1;
        while( !nodes.empty()){           
          
          VisGMRANode<TPrecision> *node = dynamic_cast<VisGMRANode<TPrecision> *>( nodes.front() );
          nodes.pop_front();

          int scale = node->getScale();
          
          double xStart = xLeft + node->xPos / maxWidth * width;
          double w = node->nPoints  / maxWidth;

          float col[3];
          data.treeColor.getColor(node->ratio, col);
                   
          float alpha = 0.75;
          if( node->ID == data.selectedNode){
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
         
          if( node->ID  == data.selectedNode){
            selectedScale = scale;
            selectedXstart = xStart;
            selectedWidth = wnode;
          }

          std::vector< GMRANode<TPrecision>* > children = node->getChildren();
          int offset = 0;
          for(typename std::vector< GMRANode<TPrecision> * >::iterator it =
            children.begin(); it != children.end(); ++it){
            nodes.push_back(*it);
          }


        }    

        glLineWidth(2);
        if(selectedScale != -1){
            glColor3f(0, 1, 0);
                
            glBegin(GL_LINE_LOOP);           
            glVertex2f(selectedXstart, yTop + height * (1.0 - selectedScale/((double)nScales)) ) ;
            glVertex2f(selectedXstart, yTop + height * (1.0 - (selectedScale+0.9)/(nScales)) );
            glVertex2f(selectedXstart + selectedWidth, yTop + height * (1.0 - (selectedScale+0.9)/(nScales)) );
            glVertex2f(selectedXstart + selectedWidth, yTop + height * (1.0 - selectedScale/((double)nScales)) );
            glEnd();

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

      mod = glutGetModifiers();
      if ( state == GLUT_DOWN ){

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
      glutPostRedisplay();
       

      data.setSelected( selected) ;


      }
    };



    // catch mouse move events
    void motion(int x, int y){

    };




    void passive(int x, int y){
      
    };



};

#endif
