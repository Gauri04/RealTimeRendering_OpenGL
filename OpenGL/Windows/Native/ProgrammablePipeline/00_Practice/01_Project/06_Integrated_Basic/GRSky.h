#pragma once
#include"CommonCode.h"
#include"Sphere.h"


extern "C" void GROpenLogFileSky();

extern "C" void GRPrintLogSky(const char*);

extern "C" void GRInitializeSky();

extern "C" void GRResizeSky(int, int);

extern "C" void GRDisplaySky();

extern "C" void GRUninitializeSky();

extern "C" void GRSetViewMatrixSky(vmath::vec3, vmath::vec3, vmath::vec3);

extern "C" void GRIncViewXSky();

extern "C" void GRIncViewYSky();

extern "C" void GRIncViewZSky();

extern "C" void GRDecViewXSky();

extern "C" void GRDecViewYSky();

extern "C" void GRDecViewZSky();

extern "C" void GRPrintVarsSky();

extern "C" void GRIncScaleSky();

extern "C" void GRDecScaleSky();

extern "C" void GRSetVarsSky();





