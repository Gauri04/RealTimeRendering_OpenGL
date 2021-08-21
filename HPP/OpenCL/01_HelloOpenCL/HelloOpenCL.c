#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<CL/OpenCL.h>

#pragma comment(lib, "OpenCL.lib")


cl_int ret_ocl;
cl_platform_id oclPlatformID;
cl_device_id oclComputeDeviceID;
cl_context oclContext;
cl_command_queue oclCommandQueue;
cl_program oclProgram;
cl_kernel oclKernel;

char* oclSourceCode = NULL;
size_t sizeKernelCodeLength;

float* hostInput1 = NULL;
float* hostInput2 = NULL;
float* hostOutput = NULL;

cl_mem deviceInput1 = NULL;
cl_mem deviceInput2 = NULL;
cl_mem deviceOutput = NULL;

int main(void)
{
	void cleanup(void);
	char* loadOclProgramSource(const char*, const char*, size_t *);

	int inputLength;

	inputLength = 5;
	hostInput1 = (float*)malloc(inputLength * sizeof(float));
	if (hostInput1 == NULL)
	{
		printf("\n CPU memory fatal error : Cannot allocate enough memory for HostInputArray1, exiting now..");
		exit(EXIT_FAILURE);
	}

	hostInput2 = (float*)malloc(inputLength * sizeof(float));
	if (hostInput2 == NULL)
	{
		printf("\n CPU memory fatal error : Cannot allocate enough memory for HostInputArray2, exiting now..");
		free(hostInput1);
		exit(EXIT_FAILURE);
	}

	hostOutput = (float*)malloc(inputLength * sizeof(float));
	if (hostOutput == NULL)
	{
		printf("\n CPU memory fatal error : Cannot allocate enough memory for HostInputArray1, exiting now..");
		free(hostInput1);
		free(hostInput2);
		exit(EXIT_FAILURE);
	}

	hostInput1[0] = 101.0;
	hostInput1[1] = 102.0;
	hostInput1[2] = 103.0;
	hostInput1[3] = 104.0;
	hostInput1[4] = 105.0;

	hostInput2[0] = 201.0;
	hostInput2[1] = 202.0;
	hostInput2[2] = 203.0;
	hostInput2[3] = 204.0;
	hostInput2[4] = 205.0;

	ret_ocl = clGetPlatformIDs(1, &oclPlatformID, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clGetPlatformID failed, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	ret_ocl = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, 1, &oclComputeDeviceID, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clGetDeviceIDs failed, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	oclContext = clCreateContext(NULL, 1, &oclComputeDeviceID, NULL, NULL, &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clGetDeviceIDs failed, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	oclCommandQueue = clCreateCommandQueue(oclContext, oclComputeDeviceID, 0, &ret_ocl);

	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clCreateCommandQueue failed, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	cl_int status = 0;
	oclSourceCode = loadOclProgramSource("VecAdd.cl", "", &sizeKernelCodeLength);
	oclProgram = clCreateProgramWithSource(oclContext, 1, (const char **)&oclSourceCode, &sizeKernelCodeLength, &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clCreateProgramWithSource failed, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	ret_ocl = clBuildProgram(oclProgram, 0, NULL, NULL, NULL, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clBuildProgram failed, error code : %d. Exiting now ..", ret_ocl);
		size_t len;
		char buffer[2048];
		clGetProgramBuildInfo(oclProgram, oclComputeDeviceID, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("\n OpenCL program build log : %s", buffer);
		cleanup();
		exit(EXIT_FAILURE);
	}

	oclKernel = clCreateKernel(oclProgram, "vecAdd", &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clCreateKernel failed, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	int size = inputLength * sizeof(cl_float);

	deviceInput1 = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clCreateBuffer failed for 1st input array, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	deviceInput2 = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clCreateBuffer failed for 2nd input array, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	deviceOutput = clCreateBuffer(oclContext, CL_MEM_WRITE_ONLY, size, NULL, &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clCreateBuffer failed for output array, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	ret_ocl = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void *)&deviceInput1);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clSetKernelArg() failed for 1st input array, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	ret_ocl = clSetKernelArg(oclKernel, 1, sizeof(cl_mem), (void*)&deviceInput2);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clSetKernelArg() failed for 2nd input array, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	
	ret_ocl = clSetKernelArg(oclKernel, 2, sizeof(cl_mem), (void*)&deviceOutput);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clSetKernelArg() failed for output array, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	ret_ocl = clSetKernelArg(oclKernel, 3, sizeof(cl_int), (void*)&inputLength);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clSetKernelArg() failed for 4th argument, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	ret_ocl = clEnqueueWriteBuffer(oclCommandQueue, deviceInput1, CL_FALSE, 0, size, hostInput1, 0, NULL, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clEnqueueWriteBuffer() failed for 1st input device, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	ret_ocl = clEnqueueWriteBuffer(oclCommandQueue, deviceInput2, CL_FALSE, 0, size, hostInput2, 0, NULL, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clEnqueueWriteBuffer() failed for 2nd input device, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// run kernel
	size_t global_size = 5;
	ret_ocl = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);	// 1 in the param is for N-D, n = 1 i.e 1 dimension
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OpenCL error - clEnqueNDRRangeKernel() failed, error code : %d. Exiting now ..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	clFinish(oclCommandQueue);

	ret_ocl = clEnqueueReadBuffer(oclCommandQueue, deviceOutput, CL_TRUE, 0, size, hostOutput, 0, NULL, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
	printf("\n OpenCL error - clEnqueReadBuffer() failed, error code : %d. Exiting now ..", ret_ocl);
	cleanup();
	exit(EXIT_FAILURE);
	}

	// show result
	int i;
	for (i = 0; i < inputLength; i++)
	{
		printf("\n %f + %f = %f", hostInput1[i], hostInput2[i], hostOutput[i]);
	}
	cleanup();

	return(0);
}

void cleanup()
{
	// ocl objects
	if (oclSourceCode)
	{
		free((void*)oclSourceCode);
		oclSourceCode = NULL;
	}
	if (oclKernel)
	{
		free((void*)oclKernel);
		oclKernel = NULL;
	}
	if (oclProgram)
	{
		free((void*)oclProgram);
		oclProgram = NULL;
	}
	if (oclCommandQueue)
	{
		free((void*)oclCommandQueue);
		oclCommandQueue = NULL;
	}
	if (oclContext)
	{
		free((void*)oclContext);
		oclContext = NULL;
	}
	// device objects
	if (deviceInput1)
	{
		clReleaseMemObject(deviceInput1);
		deviceInput1 = NULL;
	}
	if (deviceInput2)
	{
		clReleaseMemObject(deviceInput2);
		deviceInput2 = NULL;
	}
	if (deviceOutput)
	{
		clReleaseMemObject(deviceOutput);
		deviceOutput = NULL;
	}
	// host objects
	if (hostInput1)
	{
		free(hostInput1);
		hostInput1 = NULL;
	}
	if (hostInput2)
	{
		free(hostInput2);
		hostInput2 = NULL;
	}
	if (hostOutput)
	{
		free(hostOutput);
		hostOutput = NULL;
	}
}

char* loadOclProgramSource(const char* filename, const char* preamble, size_t* sizeFinalLength)
{
	// variable declaration
	FILE* pFile = NULL;
	size_t sizeSourceLength;

	pFile = fopen(filename, "rb");		// read-binary
	if (pFile == NULL)
	{
		return(NULL);
	}

	size_t sizePreambleLength = (size_t)strlen(preamble);
	// get length of source code
	fseek(pFile, 0, SEEK_END);
	sizeSourceLength = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	// read code in buffer
	char* sourceString = (char*)malloc(sizeSourceLength + sizePreambleLength + 1);
	memcpy(sourceString, preamble, sizePreambleLength);
	if (fread((sourceString) + sizePreambleLength, sizeSourceLength, 1, pFile) != 1)
	{
		fclose(pFile);
		free(sourceString);
		return(0);
	}

	fclose(pFile);

	if (sizeFinalLength != 0)
	{
		*sizeFinalLength = sizeSourceLength + sizePreambleLength;
	}
	sourceString[sizeSourceLength + sizePreambleLength] = '\0';

	return(sourceString);
}


/* Compile and linking steps :
cl.exe /c /EHsc /I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\include" HelloOpenCL.c /link "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\lib\x64\"
link.exe HelloOpenCL.obj /LIBPATH:"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\lib\x64" /MACHINE:x64 /SUBSYSTEM:CONSOLE

*/







