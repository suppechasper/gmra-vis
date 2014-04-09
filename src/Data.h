#ifndef DATA_H
#define DATA_H

#include "DenseMatrix.h"
#include "Linalg.h"
#include "IPCATree.h"
#include "colormapper.h"

#include "GMRATree.h"

#include <vector>
#include <map>
#include <set>
#include <algorithm>



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

};





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







template<typename TPrecision>
class Data{

  public:
    IPCATree<TPrecision> &tree;
    FortranLinalg::DenseVector<int> labels; 
    FortranLinalg::DenseMatrix<TPrecision> points;

    //Current scatter plot projection
    FortranLinalg::DenseMatrix<TPrecision> V;
    FortranLinalg::DenseVector<TPrecision> center;

    //Projected and scaled points
    FortranLinalg::DenseMatrix<TPrecision> P;

    std::vector< VisGMRANode<TPrecision> * > currentScale;

    int nScales;
    std::list<int> currentSubScales;
    
    std::map<int, VisGMRANode<TPrecision> *> nodeMap;
    int maxScale;

    int selectedNode;
    int selectedIndex;


    TPrecision minPatch;
    TPrecision maxPatch;

    ColorMapper<float> treeColor;

    Data( IPCATree<TPrecision> &t, FortranLinalg::DenseVector<int> &l, FortranLinalg::DenseMatrix<TPrecision> X) : 
      tree(t), labels(l), points(X) {
        using namespace FortranLinalg;


        minPatch = Linalg<double>::MinAll(X);
        maxPatch = Linalg<double>::MaxAll(X);
        maxScale = -1;
        selectedNode = -1;
        selectedIndex = -1;
        
        IPCANode<TPrecision> *root = (IPCANode<TPrecision>*) tree.getRoot();
        V = Linalg<TPrecision>::ExtractColumns(root->phi, 0, 2 );
        
        center = Linalg<TPrecision>::Copy(root->center);
       



        tree.setRoot( decorate(root, NULL) );
        
        //Compare densities and store statistics for each node
        std::list<GMRANode<TPrecision> *> nodes;
        nodes.push_back( tree.getRoot() );


        std::list<int> scales;
        scales.push_back(0);

        std::list<int> xpos;
        xpos.push_back(0);



        int nodeCount = 0;
        while( !nodes.empty() ){
          VisGMRANode<TPrecision> *node = (VisGMRANode<TPrecision> *) nodes.front();
          nodes.pop_front();

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

          std::vector<int> pts = node->getPoints();

          int nOrig = 0;
          for(std::vector<int>::iterator it = pts.begin(); it != pts.end(); ++it){
            nOrig += labels(*it);
          }  

          node->xPos= pos;
          node->nOrig = nOrig;
          node->nPoints = pts.size();
          node->ratio = nOrig / (double) pts.size();
          node->ID= nodeCount;

          if(maxScale < scale){
            maxScale = scale;
          }

          nodeMap[nodeCount] = node;
          nodeCount++;
        }

      };


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
