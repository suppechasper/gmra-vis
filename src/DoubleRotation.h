#ifndef DOUBLEROTATION_H
#define DOUBLEROTATION_H

#include "DenseVector.h"
#include "DenseMatrix.h"

#include "Animation.h"

#include "Rotation.h"



template<typename TPrecision, typename LabelType>
class DoubleRotation : public Animation{

  public:
    enum RotationType{ Primary, Secondary};
  private:





  public:

    Rotation<TPrecision, LabelType> Rone;
    Rotation<TPrecision, LabelType> Rtwo;

    FortranLinalg::DenseVector<TPrecision> t2;
    bool target2;

 DoubleRotation(Data<TPrecision, LabelType> &d) :Rone(d, Rotation<TPrecision, LabelType>::Primary),
      Rtwo(d,Rotation<TPrecision, LabelType>::Secondary){ 

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
