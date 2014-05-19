#ifndef DATA_H
#define DATA_H

#include "DenseMatrix.h"
#include "Linalg.h"
#include "IPCATree.h"
#include "ColorMap.h"
#include "GMRATree.h"
#include "ColorMapFactories.h"
#include "Vector.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>


// * * * Display Node * * * //
class DisplayNode{
  
public:
  DisplayNode(int index){ nodeIndex = index; }
  
  void setColor(ColorF col) { _color = col; }
  void setSelectecColor(ColorF col) { _selectedColor = col;}
  void setQuads(std::array<Vector2f, 4> q){ quads = q; }
  
  int getIndex() { return nodeIndex; }
  std::array<Vector2f, 4> getQuads() { return quads; }
  ColorF getColor(){ return _color; }

protected:
  ColorF _color;
  ColorF _selectedColor;
  std::array<Vector2f, 4> quads;
  float quad1[2];
  float quad2[2];
  float quad3[2];
  float quad4[2];
  int nodeIndex;
};

// * * * VisGMRANode * * * //
template< typename TPrecision>
class VisGMRANode : public GMRANodeDecorator<TPrecision>{

public:
  VisGMRANode(GMRANode<TPrecision> *n) : 
     GMRANodeDecorator<TPrecision>(n){

    nPoints = this->getPoints().size();
    nOrig = nPoints;
    ratio = 1.0;
  };

  int ID;
  int xPos;
  int nPoints;
  int nOrig;
  double ratio;
  double entropy;
  int numLabels;
  int label; // the maximal probability label

  // Visualization variables
  // float color[3];
  ColorF color;
  float quad1[2];
  float quad2[2];
  float quad3[2];
  float quad4[2];

  double wnode;
  double xStart;


};

// * * * ExtractScale * * * //
template <typename TPrecision>
class ExtractScale : public Visitor<TPrecision>{

private:
  std::vector< VisGMRANode<TPrecision> * > atScale;

  int scale;

public:

  ExtractScale(int s){
    scale = s;
  };

  ~ExtractScale(){
  };

  void init(int s){
    scale=s;
  }

  void visit(GMRANode<TPrecision> *node){
    if(node->getScale() == scale){
      atScale.push_back( dynamic_cast<VisGMRANode<TPrecision> *>( node ) );
    }
  };

  std::vector<VisGMRANode<TPrecision> * > getNodes(){
    return atScale;
  };

};


// * * * Data * * * //
template<typename  TPrecision, typename LabelType>
class Data{
  
public:
  IPCATree<TPrecision> &tree;
  
  // The label of each of the points
  FortranLinalg::DenseVector<LabelType> labels; 
  
  // The data points
  FortranLinalg::DenseMatrix<TPrecision> points;
  
  //Current scatter plot projection
  FortranLinalg::DenseMatrix<TPrecision> V;
  FortranLinalg::DenseVector<TPrecision> center;
  
  //Projected and scaled points
  FortranLinalg::DenseMatrix<TPrecision> P;
  
  std::vector< VisGMRANode<TPrecision> * > currentScale;
  
  // The icicle display tree
  std::map<int, DisplayNode* > displayTree;

  // Scales 
  int nScales;
  std::list<int> currentSubScales;
  int maxScale;    
  
  // A map of the nodes
  std::map<int, VisGMRANode<TPrecision> *> nodeMap;
  
  // The selected node
  int selectedNode;
  int selectedIndex;

  ColorF selectedNodeColor;

  // Min/Max values for colormapping
  int minRatio, maxRatio;  
  double minEntropy, maxEntropy;
  std::vector<float> minCenter, maxCenter;
  float minCent, maxCent;

  int maxNodeSize;

  TPrecision minPatch;
  TPrecision maxPatch;
  
  // Colormaps 
  std::map<LabelType, int> labelIndex;
  std::string colormapScheme;
  Colormap * colormap;
  Colormap * pcColors;
   
  //--- Constructor ---//
 Data( IPCATree<TPrecision> &t,
       FortranLinalg::DenseVector<LabelType> &l, 
       FortranLinalg::DenseMatrix<TPrecision> X,
       std::string colormapSchemeIn) 
   : tree(t), labels(l), points(X), maxScale(-1), 
    selectedNode(-1), selectedIndex(-1), selectedNodeColor(white), minRatio(1), maxRatio(1),
    colormapScheme(colormapSchemeIn) {
   using namespace FortranLinalg;

   minPatch = Linalg<double>::MinAll(X);
   maxPatch = Linalg<double>::MaxAll(X);
       
   // Get the root of the tree
   IPCANode<TPrecision> *root = (IPCANode<TPrecision>*) tree.getRoot();
   V = Linalg<TPrecision>::ExtractColumns(root->phi, 0, 2 );
   center = Linalg<TPrecision>::Copy(root->center);
   tree.setRoot( decorate(root, NULL) );
        
   //Compare densities and store statistics for each node
   std::list<GMRANode<TPrecision> *> nodes;
   nodes.push_back( tree.getRoot() );

   // Find all of the unique labels
   std::map<LabelType, double> labelWeights;
   for(int i = 0; i < labels.N(); i++){
     typename std::map<LabelType, double>::iterator it = labelWeights.find(labels(i));
     if(it != labelWeights.end())
       it->second += 1.0;
     else
       labelWeights[labels(i)] = 1.0;
   }

   // Find the weights and fill the colormap index map
   int count = 0;
   for(typename std::map<LabelType, double>::iterator it = labelWeights.begin(); it != labelWeights.end(); ++it){
     it->second = 1.0/it->second;	
     labelIndex[it->first] = count;
     count ++;
   }

   // Set the min and max 
   minEntropy = 0;
   maxEntropy = -log(1.0/(double)labelWeights.size());
   
   std::list<int> scales;
   scales.push_back(0);
	
   std::list<int> xpos;
   xpos.push_back(0);

   // Set the colormap for the parallel coordinates
   ColormapFactory * factory = new PairedDiscreteColormapFactory(root->sigma.N());
   pcColors = factory->getColormap();
   

   // Find the min/max center values
   minCenter.resize(root->getCenter().N());
   maxCenter.resize(root->getCenter().N());
   FortranLinalg::DenseVector<TPrecision> center = root->getCenter();
   TPrecision maxC = FortranLinalg::Linalg<TPrecision>::Max(center);
   TPrecision minC = FortranLinalg::Linalg<TPrecision>::Min(center);
   FortranLinalg::DenseMatrix<TPrecision> phi = root->phi;
   TPrecision maxP = FortranLinalg::Linalg<TPrecision>::MaxAll(phi);
   FortranLinalg::DenseVector<TPrecision> sigma = root->sigma;
   TPrecision maxS = FortranLinalg::Linalg<TPrecision>::Max(sigma);
   minCent = minC - maxP*(3*maxS);
   maxCent = maxC + maxP*(3*maxS);
   
   maxNodeSize = 0;
   int nodeCount = 0;
   while( !nodes.empty() ){
     VisGMRANode<TPrecision> *node = (VisGMRANode<TPrecision> *) nodes.front();
     IPCANode<TPrecision> *inode = dynamic_cast<IPCANode<TPrecision> *>(node->getDecoratedNode());
     nodes.pop_front();

     // Find the maximum number of points in a node
     if(node->getPoints().size() != tree.getRoot()->getPoints().size())
     maxNodeSize = maxNodeSize > node->getPoints().size() ? maxNodeSize : node->getPoints().size();

     int scale = scales.front();
     scales.pop_front();
	  
     int pos = xpos.front();
     xpos.pop_front();

     std::vector< GMRANode<TPrecision>* > children = node->getChildren();
     int offset = 0;
     for(typename std::vector< GMRANode<TPrecision> * >::iterator it =
	   children.begin(); it != children.end(); ++it){
       scales.push_back(scale+1);
       nodes.push_back(*it);
       xpos.push_back(pos+offset);
       offset += (*it)->getPoints().size();
     }

     // Get the points in the node
     std::vector<int> pts = node->getPoints();

     int nOrig = 0;
     for(std::vector<int>::iterator it = pts.begin(); it != pts.end(); ++it){
       nOrig += labels(*it);
     }  
    
     //--- Calculate the probability of each tissue type in this node ---//
     
     // Find the number of pts in each label in this node
     std::map<int, double> probs;
     for(std::vector<int>::iterator it = pts.begin(); it != pts.end(); ++it){
       std::map<int, double>::iterator labelKey = probs.find(labels(*it));
       if(labelKey != probs.end())
	 labelKey->second += 1.0;
       else
	 probs[labels(*it)] = 1.0;
     }
     
     // Multiply by label weights, save the sum
     double sum = 0.0;
     for(std::map<int, double>::iterator it = probs.begin(); it != probs.end(); ++it){
       it->second = (double)it->second*labelWeights[it->first];
       sum += it->second;
     }
     
     // Divide by the sum
     double check2 = 0.0;
     for(std::map<int, double>::iterator it = probs.begin(); it != probs.end(); ++it){
       it->second /= sum;
       check2 += it->second;
     }
	 
     // Find the label with max probability
     int maxLabel = probs.begin()->first;
     float maxProb = probs.begin()->second;
     for(std::map<int, double>::iterator it = probs.begin(); it != probs.end(); ++it)
       if(it->second > maxProb){
	 maxLabel = it->first;
	 maxProb = it->second;
       }
     
     // Calculate an entropy value here
     double entropy = 0.0;
     for(std::map<int, double>::iterator it = probs.begin(); it != probs.end(); ++it)
       entropy += it->second*log(it->second); 
     if(entropy != 0.0)
       entropy *= -1.0;
     
     // Save the node information
     node->xPos= pos;
     node->nOrig = nOrig;
     node->nPoints = pts.size();
     node->ratio = nOrig / (double) pts.size();
     node->ID= nodeCount;
     node->entropy = entropy;
     node->label = maxLabel;
     node->numLabels = probs.size();
     
     // Save the data-wide information
     minRatio = minRatio < node->ratio ? minRatio : node->ratio;
     maxRatio = maxRatio > node->ratio ? maxRatio : node->ratio;
     maxScale = maxScale > scale ? maxScale : scale;
     
     nodeMap[nodeCount] = node;
     nodeCount++;
   }

   // Set the colormap for the passed in scheme
   if(colormapScheme == "entropy"){
     ColormapFactory * factory;
     if(labelWeights.size() < 8)
       factory = new Set1DiscreteGreyscaleColormapFactory(labelWeights.size());
     else
       factory = new Paired12DiscreteGreyscaleColormapFactory(labelWeights.size());
     colormap = factory->getColormap();
     dynamic_cast<TwoDDiscreteColormap*>(colormap)->setFactoryRange(minEntropy, 
								    maxEntropy);
   }
   else if(colormapScheme == "ratio"){
     ColormapFactory * factory = new YellowWhiteBlueColormapFactory();
       colormap = factory->getColormap();
       colormap->setRange(minRatio, maxRatio);
   }
   else if(colormapScheme == "other"){
     std::cout << "implement other color technique!" << std::endl;
   }
 }
  
  //--- Set the selected node ---//
  void setSelected(int index){

  
    using namespace FortranLinalg;

    selectedNode = index;
    P.deallocate();
    if(selectedNode != -1){
      VisGMRANode<TPrecision> *vnode  = dynamic_cast<VisGMRANode<TPrecision> *>( nodeMap[selectedNode] );
      IPCANode<TPrecision> *node= dynamic_cast<IPCANode<TPrecision> *>( vnode->getDecoratedNode() );

      if(node != NULL){
	std::vector<int> pts = node->getPoints();
	DenseMatrix<TPrecision> XP(points.M(), pts.size() );
	for(int i=0; i<pts.size(); i++){
	  Linalg<TPrecision>::SetColumn(XP, i, points, pts[i]);
	} 

	DenseMatrix<TPrecision> phi(XP.M(), 2);
	Linalg<TPrecision>::Zero(phi);
	for(int i=0; i<std::min(2, (int) node->phi.N()); i++ ){
	  Linalg<TPrecision>::SetColumn(phi, i, node->phi, i);
	}

	Linalg<TPrecision>::SubtractColumnwise(XP, node->center, XP);
	P = Linalg<TPrecision>::Multiply(phi, XP, true);

	XP.deallocate();
	phi.deallocate(); 

	TPrecision maxP = Linalg<TPrecision>::MaxAll(P);
	TPrecision minP = Linalg<TPrecision>::MinAll(P);
	TPrecision scaling = std::max(fabs(maxP), fabs(minP));

	Linalg<TPrecision>::Scale(P, 1.0/scaling, P);
	ExtractScale<TPrecision> extract( vnode->getScale() );
	tree.breadthFirstVisitor( &extract);
	currentScale = extract.getNodes();
      }
    }
    else{
      P = DenseMatrix<TPrecision>();  
    }
  };

  void setProjection(FortranLinalg::DenseVector<TPrecision> &v, int index){
    FortranLinalg::Linalg<TPrecision>::SetColumn(V, index, v);
  };
  
  
private:
  
  GMRANode<TPrecision> *decorate(GMRANode<TPrecision> *gnode,  GMRANode<TPrecision> *parent){
    using namespace FortranLinalg;
    
    VisGMRANode<TPrecision> *vnode = new VisGMRANode<TPrecision>( gnode );          
      vnode->setParent(parent);

      std::vector< GMRANode<TPrecision>* > &children = vnode->getChildren();
      for( int i=0; i < children.size(); i++ ){ 
        children[i] =  decorate( children[i], vnode );
      }

      return vnode;

    };

};


#endif
