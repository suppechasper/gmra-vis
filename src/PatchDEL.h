#ifndef PATCHDEL_H
#define PATCHDEL_H

#include "Display.h"
#include "DenseVector.h"
#include "DenseMatrix.h"


template<typename TPrecision>
class PatchDEL : public DisplayElement{

  private:


#define BUFSIZE 512



    int xM, yM;
    int mod, cur_button; 
    int pickH, pickW;  

   
    Data<TPrecision> &data;


  public:

    PatchDEL(Data<TPrecision> &d) : data(d){ 
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
      int sel = data.selectedPatch;
      if(sel >= 0){
        IPCANode<TPrecision> *node = (IPCANode<TPrecision>*) data.nodeMap[sel];

        DenseVector<TPrecision> c2 = node->center; 
        
        DenseMatrix<TPrecision> phi2 = node->phi;
       
        DenseVector<TPrecision> s2 = node->sigma2;
       
        DenseVector<TPrecision> v(c2.N());
        DenseVector<TPrecision> w(c2.N());

        int nDim = s2.N();
        int spacing = 2;
        int pw = ( width - spacing ) / 3 ;
        int ph = height / nDim;
        for(int i=0; i < nDim; i++){
          
          drawPatch(c2, xLeft + pw, yTop + i*ph, pw-spacing, ph-spacing); 
          
          Linalg<TPrecision>::ExtractColumn(phi2, i, v);
          Linalg<TPrecision>::Scale(v, sqrt(s2(i)), v);

          Linalg<TPrecision>::Add(v, c2, w);          
          drawPatch(w, xLeft, yTop + i*ph, pw-spacing, ph-spacing); 
          Linalg<TPrecision>::Subtract(c2, v, w);          
          drawPatch(w, xLeft + 2*pw, yTop + i*ph, pw-spacing, ph-spacing); 

        } 

        v.deallocate();
        w.deallocate();
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


private:

  void drawPatch(FortranLinalg::DenseVector<TPrecision> &v, int x, int y, int w, int h){
        using namespace FortranLinalg;
       int r = sqrt( v.N() );
       int index = 0;
       int ws = w/(r+1.0);
       int hs = h/(r+1.0);       
       TPrecision ma = Linalg<TPrecision>::Max(v); 
       TPrecision mi = Linalg<TPrecision>::Min(v); 
       glBegin(GL_QUADS);
       for(int i = 0; i < r; i++){
         for(int j = 0; j < r; j++){
           double col = (v(index)-data.minPatch)/(data.maxPatch-data.minPatch); 
           glColor3f( col, col, col );
           glVertex2f( x + i*ws, y + j*hs);
           glVertex2f( x + (i+1)*ws, y + j*hs);
           glVertex2f( x + (i+1)*ws, y + (j+1)*hs);
           glVertex2f( x + i*ws, y + (j+1)*hs);
           ++index;
         }
       }  
       glEnd();
  };

};

#endif
