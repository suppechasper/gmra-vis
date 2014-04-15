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

class Tuple{
public:
  Tuple(): r(0.0), g(0.0), b(0.0){}

  Tuple(float _first, float _second, float _third){
    r = _first; g = _second; b = _third;
  }
  Tuple(int _first, int _second, int _third){
    r = (float)_first/255.f; g = (float)_second/255.f; b = (float)_third/255.f;
  }
  Tuple(const Tuple& t){
    r = t.r;
    g = t.g;
    b = t.b;
  }

  float x(){ return r; }
  float y(){ return g; }
  float z(){ return b; }
  
  void print(){
    std::cout << "(" << r << ", " << g << ", " << b << ",)" << std::endl;
  }

  float r, g, b;
};

template<class T>
class DiscreteColorMapper{

protected:
  std::vector<Tuple> colors;
  std::vector<Tuple> colormap;
 
public:
  DiscreteColorMapper(bool paired = false){
    if(paired){
      colors.push_back(Tuple(166,206,227));
      colors.push_back(Tuple(31,120,180));
      colors.push_back(Tuple(178,223,138));
      colors.push_back(Tuple(51,160,44));
      colors.push_back(Tuple(251,154,153));
      colors.push_back(Tuple(227,26,28));
      colors.push_back(Tuple(253,191,111));
      colors.push_back(Tuple(255,127,0));
      colors.push_back(Tuple(202,178,214));
      colors.push_back(Tuple(106,61,154));
      set(10);
    }
    else{
      colors.push_back(Tuple(228,26,28));
      colors.push_back(Tuple(55,126,184));
      colors.push_back(Tuple(77,175,74));
      colors.push_back(Tuple(152,78,163));
      colors.push_back(Tuple(255,127,0));
      colors.push_back(Tuple(255,255,51));
      colors.push_back(Tuple(166,86,40));
      colors.push_back(Tuple(247,129,191));
      colors.push_back(Tuple(153,153,153));
      set(9); 
   }
  }

  void set(int numColors){
    if(numColors > colors.size()){
      std::cout << "ERROR!  MORE THAN 9 COLORS - ADD MORE COLORS TO MAP!!" << std::endl;
      exit(0);
    }
    else{
      for(int i = 0; i < numColors; i++)
	colormap.push_back(colors[i]);
    }
  }
  
  Tuple getColor(int value){
    if(value > colormap.size()) // return black if out of range
      return Tuple(0.f, 0.f, 0.f);
    else
      return colormap[value];
  }
};

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

  void set(T Rmin, T Rmax, T Gmin, T Gmax, T Bmin, T Bmax){

    rmin = Rmin;
    rmax = Rmax;
    gmin = Gmin;
    gmax = Gmax;
    bmin = Bmin;
    bmax = Bmax;

    rmid = (Rmin + Rmax) * 0.5;
    gmid = (Gmin + Gmax) * 0.5;
    bmid = (Bmin + Bmax) * 0.5;

    /*  std::cout << "set min: " << Rmin << " " << Gmin << " " << Bmin << std::endl;
    std::cout << "set mid: " << rmid << " " << gmid << " " << bmid << std::endl;
    std::cout << "set max: " << Rmax << " " << Gmax << " " << Bmax << std::endl;
    */
  } 

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

    /*std::cout << "Red: " << rmin << ", " << rmid << ", " << rmax << std::endl;
    std::cout << "Green: " << gmin << ", " << gmid << ", " << gmax << std::endl;
    std::cout << "Blue: " << bmin << ", " << bmid << ", " << bmax << std::endl;
    */

    //  std::cout << "value: " << value << " rangeMin: " << rangeMin << " rangeMid: " << rangeMid << " rangeMax: " << rangeMax << std::endl;

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
