
#ifndef _ETC_H
#define _ETC_H
template<class TypeIn, class TypeOut>                              // affine()
inline TypeOut affine(const TypeIn &i,  const TypeIn &val, const TypeIn &I, const TypeOut &o, const TypeOut &O){
    if (i == I) return O; else return (O - o)*(val - i) / (I - i) + o; }
#endif
