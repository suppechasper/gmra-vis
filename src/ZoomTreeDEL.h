#ifndef ZOOMTREEDEL_H
#define ZOOMTREEDEL_H

#include "Display.h"
#include "GMRATree.h"
#include <algorithm>
#include "ColorMap.h"

// * * * Zoom Tree * * * //
template<typename TPrecision, typename LabelType>
class ZoomTreeDEL : public DisplayElement{

 private:

#define BUFSIZE 512
  
  int nTop;
  int xM, yM;
  int mod, cur_button; 
  int lw;
  int nScales;
  int selected;
   
  // The maximal & minimal width of the tree
  double maxWidth, minWidth;

  Data<TPrecision, LabelType> &data;

  bool vertical;
    
  public:

  //--- Contructor ---//
  ZoomTreeDEL( Data<TPrecision, LabelType> &d, bool isVertical = true ) 
    : DisplayElement(), data(d), vertical(isVertical){ 
      pickW = 2;
      pickH = 2;
      lw = 1;
      selected = -1;
      nScales = data.maxScale + 1;
  };

  //--- Initialize the tree ---//
  void init(){};
  
  //--- Set the position of the tree ---//
  void location(int xPos, int yPos, int w, int h){
    
    // The position of the tree on the screen
    xLeft = xPos;
    yTop = yPos;
    
    // The height and width
    width = w;
    height = h;
    if(vertical){
      width = h;
      height = w;
    }
  };

  //--- Reshape the tree ---//
  void reshape(int w, int h){
    DisplayElement::reshape(w, h);

    // Reflect the new size of the window to the tree
    int newW, newH;
    int newXpos, newYpos;
    if(vertical){
      newW = height*wRatio;
      newH = width*hRatio;
      newXpos = xLeft;
      newYpos = yTop*wRatio;
    }
    else{
      newW = width*wRatio;
      newH = height*hRatio;
      newXpos = xLeft;
      newYpos = yTop*hRatio;
    } 

    // Change the location & size of the tree
    location(newXpos, newYpos, newW, newH);  
  };

  //--- Check if the mouse is inside the tree ---//
  bool isInside(int x, int y){
    bool inside;
    if(vertical){
      inside = (x > yTop && x < (yTop+height) &&
		y > xLeft && y < (xLeft+width));
    }
    else{
      inside = (x>xLeft && x < (xLeft+width) &&
		y>yTop && y < (yTop+height));
    }
    return inside;
  };
  
  //--- Display the scaled icicle tree ---//
  void display(void){

    if(data.selectedNode != -1){      
      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();
      
      glLineWidth(lw);
      
      // Get the selected node
      VisGMRANode<TPrecision> *sNode = dynamic_cast<VisGMRANode<TPrecision>*>(data.nodeMap[data.selectedNode]);
      std::list<GMRANode<TPrecision> *> nodes;
      nodes.push_back( sNode );    
           
      // Get the minimal and maximal width of a node
      minWidth = 2.0;
      maxWidth = sNode->getPoints().size();      
   
      // The scale of this node
      int startScale = sNode->getScale();
      int startX = sNode->xPos;
	    
      while( !nodes.empty()){           
	
	VisGMRANode<TPrecision> *node = dynamic_cast<VisGMRANode<TPrecision> *>( nodes.front() );
	nodes.pop_front();    
      
	// The scale of this node
	int scale = node->getScale()-startScale;
	float nodeWidth = height * ((1.0 - scale/(double)nScales) - (1.0 - (scale+0.9)/nScales));
	
	if(scale >= nScales ){
	  break;
       }

      // The starting xposition
      int xStart = xLeft + (node->xPos-startX) / maxWidth * width;
      double yStart = yTop + (nodeWidth*scale) + (lw*scale);

      // Scale width of node by the number of points in that node
      double w = node->nPoints  / maxWidth;
      double wnode = w*width;
      wnode = wnode < minWidth ? minWidth : wnode;

      // Set the color of the node
      ColorF col;
      if(data.colormapScheme == "entropy")
	col = dynamic_cast<TwoDDiscreteColormap*>(data.colormap)->
	  getColor(data.labelIndex[node->label], node->entropy);
      else if(data.colormapScheme == "ratio")
	col = data.colormap->getColor(node->ratio);
      else if(data.colormapScheme == "other")
	std::cout << "not sure on other!" << std::endl;
      float alpha = 1.0;
      //  if( node->ID == data.selectedNode )
      //	alpha = 1;   
      glColor4f(col.r(), col.g(), col.b(), alpha);
      
      // Draw the node
      glPushName( node->ID );         
      glBegin(GL_QUADS);           
      if(!vertical){
	glVertex2f(xStart,         yStart);
	glVertex2f(xStart,         yStart + nodeWidth);
	glVertex2f(xStart + wnode, yStart + nodeWidth);
	glVertex2f(xStart + wnode, yStart );
      }
      else{
	glVertex2f(yStart,             xStart);
	glVertex2f(yStart,             xStart + wnode);
	glVertex2f(yStart + nodeWidth, xStart + wnode);
	glVertex2f(yStart + nodeWidth, xStart);
      }      
      glEnd();
      glPopName();    
      
      // Get the node's children
      std::vector< GMRANode<TPrecision>* > children = node->getChildren();
      for(typename std::vector< GMRANode<TPrecision> * >::iterator it =
	    children.begin(); it != children.end(); ++it){
	nodes.push_back(*it);
      }
      }
    }
  }

  void mouse(int button, int state, int x, int y){
    std::cout << "MOUSE !!" << std::endl;
    xM = x;
    yM = y;
    if(!isInside(x, y)){ return; };
    
    if ( state == GLUT_DOWN ){
      std::cout << "pickW: " << pickW << " h: " << pickH << std::endl;

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



  // Catch mouse move events
  void motion(int x, int y){};
  
  // Passive actions
  void passive(int x, int y){};

  void idle(){};
  void keyboard(unsigned char key, int x, int y){};
  void special(int key, int x, int y){};
  
};

#endif
