#ifndef SINGLEPATCHRECONSTRUCTDEL_H
#define SINGLEPATCHRECONSTRUCTDEL_H

#include "Display.h"
#include "DenseVector.h"
#include "DenseMatrix.h"


template<typename TPrecision, typename LabelType>
class SinglePatchReconstructDEL : public DisplayElement{

  private:


#define BUFSIZE 512



    int xM, yM;
    int mod, cur_button; 
    int pickH, pickW;  

   
    Data<TPrecision, LabelType> &data;


  public:

 SinglePatchReconstructDEL(Data<TPrecision, LabelType> &d) : data(d){ 
      pickW = 2;
      pickH = 2;
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
        using namespace FortranLinalg;
      int sel = data.selectedIndex;
      int selPatch = data.selectedNode;
      if(sel >= 0 && selPatch != -1){
       IPCANode<TPrecision> *node= (IPCANode<TPrecision> *) data.nodeMap[selPatch];
       
       DenseVector<TPrecision> vo = Linalg<TPrecision>::ExtractColumn(data.points, sel);
       Linalg<TPrecision>::Subtract(vo, node->center, vo);
       DenseVector<TPrecision> vp = Linalg<TPrecision>::Multiply(node->phi, vo, true);
       DenseVector<TPrecision> v = Linalg<TPrecision>::Multiply(node->phi, vp);
       Linalg<TPrecision>::Add(v, node->center, v);
 
       int r = sqrt( v.N() );
       int index = 0;
       static int vs = width/(r+1.0);
       glBegin(GL_QUADS);
       for(int i = 0; i < r; i++){
         for(int j = 0; j < r; j++){
           double col = (v(index)-data.minPatch)/(data.maxPatch - data.minPatch); 
           glColor3f( col, col, col );
           glVertex2f( xLeft + i*vs, yTop + j*vs);
           glVertex2f( xLeft + (i+1)*vs, yTop + j*vs);
           glVertex2f( xLeft + (i+1)*vs, yTop + (j+1)*vs);
           glVertex2f( xLeft + i*vs, yTop + (j+1)*vs);
           ++index;
         }
       }  
       glEnd();
       vo.deallocate();
       vp.deallocate();
       v.deallocate();
       }

    };

  
    void idle(){

    };


    void keyboard(unsigned char key, int x, int y){
    };



    void special(int key, int x, int y){
    };




    void mouse(int button, int state, int x, int y){

    };



    // catch mouse move events
    void motion(int x, int y){

    };




    void passive(int x, int y){

    };



};

#endif
