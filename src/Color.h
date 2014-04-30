// -*- C++ -*-
// <@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@>//
//                     __    __  __  ____  _  _  _____                     //
//                    /__\  (  \/  )(_  _)( \( )(  _  )                    //
//                   /(__)\  )    (  _)(_  )  (  )(_)(                     //
//                  (__)(__)(_/\/\_)(____)(_)\_)(_____)                    //
//                                       Building Blocks                   //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// Color.h
// Written by Kristi Potter, 2013
// Class to define color.
// <@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@>//
#ifndef _COLOR_H
#define _COLOR_H

#include "Vector.h"
#include <typeinfo>

// ~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~ //
// A color is a 4 dimensional Vector
// ~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~ //
template< typename Type >
class Color : public Vec< Type, 4>
{
public:
    
  //  - + - + - + - + - + - + Constructors + - + - + - + - + - + - + - //
  Color() : Vec<Type, 4>() {}
  Color(const Color<Type> &c);
  Color(const Vec<Type, 4> &v);
  Color(const Type r, const Type g, const Type b, const Type a = static_cast<Type>(1.0));
  virtual ~Color() {}
    
  //  - + - + - + - + - + - + RGBA Accesssors + - + - + - + - + - + - //
  void r(const Type red)   { this->v[0] = red; }
  void g(const Type green) { this->v[1] = green; }
  void b(const Type blue)  { this->v[2] = blue; }
  void a(const Type alpha) { this->v[3] = alpha; }
  Type r() const { return this->v[0]; }
  Type g() const { return this->v[1]; }
  Type b() const { return this->v[2]; }
  Type a() const { return this->v[3]; }
  
  // string r_toString() const { stringstream ss; ss << r(); return ss.str(); }

  //  - + - + - Conversions between dimensions and Types - + - + - //
  //  Return a 3D (vs 4D) version of the color
  Vec<Type, 3> to3d() const { return Vec<Type, 3>(this->v[0], this->v[1], this->v[2]); }
   
  // Return an integer version of the color
  Color<int> toInt() const;
  // Return a float version of the color
  Color<float> toFloat() const;
 
  // Return a QColor version (make sure we are ints)
  //QColor qColor() const{ Color<int> ci = this->toInt(); return QColor(ci.r(), ci.g(), ci.b()); }

protected:
  // Return the type of the color
  char detectType() const;

};// end of Color
// ~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~ //
// Implementation of Color
// ~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~ //
template<class Type> Color<Type>::Color(const Color<Type> &c)
{ for(int i=0; i<4; i++) this->v[i] = c.v[i]; }
template<class Type> Color<Type>::Color(const Vec<Type,4> &V)
{ for(int i=0; i<4; i++) this->v[i] = V.v[i]; }
template<class Type> Color<Type>::Color(const Type r, const Type g, const Type b, const Type a)
{this->v[0] = r; this->v[1] = g; this->v[2] = b; this->v[3] = a; }
template<class Type>
Color<int> Color<Type>::toInt() const
{ char type = detectType(); Color<int> color = Color<int>(0, 0, 0);
  if(type == 'f' || type == 'd') for(int i = 0; i < 4; i++) color[i] = static_cast<int>(this->v[i]*255.0);
  else if(type == 'i' || type == 'c') for(int i = 0; i < 4; i++) color[i] = static_cast<int>(this->v[i]);
  else std::cerr << "I dont know what type this Color is, so I'm returning black from Color::toInt()." << std::endl;
  return color;
}
template<class Type>
Color<float> Color<Type>::toFloat() const
{ char type = detectType(); Color<float> color = Color<float>(0, 0, 0);
  if(type == 'f' || type == 'd') for(int i = 0; i < 4; i++)color[i] = static_cast<float>(this->v[i]);
  else if(type == 'i' || type == 'c') 
    for(int i = 0; i < 4; i++){     
      if(i == 3){
	if(static_cast<float>(this->v[i]) == 1.f)
	  color[i] = 1.f;
      }
      else color[i] = static_cast<float>(this->v[i])/255.0;
    }
  else std::cerr << "I dont know what type this Color is, so I'm returning black from Color::toFloat()." << std::endl;
  return color;
}
/* I would like to add this method but I'm not sure how:
template<class Type>
std::string Color<Type>::toRgba() const
{ char type = detectType(); Color<int> color = Color<int>(0, 0, 0); std::string rgbaStr = "rgba(0,0,0,1)";
  if(type == 'f' || type == 'd') for(int i = 0; i < 4; i++) color[i] = static_cast<int>(this->v[i]*255.0);
  else if(type == 'i' || type == 'c') for(int i = 0; i < 4; i++) color[i] = static_cast<int>(this->v[i]);
  else {
    std::cerr << "I dont know what type this Color is, so I'm returning black." << std::endl;
    return rgbaStr;
  }
  rgbaStr = "rgba("+std::to_string(color[0])+","+std::to_string(color[1])+","+std::to_string(color[2])+","+std::to_string(color[3])+")";
  return rgbaStr;
} */
template<class Type>
char Color<Type>::detectType() const
{ char type;
  if(typeid(Type).name()[0] == 'i') type = 'i';
  else if(typeid(Type).name()[0] == 'f') type = 'f';
  else if(typeid(Type).name()[0] == 'd') type = 'd';
  else if(typeid(Type).name()[0] == 'c') type = 'c';
  return type;
}

    
// -<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>- //
// Define some common Color Types  
// -<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>- //
typedef Color<float> ColorF;
typedef Color<double> ColorD;
typedef Color<int> ColorI;



#endif
