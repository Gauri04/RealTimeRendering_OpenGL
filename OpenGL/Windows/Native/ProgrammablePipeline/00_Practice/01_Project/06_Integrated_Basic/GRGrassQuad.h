#pragma once
#include"CommonCode.h"


extern "C" void GROpenLogFileGrassQuad();

extern "C" void GRPrintLogGrassQuad(const char*);

extern "C" void GRInitializeGrassQuad();

extern "C" void GRResizeGrassQuad(int, int);

extern "C" void GRDisplayGrassQuad();

extern "C" void GRUninitializeGrassQuad();

extern "C" void GRSetViewMatrixGrassQuad(vmath::vec3, vmath::vec3, vmath::vec3);

extern "C" void GRIncAngleXGrassQuad();

extern "C" void GRIncAngleYGrassQuad();

extern "C" void GRDecAngleYGrassQuad();

extern "C" void GRIncAngleZGrassQuad();

extern "C" void GRIncModelXGrassQuad();

extern "C" void GRIncModelYGrassQuad();

extern "C" void GRIncModelZGrassQuad();

extern "C" void GRDecModelXGrassQuad();

extern "C" void GRDecModelYGrassQuad();

extern "C" void GRDecModelZGrassQuad();

extern "C" void GRPrintVarsGrassQuad();

extern "C" void GRSetVarsGrassQuad();





