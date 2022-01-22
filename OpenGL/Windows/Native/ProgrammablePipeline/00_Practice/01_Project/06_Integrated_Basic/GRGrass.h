#pragma once
#include"CommonCode.h"
#include"FreeImage.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include<vector>


extern "C" void GROpenLogFileGrass();

extern "C" void GRPrintGrass(const char*);

extern "C" void GRInitializeGrass();

extern "C" void GRResizeGrass(int, int);

extern "C" void GRDisplayGrass();

extern "C" void GRSetViewMatrixGrass(vmath::vec3, vmath::vec3, vmath::vec3);

extern "C" void GRIncAngleXGrass();

extern "C" void GRIncAngleYGrass();

extern "C" void GRDecAngleYGrass();

extern "C" void GRIncAngleZGrass();

extern "C" void GRIncModelXGrass();

extern "C" void GRIncModelYGrass();

extern "C" void GRIncModelZGrass();

extern "C" void GRDecModelXGrass();

extern "C" void GRDecModelYGrass();

extern "C" void GRDecModelZGrass();

extern "C" void GRPrintVarsGrass();

extern "C" void GRSetVarsGrass();

extern "C" void GRUninitializeGrass();




