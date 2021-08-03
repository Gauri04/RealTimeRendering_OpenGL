#include<windows.h>
#include<stdio.h>
#include<d3d11.h>
#include<math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global fuctions declaration
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

// global variables declaration
DWORD grdwStyle;
WINDOWPLACEMENT grgwpPrev = { sizeof(WINDOWPLACEMENT) };
bool grgbFullScreen = false;
HWND grghwnd = NULL;
bool grgbActiveWindow = false;
HDC grghdc = NULL;
HGLRC grghrc = NULL;
FILE *grgpFile = NULL;

IDXGISwapChain* gpIDXGISwapChain = NULL;
ID3D11Device* gpID3D11Device = NULL;
ID3D11DeviceContext* gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView* gpID3D11RenderTargetView = NULL;
float gClearColor[4];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Function declaration
	HRESULT Initialize(void);
	void Display(void);
	
	// variables declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("D3D11");
	int grDesktopWidth, grDesktopHeight;
	int grWndXPos, grWndYPos;
	bool grbDone = false;
	HRESULT hr;
	
	if(fopen_s(&grgpFile, "GRLog.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot open desired file"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf(grgpFile, "Log file created successfully. \n Program started successfully\n **** Logs ***** \n");
	}
	
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	RegisterClassEx(&wndclass);
	
	// Get width and height of desktop screen
	grDesktopWidth = GetSystemMetrics(SM_CXSCREEN);
	grDesktopHeight = GetSystemMetrics(SM_CYSCREEN);
	
	// Get center horizontal point
	grDesktopWidth = grDesktopWidth / 2;
	// Get center vertical point
	grDesktopHeight = grDesktopHeight / 2;
	
	// X position = center horizontal coordinate of screen - center horizontal coordinate of window
	grWndXPos = grDesktopWidth - 400;
	
	// X position = center horizontal coordinate of screen - center horizontal coordinate of window
	grWndYPos = grDesktopHeight - 300;
	
	
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
				szAppName,
				TEXT("D3D11"),
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
				grWndXPos,
				grWndYPos,
				WIN_WIDTH,
				WIN_HEIGHT,
				NULL,
				NULL,
				hInstance,
				NULL);
				
	grghwnd = hwnd;
	
	hr = Initialize();
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n error in initialize");
		fclose(grgpFile);
		DestroyWindow(grghwnd);
	}
	else
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n success in initialize");
		fclose(grgpFile);
	}
	
	ShowWindow(hwnd, iCmdShow);
	
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	
	while(grbDone == false)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			grbDone = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if(grgbActiveWindow == true)
			{
				//update function
				
				//display function
				Display();
			}
		}
		
	}
	
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declaration
	void ToggleFullScreen(void);
	HRESULT Resize(int, int);
	void Uninitialize(void);

	// variables
	HRESULT hr;
	
	switch(iMsg)
	{
		case WM_SETFOCUS : 
			grgbActiveWindow = true;
			break;
			
		case WM_KILLFOCUS :
			grgbActiveWindow = false;
			break;
			
		case WM_ERASEBKGND :
			return(0);
		
			
		case WM_SIZE :
			if (gpID3D11DeviceContext)
			{
				hr = Resize(LOWORD(lParam), HIWORD(lParam));
				if (FAILED(hr))
				{
					fopen_s(&grgpFile, "GRLog.txt", "a+");
					fprintf(grgpFile, "\n error in WM_SIZE");
					fclose(grgpFile);
					return(hr);
				}
				else
				{
					fopen_s(&grgpFile, "GRLog.txt", "a+");
					fprintf(grgpFile, "\n success in WM_SIZE");
					fclose(grgpFile);
				}
			}
			
			break;
		
		case WM_KEYDOWN : 
			switch(wParam)
			{
				case VK_ESCAPE :
					DestroyWindow(hwnd);
					break;
				
				case 0x46 : 
				case 0x66 :
					ToggleFullScreen();
					break;
				
				default : 
					break;
			}
			break;
			
		case WM_CLOSE :
			DestroyWindow(hwnd);
			break;
		
		case WM_DESTROY :
			Uninitialize();
			PostQuitMessage(0);
			break;
		
	}
	
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen()
{
	MONITORINFO mi = { sizeof(MONITORINFO) };
	
	if(grgbFullScreen == false)
	{
		grdwStyle = GetWindowLong(grghwnd, GWL_STYLE);
		if(grdwStyle & WS_OVERLAPPEDWINDOW)
		{
			if( GetWindowPlacement(grghwnd, &grgwpPrev) && GetMonitorInfo(MonitorFromWindow(grghwnd, MONITORINFOF_PRIMARY), &mi) )
			{
				SetWindowLong( grghwnd, GWL_STYLE, (grdwStyle & ~ WS_OVERLAPPEDWINDOW) );
				SetWindowPos( grghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
					(mi.rcMonitor.right - mi.rcMonitor.left), (mi.rcMonitor.bottom - mi.rcMonitor.top), SWP_NOZORDER | SWP_FRAMECHANGED );
			}
		}
		ShowCursor(false);
		grgbFullScreen = true;
	}
	else
	{
		SetWindowLong( grghwnd, GWL_STYLE, (grdwStyle | WS_OVERLAPPEDWINDOW) );
		SetWindowPlacement(grghwnd, &grgwpPrev);
		SetWindowPos(grghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(true);
		grgbFullScreen = false;
	}
}

HRESULT D3DInfo()
{
	IDXGIFactory* pIDXGIFactory = NULL;
	IDXGIAdapter* pIDXGIAdapter = NULL;
	DXGI_ADAPTER_DESC dxgiAdapterDesc;
	HRESULT hr;
	char str[255];

	// code
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pIDXGIFactory);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n CreateDXGIFactory() failed.. Exiting Now");
		fclose(grgpFile);
		return hr;
	}

	if (pIDXGIFactory->EnumAdapters(0, &pIDXGIAdapter) == DXGI_ERROR_NOT_FOUND)
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n DXGIAdapter cannot be found.. Exiting Now");
		fclose(grgpFile);
		return hr;
	}
	// if success
	ZeroMemory((void*)&dxgiAdapterDesc, sizeof(DXGI_ADAPTER_DESC));
	hr = pIDXGIAdapter->GetDesc(&dxgiAdapterDesc);
	// convert wchar string to char
	WideCharToMultiByte(CP_ACP, 0, dxgiAdapterDesc.Description, 255, str, 255, NULL, NULL);
	fopen_s(&grgpFile, "GRLog.txt", "a+");
	fprintf(grgpFile, "\n Graphic card name : %s", str);
		fprintf(grgpFile, "\n Graphic card VRAM : %I64d bytes", __int64(dxgiAdapterDesc.DedicatedVideoMemory));
	fprintf(grgpFile, "\n VRAM in GB : %d", (int)(ceil((dxgiAdapterDesc.DedicatedVideoMemory) / 1024 / 1024 / 1024)));
	fclose(grgpFile);

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
}

HRESULT Initialize()
{
	// fn
	HRESULT D3DInfo();
	HRESULT Resize(int, int);

	//vars
	HRESULT hr;
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	D3D_FEATURE_LEVEL d3dFeatureLevelRequired = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL d3dFeatureLevelAcquired = D3D_FEATURE_LEVEL_10_0;
	UINT createDeviceFlags = 0;
	UINT numDriverTypes = 0;
	UINT numFeatureLevels = 1;
	D3D_DRIVER_TYPE d3dDriverType;
	D3D_DRIVER_TYPE d3dDriverTypes[] =			// driver types in the sequence that we need
	{
		D3D_DRIVER_TYPE_HARDWARE,			// hardware driver - similar as from linux
		D3D_DRIVER_TYPE_WARP,				// windows advanced rasterization presenter
		D3D_DRIVER_TYPE_REFERENCE		// software driver, 4th array can be our own rasterizer (custom) made through code
	};

	hr = D3DInfo();
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n error in d311dinfo");
		fclose(grgpFile);
	}

	
	ZeroMemory((void*)&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	dxgiSwapChainDesc.BufferCount = 1;									// min 1 buffer, swapchain already gives 1 buffer by default + this one = 2
	dxgiSwapChainDesc.BufferDesc.Width = WIN_WIDTH;
	dxgiSwapChainDesc.BufferDesc.Height = WIN_HEIGHT;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;			// 60 frames
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;			// per 1 second
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = grghwnd;							
	// quality, MSAA - Multi Sampling and Anti Aliasing
	dxgiSwapChainDesc.SampleDesc.Count = 1;		// sample count, max we can use 8
	dxgiSwapChainDesc.SampleDesc.Quality = 0;	// default quality
	dxgiSwapChainDesc.Windowed = TRUE;			// show my window size, not fullscreen window

	numDriverTypes = sizeof(d3dDriverTypes) / sizeof(d3dDriverTypes[0]);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		d3dDriverType = d3dDriverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,										// hardware adapter
			d3dDriverType,								// driver type
			NULL,										// we dont have custom rasterizer so null
			createDeviceFlags,							// we dont need any device flags so os can take default flags
			&d3dFeatureLevelRequired,					// array of feature levels required
			numFeatureLevels,							// number of array elements of feature levels
			D3D11_SDK_VERSION,							// use directx 11 sdk
			&dxgiSwapChainDesc,							// get swap chain desc
			&gpIDXGISwapChain,							// get swap chain 
			&gpID3D11Device,							// get d3d11 device
			&d3dFeatureLevelAcquired,					// get feature level
			&gpID3D11DeviceContext						// get d3d11 context
		);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n error in D3D11CreateDeviceAndSwapChain");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n success in D3D11CreateDeviceAndSwapChain");
		fprintf(grgpFile, "\n Chosen driver type is : ");
		if (d3dDriverType == D3D_DRIVER_TYPE_HARDWARE)
		{
			fprintf(grgpFile, "\t D3D_DRIVER_TYPE_HARDWARE");
		}
		else if (d3dDriverType == D3D_DRIVER_TYPE_WARP)
		{
			fprintf(grgpFile, "\t D3D_DRIVER_TYPE_WARP");
		}
		else if (d3dDriverType == D3D_DRIVER_TYPE_REFERENCE)
		{
			fprintf(grgpFile, "\t D3D_DRIVER_TYPE_REFERENCE");
		}
		else
		{
			fprintf(grgpFile, "\t unknown driver type");
		}

		fprintf(grgpFile, "\n Suppoerted hoghest feature level is : ");
		if (d3dFeatureLevelAcquired == D3D_FEATURE_LEVEL_11_0)
		{
			fprintf(grgpFile, "\t D3D_FEATURE_LEVEL_11_0");
		}
		else if (d3dFeatureLevelAcquired == D3D_FEATURE_LEVEL_10_0)
		{
			fprintf(grgpFile, "\t D3D_FEATURE_LEVEL_10_0");
		}
		else if (d3dFeatureLevelAcquired == D3D_FEATURE_LEVEL_10_1)
		{
			fprintf(grgpFile, "\t D3D_FEATURE_LEVEL_10_1");
		}
		else
		{
			fprintf(grgpFile, "\t Unknown");
		}
		fclose(grgpFile);
	}
	gClearColor[0] = 0.0f;
	gClearColor[1] = 0.0f;
	gClearColor[2] = 1.0f;
	gClearColor[3] = 1.0f;

	hr = Resize(WIN_WIDTH, WIN_HEIGHT);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n error in calling Resize");
		fclose(grgpFile);
	}
	else
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n success in calling Resize");
		fclose(grgpFile);
	}

	return(hr);
}

HRESULT Resize(int width, int height)
{	
	HRESULT hr;

	if (gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}
	gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);		// buffer count, width, height, format, flags- we have given 0 for default

	ID3D11Texture2D* pID3D11Texture2D_BackBuffer = NULL;		// use texture buffer as it has color as well as depth
	gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pID3D11Texture2D_BackBuffer);		// get buffer filled from os
	hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D_BackBuffer, NULL, &gpID3D11RenderTargetView); // get render target view from above buffer
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n error in CreateRenderTargetView");
		fclose(grgpFile);
	}
	else
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n succcess in CreateRenderTargetView");
		fclose(grgpFile);
	}

	pID3D11Texture2D_BackBuffer->Release();
	pID3D11Texture2D_BackBuffer = NULL;

	// set my rendertarget view OM stage in pipeline
	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, NULL);

	// set viewport
	D3D11_VIEWPORT d3dViewport;
	ZeroMemory((void*)&d3dViewport, sizeof(d3dViewport));

	d3dViewport.TopLeftX = 0;
	d3dViewport.TopLeftY = 0;
	d3dViewport.Width = (float)width;
	d3dViewport.Height = (float)height;
	d3dViewport.MinDepth = 0.0f;
	d3dViewport.MaxDepth = 1.0f;
	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

	return(hr);
}

void Display(void)
{
	// code
	// clear color
	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, gClearColor);

	// render buffer
	gpIDXGISwapChain->Present(0, 0);			// 1st param - is it needed to synchronize monitors refresh rate, if yes then details, 2nd param - how many framebuffers
}

void Uninitialize(void)
{
	// code
	if (gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;

	}
	if (gpIDXGISwapChain)
	{
		gpIDXGISwapChain->Release();
		gpIDXGISwapChain = NULL;
	}
	if (gpID3D11DeviceContext)
	{
		gpID3D11DeviceContext->Release();
		gpID3D11DeviceContext = NULL;
	}
	if (gpID3D11Device)
	{
		gpID3D11Device->Release();
		gpID3D11Device = NULL;
	}

	if(grgpFile)
	{
		fopen_s(&grgpFile, "GRLog.txt", "a+");
		fprintf(grgpFile, "\n **** End ****\nLog File closed successfully. \n Program terminated successfully");
		fclose(grgpFile);
		grgpFile = NULL;
	}
}









