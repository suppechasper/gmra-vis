#ifndef MULTISCALEPROJECTIONDEL_H
#define MULTISCALEPROJECTIONDEL_H

#include "Display.h"
#include "DenseVector.h"
#include "DenseMatrix.h"

#include "DoubleRotation.h"

#include <math.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#include "Font.h"
#include "ColorDefines.h"

template<typename TPrecision, typename LabelType>
class MultiscaleProjectionDEL : public DisplayElement{

  private:


#define BUFSIZE 512


    GLuint selected;

    int size; 

    int xM, yM;
    int mod, cur_button; 
    int pickH, pickW;  
    float alpha;

    int maxRadius;

    int nScales;

    Data<TPrecision, LabelType> &data;

    FortranLinalg::DenseMatrix<TPrecision> P;

    //Circle
    FortranLinalg::DenseMatrix<TPrecision> B;

    DoubleRotation<TPrecision, LabelType> rotation;
    Animator &animator;

    std::set<int> nodes;

    bool drawChildren;
  
  public:

 MultiscaleProjectionDEL(Data<TPrecision, LabelType> &d, Animator &anim) 
      : data(d), animator(anim), rotation(d){
        using namespace FortranLinalg;

        pickW = 5;
        pickH = 5;
        alpha = 0.1;


        selected = 0;

        //Set inital projection to root pca plane
        VisGMRANode<TPrecision> *vroot = (VisGMRANode<TPrecision>*) data.tree.getRoot();
        IPCANode<TPrecision> *root = (IPCANode<TPrecision>*) vroot->getDecoratedNode();
        P = Linalg<TPrecision>::ExtractColumns(root->phi, 0, 2);

        maxRadius = root->getL2Radius();

        //Sample unit circle
        B = DenseMatrix<TPrecision>(2, 40);
        for(int i=0; i<B.N(); i++){
          B(0, i) = cos( (2.0*i)/B.N() * M_PI );
          B(1, i) = sin( (2.0*i)/B.N() * M_PI );
        }


        drawChildren = true;
        selected = -1; 
        nScales = 2;

      };



    void location(int xPos, int yPos, int w, int h){
      width = w;
      height = h;
      xLeft = xPos;
      yTop = yPos;
      size = std::min(w, h)/2;
    };



    void init(){  

    };


    void display(void){

      if(data.selectedNode != -1){
        glMatrixMode(GL_MODELVIEW); 	
        glLoadIdentity();

        glPointSize(6);
        int x1;
        int x2;



        glLineWidth(2);

          //Note this only works since the GMRA tree is an IPCA tree. This can
          //be made independandt by creating a GWT and retrieving the GWTInfo
          //from the tree. WOuld ahve to be consolidated with the VisInfo that
          //is currently stored instead of the GWTInfo


	drawNode(0, 0.25);
/*
        for(std::set<int>::iterator it = nodes.begin();
            it != nodes.end(); ++it){

          int nodeID = *it;
          float alpha = 0.5;

          if(nodeID == data.selectedNode ){
            alpha = 1;
          }

          drawNode(nodeID, alpha);
        }

*/

        if(drawChildren){
          drawScale(data.nodeMap[data.selectedNode], 0);
        }
        else{
          for(typename std::vector< VisGMRANode<TPrecision> * >::iterator it = data.currentScale.begin();
              it != data.currentScale.end(); ++it){

            VisGMRANode<TPrecision> *node = *it;
            float alpha = 0.5;

            if(node->ID == data.selectedNode ){
              alpha = 1;
            }

            drawNode(node, alpha);
          }

        }






        if(isInside(xM, yM)){
          //draw pick rectangle
          glColor4f(0.75, 0.75, 0.75, 0.75);
          glLineWidth(2);
          double pw = pickW / 2.0;
          double ph = pickH / 2.0;
          glBegin(GL_LINE_LOOP);
          glVertex2f( xM-pw, yM-ph );
          glVertex2f( xM+pw, yM-ph );
          glVertex2f( xM+pw, yM+ph );
          glVertex2f( xM-pw, yM+ph );
          glEnd();
        }

      }
    };


    void reshape(int w, int h){

    };    


    void idle(){

    };


    void keyboard(unsigned char key, int x, int y){
      //std::vector<int> keep;
      if(!isInside(x, y)){ return; };
      switch(key)
      {
        case '+':
          nScales++;
          break;
        case '-':
          nScales--;
          if(nScales < 1){
            nScales = 1;
          }
          break;
      }
      glutPostRedisplay();
    };



    void special(int key, int x, int y){
      switch(key)
      {
        case GLUT_KEY_LEFT:
          alpha -= 0.01;
          break;
        case GLUT_KEY_RIGHT:
          alpha += 0.01;
          break;
      }
      if(alpha < 0){
        alpha=0;
      }
      else if(alpha > 1){
        alpha=1;
      }

      glutPostRedisplay();
    };




    void mouse(int button, int state, int x, int y){
        using namespace FortranLinalg;
      xM = x;
      yM = y;
      if(!isInside(x, y)){ return; };

      if(selected != -1 ){
        VisGMRANode<TPrecision> *vnode = (VisGMRANode<TPrecision> *) data.nodeMap[selected];
        if(vnode != NULL){
          IPCANode<TPrecision> *node= dynamic_cast<IPCANode<TPrecision> *>( vnode->getDecoratedNode() );

          if(button == GLUT_LEFT_BUTTON){
            //data.setSelected(selected);

            DenseVector<TPrecision> t = 
              Linalg<TPrecision>::ExtractColumn(node->phi, 0);
            rotation.setTarget1(t);

            DenseVector<TPrecision> t2 =
              Linalg<TPrecision>::ExtractColumn(node->phi, 1);
            rotation.setTarget2(t2);

            animator.setAnimation(&rotation);
          }
          else if(button == GLUT_RIGHT_BUTTON){
            if(glutGetModifiers() == GLUT_ACTIVE_CTRL){
              nodes.erase(selected);
            }
            else{
              std::vector<GMRANode<TPrecision> *> children = node->getChildren();
              for(typename std::vector<GMRANode<TPrecision> *>::iterator it = children.begin(); it !=
                  children.end(); ++it){
                VisGMRANode<TPrecision> *node = dynamic_cast<VisGMRANode<TPrecision> *>( *it );
                nodes.insert(vnode->ID);
              } 
            }
          }
        }

        selected = -1;
      }
    };





    // catch mouse move events
    void motion(int x, int y){};




    void passive(int x, int y){

      xM = x;
      yM = y;

      if(!isInside(x, y)){ return; };

      GLint vp[4];
      glGetIntegerv(GL_VIEWPORT, vp);
      GLuint selectBuf[512];
      glSelectBuffer(512, selectBuf);
      glRenderMode(GL_SELECT);
      glInitNames();
      glPushName(-1);

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluPickMatrix(x, vp[3]-y, pickW, pickH, vp);
      glOrtho (0, vp[2], vp[3], 0, 0, 1);

      display();
      GLint hits = glRenderMode(GL_RENDER);

      selected = -1;
      for(int i=0; i<hits; i++){
        GLuint tmp = selectBuf[i*4 + 3];
        if(tmp != -1){
          selected = tmp;
          break;
        }
      }


      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glutPostRedisplay();


    };



  private:

    double toScreenX(TPrecision x){
      return xLeft + ( x/(maxRadius) + 1.0) * size; 
    };

    double toScreenY(TPrecision y){
      return yTop + ( y/(maxRadius)  + 1.0) * size; 
    };

    double toDataX(int x){
      return ( (x - xLeft )  / (double) size -1.0 ) * maxRadius;
    };

    double toDataY(int y){
      return ( (y - yTop) / (double) size - 1.0 ) * maxRadius;
    }; 


    void drawScale(GMRANode<TPrecision> *node, int current){
      if(current >= nScales){
        return;
      }
      drawNode(node, 1);
      for(typename std::vector<GMRANode<TPrecision> *>::iterator it = node->getChildren().begin(); it
          != node->getChildren().end(); ++it){
        drawScale(*it, current+1);
      } 
    };

    void drawNode(int nodeID, float alpha){
      drawNode(data.nodeMap[nodeID], alpha);
    };

    void drawNode(GMRANode<TPrecision> *gnode, float alpha){

      using namespace FortranLinalg;
      VisGMRANode<TPrecision> *vnode  = dynamic_cast<VisGMRANode<TPrecision> *>( gnode);
      IPCANode<TPrecision> *node= dynamic_cast<IPCANode<TPrecision> *>( vnode->getDecoratedNode() );


     
     /*
      IPCANode<TPrecision> *selectNode = (IPCANode<TPrecision> *)
        data.nodeMap[data.selectedNode];
      VisInfo *svnode = (VisInfo*) selectNode->getGWTInfo();
      int refScale = svnode->scale;
      double d = svnode-scale - vnode->scale;
      */



      //Compute projection of this nodes covariance matrix
      DenseMatrix<TPrecision> A = Linalg<TPrecision>::Copy(node->phi);
      for(int i=0; i < A.N(); i++){
        Linalg<TPrecision>::ScaleColumn(A, i, sqrt(2*node->sigma(i)) );
      }
      DenseMatrix<TPrecision> C1 = Linalg<TPrecision>::Multiply(data.V, A, true);

      DenseMatrix<TPrecision> C = Linalg<TPrecision>::Multiply(C1, C1, false,
          true);

      //Compute svd in projected space
    //  SVD<TPrecision> svd(C);

      //and transform the unit circle according to projected covariance
   //   DenseMatrix<TPrecision> BC1 = Linalg<TPrecision>::Copy(B);//Multiply(svd.U, B, true);
   //   for(int i=0; i<svd.S.N(); i++){
   //     Linalg<TPrecision>::ScaleRow(BC1, i, 3*svd.S(i) );
   //   }
   //   DenseMatrix<TPrecision> BC = Linalg<TPrecision>::Multiply(svd.U, BC1);
      DenseMatrix<TPrecision> BC = Linalg<TPrecision>::Multiply(C, B);

      //Project mean
      DenseVector<TPrecision> xm = Linalg<TPrecision>::Multiply(data.V,
          node->center, true);

      DenseVector<TPrecision> xrm = Linalg<TPrecision>::Multiply(data.V,
          data.center, true);

      //Projection distance
      DenseVector<TPrecision> xmp = Linalg<TPrecision>::Multiply(data.V, xm);
      Linalg<TPrecision>::Subtract(xmp, node->center, xmp);
      TPrecision l = Linalg<TPrecision>::Length(xmp);

      alpha = 1 - l/maxRadius;

      Linalg<TPrecision>::Subtract(xm, xrm, xm);
      Linalg<TPrecision>::AddColumnwise(BC, xm, BC);

      ColorF col = green;
      if(vnode->ID == 0){
	col = dkgray;
      }
      else if(vnode->ID != data.selectedNode) {
	//	col = data.colormap->getColor(data.labelIndex[vnode->label], vnode->entropy);
	col = dynamic_cast<TwoDDiscreteColormap*>(data.colormap)->
	  getColor(data.labelIndex[vnode->label], vnode->entropy);
      }

      //draw circle
      glLoadName(vnode->ID);
      glColor4f(col[0], col[1], col[2], alpha);
      glBegin(GL_LINE_LOOP);
      for(int i=0; i<BC.N(); i++){
        glVertex2f(toScreenX( BC(0, i) ), toScreenY( BC(1,i) ) );
      }
      glEnd();


      //cleanup
      A.deallocate();
      C.deallocate();
      C1.deallocate();
      BC.deallocate();
     // BC1.deallocate();
     // svd.deallocate();
      xm.deallocate();
      xrm.deallocate();
    };



};

#endif
