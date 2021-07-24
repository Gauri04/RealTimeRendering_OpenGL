#pragma once
#include<cuda_gl_interop.h>
#include<cuda_runtime.h>

void launchCudaKernel(float4*, int, int, float);