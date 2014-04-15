#ifndef PARALLELDEL_H
#define PARALLELDEL_H

#include "DisplayElement.h"
#include "Data.h"
#include <map>

template<class TypeIn, class TypeOut>                              // affine()
inline TypeOut affine(const TypeIn &i,  const TypeIn &val, const TypeIn &I, const TypeOut &o, const TypeOut &O){
    if (i == I) return O; else return (O - o)*(val - i) / (I - i) + o; }

template<typename TPrecision>
class ParallelDEL : public DisplayElement{

 private:
  
  // The picking buffer
#define BUFSIZE 512
  
  // The data
  Data<TPrecision> &data;

 public:
  
  //--- Constructor ---//
  ParallelDEL(Data<TPrecision> &d) : DisplayElement(), data(d){
    pickW = pickH = 2;
    // createCoordinates();
  }

  //--- Create display ---//
  void createCoordinates(){
    
    // Get the root of the tree
    VisGMRANode<TPrecision> *rootNode = (VisGMRANode<TPrecision> *) data.tree.getRoot();
    std::list<GMRANode<TPrecision> *> nodes;
    nodes.push_back(rootNode);


    // Set the size of the min/max center arrays
    // int N = dynamic_cast<IPCANode<TPrecision> *>(rootNode->getDecoratedNode())->getCenter().N();      
    //data.minCenter.resize(N);
    //data.maxCenter.resize(N);

    // Iterate through the nodes
    while( !nodes.empty()){   
      VisGMRANode<TPrecision> *vnode = dynamic_cast<VisGMRANode<TPrecision> *>( nodes.front() );
      IPCANode<TPrecision> *node = dynamic_cast<IPCANode<TPrecision> *>(vnode->getDecoratedNode());      
      nodes.pop_front();
     
      std::vector< GMRANode<TPrecision>* > children = node->getChildren();
      for(typename std::vector< GMRANode<TPrecision> * >::iterator it =
            children.begin(); it != children.end(); ++it){
	nodes.push_back(*it);
      }
    }

    // typename std::map<int, VisGMRANode<TPrecision>* >::iterator nodeMapIt = data.nodeMap.begin();
    //while(nodeMapIt != data.nodeMap.end()){
    // VisGMRANode<TPrecision> *rootNode = (VisGMRANode<TPrecision> *) (data.tree.getRoot());
    // IPCANode<TPrecision> *node = dynamic_cast<IPCANode<TPrecision> *>( rootNode->getDecoratedNode() );//;//data.nodeMap[nodeMapIt->first] );
    
    //std::cout << "rootNode; " << rootNode->ID << std::endl;
    // std::cout << "nodemap " << nodeMapIt->first << std::endl;
    /* if(node != NULL){
      
      std::cout << "here: " << node->getPoints().size() << std::endl;
      

      // Get the phi matrix
      FortranLinalg::DenseMatrix<TPrecision> phi = node->phi;

      std::cout << "m: " << phi.M() << " n: " << phi.N() << std::endl;
    
      std::cout << "cneter: " << node->getCenter().N()<< std::endl;
      }*/
    //++nodeMapIt;
    // }

  }
  
  //--- Set the location & dimensions of the display ---//
  void location(int xPos, int yPos, int w, int h){

    // The position of the tree on the screen
    xLeft = xPos;
    yTop = yPos;

    width = w; 
    height = h;
  }

  //--- How to display the plot ---//
  void display(){

    glMatrixMode(GL_MODELVIEW); 	
    glLoadIdentity();
      
    //glPushName(-1); 
    glPointSize(6);
    int x1;
    int x2;
    float col[4];
    //col[3] = alpha;

    if(data.selectedNode != -1){
    
      VisGMRANode<TPrecision> *vnode = dynamic_cast<VisGMRANode<TPrecision> *>(data.nodeMap[data.selectedNode]);
      IPCANode<TPrecision> *node = dynamic_cast<IPCANode<TPrecision> *>(vnode->getDecoratedNode());
      
      FortranLinalg::DenseVector<TPrecision> center = node->getCenter();
      FortranLinalg::DenseMatrix<TPrecision> phi = node->phi;
      FortranLinalg::DenseVector<TPrecision> sigma = node->sigma;
      /* std::cout << "center.N(): " << center.N() << std::endl;
	 std::cout << "(rows) phi.M(): " << phi.M() << " phi.N(): " << phi.N() << std::endl; 
	 std::cout << "sigma.N(): " << sigma.N() << std::endl;*/
      
      int numAxes = center.N();
      int space = width/numAxes;
      
      // Draw the axes
      glLineWidth(1);
      glColor3f(0.75, 0.75, 0.75);
      glBegin(GL_LINES);
      double xStart = xLeft;
      for(int i = 0; i < numAxes; i++){
	glVertex2f(xStart, yTop);
	glVertex2f(xStart, yTop+height);
	xStart += space;
      }
      glEnd();
  
     
      // Draw the + std dev
      // std::cout << "signa: " << sigma.N() << std::endl;
      glLineWidth(1);
      // glColor3f(1.0, 0.25, 0.25);
      //      for(int pc = 0; pc < sigma.N(); pc++){
	for(int pc = 0; pc < 1; pc++){
	Tuple color = data.pcColors.getColor(pc);
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_STRIP);
	xStart = xLeft;
	for(int d = 0; d < center.N(); d++){
	  float ptLocation = affine(data.minCenter[d], (float)(center(d) + phi(d, pc)*(sigma(pc)*sigma(pc)*sigma(pc))), 
				    data.maxCenter[d], yTop, yTop+height);  
	  glVertex2f(xStart,ptLocation);
	  xStart += space;
	}
	glEnd();
      }
      
      // Draw the - std dev
      glLineWidth(1);
      //for(int pc = 0; pc < sigma.N(); pc++){
      for(int pc = 0; pc < 1; pc++){
	Tuple color = data.pcColors.getColor(pc);
	glColor3f(color.r-.35, color.g-.35, color.b-.35);
	glBegin(GL_LINE_STRIP);
	xStart = xLeft;
	for(int d = 0; d < center.N(); d++){
	  float ptLocation = affine(data.minCenter[d], (float)(center(d) - phi(d, pc)*(sigma(pc)*sigma(pc)*sigma(pc))), 
				    data.maxCenter[d], yTop, yTop+height);  
	  glVertex2f(xStart,ptLocation);
	  xStart += space;
	}
	glEnd();
	}
      
      // Draw the central line
      glLineWidth(2);
      glBegin(GL_LINE_STRIP);
      glColor3f(0.75, 0.75, 0.75);
      xStart = xLeft;
      for(int i = 0; i < center.N(); i++){
	float ptLocation = affine((float)data.minCenter[i], (float)center(i), (float)data.maxCenter[i], yTop, yTop+height);  
	glVertex2f(xStart, ptLocation);
	xStart += space;
      }
      glEnd();
    
    }
  }

  //--- Keyboard action ---//
  void keyboard(unsigned char key, int x, int y){
  }
 
  //--- Special ---//
  void special(int key, int x, int y){}

  //--- Mouse action ---//
  void mouse(int button, int state, int x, int y){}

  //--- Motion action ---//
  virtual void motion(int x, int y){}
  
  //--- Passive action ---//
  virtual void passive(int x, int y){}




};

#endif
