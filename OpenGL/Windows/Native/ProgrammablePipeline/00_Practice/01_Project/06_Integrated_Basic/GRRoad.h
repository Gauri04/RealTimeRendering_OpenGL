#pragma once
#include"CommonCode.h"


extern "C" void GROpenLogFileRoad();

extern "C" void GRPrintLog(const char*);

extern "C" void GRInitializeRoad();

extern "C" void GRResizeRoad(int, int);

extern "C" void GRDisplayRoad();

extern "C" void GRSetViewMatrixRoad(vmath::vec3, vmath::vec3, vmath::vec3);

extern "C" void GRIncAngleXRoad();

extern "C" void GRIncAngleYRoad();

extern "C" void GRDecAngleYRoad();

extern "C" void GRIncAngleZRoad();

extern "C" void GRIncModelXRoad();

extern "C" void GRIncModelYRoad();

extern "C" void GRIncModelZRoad();

extern "C" void GRDecModelXRoad();

extern "C" void GRDecModelYRoad();

extern "C" void GRDecModelZRoad();

extern "C" void GRPrintVarsRoad();

extern "C" void GRSetVarsRoad();

extern "C" void GRUninitializeRoad();



