#pragma once
#include"CommonCode.h"



extern "C" void GROpenLogFileTree();

extern "C" void GRPrintTree(const char*);

extern "C" void GRInitializeTree();

extern "C" void GRResizeTree(int, int);

extern "C" void GRDisplayTree();

extern "C" void GRSetViewMatrixTree(vmath::vec3, vmath::vec3, vmath::vec3);

extern "C" void GRIncAngleXTree();

extern "C" void GRIncAngleYTree();

extern "C" void GRDecAngleYTree();

extern "C" void GRIncAngleZTree();

extern "C" void GRIncModelXTree();

extern "C" void GRIncModelYTree();

extern "C" void GRIncModelZTree();

extern "C" void GRDecModelXTree();

extern "C" void GRDecModelYTree();

extern "C" void GRDecModelZTree();

extern "C" void GRIncModelXHouse();

extern "C" void GRIncModelYHouse();

extern "C" void GRIncModelZHouse();

extern "C" void GRDecModelXHouse();

extern "C" void GRDecModelYHouse();

extern "C" void GRDecModelZHouse();

extern "C" void GRPrintVarsTree();

extern "C" void GRSetVarsTree();

extern "C" void GRUninitializeTree();




