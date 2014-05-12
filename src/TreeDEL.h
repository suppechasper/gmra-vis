#ifndef TREEDEL_H
#define TREEDEL_H

#include "Display.h"
#include "GMRATree.h"
#include "ColorMap.h"
#include "Vector.h"
#include <algorithm>

// * * * Icicle tree diagram * * * //
template<typename TPrecision, typename LabelType>
class TreeDEL : public DisplayElement{

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
 TreeDEL(Data<TPrecision, LabelType> &d, bool isVertical = true) 
   : DisplayElement(), data(d), vertical(isVertical){ 
    pickW = 2;
    pickH = 2;
    lw = 1;
    selected = -1;
    nScales = data.maxScale + 1;
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
    createTree();
  };

  //--- Reshape the tree ---//
  void reshape(int w, int h){
    DisplayElement::reshape(w, h);

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

    // Get the root of the tree
    VisGMRANode<TPrecision> *rootNode = (VisGMRANode<TPrecision> *) data.tree.getRoot();
    std::list<GMRANode<TPrecision> *> nodes;
    nodes.push_back(rootNode);

    // Get the maximal width of a node
    minWidth = 2;
    maxWidth = rootNode->getPoints().size();         

    // Iterate through the nodes
    while( !nodes.empty()){   
      VisGMRANode<TPrecision> *node = dynamic_cast<VisGMRANode<TPrecision> *>( nodes.front() );
      nodes.pop_front();
      
      // Create a new display node
      DisplayNode * dNode = new DisplayNode(node->ID);

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
      std::array<Vector2f, 4> quads;
      if(!vertical){
	quads[0] = Vector2f(xStart,         yStart);
	quads[1] = Vector2f(xStart,         yStart + nodeWidth);
	quads[2] = Vector2f(xStart + wnode, yStart + nodeWidth);
	quads[3] = Vector2f(xStart + wnode, yStart);
	
	/*	node->quad1[0] = xStart;
	node->quad1[1] = yStart;
	node->quad2[0] = xStart;
	node->quad2[1] = yStart + nodeWidth;
	node->quad3[0] = xStart + wnode;
	node->quad3[1] = yStart + nodeWidth;
	node->quad4[0] = xStart + wnode;
	node->quad4[1] = yStart;
	*/
      }
      else{
	quads[0] = Vector2f(yStart,             xStart);
	quads[1] = Vector2f(yStart,             xStart + wnode);
	quads[2] = Vector2f(yStart + nodeWidth, xStart + wnode);
	quads[3] = Vector2f(yStart + nodeWidth, xStart);

	/*	node->quad1[0] = yStart;
	node->quad1[1] = xStart;
	node->quad2[0] = yStart;
	node->quad2[1] = xStart + wnode;
	node->quad3[0] = yStart + nodeWidth;
	node->quad3[1] = xStart + wnode;
	node->quad4[0] = yStart + nodeWidth;;
	node->quad4[1] = xStart;
	*/
      }
      dNode->setQuads(quads);
       
      // Get the color of the node
      ColorF color;
      if(data.colormapScheme == "entropy"){
	//std::cout << "label weight: " << data.labelWeights.size() << std::endl;
	color = dynamic_cast<TwoDDiscreteColormap*>(data.colormap)->
	  getColor(data.labelIndex[node->label], node->entropy);
      }
      else if(data.colormapScheme == "ratio")
	color = data.colormap->getColor(node->ratio);
      else if(data.colormapScheme == "other"){
	std::cout << "not sure on other!" << std::endl;
	color = red;
      }
      dNode->setColor(color);
      node->color = color;

      // Add the display node to the tree
      data.displayTree[dNode->getIndex()] = dNode;
      
      std::vector< GMRANode<TPrecision>* > children = node->getChildren();
      for(typename std::vector< GMRANode<TPrecision> * >::iterator it =
            children.begin(); it != children.end(); ++it){
	nodes.push_back(*it);
      }
      // nodes.clear();
      //std::cout << "done!" << std::endl;
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
      
      // Get the associated display node
      DisplayNode * dNode = data.displayTree[node->ID];

      // Get the color of the node
      // glColor4f(node->color.r(), node->color.g(), node->color.b(), alpha);
      float alpha = 1.00;
      //  if(data.selectedNode == dNode->getIndex())
      // 	alpha = 1.0;
      ColorF col = dNode->getColor();
      glColor4f(col.r(), col.g(), col.b(), alpha);

      // Draw the quad
      std::array<Vector2f, 4> quads = dNode->getQuads();
      glPushName( dNode->getIndex());
      glBegin(GL_QUADS);        
      glVertex2f(quads[0][0], quads[0][1]);
      glVertex2f(quads[1][0], quads[1][1]);
      glVertex2f(quads[2][0], quads[2][1]);
      glVertex2f(quads[3][0], quads[3][1]);					
      glEnd();    
      glPopName();         
      
      // If this node is the selected one, highlight it
      if(data.selectedNode == dNode->getIndex()){
	glLineWidth(2);
	glColor3f(backgroundColor[0]+.1, backgroundColor[1]+.1, backgroundColor[2]+.1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(quads[0][0], quads[0][1]);
	glVertex2f(quads[1][0], quads[1][1]);
	glVertex2f(quads[2][0], quads[2][1]);
	glVertex2f(quads[3][0], quads[3][1]);					
	glEnd();  

	selectedScale = node->getScale();
      }
	

      
      // Save the selected node info
      /*  if( node->ID  == data.selectedNode){
	selectedQ1[0] = node->quad1[0];
	selectedQ1[1] = node->quad1[1];
	selectedQ2[0] = node->quad2[0];
	selectedQ2[1] = node->quad2[1];
	selectedQ3[0] = node->quad3[0];
	selectedQ3[1] = node->quad3[1];
	selectedQ4[0] = node->quad4[0];
	selectedQ4[1] = node->quad4[1];

	selectedScale = node->getScale();
	}*/

      // Get the node's children
      std::vector< GMRANode<TPrecision>* > children = node->getChildren();
      for(typename std::vector< GMRANode<TPrecision> * >::iterator it =
            children.begin(); it != children.end(); ++it){
	nodes.push_back(*it);
      }
      // nodes.clear();
    }    
    
    // Draw the selection rectangle
    /* glLineWidth(1);
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
    */
    
    // Highlight the part of the tree selected
    if(data.selectedNode != -1){  

      /*  std::list<GMRANode<TPrecision> *> nodesSelect;
      nodesSelect.push_back( (VisGMRANode<TPrecision> *) data.nodeMap[data.selectedNode]);
      while( !nodesSelect.empty()){           
	
	VisGMRANode<TPrecision> *nodeSelect = dynamic_cast<VisGMRANode<TPrecision> *>( nodesSelect.front() );
	nodesSelect.pop_front();
	
	
	// Get the color of the node
	glColor3f(nodeSelect->color[0], nodeSelect->color[1], nodeSelect->color[2]);
	glLineWidth(1);
	glBegin(GL_QUADS);        
	glVertex2f(nodeSelect->quad1[0], nodeSelect->quad1[1]);
	glVertex2f(nodeSelect->quad2[0], nodeSelect->quad2[1]);			
	glVertex2f(nodeSelect->quad3[0], nodeSelect->quad3[1]);				
	glVertex2f(nodeSelect->quad4[0], nodeSelect->quad4[1]);					
	glEnd(); 
      */
	// Draw the quad     
	/*	glLineWidth(1);
	glColor3f(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
	glBegin(GL_LINE_LOOP);        
	glVertex2f(nodeSelect->quad1[0], nodeSelect->quad1[1]);
	glVertex2f(nodeSelect->quad2[0], nodeSelect->quad2[1]);			
	glVertex2f(nodeSelect->quad3[0], nodeSelect->quad3[1]);				
	glVertex2f(nodeSelect->quad4[0], nodeSelect->quad4[1]);					
	glEnd(); 
	*/

	// Get the node's children
      /*	std::vector< GMRANode<TPrecision>* > children = nodeSelect->getChildren();
	for(typename std::vector< GMRANode<TPrecision> * >::iterator it =
	      children.begin(); it != children.end(); ++it){
	  nodesSelect.push_back(*it);
	}
	}*/


      // Draw the selection rectangle
      /* glLineWidth(2);
      if(selectedScale != -1){
	// glColor3f(0, 1, 0);
	glColor3f(backgroundColor[0]+.1, backgroundColor[1]+.1, backgroundColor[2]+.1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(selectedQ1[0], selectedQ1[1]);
	glVertex2f(selectedQ2[0], selectedQ2[1]);
	glVertex2f(selectedQ3[0], selectedQ3[1]);
	glVertex2f(selectedQ4[0], selectedQ4[1]);
	glEnd();      
	} */   
    }
  }
  
  // --- Mouse actions --- //
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
        int tmp = selectBuf[3];
        if(tmp != -1){
          selected = tmp;
	  data.selectedIndex = -1;
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
