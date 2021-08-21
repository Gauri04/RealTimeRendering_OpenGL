__kernel void sinwaveKernel(__global float4 *pos, int meshWidth, int meshHeight, float time)
{
	int x = get_global_id(0);
	int y = get_global_id(1);
	float u = x / (float)meshWidth;
	float v = y / (float)meshHeight;
	u = u * 2.0f - 1.0f;
	v = v * 2.0f - 1.0f;
	float frequency = 4.0f;
	float w = (float)sin(u * frequency + time) * cos(v * frequency + time) * 0.5f;
	pos[y * meshWidth + x] = (float4)(u, w, v, 1.0f);

}

/*
void launchOpenCLKernel(float4 *pos, int meshWidth, int meshHeight, float time)
{
	dim3 block = dim3(8, 8, 1);
	dim3 grid = dim3(meshWidth / block.x, meshHeight / block.y, 1);
	sinwaveKernel << <grid, block >> > (pos, meshWidth, meshHeight, time);
}
*/
