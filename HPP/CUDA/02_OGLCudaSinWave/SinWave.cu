#include<stdio.h>
#include<vector_types.h>



__global__ void sinwaveKernel(float4 * pos, int meshWidth, int meshHeight, float time)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	float u = x / (float)meshWidth;
	float v = y / (float)meshHeight;
	u = u * 2.0f - 1.0f;
	v = v * 2.0f - 1.0f;
	float frequency = 4.0f;
	float w = sinf(u * frequency + time) * cosf(v * frequency + time) * 0.5f;
	pos[y * meshWidth + x] = make_float4(u, w, v, 1.0f);

}

void launchCudaKernel(float4 *pos, int meshWidth, int meshHeight, float time)
{
	dim3 block = dim3(8, 8, 1);
	dim3 grid = dim3(meshWidth / block.x, meshHeight / block.y, 1);
	sinwaveKernel << <grid, block >> > (pos, meshWidth, meshHeight, time);
}
