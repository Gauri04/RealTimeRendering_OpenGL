#include<windows.h>
#include<stdio.h>
#include<d3d11.h>
#include<d3dcompiler.h>
#include<math.h>
#include"GRWindow.h"
#include"XNAMath_204\xnamath.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma warning(disable : 4838)


#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// project specific
#define CHECK_IMAGE_WIDTH 64
#define CHECK_IMAGE_HEIGHT 64

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
FILE* grgpFile = NULL;
char logFileName[] = "GRLog.txt";

IDXGISwapChain* gpIDXGISwapChain = NULL;
ID3D11Device* gpID3D11Device = NULL;
ID3D11DeviceContext* gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView* gpID3D11RenderTargetView = NULL;
float gClearColor[4];

ID3D11VertexShader* gpID3D11VertexShader = NULL;
ID3D11PixelShader* gpID3D11PixelShader = NULL;
ID3D11Buffer* gpiD3D11Buffer_VertexBuffer_PositionRectangle = NULL;
ID3D11Buffer* gpiD3D11Buffer_VertexBuffer_TexCoordRectangle = NULL;
ID3D11InputLayout* gpID3D11InputLayout = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer = NULL;
ID3D11RasterizerState* gpiD3D11RasterizerState = NULL;
ID3D11DepthStencilView* gpiD3D11DepthStencilView;
ID3D11ShaderResourceView* gpID3D11ShaderResourceView_TextureRectangle = NULL;
ID3D11SamplerState* gpID3D11SamplerState_SamplerTextureRectangle = NULL;
D3D11_MAPPED_SUBRESOURCE gMappedSubResource_Texcoords;
int grgKeyPressed;
unsigned char checkImage[CHECK_IMAGE_HEIGHT][CHECK_IMAGE_WIDTH][4];
unsigned char* grgCheckImage = NULL;

struct CBUFFER
{
	XMMATRIX WorldViewProjectionMatrix;
};

XMMATRIX gPerspectiveProjectionMatrix;

float anglePyramid = 0.0f;

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

	if (fopen_s(&grgpFile, logFileName, "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot open desired file"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf(grgpFile, "Log file created successfully. \n Program started successfully\n **** Logs ***** \n");
		fclose(grgpFile);
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
		errno_t err = fopen_s(&grgpFile, logFileName, "a+");
		if (!err && grgpFile != NULL)
		{
			fprintf(grgpFile, "\n error in initialize");
			fclose(grgpFile);
		}
		DestroyWindow(grghwnd);
	}
	else
	{
		errno_t err = fopen_s(&grgpFile, logFileName, "a+");
		if (!err && grgpFile != NULL)
		{
			fprintf(grgpFile, "\n success in initialize");
			fclose(grgpFile);
		}
	}

	ShowWindow(hwnd, iCmdShow);

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	while (grbDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				grbDone = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (grgbActiveWindow == true)
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

	switch (iMsg)
	{
	case WM_SETFOCUS:
		grgbActiveWindow = true;
		break;

	case WM_KILLFOCUS:
		grgbActiveWindow = false;
		break;

	case WM_ERASEBKGND:
		return(0);


	case WM_SIZE:
		if (gpID3D11DeviceContext)
		{
			hr = Resize(LOWORD(lParam), HIWORD(lParam));
			fopen_s(&grgpFile, logFileName, "a+");
			if (FAILED(hr))
			{
				
				fprintf(grgpFile, "\n error in WM_SIZE");
				fclose(grgpFile);
				return(hr);
			}
			else
			{
				fprintf(grgpFile, "\n success in WM_SIZE");
				fclose(grgpFile);
			}
		}

		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		case 0x46:
		case 0x66:
			ToggleFullScreen();
			break;

		default:
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		Uninitialize();
		PostQuitMessage(0);
		break;

	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen()
{
	MONITORINFO mi = { sizeof(MONITORINFO) };

	if (grgbFullScreen == false)
	{
		grdwStyle = GetWindowLong(grghwnd, GWL_STYLE);
		if (grdwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(grghwnd, &grgwpPrev) && GetMonitorInfo(MonitorFromWindow(grghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(grghwnd, GWL_STYLE, (grdwStyle & ~WS_OVERLAPPEDWINDOW));
				SetWindowPos(grghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
					(mi.rcMonitor.right - mi.rcMonitor.left), (mi.rcMonitor.bottom - mi.rcMonitor.top), SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(false);
		grgbFullScreen = true;
	}
	else
	{
		SetWindowLong(grghwnd, GWL_STYLE, (grdwStyle | WS_OVERLAPPEDWINDOW));
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
		errno_t err = fopen_s(&grgpFile, logFileName, "a+");
		if (!err && grgpFile != NULL)
		{
			fprintf(grgpFile, "\n CreateDXGIFactory() failed.. Exiting Now");
			fclose(grgpFile);
		}

		return hr;
	}

	if (pIDXGIFactory->EnumAdapters(0, &pIDXGIAdapter) == DXGI_ERROR_NOT_FOUND)
	{
		errno_t err = fopen_s(&grgpFile, logFileName, "a+");
		if (!err && grgpFile != NULL)
		{
			fprintf(grgpFile, "\n DXGIAdapter cannot be found.. Exiting Now");
			fclose(grgpFile);
		}
		return hr;
	}
	// if success
	ZeroMemory((void*)&dxgiAdapterDesc, sizeof(DXGI_ADAPTER_DESC));
	hr = pIDXGIAdapter->GetDesc(&dxgiAdapterDesc);
	// convert wchar string to char
	WideCharToMultiByte(CP_ACP, 0, dxgiAdapterDesc.Description, 255, str, 255, NULL, NULL);
	
	errno_t err = fopen_s(&grgpFile, logFileName, "a+");
	if (!err && grgpFile != NULL)
	{
		fprintf(grgpFile, "\n Graphic card name : %s", str);
		fprintf(grgpFile, "\n Graphic card VRAM : %I64d bytes", __int64(dxgiAdapterDesc.DedicatedVideoMemory));
		fprintf(grgpFile, "\n VRAM in GB : %d", (int)(ceil((dxgiAdapterDesc.DedicatedVideoMemory) / 1024 / 1024 / 1024)));
		fclose(grgpFile);
	}
	return(hr);
	
}

HRESULT Initialize()
{
	// fn
	HRESULT D3DInfo();
	HRESULT Resize(int, int);
	void Uninitialize(void);
	HRESULT LoadD3DTexture(ID3D11ShaderResourceView**);

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
		fopen_s(&grgpFile, logFileName, "a+");
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
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n error in D3D11CreateDeviceAndSwapChain");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n Success in D3D11CreateDeviceAndSwapChain");
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

	// *********************************************** ********* Shader code ******************************************************************
	// Create vertex shader source code
	const char* vertexShadereSourceCode =
		"cbuffer ConstantBuffer" \
		"{" \
		"float4x4 worldViewProjectionMatrix;" \
		"}" \
		"struct vertex_output{" \
		"float4 position:SV_POSITION;" \
		"float2 texcoord:TEXCOORD;" \
		"};" \
		"vertex_output main(float4 pos : POSITION, float2 tex : TEXCOORD)" \
		"{" \
		"vertex_output output;" \
		"output.position = mul(worldViewProjectionMatrix, pos);" \
		"output.texcoord = tex;" \
		"return(output);" \
		"}";

	ID3DBlob* pID3DBlob_VertexShaderCode = NULL;
	ID3DBlob* pID3DBlob_Error = NULL;

	// Compile Shader source code
	hr = D3DCompile(vertexShadereSourceCode,
		lstrlenA(vertexShadereSourceCode) + 1,
		"VS",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		0,
		0,
		&pID3DBlob_VertexShaderCode,
		&pID3DBlob_Error);

	// Compilation error check
	if (FAILED(hr))
	{
		if (pID3DBlob_Error != NULL)
		{
			fopen_s(&grgpFile, logFileName, "a+");
			fprintf(grgpFile, "\n D3DCompile() failed for vertex shader : %s", (char*)pID3DBlob_Error->GetBufferPointer());
			fclose(grgpFile);
			pID3DBlob_Error->Release();
			pID3DBlob_Error = NULL;
			return(hr);
		}
		else
		{
			fopen_s(&grgpFile, logFileName, "a+");
			fprintf(grgpFile, "\n Error in COM while compiling vertex shader");
			fclose(grgpFile);
			return(hr);
		}
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n D3DCompile() succeeded for vertex shader");
		fclose(grgpFile);
	}

	// Create shader binary code
	hr = gpID3D11Device->CreateVertexShader(pID3DBlob_VertexShaderCode->GetBufferPointer(), pID3DBlob_VertexShaderCode->GetBufferSize(), NULL, &gpID3D11VertexShader); // get vs binary code in gpID3D11VertexShader
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateVertexShader() Failed");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateVertexShader() succeeded");
		fclose(grgpFile);
	}

	// set the shader for the pipeline
	gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, 0, 0);

	// write fragment shader (pixel shader) code
	const char* pixelShaderSourceCode =
		"struct vertex_output{" \
		"float4 position:SV_POSITION;" \
		"float2 texcoord:TEXCOORD;" \
		"};" \
		"Texture2D myTexture2D;" \
		"SamplerState mySamplerState;" \
		"float4 main(vertex_output input) : SV_TARGET" \
		"{" \
		"float4 color = myTexture2D.Sample(mySamplerState, input.texcoord);" \
		"return(color);" \
		"}";
	// Texture2D is similar to OGL's texture() function that we use in fragment shader
	// SamplerState is similar to OGL's sampler2D type that we use in gragment shader 

	ID3DBlob* pID3DBlob_PixelShaderCode = NULL;
	pID3DBlob_Error = NULL;
	hr = D3DCompile(pixelShaderSourceCode,
		lstrlenA(pixelShaderSourceCode) + 1,
		"PS",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		0,
		0,
		&pID3DBlob_PixelShaderCode,
		&pID3DBlob_Error);

	if (FAILED(hr))
	{
		if (pID3DBlob_Error != NULL)
		{
			fopen_s(&grgpFile, logFileName, "a+");
			fprintf(grgpFile, "\n D3DCompile() failed for pixel shader : %s", (char*)pID3DBlob_Error->GetBufferPointer());
			fclose(grgpFile);
			pID3DBlob_Error->Release();
			pID3DBlob_Error = NULL;
			return(hr);
		}
		else
		{
			fopen_s(&grgpFile, logFileName, "a+");
			fprintf(grgpFile, "\n COM error while compiling pixel shader");
			fclose(grgpFile);
		}
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n D3DCompile() succeeded for vertex shader");
		fclose(grgpFile);
	}

	// create binary code for pixel shader
	hr = gpID3D11Device->CreatePixelShader(pID3DBlob_PixelShaderCode->GetBufferPointer(), pID3DBlob_PixelShaderCode->GetBufferSize(), NULL, &gpID3D11PixelShader);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "ID3D11Device::CreatePixelShader() failed");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreatePixelShader() succeeded");
		fclose(grgpFile);
	}

	// set pixel shader for pipeline
	gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, 0, 0);
	pID3DBlob_PixelShaderCode->Release();
	pID3DBlob_PixelShaderCode = NULL;

	// create and set input layout
	D3D11_INPUT_ELEMENT_DESC d3d11InputElementDesc[2];

	d3d11InputElementDesc[0].SemanticName = "POSITION";					// this is the 'POSITION' which is there in vertex shader in main - main(float4 pos : POSITION)
	d3d11InputElementDesc[0].SemanticIndex = 0;							// if we are going to send multiple geometries in same semantics, then they are separted by indices
	d3d11InputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;			// format of data to be passed - 3 vertices (x,y,z is rgb) and type (float)
	d3d11InputElementDesc[0].InputSlot = 0;								// we have given slot 0 for position, can give slot 1 for color etc. line enums in OGL (GR_ATTRIB_POSITION)
	d3d11InputElementDesc[0].AlignedByteOffset = 0;						// how many gap is there in case of multiple geometries in senamtics
	d3d11InputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// we will draw vertex, not instance so PER_VERTEX_DATA 
	d3d11InputElementDesc[0].InstanceDataStepRate = 0;						// if data is per instance, then how many gap is there

	d3d11InputElementDesc[1].SemanticName = "TEXCOORD";					
	d3d11InputElementDesc[1].SemanticIndex = 0;							
	d3d11InputElementDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;			
	d3d11InputElementDesc[1].InputSlot = 1;								
	d3d11InputElementDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;		// similar to glPixelStorei(GL_UNPACK_ALIGNMENT) from OGL						
	d3d11InputElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	
	d3d11InputElementDesc[1].InstanceDataStepRate = 0;						

	hr = gpID3D11Device->CreateInputLayout(d3d11InputElementDesc, _ARRAYSIZE(d3d11InputElementDesc), pID3DBlob_VertexShaderCode->GetBufferPointer(), pID3DBlob_VertexShaderCode->GetBufferSize(), &gpID3D11InputLayout);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateInputLayout() failed");
		fclose(grgpFile);
		if (pID3DBlob_VertexShaderCode)
		{
			pID3DBlob_VertexShaderCode->Release();
			pID3DBlob_VertexShaderCode = NULL;
		}
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateInputLayout() succeeded");
		fclose(grgpFile);
		if (pID3DBlob_VertexShaderCode)
		{
			pID3DBlob_VertexShaderCode->Release();
			pID3DBlob_VertexShaderCode = NULL;
		}
	}
	// set input layout in pipeline
	gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

	// declare vertices, colors, normals, texcoords of geometry

	float SquareTexCoord[] =
	{
		+0.0f, +0.0f,
		+0.0f, +1.0f,
		+1.0f, +0.0f,

		+1.0f, +0.0f,
		+0.0f, +1.0f,
		+1.0f, +1.0f
	};
	float verticesSquare[] =
	{
		-1.0f, +1.0f, -1.0f,
		+1.0f, +1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		+1.0f, +1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f
	};

	// Initialize buffer description structure and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;


	// define and set constant buffer to hold uniform
	ZeroMemory((void*)&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.ByteWidth = sizeof(CBUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hr = gpID3D11Device->CreateBuffer(&bufferDesc, NULL, &gpID3D11Buffer_ConstantBuffer);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() failed for constant buffer");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() succeeded for constant buffer");
		fclose(grgpFile);
	}


	// ******************************************************** vertices for square *******************8
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;								// DirectX favors dynamic draw unlike static draw in OGL because DirectX favors multithreading
	bufferDesc.ByteWidth = sizeof(float) * _ARRAYSIZE(verticesSquare);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = gpID3D11Device->CreateBuffer(&bufferDesc, NULL, &gpiD3D11Buffer_VertexBuffer_PositionRectangle);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() failed for vertex buffer - position square");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() succeeded for vertex buffer - position square");
		fclose(grgpFile);
	}
	// Push the data in this buffer by  Map and Unmap
	// copy vertices into above vertex buffer
	ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	gpID3D11DeviceContext->Map(gpiD3D11Buffer_VertexBuffer_PositionRectangle, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, verticesSquare, sizeof(verticesSquare));
	gpID3D11DeviceContext->Unmap(gpiD3D11Buffer_VertexBuffer_PositionRectangle, 0);

	// ******************************************************** texture for square *******************8
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;								// DirectX favors dynamic draw unlike static draw in OGL because DirectX favors multithreading
	bufferDesc.ByteWidth = sizeof(float) * _ARRAYSIZE(SquareTexCoord);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = gpID3D11Device->CreateBuffer(&bufferDesc, NULL, &gpiD3D11Buffer_VertexBuffer_TexCoordRectangle);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() failed for vertex buffer - texcoord square");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() succeeded for vertex buffer - texcoord square");
		fclose(grgpFile);
	}
	// Push the data in this buffer by  Map and Unmap
	// copy vertices into above vertex buffer
	ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	gpID3D11DeviceContext->Map(gpiD3D11Buffer_VertexBuffer_TexCoordRectangle, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, SquareTexCoord, sizeof(SquareTexCoord));
	gpID3D11DeviceContext->Unmap(gpiD3D11Buffer_VertexBuffer_TexCoordRectangle, 0);



	// set this constant buffer (for uniform) in pipeline
	gpID3D11DeviceContext->VSSetConstantBuffers(0,				// position is set to 0th index hence we have passed the index here
		1,														// how many buffers
		&gpID3D11Buffer_ConstantBuffer);

	// create and set rasterizer state
	D3D11_RASTERIZER_DESC d3d11RasterizerDesc;
	ZeroMemory((void *)&d3d11RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3d11RasterizerDesc.AntialiasedLineEnable = FALSE;
	d3d11RasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3d11RasterizerDesc.DepthBias = 0;
	d3d11RasterizerDesc.DepthBiasClamp = 0.0f;
	d3d11RasterizerDesc.DepthClipEnable = TRUE;
	d3d11RasterizerDesc.FillMode = D3D11_FILL_SOLID;		// we can also write D3D11_FILL_WIRE for wireframe
	d3d11RasterizerDesc.FrontCounterClockwise = FALSE;
	d3d11RasterizerDesc.MultisampleEnable = FALSE;			// if we set it to true tthen it gives rich quality but slow peerformance
	d3d11RasterizerDesc.ScissorEnable = FALSE;
	d3d11RasterizerDesc.SlopeScaledDepthBias = 0.0f;
	// Set culling - OFF
	hr = gpID3D11Device->CreateRasterizerState(&d3d11RasterizerDesc, &gpiD3D11RasterizerState);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n error in CreateRasterizerState");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n success in CreateRasterizerState");
		fclose(grgpFile);
	}

	// texture
	hr = LoadD3DTexture(&gpID3D11ShaderResourceView_TextureRectangle);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n error in LoadD3DTexture");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n success in LoadD3DTexture");
		fclose(grgpFile);
	}
	
	D3D11_SAMPLER_DESC d3d11SamplerDesc;
	ZeroMemory((void*)&d3d11SamplerDesc, sizeof(d3d11SamplerDesc));
	d3d11SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;			// mipmap linear for minification, magnification
	d3d11SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;				// smililar to WRAP_T from OGL
	d3d11SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3d11SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = gpID3D11Device->CreateSamplerState(&d3d11SamplerDesc, &gpID3D11SamplerState_SamplerTextureRectangle);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n error in CreateSamplerState for texture");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n success in CreateSamplerState for texture");
		fclose(grgpFile);
	}

	// initialize global mappedSubResource
	ZeroMemory(&gMappedSubResource_Texcoords, sizeof(D3D11_MAPPED_SUBRESOURCE));

	gClearColor[0] = 0.0f;
	gClearColor[1] = 0.0f;
	gClearColor[2] = 0.0f;
	gClearColor[3] = 1.0f;

	gpID3D11DeviceContext->RSSetState(gpiD3D11RasterizerState);

	gPerspectiveProjectionMatrix = XMMatrixIdentity();

	hr = Resize(WIN_WIDTH, WIN_HEIGHT);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n error in calling Resize");
		fclose(grgpFile);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n success in calling Resize");
		fclose(grgpFile);
	}

	return(hr);
}

HRESULT LoadD3DTexture(ID3D11ShaderResourceView** ppID3D11ShaderResourceView)
{
	// function declaration
	void MakeCheckImage(void);

	// variables
	HRESULT hr;
	HBITMAP hBitmap;
	BITMAP bmp;
	ID3D11Resource *textureResource;
	BITMAPINFOHEADER bi;
	D3D11_TEXTURE2D_DESC d3d11Texture2DDesc;
	D3D11_SUBRESOURCE_DATA d3d11SubResourceData;
	MakeCheckImage();

	ZeroMemory((void*)&d3d11Texture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3d11Texture2DDesc.Width = (UINT)CHECK_IMAGE_HEIGHT;
	d3d11Texture2DDesc.Height = (UINT)CHECK_IMAGE_WIDTH;
	d3d11Texture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						// 32 bit depth
	d3d11Texture2DDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3d11Texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3d11Texture2DDesc.SampleDesc.Count = 1;								// count can be 1 to 4 as per needed quality
	d3d11Texture2DDesc.SampleDesc.Quality = 0;								// default quality
	d3d11Texture2DDesc.ArraySize = 1;
	d3d11Texture2DDesc.MipLevels = 1;
	d3d11Texture2DDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;									// default
	d3d11Texture2DDesc.MiscFlags = 0;

	ZeroMemory((void*)&d3d11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubResourceData.pSysMem = checkImage;
	d3d11SubResourceData.SysMemPitch = CHECK_IMAGE_WIDTH * 4;


	ID3D11Resource* pIShaderResource = NULL;
	ID3D11RenderTargetView* pID3D11RTV = NULL;
	ID3D11Texture2D* pID3D11Texture2D_Position = NULL;


	hr = gpID3D11Device->CreateTexture2D(&d3d11Texture2DDesc, &d3d11SubResourceData, &pID3D11Texture2D_Position);
	hr = gpID3D11Device->CreateShaderResourceView(pID3D11Texture2D_Position, NULL, ppID3D11ShaderResourceView);

	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n error in CreateWICTextureFromFile");
		fclose(grgpFile);
	}
	else 
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n success CreateWICTextureFromFile");
		fclose(grgpFile);
	}
	//}
	//DirectX::CreateWICTextureFromMemory is used for 
	return(hr);
}

void MakeCheckImage(void)
{
	int i, j, c;
	for (i = 0; i < CHECK_IMAGE_HEIGHT; i++)
	{
		for (j = 0; j < CHECK_IMAGE_WIDTH; j++)
		{
			c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
			
			checkImage[i][j][0] = (unsigned char)c;
			checkImage[i][j][1] = (unsigned char)c;
			checkImage[i][j][2] = (unsigned char)c;
			checkImage[i][j][3] = (unsigned char)255;

		}
	}
}

HRESULT Resize(int width, int height)
{
	HRESULT hr;

	if (height < 0)
		height = 1;
	// here there should be release of previous depthStencilView
	if (gpiD3D11DepthStencilView)
	{
		gpiD3D11DepthStencilView->Release();
		gpiD3D11DepthStencilView = NULL;
	}

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
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n error in CreateRenderTargetView");
		fclose(grgpFile);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n succcess in CreateRenderTargetView");
		fclose(grgpFile);
	}
	pID3D11Texture2D_BackBuffer->Release();
	pID3D11Texture2D_BackBuffer = NULL;


	// depth code, it is in resize because depth is size dependant
	// just like needed for RTV, DSV also needs a texture buffer
	D3D11_TEXTURE2D_DESC d3d11Texture2DDesc;
	ZeroMemory((void *)&d3d11Texture2DDesc, sizeof(d3d11Texture2DDesc));
	d3d11Texture2DDesc.Width = (UINT)width;
	d3d11Texture2DDesc.Height = (UINT)height;
	d3d11Texture2DDesc.Format = DXGI_FORMAT_D32_FLOAT;						// 32 bit depth
	d3d11Texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11Texture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3d11Texture2DDesc.SampleDesc.Count = 1;								// count can be 1 to 4 as per needed quality
	d3d11Texture2DDesc.SampleDesc.Quality = 0;								// default quality
	d3d11Texture2DDesc.ArraySize = 1;
	d3d11Texture2DDesc.MipLevels = 1;
	d3d11Texture2DDesc.CPUAccessFlags = 0;									// default
	d3d11Texture2DDesc.MiscFlags = 0;

	ID3D11Texture2D* pID3D11Texture2D_Depthbuffer = NULL;
	hr = gpID3D11Device->CreateTexture2D(&d3d11Texture2DDesc, NULL, &pID3D11Texture2D_Depthbuffer);
	//params - our buffer desc, subResource - this is used if we have some static data, empty texture buffer which we will get filled by function
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n error in CreateTexture2D for depth buffer");
		fclose(grgpFile);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n succcess in CreateTexture2D for depth buffer");
		fclose(grgpFile);
	}
	
	D3D11_DEPTH_STENCIL_VIEW_DESC d3d11DepthStencilViewDesc;
	ZeroMemory((void*)&d3d11DepthStencilViewDesc, sizeof(d3d11DepthStencilViewDesc));
	d3d11DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	d3d11DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;			// MS means multi sample

	hr = gpID3D11Device->CreateDepthStencilView(pID3D11Texture2D_Depthbuffer, &d3d11DepthStencilViewDesc, &gpiD3D11DepthStencilView);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n error in CreateDepthStencilView for depth stencil view buffer");
		fclose(grgpFile);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n succcess in CreateDepthStencilView for depth stencil view buffer");
		fclose(grgpFile);
	}

	
	// set my rendertargetview (RTV) and depthStencilView (DSV) OM stage in pipeline
	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, gpiD3D11DepthStencilView);

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

	gPerspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	return(hr);
}

void Display(void)
{
	// function declaration
	void Update();
	// code
	float grsquareOneVertices[18] = {0};
	float grsquareTwoVertices[18] = {0};

	// clear color
	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, gClearColor);
	gpID3D11DeviceContext->ClearDepthStencilView(gpiD3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//gpID3D11DeviceContext->ClearDepthStencilView(gpiD3D11DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 1); // for clear stencil buffer, not needed now as we dont have stencil

	// ************************* Square *************************/
	
	grsquareOneVertices[0] = -2.0;
	grsquareOneVertices[1] = 1.0;
	grsquareOneVertices[2] = 0.0;
		//2
	
	grsquareOneVertices[3] = 0.0;
	grsquareOneVertices[4] = 1.0;
	grsquareOneVertices[5] = 0.0;
		//3
	grsquareOneVertices[6] = -2.0;
	grsquareOneVertices[7] = -1.0;
	grsquareOneVertices[8] = 0.0;
		//4
	grsquareOneVertices[9] = -2.0;
	grsquareOneVertices[10] = -1.0;
	grsquareOneVertices[11] = 0.0;
		//5
	grsquareOneVertices[12] = 0.0;
	grsquareOneVertices[13] = 1.0;
	grsquareOneVertices[14] = 0.0;
		//6
	grsquareOneVertices[15] = 0.0;
	grsquareOneVertices[16] = -1.0;
	grsquareOneVertices[17] = 0.0;


	gpID3D11DeviceContext->Map(gpiD3D11Buffer_VertexBuffer_PositionRectangle, 0, D3D11_MAP_WRITE_DISCARD, 0, &gMappedSubResource_Texcoords);
	memcpy(gMappedSubResource_Texcoords.pData, grsquareOneVertices, sizeof(grsquareOneVertices));
	gpID3D11DeviceContext->Unmap(gpiD3D11Buffer_VertexBuffer_PositionRectangle, 0);
	

	UINT stride = sizeof(float) * 3; // jump
	UINT offset = 0;
	// set vertex buffer into pipeline-position
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpiD3D11Buffer_VertexBuffer_PositionRectangle, &stride, &offset);

	stride = sizeof(float) * 2; // jump
	offset = 0;
	// set vertex buffer into pipeline - texcoord
	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpiD3D11Buffer_VertexBuffer_TexCoordRectangle, &stride, &offset);
	//set primitive topology
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// above function is just like glDrawArrays in OGL

	// do transformations as needed
	XMMATRIX translationMatrix = XMMatrixIdentity();
	translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 6.0f);
	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMMATRIX viewMatrix = XMMatrixIdentity();

	worldMatrix = translationMatrix;
	XMMATRIX wvpMatrix = worldMatrix * viewMatrix * gPerspectiveProjectionMatrix;

	// push transformation matrices into shaders
	CBUFFER constantBuffer;
	ZeroMemory(&constantBuffer, sizeof(constantBuffer));
	constantBuffer.WorldViewProjectionMatrix = wvpMatrix;
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

	// texture
	gpID3D11DeviceContext->PSSetShaderResources(0, 1, &gpID3D11ShaderResourceView_TextureRectangle);
	gpID3D11DeviceContext->PSSetSamplers(0, 1, &gpID3D11SamplerState_SamplerTextureRectangle);

	// draw
	gpID3D11DeviceContext->Draw(6, 0);
	//gpID3D11DeviceContext->Draw(6, 6);
	//gpID3D11DeviceContext->Draw(6, 12);
	//gpID3D11DeviceContext->Draw(6, 18);
	//gpID3D11DeviceContext->Draw(6, 24);
	//gpID3D11DeviceContext->Draw(6, 30);




	// ***************** Second Checkerboard
	/*
	grsquareTwoVertices[0] = 2.41421;
	grsquareTwoVertices[1] = 1.0;
	grsquareTwoVertices[2] = -1.41421;
	//2

	grsquareTwoVertices[3] = 1.0;
	grsquareTwoVertices[4] = 1.0;
	grsquareTwoVertices[5] = 0.0;
	//3

	grsquareTwoVertices[6] = 1.0;
	grsquareTwoVertices[7] = -1.0;
	grsquareTwoVertices[8] = 0.0;
	//4
	
	grsquareTwoVertices[9] = 1.0;
	grsquareTwoVertices[10] = -1.0;
	grsquareTwoVertices[11] = 0.0;
	//5
	grsquareTwoVertices[12] = 2.41421;
	grsquareTwoVertices[13] = 1.0;
	grsquareTwoVertices[14] = -1.41421;
	//6
	grsquareTwoVertices[15] = 2.41421;
	grsquareTwoVertices[16] = -1.0;
	grsquareTwoVertices[17] = -1.41421;
	*/



	//-------------------------
	grsquareTwoVertices[0] = 1.0;
	grsquareTwoVertices[1] = 1.0;
	grsquareTwoVertices[2] = 0.0;
	//2

	grsquareTwoVertices[3] = 2.41421;
	grsquareTwoVertices[4] = 1.0;
	grsquareTwoVertices[5] = 1.41421;
	//3

	grsquareTwoVertices[6] = 1.0;
	grsquareTwoVertices[7] = -1.0;
	grsquareTwoVertices[8] = 0.0;
	//4

	grsquareTwoVertices[9] = 1.0;
	grsquareTwoVertices[10] = -1.0;
	grsquareTwoVertices[11] = 0.0;
	//5
	grsquareTwoVertices[12] = 2.41421;
	grsquareTwoVertices[13] = 1.0;
	grsquareTwoVertices[14] = 1.41421;
	//6
	grsquareTwoVertices[15] = 2.41421;
	grsquareTwoVertices[16] = -1.0;
	grsquareTwoVertices[17] = 1.41421;


	gpID3D11DeviceContext->Map(gpiD3D11Buffer_VertexBuffer_PositionRectangle, 0, D3D11_MAP_WRITE_DISCARD, 0, &gMappedSubResource_Texcoords);
	memcpy(gMappedSubResource_Texcoords.pData, grsquareTwoVertices, sizeof(grsquareTwoVertices));
	gpID3D11DeviceContext->Unmap(gpiD3D11Buffer_VertexBuffer_PositionRectangle, 0);


	stride = sizeof(float) * 3; // jump
	offset = 0;
	// set vertex buffer into pipeline-position
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpiD3D11Buffer_VertexBuffer_PositionRectangle, &stride, &offset);

	stride = sizeof(float) * 2; // jump
	offset = 0;
	// set vertex buffer into pipeline - texcoord
	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpiD3D11Buffer_VertexBuffer_TexCoordRectangle, &stride, &offset);
	//set primitive topology
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// above function is just like glDrawArrays in OGL

	// do transformations as needed
	 translationMatrix = XMMatrixIdentity();
	translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 6.0f);
	worldMatrix = XMMatrixIdentity();
	viewMatrix = XMMatrixIdentity();

	worldMatrix = translationMatrix;
	wvpMatrix = worldMatrix * viewMatrix * gPerspectiveProjectionMatrix;

	// push transformation matrices into shaders
	constantBuffer;
	ZeroMemory(&constantBuffer, sizeof(constantBuffer));
	constantBuffer.WorldViewProjectionMatrix = wvpMatrix;
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

	// texture
	gpID3D11DeviceContext->PSSetShaderResources(0, 1, &gpID3D11ShaderResourceView_TextureRectangle);
	gpID3D11DeviceContext->PSSetSamplers(0, 1, &gpID3D11SamplerState_SamplerTextureRectangle);

	// draw
	gpID3D11DeviceContext->Draw(6, 0);

	Update();
	// render buffer - switch front and back
	gpIDXGISwapChain->Present(0, 0);			// 1st param - is it needed to synchronize monitors refresh rate, if yes then details, 2nd param - how many framebuffers
}

void Update()
{
	anglePyramid = anglePyramid + 0.001f;
	if (anglePyramid >= 360.0f)
	{
		anglePyramid = 0.0f;
	}
}

void Uninitialize(void)
{
	// code
	free(grgCheckImage);
	if (gpID3D11SamplerState_SamplerTextureRectangle)
	{
		gpID3D11SamplerState_SamplerTextureRectangle->Release();
		gpID3D11SamplerState_SamplerTextureRectangle = NULL;
	}
	if (gpID3D11ShaderResourceView_TextureRectangle)
	{
		gpID3D11ShaderResourceView_TextureRectangle->Release();
		gpID3D11ShaderResourceView_TextureRectangle = NULL;
	}
	if (gpiD3D11RasterizerState)
	{
		gpiD3D11RasterizerState->Release();
		gpiD3D11RasterizerState = NULL;
	}
	if (gpID3D11Buffer_ConstantBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();
		gpID3D11Buffer_ConstantBuffer = NULL;
	}
	if (gpID3D11InputLayout)
	{
		gpID3D11InputLayout->Release();
		gpID3D11InputLayout = NULL;
	}
	if (gpiD3D11Buffer_VertexBuffer_TexCoordRectangle)
	{
		gpiD3D11Buffer_VertexBuffer_TexCoordRectangle->Release();
		gpiD3D11Buffer_VertexBuffer_TexCoordRectangle = NULL;
	}
	if (gpiD3D11Buffer_VertexBuffer_PositionRectangle)
	{
		gpiD3D11Buffer_VertexBuffer_PositionRectangle->Release();
		gpiD3D11Buffer_VertexBuffer_PositionRectangle = NULL;
	}
	if (gpID3D11PixelShader)
	{
		gpID3D11PixelShader->Release();
		gpID3D11PixelShader = NULL;
	}
	if (gpID3D11VertexShader)
	{
		gpID3D11VertexShader->Release();
		gpID3D11VertexShader = NULL;
	}

	if (gpiD3D11DepthStencilView)
	{
		gpiD3D11DepthStencilView->Release();
		gpiD3D11DepthStencilView = NULL;

	}
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

	if (grgpFile)
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n **** End ****\nLog File closed successfully. \n Program terminated successfully");
		fclose(grgpFile);
		grgpFile = NULL;
	}
}









