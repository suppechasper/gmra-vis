// -*- C++ -*-
// <@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@>//
//                     __    __  __  ____  _  _  _____                     //
//                    /__\  (  \/  )(_  _)( \( )(  _  )                    //
//                   /(__)\  )    (  _)(_  )  (  )(_)(                     //
//                  (__)(__)(_/\/\_)(____)(_)\_)(_____)                    //
//                                       Building Blocks                   //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// Vector.h
// Written by Kristi Potter, 2010
// Class for generic vector functions
// <@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@>//

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>
#include <math.h>
#include <cassert>

// Compile-time definition of the dimension for converting different vector dimensions
template<int Dim> class plusminus {public: enum PM { PLUS = Dim+1, MINUS = Dim-1 }; };
template<> class plusminus<1> {public: enum PM { PLUS = 2, MINUS = 2 }; };

// ~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~ //
// Definition of a variable type and dimension vector class.
// ~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~ //
template< typename Type, int Dim >
class Vec
{
public:
  
  // public data accessor (vector.v is the data)
  Type v[Dim];
  
  //  - + - + - + - + - + Constructors (up to dim 4) + - + - + - + - + - + - //
  Vec() {}
  Vec(Type v1);
  Vec(Type v1, Type v2);
  Vec(Type v1, Type v2, Type v3);
  Vec(Type v1, Type v2, Type v3, Type v4);
  Vec(Type *V, int numVals=-1);
  Vec(const Vec &v);
  Vec(const Vec<Type, plusminus<Dim>::PLUS > &v);
  Vec(const Vec< Type, plusminus<Dim>::MINUS > &v, Type last = 1);
  virtual ~Vec() {}
  
  // - + - + - + - + - + - + - + Accesssors + - + - + - + - + - + - + - + - //
  Type &operator[](int i)            { return v[i]; }
  const Type operator[](int i) const { return v[i]; }
  Vec &operator=(const Vec &v)
  { for(int i=0; i<Dim; ++i) this->v[i] = v.v[i]; return *this;}
  Vec &operator=(const Vec<Type, Dim+1> &v)
  { for(int i=0; i<Dim; ++i) this->v[i] = v.v[i]; return *this;}
  
  inline Type &x() { return operator[](0); }
  inline Type &y() { return operator[](1); }
  inline Type &z() { return operator[](2); }
  inline Type &w() { return operator[](3); }
  inline const Type x() const { return operator[](0); }
  inline const Type y() const { return operator[](1); }
  inline const Type z() const { return operator[](2); }
  inline const Type w() const { return operator[](3); }

  void x(Type x) { v[0] = x; }
  void y(Type y) { v[1] = y; }
  void z(Type z) { v[2] = z; }
  void w(Type w) { v[3] = w; }

  // - + - + - + - + - + - + -  Dim/Len/Size - + - + - + - + - + - + - + - //
  int len()  { return Dim; }
  int dim()  { return Dim; }
  int size() { return Dim; }
  
  // - + - + - + - + - + - + - Math Operators - + - + - + - + - + - + - + -//
  Vec operator-    () const                // negate
  { Vec V; for(int i=0; i<Dim; i++) V[i] = -(this->v[i]); return V; }
  Vec operator+    (const Type& v) const   // +
  { Vec V; for(int i=0; i<Dim; i++) V[i] = this->v[i] + v; return V; }
  Vec operator+    (const Vec &v) const // +
  { Vec V; for(int i=0; i<Dim; i++) V[i] = this->v[i] + v.v[i]; return V; }
  Vec &operator+=  (const Type& v)         // +=
  { for(int i=0; i<Dim; i++) this->v[i] += v; return *this;}
  Vec &operator+=  (const Vec &v)       // +=
  { for(int i=0; i<Dim; i++) this->v[i] += v.v[i]; return *this;}
  Vec operator-    (const Type& v) const   // -
  { Vec V; for(int i=0; i<Dim; i++) V[i] = this->v[i] - v; return V; }
  Vec operator-    (const Vec &v) const // -
  { Vec V; for(int i=0; i<Dim; i++) V[i] = this->v[i] - v.v[i]; return V; }
  Vec &operator-=  (const Type& v)         // -=
  { for(int i=0; i<Dim; i++) this->v[i] -= v; return *this;}
  Vec &operator-=  (const Vec &v)       // -=
  { for(int i=0; i<Dim; i++) this->v[i] -= v.v[i]; return *this;}
  Vec operator*    (const Type& v) const   // *
  { Vec V; for(int i=0; i<Dim; i++) V[i] = this->v[i] * v; return V; }
  Vec operator*    (const Vec &v) const // *
  { Vec V; for(int i=0; i<Dim; i++) V[i] = this->v[i] * v.v[i]; return V; }
  Vec &operator*=  (const Type& v)         // *=
  { for(int i=0; i<Dim; i++) this->v[i] *= v; return *this;}
  Vec &operator*=  (const Vec &v)       // *=
  { for(int i=0; i<Dim; i++) this->v[i] *= v.v[i]; return *this;}
  Vec operator/    (const Type& v) const   // /
  { Vec V; for(int i=0; i<Dim; i++) V[i] = this->v[i] / v; return V; }
  Vec operator/    (const Vec &v) const // /
  { Vec V; for(int i=0; i<Dim; i++) V[i] = this->v[i] / v.v[i]; return V; }
  Vec &operator/=  (const Type& v)        // /=
  { for(int i=0; i<Dim; i++) this->v[i] /= v; return *this;}
  Vec &operator/=  (const Vec &v)      // /=
  { for(int i=0; i<Dim; i++) this->v[i] /= v.v[i]; return *this;}
    
  // - + - + - + - + - + Comparison Operators + - + - + - + - + - + - //
  bool operator==     (const Vec &v) const // ==
  { for(int i=0; i<Dim; i++) if(this->v[i] != v[i]) return false;  return true;  }
  bool operator!=     (const Vec &v) const // !=
  { return !(operator==(v)); }
  bool operator<      (const Vec &v) const // <
  { for(int i=0; i<Dim; i++) if(this->v[i] > v.v[i]) return false; return true;  }
  bool operator<=     (const Vec &v) const // <=
  { for(int i=0; i<Dim; i++) if(this->v[i] > v.v[i]) return false; return true;  }
  bool operator>      (const Vec &v) const // >
  { for(int i=0; i<Dim; i++) if(this->v[i] < v.v[i]) return false; return true;  }
  bool operator>=     (const Vec &v) const // >=
  { for(int i=0; i<Dim; i++) if(this->v[i] < v.v[i]) return false; return true;  }

  // - + - + - + - + - + - + Vector Operators + - + - + - + - + - + - + - //
  Vec abs() const                          // abs
  { Vec V; for(int i=0; i<Dim; i++) V[i] = abs(this->v[i]); return V; }
  Type L1norm() const                         // L1norm
  { Type t = 0; for(int i=0; i<Dim; i++) t+=this->v[i]; return t; }
  Type L2norm() const                         // L2norm
  { Type t = 0; for(int i=0; i<Dim; i++) t+=(this->v[i]*this->v[i]); return sqrt(t); }
  Type norm() const                           // norm
  { return this->L2norm();} 
  Type length() const                         // length
  { return this->L2norm(); } 
  Type magnitude() const                      // magnitude
  { return this->L2norm(); }
  Type normalize()                            // normalize
  { Type t = this->L2norm();  (*this)/=t; return t; }
  Type dot(const Vec &V) const             // dot
  { Type t=0; for(int i=0; i<Dim; i++) t += (*this)[i]*V[i]; return t; }
    
};// end Vector
  
// -<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>- //
// Define some common Vector Types  
// -<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>-<~>- //
typedef Vec<float, 2>           Vector2f;
typedef Vec<float, 3>           Vector3f;
typedef Vec<float, 4>           Vector4f;
typedef Vec<double, 2>          Vector2d;
typedef Vec<double, 3>          Vector3d;
typedef Vec<double, 4>          Vector4d;
typedef Vec<int, 2>             Vector2i;
typedef Vec<int, 3>             Vector3i;
typedef Vec<int, 4>             Vector4i;
typedef Vec<unsigned int, 2>    Vector2ui;
typedef Vec<unsigned int, 3>    Vector3ui;
typedef Vec<unsigned int, 4>    Vector4ui;
typedef Vec<short, 2>           Vector2s;
typedef Vec<short, 3>           Vector3s;
typedef Vec<short, 4>           Vector4s;
typedef Vec<unsigned short, 2>  Vector2us;
typedef Vec<unsigned short, 3>  Vector3us;
typedef Vec<unsigned short, 4>  Vector4us;
typedef Vec<char, 2>            Vector2b;
typedef Vec<char, 3>            Vector3b;
typedef Vec<char, 4>            Vector4b;
typedef Vec<unsigned char, 2>   Vector2ub;
typedef Vec<unsigned char, 3>   Vector3ub;
typedef Vec<unsigned char, 4>   Vector4ub;

typedef Vector3f                   Vertex;
typedef Vector3i                   TriIndexs;
typedef Vector4i                   QuadIndexs;


  
// ~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~ //
// Implementation of Vector
// ~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~@~ //
template<class Type, int Dim> Vec<Type,Dim>::Vec(const Vec<Type,Dim> &V)
{ for(int i=0; i<Dim; i++) this->v[i] = V.v[i]; }
template<class Type, int Dim> Vec<Type,Dim>::Vec(Type v1)
{ assert(Dim >= 1); for(int i=0; i<Dim; i++)  v[i] = v1;  }
template<class Type, int Dim> Vec<Type,Dim>::Vec(Type v1, Type v2)
{ assert(Dim >= 2); v[0] = v1; v[1] = v2; for(int i=2; i<Dim; i++) v[i] = v2;  }
template<class Type, int Dim> Vec<Type,Dim>::Vec(Type v1, Type v2, Type v3)
{ assert(Dim >= 3); v[0] = v1; v[1] = v2; v[2] = v3; for(int i=3; i<Dim; i++) v[i] = v3;  }
template<class Type, int Dim> Vec<Type,Dim>::Vec(Type v1, Type v2, Type v3, Type v4)
{ assert(Dim >= 4); v[0] = v1; v[1] = v2; v[2] = v3; v[3] = v4; for(int i=4; i<Dim; i++) v[i] = v4;  }
template<class Type, int Dim> Vec<Type,Dim>::Vec(Type *V, int numVals)
{ if(numVals == -1) for(int i=0; i<Dim; i++) this->v[i] = V[i];
  else for(int i=0; i< (Dim<numVals?Dim:numVals); i++) this->v[i] = V[i]; }
template<class Type, int Dim> Vec<Type,Dim>::Vec(const Vec<Type, plusminus<Dim>::PLUS > &v)
{ for(int i=0; i<Dim; ++i) this->v[i] = v.v[i]; }
template<class Type, int Dim> Vec<Type,Dim>::Vec(const Vec< Type, plusminus<Dim>::MINUS > &v, Type last)
{ for(int i=0; i<Dim-1; ++i) this->v[i] = v.v[i]; this->v[Dim-1] = last;  }
  
// -(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)- //
// Operators external to the class
// -(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)-(-)- //
template<class Type, int Dim>
std::ostream& operator<<(std::ostream &os, const Vec<Type,Dim> &v)  // <<
{ for(int i=0; i<Dim; ++i) os << v[i] << " "; return os; }
template<class Type, int Dim>
std::istream& operator>>(std::istream &is, Vec<Type,Dim> &v)        // >>
{ for(int i=0; i<Dim; ++i) is >> v[i]; return is; }
template<class Type, int Dim>                                          
Vec<Type,Dim> operator+(const Type &val, const Vec<Type,Dim> &v) // +
{ Vec<Type,Dim> V; for(int i=0; i<Dim; ++i) V[i] = val + v.v[i]; return V; }
template<class Type, int Dim>
Vec<Type,Dim> operator*(const Type &val, const Vec<Type,Dim> &v) // *
{ Vec<Type,Dim> V; for(int i=0; i<Dim; ++i) V[i] = val * v.v[i]; return V; }
template<class Type1, class Type2, int Dim>
Vec<Type2, Dim> convertVectorTypes(const Vec<Type1, Dim> &v)    // convertVectorTypes
{ Vec<Type2, Dim> V; for(int i = 0; i<Dim; i++) V[i] = static_cast<Type2>(v[i]); return V; }
template<class Type>                                                  // cross
Vec<Type,3> cross(const Vec<Type,3> &a, const Vec<Type,3> &b)
{ return Vec<Type,3>(a[1]*b[2] - a[2]*b[1],  a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]); }
template< class Type, int Dim, class Type2 > inline                  // lerp()
Vec<Type, Dim> lerp(const Vec< Type, Dim > &v0, const Vec< Type,Dim > &v1, Type2 t)
{ Vec<Type,Dim> V; for(int i=0; i<Dim; ++i) V[i] = v0[i]+t*(v1[i]-v0[i]); return V;}

// -:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:- //
// Vector Constants
// -:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:- //
  
// Vector2 constants
//zero
const Vector2f Vector2f_zero = Vector2f(0.0f);
const Vector2d Vector2d_zero = Vector2d(0.0);
const Vector2i Vector2i_zero = Vector2i(int(0));
  
//one
const Vector2f Vector2f_one = Vector2f(1.0f);
const Vector2d Vector2d_one = Vector2d(1.0);
const Vector2i Vector2i_one = Vector2i(int(1));

//-one
const Vector2f Vector2f_neg_one = Vector2f(-1.0f);
const Vector2d Vector2d_neg_one = Vector2d(-1.0);
const Vector2i Vector2i_neg_one = Vector2i(int(-1));
  
// x
const Vector2f Vector2f_x = Vector2f(1.0f, 0.0f);
const Vector2d Vector2d_x = Vector2d(1.0, 0.0);
const Vector2i Vector2i_x = Vector2i(int(1), int(0));
  
// -x
const Vector2f Vector2f_neg_x = Vector2f(-1.0f, 0.0f);
const Vector2d Vector2d_neg_x = Vector2d(-1.0, 0.0);
const Vector2i Vector2i_neg_x = Vector2i(int(-1), int(0));
  
// y
const Vector2f Vector2f_y = Vector2f(0.0f, 1.0f);
const Vector2d Vector2d_y = Vector2d(0.0, 1.0);
const Vector2i Vector2i_y = Vector2i(int(0), int(1));
  
// -y
//const Vector2f Vector2f_neg_y = Vector2f(0.0f, -1.0f);
const Vector2d Vector2d_neg_y = Vector2d(0.0, -1.0);
const Vector2i Vector2i_neg_y = Vector2i(int(0), int(-1));
  
// xy
const Vector2f Vector2f_xy = Vector2f(1.0f, 1.0f);
const Vector2d Vector2d_xy = Vector2d(1.0, 1.0);
const Vector2i Vector2i_xy = Vector2i(int(1), int(1));
  
// -xy
const Vector2f Vector2f_neg_xy = Vector2f(-1.0f, -1.0f);
const Vector2d Vector2d_neg_xy = Vector2d(-1.0, -1.0);
const Vector2i Vector2i_neg_xy = Vector2i(int(-1), int(-1));
  
// Vector3 constants
//zero
const Vector3f Vector3f_zero = Vector3f(0.0f);
const Vector3d Vector3d_zero = Vector3d(0.0);
const Vector3i Vector3i_zero = Vector3i(int(0));

//one
const Vector3f Vector3f_one = Vector3f(1);
const Vector3d Vector3d_one = Vector3d(1);
const Vector3i Vector3i_one = Vector3i(1);
  
//-one
const Vector3f Vector3f_neg_one = Vector3f(-1);
const Vector3d Vector3d_neg_one = Vector3d(-1);
const Vector3i Vector3i_neg_one = Vector3i(-1);
  
//.5
const Vector3f Vector3f_half = Vector3f(.5);
const Vector3d Vector3d_half = Vector3d(.5);
  
//-.5
const Vector3f Vector3f_neg_half = Vector3f(-.5);
const Vector3d Vector3d_neg_half = Vector3d(-.5);
  
//x axis
const Vector3f Vector3f_x = Vector3f(1,0,0);
const Vector3d Vector3d_x = Vector3d(1,0,0);
const Vector3f Vector3f_neg_x = Vector3f(-1,0,0);
const Vector3d Vector3d_neg_x = Vector3d(-1,0,0);
  
//y axis
const Vector3f Vector3f_y = Vector3f(0.0f,1,0);
const Vector3d Vector3d_y = Vector3d(0.0,1,0);
const Vector3f Vector3f_neg_y = Vector3f(0.0f,-1,0);
const Vector3d Vector3d_neg_y = Vector3d(0.0,-1,0);
  
//z axis
const Vector3f Vector3f_z = Vector3f(0.0f,0,1);
const Vector3d Vector3d_z = Vector3d(0.0,0,1);
const Vector3f Vector3f_neg_z = Vector3f(0.0f,0,-1);
const Vector3d Vector3d_neg_z = Vector3d(0.0,0,-1);
  
//xy axis
const Vector3f Vector3f_xy = Vector3f(1,1,0);
const Vector3d Vector3d_xy = Vector3d(1,1,0);
const Vector3f Vector3f_neg_xy = Vector3f(-1,-1,0);
const Vector3d Vector3d_neg_xy = Vector3d(-1,-1,0);
  
//xz axis
const Vector3f Vector3f_xz = Vector3f(1,0,1);
const Vector3d Vector3d_xz = Vector3d(1,0,1);
const Vector3f Vector3f_neg_xz = Vector3f(-1,0,-1);
const Vector3d Vector3d_neg_xz = Vector3d(-1,0,-1);
  
//yz axis
const Vector3f Vector3f_yz = Vector3f(0.0f,1,1);
const Vector3d Vector3d_yz = Vector3d(0.0,1,1);
const Vector3f Vector3f_neg_yz = Vector3f(0.0f,-1,-1);
const Vector3d Vector3d_neg_yz = Vector3d(0.0,-1,-1);
  
//xyz axis
const Vector3f Vector3f_xyz = Vector3f(1,1,1);
const Vector3d Vector3d_xyz = Vector3d(1,1,1);
const Vector3f Vector3f_neg_xyz = Vector3f(-1,-1,-1);
const Vector3d Vector3d_neg_xyz = Vector3d(-1,-1,-1);
  
//min max
const Vector3f Vector3f_min = Vector3f(1e-8f, 1e-8f, 1e-8f);
const Vector3d Vector3d_min = Vector3d(1e-16f, 1e-16f, 1e-16f);
const Vector3f Vector3f_max = Vector3f(1e8f, 1e8f, 1e8f);
const Vector3d Vector3d_max = Vector3d(1e16f, 1e16f, 1e16f);
  
// Vector4 constants
//zero
const Vector4f Vector4f_zero = Vector4f(0.0f);
const Vector4d Vector4d_zero = Vector4d(0.0);
  
//one
const Vector4f Vector4f_one = Vector4f(1.0f);
const Vector4d Vector4d_one = Vector4d(1.0);
const Vector4f Vector4f_neg_one = Vector4f(-1.0f);
const Vector4d Vector4d_neg_one = Vector4d(-1.0);
  
  
//x axis
const Vector4f Vector4f_x = Vector4f(1,0,0,0);
const Vector4d Vector4d_x = Vector4d(1,0,0,0);
const Vector4f Vector4f_neg_x = Vector4f(-1,0,0,0);
const Vector4d Vector4d_neg_x = Vector4d(-1,0,0,0);
  
//y axis
const Vector4f Vector4f_y = Vector4f(0,1,0,0);
const Vector4d Vector4d_y = Vector4d(0,1,0,0);
const Vector4f Vector4f_neg_y = Vector4f(0,-1,0,0);
const Vector4d Vector4d_neg_y = Vector4d(0,-1,0,0);
  
//z axis
const Vector4f Vector4f_z = Vector4f(0,0,1,0);
const Vector4d Vector4d_z = Vector4d(0,0,1,0);
const Vector4f Vector4f_neg_z = Vector4f(0,0,-1,0);
const Vector4d Vector4d_neg_z = Vector4d(0,0,-1,0);
  
//w axis
const Vector4f Vector4f_w = Vector4f(0,0,0,1);
const Vector4d Vector4d_w = Vector4d(0,0,0,1);
const Vector4f Vector4f_neg_w = Vector4f(0,0,0,-1);
const Vector4d Vector4d_neg_w = Vector4d(0,0,0,-1);
 
#endif
