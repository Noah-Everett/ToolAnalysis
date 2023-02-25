///////////////////////////////////
// DetectorResponsePredictor.cpp //
///////////////////////////////////
//
// Noah Everett (noah.everett@mines.sdsmt.edu)
// Created Febuary 25, 2023
//
// Child class of abstract DetectorResponsePredictor
// that calculates the detector response for tank PMTs.
// All information (PDFs, simulations, etc.) needs to 
// be set by the user.
//
// Notation: t_ = function variable
//           m_ = member variable
//           naming: mainVariable_subCategory (use '_' like LaTeX)
//
// Units: All units are standard as follows:
//     distance [m]
//     angle    [rad]
//
///////////////////////////////////

#include "DetectorResponsePredictor_TankPMT.h"

#include <math.h>
