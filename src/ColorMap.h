// -*- C++ -*-
// <@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@>//
//                     __    __  __  ____  _  _  _____                     //
//                    /__\  (  \/  )(_  _)( \( )(  _  )                    //
//                   /(__)\  )    (  _)(_  )  (  )(_)(                     //
//                  (__)(__)(_/\/\_)(____)(_)\_)(_____)                    //
//                                       Building Blocks                   //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// Colormap.h 
// Written by Kristi Potter, 2013
// Class to define a colormap
// <@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@>//

#ifndef COLORMAP_H
#define COLORMAP_H
#include "Color.h"
#include "ColorDefines.h"
#include "etc.h"


// - - -
// Linearly interpolate between colors
inline ColorF lerpColor(ColorF a, ColorF b, float val){

    ColorF newColor;
    newColor.r(affine(0.f, val, 1.f, a.r(), b.r()));
    newColor.g(affine(0.f, val, 1.f, a.g(), b.g()));
    newColor.b(affine(0.f, val, 1.f, a.b(), b.b()));
    newColor.a(affine(0.f, val, 1.f, a.a(), b.a()));
    return newColor;
}

//-----------------------------------------------//
// Colormap (generic)
//-----------------------------------------------//
class Colormap{
  friend class ColormapFactory;

public:

    Colormap(float min = 0.f, float max = 1.f): _min(min), _max(max), _defColor(blue) {}
    Colormap(ColorF color) : _min(0.0), _max(1.0), _defColor(color){}
    Colormap(const Colormap &cm) { _min = cm._min; _max = cm._max; _defColor = cm._defColor; }
    virtual~Colormap(){}

    // - - - - - - - - - - - - - - - - - - - - - - - - - - //
    // Getters/Setters
    // - - - - - - - - - - - - - - - - - - - - - - - - - - //
    void   setRange(float min, float max){ _min = min; _max = max; }
    float  getRangeMin()                 { return _min; }
    float  getRangeMax()                 { return _max; }
    void   printRange()                  { std::cout << "[" << _min << ", " << _max << "]" << std::endl; }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - //
    // Get the color
    // - - - - - - - - - - - - - - - - - - - - - - - - - - //
    virtual ColorF getColor(float value){
      // Make the range of the data between 0 and 1
      float newVal = affine(_min, value, _max, 0.0, 1.0);
      return getColorDef(newVal);
    }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Define colormap (override this function) 0<=value<=1
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  virtual ColorF getColorDef(float value) { return _defColor; }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Get the gradient colors
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  virtual ColorF getLow()  { return _defColor; }
  virtual ColorF getMid()  { return _defColor; }
  virtual ColorF getHigh() { return _defColor; }

protected:

  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Set the gradient colors
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  virtual void setLow(ColorF col)  { _defColor = col; }
  virtual void setMid(ColorF col)  { _defColor = col; }
  virtual void setHigh(ColorF col) { _defColor = col; }

  float _min, _max;
  ColorF _defColor;
};

//-----------------------------------------------//
// Colormapping factory
//-----------------------------------------------//
class ColormapFactory{
  

public:
  Colormap * cmap;
  ColormapFactory(){ cmap = new Colormap(); }
  Colormap * getColormap(){  return cmap; }

  void setLow(ColorF col){ cmap->setLow(col); }
  void setMid(ColorF col){ cmap->setMid(col); }
  void setHigh(ColorF col){ cmap->setHigh(col); }
};

//-----------------------------------------------//
// A two-color colormap
//-----------------------------------------------//
class TwoColormap : public Colormap{

protected:

    ColorF _low, _high; // The colors to map through

public:

    // - - - - - - - - - - - - - - - - - - - - - - - - - - //
    // Constructor
    // - - - - - - - - - - - - - - - - - - - - - - - - - - //
    TwoColormap(ColorF low = black, ColorF high = white, float min = 0.0, float max = 1.0)
        : Colormap(min, max), _low(low), _high(high){}
    TwoColormap(const TwoColormap &tcm){}
    virtual ~TwoColormap(){}
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Get the colors
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  ColorF getLow()  { return _low; }
  ColorF getMid()  { return (_low+_high)/2.0; }
  ColorF getHigh() { return _high; }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Set the colors
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  void setLow(ColorF low){ _low = low; }
  void setHigh(ColorF high){ _high=high; }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Get the color
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  ColorF getColorDef(float value){ 
    ColorF col =  lerpColor(_low, _high, value);
    return col;}
};


//-----------------------------------------------//
// Two colormap factory
//-----------------------------------------------//
class TwoColormapFactory : public ColormapFactory{

public:
    TwoColormapFactory(ColorF low, ColorF high, float min=0.0, float max=1.0){
        cmap = new TwoColormap(low, high, min, max);  }
};

//-----------------------------------------------//
// A three-color colormap
//-----------------------------------------------//
class ThreeColormap : public TwoColormap{

protected:

  ColorF _mid; // The midpoint color to map through
  
public:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Constructor
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  ThreeColormap(ColorF low, ColorF mid, ColorF high, float min = 0.0, float max = 1.0)
    : TwoColormap(low, high, min, max), _mid(mid){}
  ThreeColormap(const ThreeColormap &tcm){}
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Get the mid color
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  ColorF getLow()  { return _low; }
  ColorF getMid() { return _mid; }
  ColorF getHigh() { return _high; }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Get the color
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  ColorF getColorDef(float value){
    ColorF col;
    if(value < 0.5)	col = lerpColor(_low, _mid, value*2.0);
    else                col = lerpColor(_mid, _high, (value-0.5)*2.0);
    return col;
  }
};

//-----------------------------------------------//
// Three colormap factory
//-----------------------------------------------//
class ThreeColormapFactory : public ColormapFactory{
  
public:
  ThreeColormapFactory(ColorF low, ColorF mid, ColorF high, float min=0.0, float max=1.0){
    cmap = new ThreeColormap(low, mid, high, min, max);  }
};

template<typename T, size_t N>
std::vector<T> convert_array_to_vector(const T (&source_array)[N]) {
  return std::vector<T>(source_array, source_array+N);
}

//-----------------------------------------------//
// Discrete colormap
//-----------------------------------------------//
class DiscreteColormap : public Colormap{

protected:

  std::vector<ColorF> _colors;

public:
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Constructor
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
   DiscreteColormap(std::vector<ColorF> colors=std::vector<ColorF>(), 
		   int numColors = 10, float min = 0.0, float max = 1.0)
    : Colormap(min, max) { setColors(colors, numColors); }
  DiscreteColormap(const DiscreteColormap &dcm){ _colors = dcm._colors; }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Set the colors
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  void setColors(std::vector<ColorF> colors, int numColors){
    _colors.resize(numColors);
    for(int i = 0; i < _colors.size(); i++){
      if(i >= colors.size())
	_colors[i] = black;
      else
	_colors[i] = colors[i];
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Get the color
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  virtual ColorF getColor(float value){
    return getColor(int(value));
  }
  virtual ColorF getColor(int value){
    // std::cout << "GET COLOR value: " << value << " colors " << _colors.size() << std::endl;
    if(value < 0 || value > _colors.size()){
      std::cout << "Discrete Colormap out of range, returning black!" << std::endl;
      return black;
    }
    else{
      return _colors[value];
    }
  }
};

//-----------------------------------------------//
// Discrete colormap factory
//-----------------------------------------------//
class DiscreteColormapFactory : public ColormapFactory{
  
public:
  DiscreteColormapFactory(std::vector<ColorF> colors=std::vector<ColorF>(), 
			  int numColors = 10, float min=0.0, float max=1.0){
    cmap = new DiscreteColormap(colors, numColors, min, max);  }
};

//-----------------------------------------------//
// Two-D colormap
//-----------------------------------------------//
class TwoDDiscreteColormap : public DiscreteColormap{
protected:

  ColormapFactory * _factory;
  float _minF, _maxF;

public:

  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Constructor
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  TwoDDiscreteColormap(std::vector<ColorF> colors= std::vector<ColorF>(), 
		       ColormapFactory * factory = new ColormapFactory(),
		       int numColors = 10, float min=0.f, float max=1.f, 
		       float minF = 0.f, float maxF=1.f) 
    : DiscreteColormap(colors, numColors, min, max), 
      _factory(factory),
      _minF(minF), _maxF(maxF){}
  TwoDDiscreteColormap(const TwoDDiscreteColormap &tdcm) {}

  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Getters/Setters
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  void setFactoryRange(float min, float max) { _minF = min; _maxF = max; }
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Get the color
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  ColorF getColor(float value, float valueF){
    return getColor(int(value), valueF);
  }
  ColorF getColor(int value, float valueF){
    // std::cout << "getColor: " << value << " " << valueF << std::endl;

    ColorF color = DiscreteColormap::getColor(value);
    
    // std::cout << "label color: " << color << std::endl;
    return putColorThroughNewMap(color, valueF);
  }
  // Put the new color into the color-to-white factory, return color
  ColorF putColorThroughNewMap(ColorF color, float valueF){
    _factory->setLow(color);
    Colormap * map = _factory->getColormap();
    map->setRange(_minF, _maxF);
    ColorF col = map->getColor(valueF);
    return col;
    //return color;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // Get the 1D color
  // - - - - - - - - - - - - - - - - - - - - - - - - - - //
  ColorF getColor(float value){
    return getColor(int(value));
  }
  ColorF getColor(int value){
    return DiscreteColormap::getColor(value);
  }

};

//-----------------------------------------------//
// Two-D colormap factory
//-----------------------------------------------//
class TwoDDiscreteColormapFactory : public DiscreteColormapFactory{

public:
  TwoDDiscreteColormapFactory(std::vector<ColorF> colors, ColormapFactory * factory,
		       int numColors = 10, float min=0.f, float max=1.f, 
			      float minF = 0.f, float maxF=1.f){
    cmap = new TwoDDiscreteColormap(colors, factory, numColors, min, max, minF, maxF);
  }
};

#endif // COLORMAP_H
