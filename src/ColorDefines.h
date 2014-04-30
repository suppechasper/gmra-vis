// -*- C++ -*-
// <@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@>//
//                     __    __  __  ____  _  _  _____                     //
//                    /__\  (  \/  )(_  _)( \( )(  _  )                    //
//                   /(__)\  )    (  _)(_  )  (  )(_)(                     //
//                  (__)(__)(_/\/\_)(____)(_)\_)(_____)                    //
//                                       Building Blocks                   //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ColorDefines.h
// Written by Kristi Potter, 2013
// Definitions of colors
// <@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@><@>//
#ifndef _COLOR_DEFINES_H
#define _COLOR_DEFINES_H
#include "Color.h"
#include <array>

// -:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:- //
// Color Constants
// -:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:- //
  
// RGB
const ColorF black            = ColorF(0.f, 0.f, 0.f, 1.f);
const ColorF blue             = ColorF(0.f, 0.f, 1.f, 1.f);
const ColorF green            = ColorF(0.f, 1.f, 0.f, 1.f);
const ColorF lime             = ColorF(.2f, .8f, .2f, 1.f);
const ColorF seagreen         = ColorF(.18, .75, .34, 1.f);
const ColorF red              = ColorF(1.f, 0.f, 0.f, 1.f);
const ColorF white            = ColorF(1.f, 1.f, 1.f, 1.f);
const ColorF yellow           = ColorF(1.f, .96f, 0.f, 1.f);
const ColorF olivegreen       = ColorF(.33, 0.5, 0.f, 1.f);
const ColorF cornflower       = ColorF(.33, 0.5, 1.f, 1.f);
const ColorF purple           = ColorF(.33, 0.f, .56, 1.f);
const ColorF purple2          = ColorF(.57, .17, .93, 1.f);
const ColorF dkpurple         = ColorF(.33, .1,  .55, 1.f);
const ColorF seafoam          = ColorF(.33, 1.f, .56, 1.f);
const ColorF spring           = ColorF(0.f, 1.f, .56, 1.f);
const ColorF lemon            = ColorF(1.f, 1.f, .56, 1.f);
const ColorF royalblue        = ColorF(0.f, 0.f, .56, 1.f);
const ColorF gray             = ColorF(0.5f, .5f,.5f, 1.f);
const ColorF grey             = ColorF(.05f, .05f, .02f, 1.f);
const ColorF ltgray           = ColorF(.95f, .95f,.95f, 1.f);
const ColorF dkgray           = ColorF(.35f, .35f,.35f, 1.f);
const ColorF dkgreengray      = ColorF(.15f, .2f,.15f, 1.f);
const ColorF dkblue           = ColorF(0.f, 0.f, 0.5, 1.f);
const ColorF ltsalmon         = ColorF(1.f, .63, .48, 1.f);
const ColorF orange           = ColorF(1.0, .65, 0.f, 1.f);
const ColorF maroon           = ColorF(69.0, .19, .38, 1.f);
const ColorF cyan             = ColorF(.87, 1.0, 1.f, 1.f);
const ColorF dkbluegray       = ColorF(.41, .41, .51, 1.f); 
const ColorF honeydew         = ColorF(.94, 1.f, .94, 1.f);
const ColorF slateblue        = ColorF(.51, .43, 1.f, 1.f);
const ColorF opaque           = ColorF(1.f, 1.f, 1.f, 1.f);
const ColorF transp           = ColorF(0.f, 0.f, 0.f, 0.f);
const ColorF charcoal         = ColorF(.20, .20, .20, 0.f);
const ColorF aqua             = ColorF(.31, 1.f, 1.f);
const ColorF persianGreen     = ColorF(0.f, .64f, .51f);
const ColorF blazeOrange      = ColorF(1.f, .39, 0.f);
const ColorF grenadier        = ColorF(.823, .16, .02);
const ColorF envy             = ColorF(0.f, .8, 0.f);
const ColorF flushMahogany    = ColorF(191/255.f, 58/255.f, 48/255.f);
const ColorF hokeyPokey       = ColorF(191/255.f, 169/255.f, 48/255.f);
const ColorF minsk            = ColorF(68/255.f, 43/255.f, 131/255.f);


// HSV colors
const ColorF green_hsv          = ColorF(125.0, 1.f, 1.f,   1.f);
const ColorF blue_hsv           = ColorF(230.0, 1.f, 1.f,   1.f);
const ColorF red_hsv            = ColorF(0.f,   1.f, 1.f,   1.f);
const ColorF purple_hsv         = ColorF(261.f, 1.f, 0.97f, 1.f);

// Color lists
const ColorF colorList[] = {flushMahogany, hokeyPokey, minsk, blazeOrange, persianGreen, charcoal, slateblue};

std::array<ColorF, 12> set3class12 = {ColorI(141,211,199).toFloat(),ColorI(255,255,179).toFloat(),ColorI(190,186,218).toFloat(),
				      ColorI(251,128,114).toFloat(),ColorI(128,177,211).toFloat(),ColorI(253,180,98).toFloat(),
				      ColorI(179,222,105).toFloat(),ColorI(252,205,229).toFloat(),ColorI(217,217,217).toFloat(),
				      ColorI(188,128,189).toFloat(),ColorI(204,235,197).toFloat(),ColorI(255,237,111).toFloat()};
std::array<ColorF,12> pairedclass12 = {ColorI(31,120,180).toFloat(),ColorI(178,223,138).toFloat(),
				       ColorI(51,160,44).toFloat(),ColorI(251,154,153).toFloat(),ColorI(227,26,28).toFloat(),
				       ColorI(253,191,111).toFloat(),ColorI(255,127,0).toFloat(),ColorI(202,178,214).toFloat(),
				       ColorI(106,61,154).toFloat(),ColorI(255,255,153).toFloat(),ColorI(177,89,40).toFloat(),ColorI(166,206,227).toFloat()};
std::array<ColorF,9> set1class9 = {ColorI(228,26,28).toFloat(),ColorI(55,126,184).toFloat(),ColorI(77,175,74).toFloat(),
				     ColorI(152,78,163).toFloat(),ColorI(255,127,0).toFloat(),ColorI(255,255,51).toFloat(),
				     ColorI(166,86,40).toFloat(),ColorI(247,129,191).toFloat(),ColorI(153,153,153).toFloat()};
#endif
