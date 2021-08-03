#include<stdio.h>
#include<d3d11.h> // similar as GL/gl.h is important for OGL
#include<math.h>

#pragma comment(lib, "d3d11.lib")
// directx graphics infrastructure, analogous to wgl
#pragma comment(lib, "dxgi.lib")

int main(void)
{
	// variable declarations
	IDXGIFactory* pIDXGIFactory = NULL;
	IDXGIAdapter* pIDXGIAdapter = NULL;
	DXGI_ADAPTER_DESC dxgiAdapterDesc;
	HRESULT hr;
	char str[255];

	// code
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pIDXGIFactory);
	if (FAILED(hr))
	{
		printf("\n CreateDXGIFactory() failed.. Exiting Now");
		goto cleanup;
	}

	if (pIDXGIFactory->EnumAdapters(0, &pIDXGIAdapter) == DXGI_ERROR_NOT_FOUND)
	{
		printf("\n DXGIAdapter cannot be found.. Exiting Now");
		goto cleanup;
	}
	// if success
	ZeroMemory((void*)&dxgiAdapterDesc, sizeof(DXGI_ADAPTER_DESC));
	hr = pIDXGIAdapter->GetDesc(&dxgiAdapterDesc);
	// convert wchar string to char
	WideCharToMultiByte(CP_ACP, 0, dxgiAdapterDesc.Description, 255, str, 255, NULL, NULL);
	printf("\n Graphic card name : %s", str);
	printf("\n Graphic card VRAM : %I64d bytes", __int64(dxgiAdapterDesc.DedicatedVideoMemory));
	printf("\n VRAM in GB : %d", (int)(ceil((dxgiAdapterDesc.DedicatedVideoMemory)/1024/1024/1024)));

cleanup:
	if (pIDXGIAdapter)
	{
		pIDXGIAdapter->Release();
		pIDXGIAdapter = NULL;
	}

	if (pIDXGIFactory)
	{
		pIDXGIFactory->Release();
		pIDXGIFactory = NULL;
	}

	return(0);
}

