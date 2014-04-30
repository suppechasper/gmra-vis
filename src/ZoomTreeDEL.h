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
    /*
    // Set the ranges of the colormaps
    data.treeColor->setRange(data.minRatio, data.maxRatio);
    data.entropyColor->setRange(data.minEntropy, data.maxEntropy);

    // Get the root of the tree
    VisGMRANode<TPrecision> *rootNode = (VisGMRANode<TPrecision> *) data.tree.getRoot();
    std::list<GMRANode<TPrecision> *> nodes;
    nodes.push_back(rootNode);

    // Get the maximal width of a node
    minWidth = 4;
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
      wnode = wnode < minWidth ? minWidth : wnode;
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
      ColorF labelColor = data.labelsColor->getColor(node->label);
      //  data.entropyColor.set(labelColor.r, 0.90, labelColor.g, 0.90, labelColor.b, 0.90);
      // data.entropyColor.set(labelColor.r, 1.0, labelColor.g, 1.0, labelColor.b, 1.0);
      //data.entropyColor.getColor(node->entropy, node->color);
  
      std::vector< GMRANode<TPrecision>* > children = node->getChildren();
      for(typename std::vector< GMRANode<TPrecision> * >::iterator it =
            children.begin(); it != children.end(); ++it){
	nodes.push_back(*it);
      }
      // nodes.clear();
    }*/
  }
  
  //--- Display the scaled icicle tree ---//
  void display(void){

    if(data.selectedNode != -1){      
      glMatrixMode(GL_MODELVIEW); 	
      glLoadIdentity();
      
      glLineWidth(lw);
      
      // Get the selected node
      //      VisGMRANode<TPrecision> *node = data.nodeMap[data.selectedNode];
      std::list<GMRANode<TPrecision> *> nodes;
      nodes.push_back( (VisGMRANode<TPrecision> *) data.nodeMap[data.selectedNode] );
    
      VisGMRANode<TPrecision> *node = dynamic_cast<VisGMRANode<TPrecision> *>( nodes.front() );

      VisGMRANode<TPrecision> *root = dynamic_cast<VisGMRANode<TPrecision> *>( data.tree.getRoot());

      std::cout << "root id: " << root->ID << " selected " << node->ID << std::endl;

      DisplayNode * dNode = data.displayTree[node->ID];
      
      std::cout << "node->scale: " << node->getScale() << std::endl;
      // Get the maximal width of a node
      double maxWidth = node->getPoints().size();      
   
      // The scale of this node
      int startScale = node->getScale();
      int startX = node->xPos;
	    
      while( !nodes.empty()){           
	
	VisGMRANode<TPrecision> *node = dynamic_cast<VisGMRANode<TPrecision> *>( nodes.front() );
	nodes.pop_front();    
      
	// The scale of this node
      int scale = node->getScale()-startScale;
      float nodeWidth = height * ((1.0 - scale/(double)nScales) - (1.0 - (scale+0.9)/nScales));
      
      //    if(scale >= nScales ){/
      //	break;
      // }

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



  // Catch mouse move events
  void motion(int x, int y){};
  
  // Passive actions
  void passive(int x, int y){};

  void idle(){};
  void keyboard(unsigned char key, int x, int y){};
  void special(int key, int x, int y){};
  
};

#endif
