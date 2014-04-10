#ifndef TREEDEL_H
#define TREEDEL_H

#include "Display.h"
#include "GMRATree.h"
#include <algorithm>
#include "colormapper.h"

// * * * Icicle tree diagram * * * //
template<typename TPrecision>
class TreeDEL : public DisplayElement{

 private:

#define BUFSIZE 512
  
  int nTop;
  int xM, yM;
  int mod, cur_button; 
  int lw;
  
  int selected;
  int nScales;

  // The maximal width of the tree
  double maxWidth;

  Data<TPrecision> &data;
  
  bool vertical;

 public:
  
  //--- Contructor ---//
 TreeDEL(Data<TPrecision> &d, bool isVertical = true) 
   : DisplayElement(), data(d), vertical(isVertical){ 
    pickW = 2;
    pickH = 2;
    lw = 1;
    selected = -1;
    nScales = data.maxScale + 1;

    

    // Create the tree
    createTree();
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
    createTree();
  };

 //--- Reshape the tree ---//
  void reshape(int w, int h){

    // If we don't have the window dims yet, set them
    if((windowWidth == -1) && (windowHeight == -1)){
      windowWidth = w;
      windowHeight = h;
    }
    else{
      
      // Get the ratio of the old window size to the new
      float wRatio = (float)w/(float)windowWidth;
      float hRatio = (float)h/(float)windowHeight;
  
      // Set the new window dimensions
      windowWidth = w;
      windowHeight = h;
      
      // Reflect the new size of the window to the tree
      int newW, newH;
      if(vertical){
	newW = height*wRatio;
	newH = width*hRatio;
      }
      else{
	newW = width*wRatio;
	newH = height*hRatio;
      } 

      // Change the location & size of the tree
      location(xLeft, yTop, newW, newH);
    }
  };
  
  //--- Check if the mouse is inside the tree ---//
  bool isInside(int x, int y){
    bool inside;
    if(vertical)
      inside = (x>xLeft && x < (xLeft+height) &&
		y>yTop && y < (yTop+width));
    else
      inside = (x>xLeft && x < (xLeft+width) &&
		y>yTop && y < (yTop+height));
    return inside;
  };

  //--- Precompute the tree on itialization ---//
  void createTree(){

    // Set the ranges of the colormaps
    data.treeColor.setRange(data.minRatio, data.maxRatio);
    data.entropyColor.setRange(data.minEntropy, data.maxEntropy);

    // Get the root of the tree
    VisGMRANode<TPrecision> *rootNode = (VisGMRANode<TPrecision> *) data.tree.getRoot();
    std::list<GMRANode<TPrecision> *> nodes;
    nodes.push_back(rootNode);

    // Get the maximal width of a node
    maxWidth = rootNode->getPoints().size();         
    
    // Iterate through the nodes
    while( !nodes.empty()){   
      VisGMRANode<TPrecision> *node = dynamic_cast<VisGMRANode<TPrecision> *>( nodes.front() );
      nodes.pop_front();

      // The scale of this node
      int scale = node->getScale();	
      float nodeWidth = height * ((1.0 - scale/(double)nScales) - (1.0 - (scale+0.9)/nScales));
      
      // The starting xposition
      double xStart = xLeft + node->xPos / maxWidth * width;
      node->xStart = xStart;
      double yStart = yTop + (nodeWidth*scale) + (lw*scale);

      // Scale width of node by the number of points in that node
      double w = node->nPoints  / maxWidth;
      double wnode = w*width;
      
      if( wnode < 1){
	wnode = 1;
      }
      node->wnode = wnode;
      
      // Calculate the corners of the quad
      if(!vertical){
	node->quad1[0] = xStart;
	node->quad1[1] = yStart;
	node->quad2[0] = xStart;
	node->quad2[1] = yStart + nodeWidth;
	node->quad3[0] = xStart + wnode;
	node->quad3[1] = yStart + nodeWidth;
	node->quad4[0] = xStart + wnode;
	node->quad4[1] = yStart;
      }
      else{
	node->quad1[0] = yStart;
	node->quad1[1] = xStart;
	node->quad2[0] = yStart;
	node->quad2[1] = xStart + wnode;
	node->quad3[0] = yStart + nodeWidth;
	node->quad3[1] = xStart + wnode;
	node->quad4[0] = yStart + nodeWidth;;
	node->quad4[1] = xStart;
      }
       
      // Get the color of the node
      //data.treeColor.getColor(node->ratio, node->color);
      Tuple labelColor = data.labelsColor.getColor(node->label);
      //  data.entropyColor.set(labelColor.r, 0.90, labelColor.g, 0.90, labelColor.b, 0.90);
      data.entropyColor.set(labelColor.r, 1.0, labelColor.g, 1.0, labelColor.b, 1.0);
      data.entropyColor.getColor(node->entropy, node->color);
  
      std::vector< GMRANode<TPrecision>* > children = node->getChildren();
      for(typename std::vector< GMRANode<TPrecision> * >::iterator it =
            children.begin(); it != children.end(); ++it){
	nodes.push_back(*it);
      }
      // nodes.clear();
    }
  }
  
  //--- Display the icicle tree ---//
  void display(void){
    
    glMatrixMode(GL_MODELVIEW); 	
    glLoadIdentity();
      
    glLineWidth(lw);

    // Get the tree root
    std::list<GMRANode<TPrecision> *> nodes;
    nodes.push_back( (VisGMRANode<TPrecision> *) data.tree.getRoot() );
      
    // The corners of the selected quad
    int selectedScale;
    float selectedQ1[2];
    float selectedQ2[2];
    float selectedQ3[2];
    float selectedQ4[2];

    while( !nodes.empty()){           
      
      VisGMRANode<TPrecision> *node = dynamic_cast<VisGMRANode<TPrecision> *>( nodes.front() );
      nodes.pop_front();
      
      // Get the color of the node
      glColor3f(node->color[0], node->color[1], node->color[2]);
      
      // Draw the quad
      glPushName( node->ID );
      glBegin(GL_QUADS);        
      glVertex2f(node->quad1[0], node->quad1[1]);
      glVertex2f(node->quad2[0], node->quad2[1]);			
      glVertex2f(node->quad3[0], node->quad3[1]);				
      glVertex2f(node->quad4[0], node->quad4[1]);					
      glEnd();    
      glPopName();         
      
      // Save the selected node info
      if( node->ID  == data.selectedNode){
	selectedQ1[0] = node->quad1[0];
	selectedQ1[1] = node->quad1[1];
	selectedQ2[0] = node->quad2[0];
	selectedQ2[1] = node->quad2[1];
	selectedQ3[0] = node->quad3[0];
	selectedQ3[1] = node->quad3[1];
	selectedQ4[0] = node->quad4[0];
	selectedQ4[1] = node->quad4[1];

	selectedScale = node->getScale();
      }
      
      // Get the node's children
      std::vector< GMRANode<TPrecision>* > children = node->getChildren();
      for(typename std::vector< GMRANode<TPrecision> * >::iterator it =
            children.begin(); it != children.end(); ++it){
	nodes.push_back(*it);
      }
      // nodes.clear();
    }    
    
    // Draw the selection rectangle
    glLineWidth(2);
    if(selectedScale != -1){
      // glColor3f(0, 1, 0);
      glColor3f(backgroundColor[0]+.1, backgroundColor[1]+.1, backgroundColor[2]+.1);
      glBegin(GL_LINE_LOOP);
      glVertex2f(selectedQ1[0], selectedQ1[1]);
      glVertex2f(selectedQ2[0], selectedQ2[1]);
      glVertex2f(selectedQ3[0], selectedQ3[1]);
      glVertex2f(selectedQ4[0], selectedQ4[1]);
      glEnd();      
    }
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
  void motion(int x, int y){};
  void passive(int x, int y){}
  
  void idle(){};
  void keyboard(unsigned char key, int x, int y){};
  void special(int key, int x, int y){};
  
};

#endif
