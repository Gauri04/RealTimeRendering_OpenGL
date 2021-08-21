#include<stdio.h>
#include<stdlib.h>		// for exit()
#include<string.h>		// for strlen
#include<math.h>		// for fabs()
#include<CL/OpenCL.h>
#include"helper_timer.h"

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

// odd nuber 11444777 from nvidia samples
int iNumberOfArrayElements = 11444777;
size_t localWorkSize = 256;
size_t globalWorkSize;

float* hostInput1 = NULL;
float* hostInput2 = NULL;
float* hostOutput = NULL;
float* gold = NULL;

cl_mem deviceInput1 = NULL;
cl_mem deviceInput2 = NULL;
cl_mem deviceOutput = NULL;

float timeOnCpu;
float timeOnGpu;

int main()
{
	// function declaration
	void fillFloatArrayWithRandomNumbers(float *, int);
	size_t roundGlobalSizeToNearestMultipleOfLocalSize(int, unsigned int);
	void vecAddHost(const float*, const float*, float*, int);
	char* loadOclProgramSource(const char*, const char*, size_t*);
	void cleanup();

	// code

	// allocate memory to host vars
	hostInput1 = (float*)malloc(sizeof(float) * iNumberOfArrayElements);
	if (hostInput1 == NULL)
	{
		printf("\n CPU memory fatal error : cannot allocate memory for hostInput1. Exiting Now..");
		cleanup();
		exit(EXIT_FAILURE);
	}

	hostInput2 = (float*)malloc(sizeof(float) * iNumberOfArrayElements);
	if (hostInput2 == NULL)
	{
		printf("\n CPU memory fatal error : cannot allocate memory for hostInput2. Exiting Now..");
		cleanup();
		exit(EXIT_FAILURE);
	}

	hostOutput = (float*)malloc(sizeof(float) * iNumberOfArrayElements);
	if (hostOutput == NULL)
	{
		printf("\n CPU memory fatal error : cannot allocate memory for hostOutput. Exiting Now..");
		cleanup();
		exit(EXIT_FAILURE);
	}

	gold = (float*)malloc(sizeof(float) * iNumberOfArrayElements);
	if (gold == NULL)
	{
		printf("\n CPU memory fatal error : cannot allocate memory for gold. Exiting Now..");
		cleanup();
		exit(EXIT_FAILURE);
	}

	// fill above input host vectors with arbitrary but hardcoded data
	fillFloatArrayWithRandomNumbers(hostInput1, iNumberOfArrayElements);
	fillFloatArrayWithRandomNumbers(hostInput2, iNumberOfArrayElements);

	// get OpenCL supporting platform's ID
	ret_ocl = clGetPlatformIDs(1, &oclPlatformID, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clGetPlatformIDs() failed : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	// get OCL supporting GPU device's ID
	ret_ocl = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, 1, &oclComputeDeviceID, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clGetDeviceIDs() failed : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	char gpuName[255];
	clGetDeviceInfo(oclComputeDeviceID, CL_DEVICE_NAME, sizeof(gpuName), &gpuName, NULL);
	printf("\n GPU Name : %s", gpuName);

	// create OCL compute context
	oclContext = clCreateContext(NULL, 1, &oclComputeDeviceID, NULL, NULL, &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clCreateContext() failed : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	// create command queue
	oclCommandQueue = clCreateCommandQueue(oclContext, oclComputeDeviceID, 0, &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clCreateCommandQueue() failed : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	// create OpenCL program from .cl
	
	oclSourceCode = loadOclProgramSource("VecAdd.cl", "", &sizeKernelCodeLength);
	oclProgram = clCreateProgramWithSource(oclContext, 1, (const char **)&oclSourceCode, &sizeKernelCodeLength, &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clCreateProgramWithSource() failed : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	// build the ocl program
	ret_ocl = clBuildProgram(oclProgram, 0, NULL, NULL, NULL, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clBuildProgram() failed : %d. Exiting now..", ret_ocl);
		size_t len; 
		char buffer[2048];
		clGetProgramBuildInfo(oclProgram, oclComputeDeviceID, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("\n OpenCL Program build log : %s", buffer);
		cleanup();
		exit(EXIT_FAILURE);
	}
	printf("\n debug reading source ocl code");
	// create ocl kernel by passing kernel function name that we used in our .cl file
	oclKernel = clCreateKernel(oclProgram, "vecAdd", &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clCreateKernel() failed : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// allocate device memory
	int size = sizeof(cl_float) * iNumberOfArrayElements;
	deviceInput1 = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clCreateBuffer() failed for deviceInput1 : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	deviceInput2 = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clCreateBuffer() failed for deviceInput2 : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	deviceOutput = clCreateBuffer(oclContext, CL_MEM_WRITE_ONLY, size, NULL, &ret_ocl);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clCreateBuffer() failed for deviceOutput : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set OCL kernel arguments. our ocl kernel has 4 arguments 0,1,2,3. 
	// set 0 based 0th argument i.e deviceInput1
	ret_ocl = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void *)&deviceInput1);
	// deviceInput1 maps to 'in1' param of kernel function in .cl file 
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clSetKernelArg() failed for deviceInput1 : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	// set 0 based 1st argument i.e devicceInpu2
	ret_ocl = clSetKernelArg(oclKernel, 1, sizeof(cl_mem), (void *)&deviceInput2);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clSetKernelArg() failed for deviceInput2 : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	// set 0 based 2nd argument i.e deviceOutput
	ret_ocl = clSetKernelArg(oclKernel, 2, sizeof(cl_mem), (void *)&deviceOutput);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clSetKernelArg() failed for deviceOutput : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	// set 0 based 3rd argument i.e length
	ret_ocl = clSetKernelArg(oclKernel, 3, sizeof(cl_int), (void*)&iNumberOfArrayElements);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clSetKernelArg() failed for len : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// write above hostinput buffer to device memory
	ret_ocl = clEnqueueWriteBuffer(oclCommandQueue, deviceInput1, CL_FALSE, 0, size, hostInput1, 0, NULL, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clEnqueueWriteBuffer() failed for deviceInput1 : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	ret_ocl = clEnqueueWriteBuffer(oclCommandQueue, deviceInput2, CL_FALSE, 0, size, hostInput2, 0, NULL, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clEnqueueWriteBuffer() failed for deviceInput2 : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// run the kernel
	globalWorkSize = roundGlobalSizeToNearestMultipleOfLocalSize(localWorkSize, iNumberOfArrayElements);

	// start timer
	StopWatchInterface* timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	ret_ocl = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clEnqueueNDRangeKernel() : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}
	// finish ocl command queue
	clFinish(oclCommandQueue);

	// stop timer
	sdkStopTimer(&timer);
	timeOnGpu = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);

	// read back the result from device memory to host memory
	ret_ocl = clEnqueueReadBuffer(oclCommandQueue, deviceOutput, CL_TRUE, 0, size, hostOutput, 0, NULL, NULL);
	if (ret_ocl != CL_SUCCESS)
	{
		printf("\n OCL error : clEnqueueReadBuffer() : %d. Exiting now..", ret_ocl);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// calculation on CPU
	vecAddHost(hostInput1, hostInput2, gold, iNumberOfArrayElements);
	// compare results for golden host
	const float epsilon = 0.000001f;
	bool bAccuracy = true;
	int breakValue;
	int i;
	for (i = 0; i < iNumberOfArrayElements; i++)
	{
		float val1 = gold[i];
		float val2 = hostOutput[i];
		if (fabs(val1 - val2) > epsilon)
		{
			bAccuracy = false;
			breakValue = i;
			break;
		}
	}

	if (bAccuracy == false)
	{
		printf("\n breakValue : %d", breakValue);
	}
	char str[255];
	if (bAccuracy == true)
	{
		sprintf(str, "%s", "Comparison of output arrays on CPU and GPU are accurate within limit of 0.000001f");
	}
	else
	{
		sprintf(str, "%s", "Not all comparison of output arrays on CPU and GPU are accurate within limit of 0.000001f");
	}

	printf("\n 1st array from 0th element : %.6f to %dth element : %.6f", hostInput1[0], iNumberOfArrayElements - 1, hostInput1[iNumberOfArrayElements - 1]);
	printf("\n 2nd array from 0th element : %.6f to %dth element : %.6f", hostInput2[0], iNumberOfArrayElements - 1, hostInput2[iNumberOfArrayElements - 1]);
	printf("\n GlobalWorkSize : %u and localWorkSize : %u", (unsigned int)globalWorkSize, (unsigned int)localWorkSize);
	printf("\n Sum of Each element from above two arrays creates third array as : ");
	printf("\n\t 3rd array from 0th element : %.6f to %dth element : %.6f", hostOutput[0], iNumberOfArrayElements - 1, hostOutput[iNumberOfArrayElements - 1]);
	printf("\n\t GOLD 3rd array from 0th element : %.6f to %dth element : %.6f", gold[0], iNumberOfArrayElements - 1, gold[iNumberOfArrayElements - 1]);
	printf("\n Time taken to perform above addition on CPU : %.6f (ms)", timeOnCpu);
	printf("\n Time taken to perform above addition on GPU : %.6f (ms)", timeOnGpu);
	printf("\n %s", str);

	cleanup();

	return(0);
}

void cleanup()
{
	printf("\n in cleanup");
	if (oclSourceCode)
	{
		free((void *)oclSourceCode);
		oclSourceCode = NULL;
	}
	if (oclKernel)
	{
		clReleaseKernel(oclKernel);
		oclKernel = NULL;
	}
	if (oclProgram)
	{
		clReleaseProgram(oclProgram);
		oclProgram = NULL;
	}
	if (oclCommandQueue)
	{
		clReleaseCommandQueue(oclCommandQueue);
		oclCommandQueue = NULL;
	}
	if (oclContext)
	{
		clReleaseContext(oclContext);
		oclContext = NULL;
	}

	// free device memory
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

	// free host memory
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
	if (gold)
	{
		free(gold);
		gold = NULL;
	}
	printf("\n freed the resources. Exiting now");

}

void fillFloatArrayWithRandomNumbers(float *arr, int size)
{
	int i;
	const float fscale = 1.0f / (float)RAND_MAX;
	for (i = 0; i < size; ++i)
	{
		arr[i] = fscale * rand();
	}
}

size_t roundGlobalSizeToNearestMultipleOfLocalSize(int localSize, unsigned int globalSize)
{
	unsigned int r = globalSize % localSize;
	if (r == 0)
	{
		return(globalSize);
	}
	else
	{
		return(globalSize + localSize - r);
	}
}

void vecAddHost(const float* pFloatData1, const float* pFloatData2, float* pFloatResult, int iNumElements)
{
	int i;
	StopWatchInterface* timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	for (i = 0; i < iNumElements; i++)
	{
		pFloatResult[i] = pFloatData1[i] + pFloatData2[i];
	}

	sdkStopTimer(&timer);
	timeOnCpu = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);
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
	if (fread((sourceString)+sizePreambleLength, sizeSourceLength, 1, pFile) != 1)
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

/* compile and link commands
cl.exe /c /EHsc /I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\include" VecAdd.cpp /link "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\lib\x64\"
link.exe VecAdd.obj /LIBPATH:"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\lib\x64" /MACHINE:x64 /SUBSYSTEM:CONSOLE

*/
