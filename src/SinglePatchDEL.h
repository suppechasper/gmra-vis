#ifndef SINGLEPATCHDEL_H
#define SINGLEPATCHDEL_H

#include "Display.h"
#include "DenseVector.h"
#include "DenseMatrix.h"


template<typename TPrecision, typename LabelType>
class SinglePatchDEL : public DisplayElement{

  private:


#define BUFSIZE 512



    int xM, yM;
    int mod, cur_button; 
    int pickH, pickW;  

   
    Data<TPrecision, LabelType> &data;


  public:

 SinglePatchDEL(Data<TPrecision, LabelType> &d) : data(d){ 
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
      int sel = data.selectedIndex;
      if(sel >= 0){
     
 
       int r = sqrt( data.points.M() );
       int index = 0;
       static int vs = width/(r+1.0);
       glBegin(GL_QUADS);
       for(int i = 0; i < r; i++){
         for(int j = 0; j < r; j++){
           double col = (data.points(index, sel)-data.minPatch)/(data.maxPatch - data.minPatch); 
           glColor3f( col, col, col );
           glVertex2f( xLeft + i*vs, yTop + j*vs);
           glVertex2f( xLeft + (i+1)*vs, yTop + j*vs);
           glVertex2f( xLeft + (i+1)*vs, yTop + (j+1)*vs);
           glVertex2f( xLeft + i*vs, yTop + (j+1)*vs);
           ++index;
         }
       }  
       glEnd();

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
