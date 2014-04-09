#ifndef DOUBLEROTATION_H
#define DOUBLEROTATION_H

#include "DenseVector.h"
#include "DenseMatrix.h"

#include "Animation.h"

#include "Rotation.h"



template<typename TPrecision>
class DoubleRotation : public Animation{

  public:
    enum RotationType{ Primary, Secondary};
  private:





  public:

    Rotation<TPrecision> Rone;
    Rotation<TPrecision> Rtwo;

    FortranLinalg::DenseVector<TPrecision> t2;
    bool target2;

    DoubleRotation(Data<TPrecision> &d) :Rone(d, Rotation<TPrecision>::Primary),
    Rtwo(d,Rotation<TPrecision>::Secondary){ 

      target2 = false;
    };



    void setTarget1(FortranLinalg::DenseVector<TPrecision> t){
      Rone.setTarget(t);
      t.deallocate();
    };

    void setTarget2(FortranLinalg::DenseVector<TPrecision> t){
      t2 = t;
      target2 = true;
    };


    bool isRunning(){
      return Rone.isRunning() || Rtwo.isRunning();
    };


    void step(){
      if( Rone.isRunning() ){
        Rone.step();
      }
      else if( target2 ){
        Rtwo.setTarget(t2);
        Rtwo.step();
        t2.deallocate();

        target2 = false;
      }
      else if( Rtwo.isRunning() ){
        Rtwo.step();
      }

    };



};

#endif
