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
  
  // The minimal width of the tree
  double minWidth, maxWidth;

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
      init();
  };
  
  //--- Initialize the tree ---//
  void init(){
    
    VisGMRANode<TPrecision> *sNode = dynamic_cast<VisGMRANode<TPrecision>*>(data.tree.getRoot());

    // Get the minimal and maximal width of a node
    minWidth = 2.0;
    	    

  };
  
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
  
  //--- Return the color of the node ---//
  ColorF getColor(VisGMRANode<TPrecision> *node){
    ColorF col;
   if(data.colormapScheme == "entropy")
	col = dynamic_cast<TwoDDiscreteColormap*>(data.colormap)->
	  getColor(data.labelIndex[node->label], node->entropy);
      else if(data.colormapScheme == "ratio")
	col = data.colormap->getColor(node->ratio);
      else if(data.colormapScheme == "other")
	std::cout << "not sure on other!" << std::endl;
   return col;
  }

  //--- Display the scaled icicle tree ---//
  void display(void){

    if(data.selectedNode != -1){    
      
      // Get the selected node
      VisGMRANode<TPrecision> *sNode = (VisGMRANode<TPrecision>*)(data.nodeMap[data.selectedNode]);
      std::list<GMRANode<TPrecision> *> nodes;
      nodes.push_back( sNode );    
           
      // The scale, width and position of the zoom tree
      int startScale = sNode->getScale();
      maxWidth = sNode->getPoints().size();      
      int startX = sNode->xPos;
      
      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();
      glLineWidth(lw);
      
     
      // Draw the arraw to the parent, if we have one
      VisGMRANode<TPrecision> *pNode = dynamic_cast<VisGMRANode<TPrecision>*>(data.nodeMap[data.selectedNode]->getParent());
      if(pNode != NULL){
	
	// Set the color of the node
	ColorF col = getColor(pNode);
	glColor4f(col.r(), col.g(), col.b(), 1.0);
	
	// The scale of this node
	int scale = sNode->getScale()-startScale;
	float nodeWidth = height * ((1.0 - scale/(double)nScales) - (1.0 - (scale+0.9)/nScales));   

	// The starting x and y positions
	int xStart = xLeft + (sNode->xPos-startX) / maxWidth * width;
	double yStart = yTop + (nodeWidth*scale) + (lw*scale);
      	
	// Scale width of node by the number of points in that node
	double w = sNode->nPoints  / maxWidth;
	double wnode = w*width;
	wnode = wnode < minWidth ? minWidth : wnode;
	
	// Draw the node
	glPushName( pNode->ID );         
	glBegin(GL_POLYGON);           
	if(!vertical){
	  glVertex2f(xStart,         yStart);
	  glVertex2f(xStart,         yStart -25);
	  glVertex2f(xStart + wnode, yStart -25);
	  glVertex2f(xStart + wnode, yStart );
	}
	else{	 
	  glVertex2f(yStart, xStart + (wnode*.5));
	  glVertex2f(yStart +15, xStart + wnode);
	  glVertex2f(yStart +30,             xStart + wnode);
	  glVertex2f(yStart +30,            xStart);
	  glVertex2f(yStart +15, xStart);
 	  
	}      
	glEnd();
	glColor4f(0.25, 0.25, 0.25, 1.0);
	glBegin(GL_LINE_LOOP);           
	if(!vertical){
	  glVertex2f(xStart,         yStart);
	  glVertex2f(xStart,         yStart -25);
	  glVertex2f(xStart + wnode, yStart -25);
	  glVertex2f(xStart + wnode, yStart );
	}
	else{
	  glVertex2f(yStart, xStart + (wnode*.5));
	  glVertex2f(yStart +15, xStart + wnode);
	  glVertex2f(yStart +30,             xStart + wnode);
	  glVertex2f(yStart +30,            xStart);
	  glVertex2f(yStart +15, xStart);
 
	}      
	glEnd();
	glPopName();   
      }

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
	double yStart = yTop+30 + (nodeWidth*scale) + (lw*scale);

	// Scale width of node by the number of points in that node
	double w = node->nPoints  / maxWidth;
	double wnode = w*width;
	wnode = wnode < minWidth ? minWidth : wnode;
	
	// Set the color of the node
	ColorF col = getColor(node);
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
      // Highlight the selected node
      if(data.selectedNode != -1){

	VisGMRANode<TPrecision> *node = dynamic_cast<VisGMRANode<TPrecision> *>( data.nodeMap[data.selectedNode] );  
	
	// The scale of this node
	int scale = node->getScale()-startScale;
	float nodeWidth = height * ((1.0 - scale/(double)nScales) - (1.0 - (scale+0.9)/nScales));
	
	// The starting xposition
	int xStart = xLeft + (node->xPos-startX) / maxWidth * width;
	double yStart = yTop+30 + (nodeWidth*scale) + (lw*scale);

	// Scale width of node by the number of points in that node
	double w = node->nPoints  / maxWidth;
	double wnode = w*width;
	wnode = wnode < minWidth ? minWidth : wnode;
	
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);
	glColor4f(backgroundColor.r(), backgroundColor.g(), backgroundColor.b(), 1.0);
	glVertex2f(yStart,             xStart);
	glVertex2f(yStart,             xStart + wnode);
	glVertex2f(yStart + nodeWidth, xStart + wnode);
	glVertex2f(yStart + nodeWidth, xStart);
	glEnd();
	
	glLineWidth(1.0);
	glColor4f(data.selectedNodeColor.r(), 
		  data.selectedNodeColor.g(), 
		  data.selectedNodeColor.b(), 1.0);
	glBegin(GL_LINE_LOOP);
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
	
      }
    }
  }
  //--- Event on mousing ---//
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
	  //data.selectedIndex = -1;
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
