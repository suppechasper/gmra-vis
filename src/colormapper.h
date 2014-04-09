// -*- C++ -*-
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>//
//                              Color Mapper                                //
//                                                                          //
//                       Written by Kristi Potter                           //
//                           February 5, 2010                               //
//                                                                          //
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>//

#ifndef _COLORMAPPER_H
#define _COLORMAPPER_H

#include <vector>
#include <iostream>



template<class T>
class ColorMapper
{

 public: 

  ColorMapper<T>(T rangeMin = T(0), T rangeMax = T(1))
    : rangeMin(rangeMin), rangeMax(rangeMax)
  {
    rangeMid = (rangeMax - rangeMin)/2.f + rangeMin;    

   //default colormap
    //blue to green to red
    rmin = 0;
    rmid = 1;
    rmax = 1;
    gmin = 0.25;
    gmid = 1;
    gmax = 0.45;
    bmin = 1;
    bmid = 1;
    bmax = 0;
  }

/*
  ColorMapper<T>(const ColorMapper<T> &cm)
    : rangeMin(cm.rangeMin), rangeMax(cm.rangeMax)
  {}
  */


  ColorMapper<T> &operator=(const ColorMapper<T> &cm)
  {
    rangeMid = cm.rangeMid;
    rangeMin = cm.rangeMin;
    rangeMax = cm.rangeMax;
    

    return *this;
  }




  virtual ~ColorMapper() {}

      
  
  void set(T Rmin, T Rmid, T Rmax, T Gmin, T Gmid, T Gmax, T Bmin, T Bmid, T Bmax){

    rmin = Rmin;
    rmid = Rmid;
    rmax = Rmax;
    gmin = Gmin;
    gmid = Gmid;
    gmax = Gmax;
    bmin = Bmin;
    bmid = Bmid;
    bmax = Bmax;
  };

  void setRange(T rmin, T rmid, T rmax){
    rangeMin = rmin;
    rangeMid = rmid;
    rangeMax = rmax;
  };

  void setRange(T rmin, T rmax){
    rangeMin = rmin;
    rangeMid = rmin + (rmax-rmin)/2.f;
    rangeMax = rmax;
  };

  // Return a color for a scalar
  std::vector<T> getColor(T value)
  {
   
    T r,g,b;
    if(value < rangeMid){
     r = affine(value, rangeMin, rangeMid, rmin, rmid);
     g = affine(value, rangeMin, rangeMid, gmin, gmid);
     b = affine(value, rangeMin, rangeMid, bmin, bmid);
    }
    else{     
      r = affine(value, rangeMid, rangeMax, rmid, rmax);
      g = affine(value, rangeMid, rangeMax, gmid, gmax);
      b = affine(value, rangeMid, rangeMax, bmid, bmax);
    }
    std::vector<T> color;
    color.push_back(r);
    color.push_back(g);
    color.push_back(b);
    return color;
  };  // Return a color for a scalar
  

  void getColor(T value, T color[3]){
    if(value < rangeMid){
     color[0] = affine(value, rangeMin, rangeMid, rmin, rmid);
     color[1] = affine(value, rangeMin, rangeMid, gmin, gmid);
     color[2] = affine(value, rangeMin, rangeMid, bmin, bmid);
    }
    else{     
      color[0] = affine(value, rangeMid, rangeMax, rmid, rmax);
      color[1] = affine(value, rangeMid, rangeMax, gmid, gmax);
      color[2] = affine(value, rangeMid, rangeMax, bmid, bmax);
    }
  };
    

  T rangeMin, rangeMax, rangeMid;
  
  T rmin, rmid, rmax;
  T gmin, gmid, gmax;
  T bmin, bmid, bmax;

 private:

  inline T affine(const T x, const T a, const T A, const T b, const T B)
  {
    return T((B - b)*(x - a)/float((A-a)) + b); 
  };


  

};

#endif
