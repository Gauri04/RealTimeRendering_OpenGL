#pragma once
#include"CommonCode.h"



extern "C" void GROpenLogFileTerrain();

extern "C" void GRPrintLogTerrain(const char*);

extern "C" void GRInitializeTerrain();

extern "C" void GRResizeTerrain(int, int);

extern "C" void GRDisplayTerrain();

extern "C" void GRUninitializeTerrain();

extern "C" void GRSetViewMatrixTerrain(vmath::vec3, vmath::vec3, vmath::vec3);

extern "C" void GRIncViewXTerrain();

extern "C" void GRIncViewYTerrain();

extern "C" void GRIncViewZTerrain();

extern "C" void GRDecViewXTerrain();

extern "C" void GRDecViewYTerrain();

extern "C" void GRDecViewZTerrain();

extern "C" void GRPrintVarsTerrain();

extern "C" void GRIncDepthTerrain();

extern "C" void GRDecDepthTerrain();

extern "C" void GRSetVarsTerrain();

extern "C" void GRGetViewXYZTerrain(float&, float&, float&);
extern "C" void GRGetViewEyeXYZTerrain(float&, float&, float&);

extern "C" void GRIncAngleX();
extern "C" void GRIncAngleY();
extern "C" void GRIncAngleZ();



