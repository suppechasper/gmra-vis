// -*- C++ -*-
// <@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@>//
//                     __    __  __  ____  _  _  _____                     //
//                    /__\  (  \/  )(_  _)( \( )(  _  )                    //
//                   /(__)\  )    (  _)(_  )  (  )(_)(                     //
//                  (__)(__)(_/\/\_)(____)(_)\_)(_____)                    //
//                                       Building Blocks                   //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ColorMapFactories.h
// Written by Kristi Potter
// August 2010
// <@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@>//
#ifndef COLORMAPFACTORIES_H
#define COLORMAPFACTORIES_H

#include "ColorMap.h"

//-----------------------------------------------//
// Some predefined colormaps
//-----------------------------------------------//

// Two-color
class GreyscaleColormapFactory : public TwoColormapFactory {
public:
    GreyscaleColormapFactory(float min=0.0, float max=1.0) : TwoColormapFactory(ColorF(0.f,0.f,0.f), ColorF(1.f,1.f,1.f), min, max){ }
};

class YellowBlueColormapFactory : public TwoColormapFactory{
public:
    YellowBlueColormapFactory(float min=0.0, float max=1.0) : TwoColormapFactory(ColorF(1.f, .98, 0.f), ColorF(0.f, .3, 1.f), min, max){}
};

class BlueYellowColormapFactory : public TwoColormapFactory{
public:
    BlueYellowColormapFactory(float min=0.0, float max=1.0) : TwoColormapFactory(ColorF(0.f, .3, 1.f), ColorF(1.f, .98, 0.f), min, max){}
};

class BlackYellowColormapFactory : public TwoColormapFactory{
public:
    BlackYellowColormapFactory(float min=0.0, float max=1.0) : TwoColormapFactory(ColorF(0.f, 0.f, 0.f), ColorF(1.f, .98, 0.f), min, max){}
};

class PurpleYellowColormapFactory : public TwoColormapFactory{
public:
    PurpleYellowColormapFactory(float min=0.0, float max=1.0) : TwoColormapFactory(ColorF(.22f, .08, 0.69f), ColorF(1.f, 0.83, 0.f), min, max){}
};

class MagentaLimeColormapFactory : public TwoColormapFactory{
public:
    MagentaLimeColormapFactory(float min=0.0, float max=1.0) : TwoColormapFactory(ColorF(.68, 0.04, 0.62), ColorF(.78, .96, 0.05), min, max){}
};

class TealTangerineColormapFactory : public TwoColormapFactory{
public:
  TealTangerineColormapFactory(float min=0.0, float max=1.0) : TwoColormapFactory(ColorF(.03, 0.63, 0.55), ColorF(1.f, .44, 0.05), min, max){}
};

class RedGreenColormapFactory : public TwoColormapFactory{
public:
    RedGreenColormapFactory(float min, float max) : TwoColormapFactory(red, green, min, max){}
};

// Three-color
class RainbowColormapFactory : public ThreeColormapFactory{
public:
    RainbowColormapFactory(float min=0.0, float max=1.0) :
      ThreeColormapFactory(ColorF(0,.4,.8), ColorF(.8,.8,.0), ColorF(.82,.16,.02), min, max){}
};

class GreenYellowOrangeColormapFactory : public ThreeColormapFactory{
public:
    GreenYellowOrangeColormapFactory(float min=0.0, float max=1.0) :
      ThreeColormapFactory(persianGreen, yellow, blazeOrange, min, max){}
};

class MinskTwilightColormapFactory : public ThreeColormapFactory{
public:
    MinskTwilightColormapFactory(float min=0.0, float max=1.0) :
      ThreeColormapFactory( flushMahogany, hokeyPokey, minsk, min, max){}
};

class YellowWhiteBlueColormapFactory : public ThreeColormapFactory{
public:
  YellowWhiteBlueColormapFactory(float min=0.0, float max=1.0) :
    ThreeColormapFactory( yellow, white, aqua, min, max){}
};

class RedWhiteGreenColormapFactory : public ThreeColormapFactory{
public:
   
  RedWhiteGreenColormapFactory(float min=0.0, float max=1.0) :
    ThreeColormapFactory( grenadier, white, envy, min, max){}
};

// Discrete
class Set3DiscreteColormapFactory : public DiscreteColormapFactory{

public: 
  Set3DiscreteColormapFactory(float numColors = 10, float min =0.0, float max=1.0) :
    DiscreteColormapFactory(std::vector<ColorF>(std::begin(set3class12), std::end(set3class12)), 
			    numColors, min, max){}
};

class PairedDiscreteColormapFactory : public DiscreteColormapFactory{

public: 
  PairedDiscreteColormapFactory(float numColors = 10, float min =0.0, float max=1.0) :
    DiscreteColormapFactory(std::vector<ColorF>(std::begin(pairedclass12), std::end(pairedclass12)), 
			    numColors, min, max) {}
};

// TwoD
class Paired12DiscreteGreyscaleColormapFactory : public TwoDDiscreteColormapFactory{

public:
  Paired12DiscreteGreyscaleColormapFactory(float numColors=10, float minI=0.0, float maxI= 1.0,
					 float minF=0.0, float maxF=0.0)
    : TwoDDiscreteColormapFactory(std::vector<ColorF>(std::begin(pairedclass12), std::end(pairedclass12)),
				  new GreyscaleColormapFactory(),
				  numColors, minI, maxI, minF, maxF){}
};

class Set3DiscreteGreyscaleColormapFactory : public TwoDDiscreteColormapFactory{

public:
  Set3DiscreteGreyscaleColormapFactory(float numColors=10, float minI=0.0, float maxI= 1.0,
				       float minF=0.0, float maxF=0.0)
    : TwoDDiscreteColormapFactory(std::vector<ColorF>(std::begin(set3class12), std::end(set3class12)),
				  new GreyscaleColormapFactory(),
				  numColors, minI, maxI, minF, maxF){}
};





#endif // COLORMAPFACTORIES_H
