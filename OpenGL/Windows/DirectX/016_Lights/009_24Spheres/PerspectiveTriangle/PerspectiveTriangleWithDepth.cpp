#include<windows.h>
#include<stdio.h>
#include<d3d11.h>
#include<d3dcompiler.h>
#include<math.h>
#include"XNAMath_204\xnamath.h"
#include"Sphere.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "Sphere.lib")
#pragma warning(disable : 4838)


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
FILE* grgpFile = NULL;
char logFileName[] = "GRLog.txt";

IDXGISwapChain* gpIDXGISwapChain = NULL;
ID3D11Device* gpID3D11Device = NULL;
ID3D11DeviceContext* gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView* gpID3D11RenderTargetView = NULL;
float gClearColor[4];

ID3D11VertexShader* gpID3D11VertexShaderPV = NULL;
ID3D11PixelShader* gpID3D11PixelShaderPV = NULL;
ID3D11VertexShader* gpID3D11VertexShaderPP = NULL;
ID3D11PixelShader* gpID3D11PixelShaderPP = NULL;
ID3D11Buffer* gpID3D11Buffer_VertexBuffer_Position = NULL;
ID3D11Buffer* gpID3D11Buffer_VertexBuffer_Color = NULL;
ID3D11Buffer* gpiD3D11Buffer_VertexBuffer_PositionRectangle = NULL;
ID3D11Buffer* gpiD3D11Buffer_VertexBuffer_NormalRectangle = NULL;
ID3D11Buffer* gpiD3D11Buffer_VertexBuffer_Indexbuffer = NULL;
ID3D11InputLayout* gpID3D11InputLayoutPV = NULL;
ID3D11InputLayout* gpID3D11InputLayoutPP = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBufferPV = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBufferPP = NULL;
ID3D11RasterizerState* gpiD3D11RasterizerState = NULL;
ID3D11DepthStencilView* gpiD3D11DepthStencilView;
bool isVertexLighting = true;

struct CBUFFER
{
	XMMATRIX WorldMatrix;
	XMMATRIX ViewMatrix;
	XMMATRIX ProjectionMatrix;

	XMVECTOR La;
	XMVECTOR Ld;
	XMVECTOR Ls;
	XMVECTOR LightPosition;

	XMVECTOR Ka;
	XMVECTOR Kd;
	XMVECTOR Ks;
	float MaterialShinyness;

	unsigned int KeyPressed;
};

bool grgbXKeyPressed;
bool grgbYKeyPressed;
bool grgbZKeyPressed;
float grflightAngleX = 0.0f;
float grflightAngleY = 0.0f;
float grflightAngleZ = 0.0f;
int i;

float grsphereVertices[1146];
float grsphereNormals[1146];
float grsphereTextures[764];
unsigned short grsphereElements[2280];
unsigned int grgNumVertices;
unsigned int grgNumElements;

XMMATRIX gPerspectiveProjectionMatrix;
bool gbLight = false;

float grglightAmbientColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float grglightDiffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float grglightSpecularColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float grglightPosition[] = { 1.0f, 1.0f, 1.0f, 1.0f };

float materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float materialShinyness = 128.0f;
int grgKeyPress;
float left, right, bottom, top;
bool grgbPerFragmentLighting = false;
D3D11_VIEWPORT d3dViewportDisplay;

float anglePyramid = 0.0f;

#pragma region materialProperties vars
float grgKa1[4];
float grgKd1[4];
float grgKs1[4];
float grgMaterialShinyness1;

float grgKa2[4];
float grgKd2[4];
float grgKs2[4];
float grgMaterialShinyness2;

float grgKa3[4];
float grgKd3[4];
float grgKs3[4];
float grgMaterialShinyness3;

float grgKa4[4];
float grgKd4[4];
float grgKs4[4];
float grgMaterialShinyness4;

float grgKa5[4];
float grgKd5[4];
float grgKs5[4];
float grgMaterialShinyness5;

float grgKa6[4];
float grgKd6[4];
float grgKs6[4];
float grgMaterialShinyness6;

float grgKa7[4];
float grgKd7[4];
float grgKs7[4];
float grgMaterialShinyness7;

float grgKa8[4];
float grgKd8[4];
float grgKs8[4];
float grgMaterialShinyness8;

float grgKa9[4];
float grgKd9[4];
float grgKs9[4];
float grgMaterialShinyness9;

float grgKa10[4];
float grgKd10[4];
float grgKs10[4];
float grgMaterialShinyness10;

float grgKa11[4];
float grgKd11[4];
float grgKs11[4];
float grgMaterialShinyness11;

float grgKa12[4];
float grgKd12[4];
float grgKs12[4];
float grgMaterialShinyness12;

float grgKa13[4];
float grgKd13[4];
float grgKs13[4];
float grgMaterialShinyness13;

float grgKa14[4];
float grgKd14[4];
float grgKs14[4];
float grgMaterialShinyness14;

float grgKa15[4];
float grgKd15[4];
float grgKs15[4];
float grgMaterialShinyness15;

float grgKa16[4];
float grgKd16[4];
float grgKs16[4];
float grgMaterialShinyness16;

float grgKa17[4];
float grgKd17[4];
float grgKs17[4];
float grgMaterialShinyness17;

float grgKa18[4];
float grgKd18[4];
float grgKs18[4];
float grgMaterialShinyness18;

float grgKa19[4];
float grgKd19[4];
float grgKs19[4];
float grgMaterialShinyness19;

float grgKa20[4];
float grgKd20[4];
float grgKs20[4];
float grgMaterialShinyness20;

float grgKa21[4];
float grgKd21[4];
float grgKs21[4];
float grgMaterialShinyness21;

float grgKa22[4];
float grgKd22[4];
float grgKs22[4];
float grgMaterialShinyness22;

float grgKa23[4];
float grgKd23[4];
float grgKs23[4];
float grgMaterialShinyness23;

float grgKa24[4];
float grgKd24[4];
float grgKs24[4];
float grgMaterialShinyness24;
#pragma endregion materialProperties vars

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
			if (FAILED(hr))
			{
				errno_t err = fopen_s(&grgpFile, logFileName, "a+");
				if (!err && grgpFile != NULL)
				{
					fprintf(grgpFile, "\n error in WM_SIZE");
					fclose(grgpFile);
					return(hr);
				}

			}
			else
			{
				fopen_s(&grgpFile, logFileName, "a+");
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

	case WM_CHAR:
		switch (wParam)
		{

		case 'l':
		case 'L':
			gbLight = !gbLight;
			break;

		case 'v':
		case 'V':
			isVertexLighting = !isVertexLighting;
			break;

		case 'x':
		case 'X':
			grgKeyPress = 1;
			break;

		case 'y':
		case 'Y':
			grgKeyPress = 2;
			break;

		case 'z':
		case 'Z':
			grgKeyPress = 3;
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
	void Initialize24Spheres(void);

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
		"float4x4 worldMatrix;" \
		"float4x4 viewMatrix;" \
		"float4x4 projectionMatrix;" \
		"float4 la;" \
		"float4 ld;" \
		"float4 ls;" \
		"float4 lightPosition;" \
		"float4 ka;" \
		"float4 kd;" \
		"float4 ks;" \
		"float materialShinyness;" \
		"uint keyPressed;" \
		"}" \
		"struct vertex_output" \
		"{" \
		"float4 position:SV_POSITION;" \
		"float3 phong_ads_light:COLOR;" \
		"};" \
		"vertex_output main(float4 pos : POSITION, float4 normals : NORMAL)" \
		"{" \
		"vertex_output output;" \
		"if(keyPressed == 1)" \
		"{" \
		"float4 eye_position = mul(worldMatrix, pos);" \
		"eye_position = mul(viewMatrix, eye_position);" \
		"float3 transformed_normals = (float3)normalize(mul((float3x3)worldMatrix, (float3)normals));" \
		"float3 light_direction = (float3)normalize(lightPosition - eye_position);" \
		"float3 ambient = la * ka;" \
		"float3 diffuse = ld * kd * max(dot(light_direction, transformed_normals), 0.0);" \
		"float3 reflection_vector = reflect(-light_direction, transformed_normals);" \
		"float3 viewer_vector = normalize(-eye_position.xyz);" \
		"float3 specular = ls * ks * pow(max(dot(reflection_vector, viewer_vector), 0.0), materialShinyness);" \
		"output.phong_ads_light = ambient + diffuse + specular;" \
		"}" \
		"else" \
		"{" \
		"output.phong_ads_light = float3(1.0, 1.0, 1.0);" \
		"}" \
		"float4 position = mul(worldMatrix, pos);" \
		"position = mul(viewMatrix, position);" \
		"position = mul(projectionMatrix, position);"
		"output.position = position;" \
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
	hr = gpID3D11Device->CreateVertexShader(pID3DBlob_VertexShaderCode->GetBufferPointer(), pID3DBlob_VertexShaderCode->GetBufferSize(), NULL, &gpID3D11VertexShaderPV); // get vs binary code in gpID3D11VertexShader
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



	// write fragment shader (pixel shader) code
	const char* pixelShaderSourceCode =
		"struct vertex_output" \
		"{" \
		"float4 position:SV_POSITION;" \
		"float3 phong_ads_light:COLOR;" \
		"};" \
		"float4 main(struct vertex_output input) : SV_TARGET" \
		"{" \
		"float4 color = float4(input.phong_ads_light, 1.0);" \
		"return(color);"
		"}";

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
	hr = gpID3D11Device->CreatePixelShader(pID3DBlob_PixelShaderCode->GetBufferPointer(), pID3DBlob_PixelShaderCode->GetBufferSize(), NULL, &gpID3D11PixelShaderPV);
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

	pID3DBlob_PixelShaderCode->Release();
	pID3DBlob_PixelShaderCode = NULL;

	// ********************************************** Per Pixel Shader code ******************************************************************
	// Create vertex shader source code
	const char* vertexShadereSourceCodePP =
		"cbuffer ConstantBuffer" \
		"{" \
		"float4x4 worldMatrix;" \
		"float4x4 viewMatrix;" \
		"float4x4 projectionMatrix;" \
		"float4 la;" \
		"float4 ld;" \
		"float4 ls;" \
		"float4 lightPosition;" \
		"float4 ka;" \
		"float4 kd;" \
		"float4 ks;" \
		"float materialShinyness;" \
		"uint keyPressed;" \
		"}" \
		"struct vertex_output" \
		"{" \
		"float4 position:SV_POSITION;" \
		"float3 transformed_normals:NORMAL0;" \
		"float3 light_direction:NORMAL1;" \
		"float3 viewer_vector:NORMAL2;" \
		"};" \
		"vertex_output main(float4 pos : POSITION, float4 normals : NORMAL)" \
		"{" \
		"vertex_output output;" \
		"if(keyPressed == 1)" \
		"{" \
		"float4 eye_position = mul(worldMatrix, pos);" \
		"eye_position = mul(viewMatrix, eye_position);" \
		"output.transformed_normals = (float3)mul((float3x3)worldMatrix, (float3)normals);" \
		"output.light_direction = ((float3)lightPosition - eye_position.xyz);" \
		"output.viewer_vector = -eye_position.xyz;" \
		"}" \
		"float4 position = mul(worldMatrix, pos);" \
		"position = mul(viewMatrix, position);" \
		"position = mul(projectionMatrix, position);"
		"output.position = position;" \
		"return(output);" \
		"}";

	ID3DBlob* pID3DBlob_VertexShaderCodePP = NULL;
	pID3DBlob_Error = NULL;

	// Compile Shader source code
	hr = D3DCompile(vertexShadereSourceCodePP,
		lstrlenA(vertexShadereSourceCodePP) + 1,
		"VS",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		0,
		0,
		&pID3DBlob_VertexShaderCodePP,
		&pID3DBlob_Error);

	// Compilation error check
	if (FAILED(hr))
	{
		if (pID3DBlob_Error != NULL)
		{
			fopen_s(&grgpFile, logFileName, "a+");
			fprintf(grgpFile, "\n\n D3DCompile() failed for PP vertex shader s : %s", (char*)pID3DBlob_Error->GetBufferPointer());
			fclose(grgpFile);
			pID3DBlob_Error->Release();
			pID3DBlob_Error = NULL;
			return(hr);
		}
		else
		{
			fopen_s(&grgpFile, logFileName, "a+");
			fprintf(grgpFile, "\n\n Error in COM while compiling PP vertex shader");
			fclose(grgpFile);
			return(hr);
		}
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n\n D3DCompile() succeeded for PP vertex shader");
		fclose(grgpFile);
	}

	// Create shader binary code
	hr = gpID3D11Device->CreateVertexShader(pID3DBlob_VertexShaderCodePP->GetBufferPointer(), pID3DBlob_VertexShaderCodePP->GetBufferSize(), NULL, &gpID3D11VertexShaderPP); // get vs binary code in gpID3D11VertexShader
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n\n ID3D11Device::CreateVertexShader() for PP Failed");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n\n ID3D11Device::CreateVertexShader() for PP succeeded");
		fclose(grgpFile);
	}


	// write fragment shader (pixel shader) code
	const char* pixelShaderSourceCodePP =
		"cbuffer ConstantBuffer" \
		"{" \
		"float4x4 worldMatrix;" \
		"float4x4 viewMatrix;" \
		"float4x4 projectionMatrix;" \
		"float4 la;" \
		"float4 ld;" \
		"float4 ls;" \
		"float4 lightPosition;" \
		"float4 ka;" \
		"float4 kd;" \
		"float4 ks;" \
		"float materialShinyness;" \
		"uint keyPressed;" \
		"}" \
		"struct vertex_output" \
		"{" \
		"float4 position:SV_POSITION;" \
		"float3 transformed_normals:NORMAL0;" \
		"float3 light_direction:NORMAL1;" \
		"float3 viewer_vector:NORMAL2;" \
		"};" \
		"float4 main(struct vertex_output input) : SV_TARGET" \
		"{" \
		"float3 phong_ads_light;" \
		"if(keyPressed == 1)" \
		"{" \
		"float3 normalized_transformed_normals = normalize(input.transformed_normals);" \
		"float3 normalized_light_direction = normalize(input.light_direction);" \
		"float3 normalized_viewer_vector = normalize(input.viewer_vector);" \
		"float3 ambient = (la * ka);" \
		"float3 diffuse = (ld * kd) * max(dot(normalized_light_direction, normalized_transformed_normals), 0.0);" \
		"float3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
		"float3 specular = (float3)(ls * ks) * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), materialShinyness);" \
		"phong_ads_light = ambient + diffuse + specular;" \
		"}" \
		"else" \
		"{" \
		"phong_ads_light = float3(1.0, 1.0, 1.0);" \
		"}" \
		"float4 color = float4(phong_ads_light, 1.0);" \
		"return(color);"
		"}";

	ID3DBlob* pID3DBlob_PixelShaderCodePP = NULL;
	pID3DBlob_Error = NULL;
	hr = D3DCompile(pixelShaderSourceCodePP,
		lstrlenA(pixelShaderSourceCodePP) + 1,
		"PS",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		0,
		0,
		&pID3DBlob_PixelShaderCodePP,
		&pID3DBlob_Error);

	if (FAILED(hr))
	{
		if (pID3DBlob_Error != NULL)
		{
			fopen_s(&grgpFile, logFileName, "a+");
			fprintf(grgpFile, "\n D3DCompile() failed for PP pixel shader : %s", (char*)pID3DBlob_Error->GetBufferPointer());
			fclose(grgpFile);
			pID3DBlob_Error->Release();
			pID3DBlob_Error = NULL;
			return(hr);
		}
		else
		{
			fopen_s(&grgpFile, logFileName, "a+");
			fprintf(grgpFile, "\n COM error while compiling PP pixel shader");
			fclose(grgpFile);
		}
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n D3DCompile() succeeded for PP pixel shader");
		fclose(grgpFile);
	}

	// create binary code for pixel shader
	hr = gpID3D11Device->CreatePixelShader(pID3DBlob_PixelShaderCodePP->GetBufferPointer(), pID3DBlob_PixelShaderCodePP->GetBufferSize(), NULL, &gpID3D11PixelShaderPP);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "ID3D11Device::CreatePixelShader() for PP failed");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreatePixelShader() for PP succeeded");
		fclose(grgpFile);
	}


	pID3DBlob_PixelShaderCodePP->Release();
	pID3DBlob_PixelShaderCodePP = NULL;

	// ----------------------------------------------------------------------------------------------------

	// create and set input layout
	D3D11_INPUT_ELEMENT_DESC d3d11InputElementDesc[2];

	d3d11InputElementDesc[0].SemanticName = "POSITION";					// this is the 'POSITION' which is there in vertex shader in main - main(float4 pos : POSITION)
	d3d11InputElementDesc[0].SemanticIndex = 0;							// if we are going to send multiple geometries in same semantics, then they are separted by indices
	d3d11InputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;			// format of data to be passed - 3 vertices (x,y,z is rgb) and type (float)
	d3d11InputElementDesc[0].InputSlot = 0;								// we have given slot 0 for position, can give slot 1 for color etc. line enums in OGL (GR_ATTRIB_POSITION)
	d3d11InputElementDesc[0].AlignedByteOffset = 0;						// how many gap is there in case of multiple geometries in senamtics
	d3d11InputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// we will draw vertex, not instance so PER_VERTEX_DATA 
	d3d11InputElementDesc[0].InstanceDataStepRate = 0;						// if data is per instance, then how many gap is there

	d3d11InputElementDesc[1].SemanticName = "NORMAL";
	d3d11InputElementDesc[1].SemanticIndex = 0;
	d3d11InputElementDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3d11InputElementDesc[1].InputSlot = 1;
	d3d11InputElementDesc[1].AlignedByteOffset = 0;
	d3d11InputElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3d11InputElementDesc[1].InstanceDataStepRate = 0;

	hr = gpID3D11Device->CreateInputLayout(d3d11InputElementDesc, _ARRAYSIZE(d3d11InputElementDesc), pID3DBlob_VertexShaderCode->GetBufferPointer(), pID3DBlob_VertexShaderCode->GetBufferSize(), &gpID3D11InputLayoutPV);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateInputLayout() failed for PV");
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
		fprintf(grgpFile, "\n ID3D11Device::CreateInputLayout() succeeded for PV");
		fclose(grgpFile);
		if (pID3DBlob_VertexShaderCode)
		{
			pID3DBlob_VertexShaderCode->Release();
			pID3DBlob_VertexShaderCode = NULL;
		}
	}

	hr = gpID3D11Device->CreateInputLayout(d3d11InputElementDesc, _ARRAYSIZE(d3d11InputElementDesc), pID3DBlob_VertexShaderCodePP->GetBufferPointer(), pID3DBlob_VertexShaderCodePP->GetBufferSize(), &gpID3D11InputLayoutPP);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateInputLayout() failed for PP");
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
		fprintf(grgpFile, "\n ID3D11Device::CreateInputLayout() succeeded for PP");
		fclose(grgpFile);
		if (pID3DBlob_VertexShaderCodePP)
		{
			pID3DBlob_VertexShaderCodePP->Release();
			pID3DBlob_VertexShaderCodePP = NULL;
		}
	}

	// declare vertices, colors, normals, texcoords of geometry
	getSphereVertexData(grsphereVertices, grsphereNormals, grsphereTextures, grsphereElements);
	grgNumVertices = getNumberOfSphereVertices();
	grgNumElements = getNumberOfSphereElements();



	// Initialize buffer description structure and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	// Push the data in this buffer by  Map and Unmap
	// copy vertices into above vertex buffer
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// define and set constant buffer to hold uniform
	ZeroMemory((void*)&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.ByteWidth = sizeof(CBUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hr = gpID3D11Device->CreateBuffer(&bufferDesc, NULL, &gpID3D11Buffer_ConstantBufferPV);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() failed for constant buffer PV");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() succeeded for constant buffer PV");
		fclose(grgpFile);
	}

	hr = gpID3D11Device->CreateBuffer(&bufferDesc, NULL, &gpID3D11Buffer_ConstantBufferPP);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() failed for constant buffer PP");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() succeeded for constant buffer PP");
		fclose(grgpFile);
	}

	// ******************************************************** vertices for square *******************
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;								// DirectX favors dynamic draw unlike static draw in OGL because DirectX favors multithreading
	bufferDesc.ByteWidth = grgNumVertices * 3 * sizeof(float);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = gpID3D11Device->CreateBuffer(&bufferDesc, NULL, &gpiD3D11Buffer_VertexBuffer_PositionRectangle);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() failed for vertex buffer - position sphere");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() succeeded for vertex buffer - position sphere");
		fclose(grgpFile);
	}
	// Push the data in this buffer by  Map and Unmap
	// copy vertices into above vertex buffer
	ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	gpID3D11DeviceContext->Map(gpiD3D11Buffer_VertexBuffer_PositionRectangle, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, grsphereVertices, grgNumVertices * 3 * sizeof(float));
	gpID3D11DeviceContext->Unmap(gpiD3D11Buffer_VertexBuffer_PositionRectangle, 0);

	// ******************************************************** colors for square *******************

	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;								// DirectX favors dynamic draw unlike static draw in OGL because DirectX favors multithreading
	bufferDesc.ByteWidth = grgNumVertices * 3 * sizeof(float);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = gpID3D11Device->CreateBuffer(&bufferDesc, NULL, &gpiD3D11Buffer_VertexBuffer_NormalRectangle);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() failed for vertex buffer - normal sphere");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() succeeded for vertex buffer - normal sphere");
		fclose(grgpFile);
	}
	// Push the data in this buffer by  Map and Unmap
	// copy vertices into above vertex buffer
	ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	gpID3D11DeviceContext->Map(gpiD3D11Buffer_VertexBuffer_NormalRectangle, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, grsphereNormals, grgNumVertices * 3 * sizeof(float));
	gpID3D11DeviceContext->Unmap(gpiD3D11Buffer_VertexBuffer_NormalRectangle, 0);

	// ******************************************************** sphere - index buffer  ***********************
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;								// DirectX favors dynamic draw unlike static draw in OGL because DirectX favors multithreading
	bufferDesc.ByteWidth = grgNumElements * sizeof(short);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = gpID3D11Device->CreateBuffer(&bufferDesc, NULL, &gpiD3D11Buffer_VertexBuffer_Indexbuffer);
	if (FAILED(hr))
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() failed for index buffer sphere");
		fclose(grgpFile);
		return(hr);
	}
	else
	{
		fopen_s(&grgpFile, logFileName, "a+");
		fprintf(grgpFile, "\n ID3D11Device::CreateBuffer() succeeded for index buffer sphere");
		fclose(grgpFile);
	}
	// Push the data in this buffer by Map and Unmap
	// copy vertices into above vertex buffer
	ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	gpID3D11DeviceContext->Map(gpiD3D11Buffer_VertexBuffer_Indexbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, grsphereElements, grgNumElements * sizeof(short));
	gpID3D11DeviceContext->Unmap(gpiD3D11Buffer_VertexBuffer_Indexbuffer, 0);


	// create and set rasterizer state
	D3D11_RASTERIZER_DESC d3d11RasterizerDesc;
	ZeroMemory((void*)&d3d11RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
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

	// initialize material colors
	Initialize24Spheres();
	ZeroMemory((void*)&d3dViewportDisplay, sizeof(d3dViewportDisplay));

	gClearColor[0] = 0.25f;
	gClearColor[1] = 0.25f;
	gClearColor[2] = 0.25f;
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
	ZeroMemory((void*)&d3d11Texture2DDesc, sizeof(d3d11Texture2DDesc));
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

	left = 0.0;
	top = 0.0;
	right = (float)width / 6.0;
	bottom = (float)height / 4.0;
	return(hr);
}

void Display(void)
{
	// function declaration
	void Update();
	// code

	// clear color
	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, gClearColor);
	gpID3D11DeviceContext->ClearDepthStencilView(gpiD3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//gpID3D11DeviceContext->ClearDepthStencilView(gpiD3D11DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 1); // for clear stencil buffer, not needed now as we dont have stencil

	if (isVertexLighting)
	{
		// set the shader for the pipeline
		gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShaderPV, 0, 0);

		// set pixel shader for pipeline
		gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShaderPV, 0, 0);

		// set input layout in pipeline
		gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayoutPV);

		// set this constant buffer (for uniform) in pipeline
		gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBufferPV);
	}
	else
	{
		gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShaderPP, 0, 0);
		gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShaderPP, 0, 0);
		gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayoutPP);
		gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBufferPP);
		gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBufferPP);
	}
	// ************************* Square *************************/
	UINT stride = sizeof(float) * 3; // jump
	UINT offset = 0;
	// set vertex buffer into pipeline-position
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpiD3D11Buffer_VertexBuffer_PositionRectangle, &stride, &offset);

	stride = sizeof(float) * 3; // jump
	offset = 0;
	// set vertex buffer into pipeline - color
	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpiD3D11Buffer_VertexBuffer_NormalRectangle, &stride, &offset);
	gpID3D11DeviceContext->IASetIndexBuffer(gpiD3D11Buffer_VertexBuffer_Indexbuffer, DXGI_FORMAT_R16_UINT, 0);
	//set primitive topology
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// above function is just like glDrawArrays in OGL
	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMMATRIX translateMatrix = XMMatrixIdentity();
	XMMATRIX viewMatrix = XMMatrixIdentity();
	XMMATRIX wvMatrix = XMMatrixIdentity();

	if (isVertexLighting)
	{
		translateMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);
		worldMatrix = translateMatrix;
		wvMatrix = worldMatrix * viewMatrix;
		CBUFFER constantBuffer;
		ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = gPerspectiveProjectionMatrix;

		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa1[0], grgKa1[1], grgKa1[2], grgKa1[3]);
			constantBuffer.Kd = XMVectorSet(grgKd1[0], grgKd1[1], grgKd1[2], grgKd1[3]);
			constantBuffer.Ks = XMVectorSet(grgKs1[0], grgKs1[1], grgKs1[2], grgKs1[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness1;
			constantBuffer.KeyPressed = 1;
			if (grgKeyPress == 1)
			{
				grglightPosition[0] = 0.0;
				grglightPosition[1] = (float)21 * sin(grflightAngleX);
				grglightPosition[2] = (float)21 * cos(grflightAngleX);
				grglightPosition[3] = 0.0;							// w for directional light
			}
			else if (grgKeyPress == 2)
			{
				grglightPosition[0] = (float)20 * sin(grflightAngleY);
				grglightPosition[1] = 0.0;
				grglightPosition[2] = (float)20 * cos(grflightAngleY);
				grglightPosition[3] = 0.0;							// w for directional light
			}

			else if (grgKeyPress == 3)
			{
				grglightPosition[0] = (float)20 * sin(grflightAngleZ);;
				grglightPosition[1] = (float)20 * cos(grflightAngleZ);
				grglightPosition[2] = 0.0f;
				grglightPosition[3] = 0.0;							// w for directional light
			}
			constantBuffer.LightPosition = XMVectorSet(grglightPosition[0], grglightPosition[1], grglightPosition[2], grglightPosition[3]);
			constantBuffer.La = XMVectorSet(grglightAmbientColor[0], grglightAmbientColor[1], grglightAmbientColor[2], grglightAmbientColor[3]);
			constantBuffer.Ld = XMVectorSet(grglightDiffuseColor[0], grglightDiffuseColor[1], grglightDiffuseColor[2], grglightDiffuseColor[3]);
			constantBuffer.Ls = XMVectorSet(grglightSpecularColor[0], grglightSpecularColor[1], grglightSpecularColor[2], grglightSpecularColor[3]);
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = left;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 2
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa2[0], grgKa2[1], grgKa2[2], grgKa2[3]);
			constantBuffer.Kd = XMVectorSet(grgKd2[0], grgKd2[1], grgKd2[2], grgKd2[3]);
			constantBuffer.Ks = XMVectorSet(grgKs2[0], grgKs2[1], grgKs2[2], grgKs2[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness2;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 3
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa3[0], grgKa3[1], grgKa3[2], grgKa3[3]);
			constantBuffer.Kd = XMVectorSet(grgKd3[0], grgKd3[1], grgKd3[2], grgKd3[3]);
			constantBuffer.Ks = XMVectorSet(grgKs3[0], grgKs3[1], grgKs3[2], grgKs3[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness3;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 2;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 4
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa4[0], grgKa4[1], grgKa4[2], grgKa4[3]);
			constantBuffer.Kd = XMVectorSet(grgKd4[0], grgKd4[1], grgKd4[2], grgKd4[3]);
			constantBuffer.Ks = XMVectorSet(grgKs4[0], grgKs4[1], grgKs4[2], grgKs4[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness4;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 3;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 5
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa5[0], grgKa5[1], grgKa5[2], grgKa5[3]);
			constantBuffer.Kd = XMVectorSet(grgKd5[0], grgKd5[1], grgKd5[2], grgKd5[3]);
			constantBuffer.Ks = XMVectorSet(grgKs5[0], grgKs5[1], grgKs5[2], grgKs5[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness5;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 4;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 6
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa6[0], grgKa6[1], grgKa6[2], grgKa6[3]);
			constantBuffer.Kd = XMVectorSet(grgKd6[0], grgKd6[1], grgKd6[2], grgKd6[3]);
			constantBuffer.Ks = XMVectorSet(grgKs6[0], grgKs6[1], grgKs6[2], grgKs6[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness6;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 5;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 7
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa7[0], grgKa7[1], grgKa7[2], grgKa7[3]);
			constantBuffer.Kd = XMVectorSet(grgKd7[0], grgKd7[1], grgKd7[2], grgKd7[3]);
			constantBuffer.Ks = XMVectorSet(grgKs7[0], grgKs7[1], grgKs7[2], grgKs7[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness7;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = left;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 8
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa8[0], grgKa8[1], grgKa8[2], grgKa8[3]);
			constantBuffer.Kd = XMVectorSet(grgKd8[0], grgKd8[1], grgKd8[2], grgKd8[3]);
			constantBuffer.Ks = XMVectorSet(grgKs8[0], grgKs8[1], grgKs8[2], grgKs8[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness8;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 9
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa9[0], grgKa9[1], grgKa9[2], grgKa9[3]);
			constantBuffer.Kd = XMVectorSet(grgKd9[0], grgKd9[1], grgKd9[2], grgKd9[3]);
			constantBuffer.Ks = XMVectorSet(grgKs9[0], grgKs9[1], grgKs9[2], grgKs9[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness9;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 2;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 10
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa10[0], grgKa10[1], grgKa10[2], grgKa10[3]);
			constantBuffer.Kd = XMVectorSet(grgKd10[0], grgKd10[1], grgKd10[2], grgKd10[3]);
			constantBuffer.Ks = XMVectorSet(grgKs10[0], grgKs10[1], grgKs10[2], grgKs10[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness10;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 3;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 11
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa11[0], grgKa11[1], grgKa11[2], grgKa11[3]);
			constantBuffer.Kd = XMVectorSet(grgKd11[0], grgKd11[1], grgKd11[2], grgKd11[3]);
			constantBuffer.Ks = XMVectorSet(grgKs11[0], grgKs11[1], grgKs11[2], grgKs11[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness11;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 4;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 12
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa12[0], grgKa12[1], grgKa12[2], grgKa12[3]);
			constantBuffer.Kd = XMVectorSet(grgKd12[0], grgKd12[1], grgKd12[2], grgKd12[3]);
			constantBuffer.Ks = XMVectorSet(grgKs12[0], grgKs12[1], grgKs12[2], grgKs12[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness12;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 5;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 13
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa13[0], grgKa13[1], grgKa13[2], grgKa13[3]);
			constantBuffer.Kd = XMVectorSet(grgKd13[0], grgKd13[1], grgKd13[2], grgKd13[3]);
			constantBuffer.Ks = XMVectorSet(grgKs13[0], grgKs13[1], grgKs13[2], grgKs13[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness13;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = left;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 14
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa14[0], grgKa14[1], grgKa14[2], grgKa14[3]);
			constantBuffer.Kd = XMVectorSet(grgKd14[0], grgKd14[1], grgKd14[2], grgKd14[3]);
			constantBuffer.Ks = XMVectorSet(grgKs14[0], grgKs14[1], grgKs14[2], grgKs14[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness14;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 15
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa15[0], grgKa15[1], grgKa15[2], grgKa15[3]);
			constantBuffer.Kd = XMVectorSet(grgKd15[0], grgKd15[1], grgKd15[2], grgKd15[3]);
			constantBuffer.Ks = XMVectorSet(grgKs15[0], grgKs15[1], grgKs15[2], grgKs15[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness15;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 2;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 16
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa16[0], grgKa16[1], grgKa16[2], grgKa16[3]);
			constantBuffer.Kd = XMVectorSet(grgKd16[0], grgKd16[1], grgKd16[2], grgKd16[3]);
			constantBuffer.Ks = XMVectorSet(grgKs16[0], grgKs16[1], grgKs16[2], grgKs16[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness16;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 3;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 17
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa17[0], grgKa17[1], grgKa17[2], grgKa17[3]);
			constantBuffer.Kd = XMVectorSet(grgKd17[0], grgKd17[1], grgKd17[2], grgKd17[3]);
			constantBuffer.Ks = XMVectorSet(grgKs17[0], grgKs17[1], grgKs17[2], grgKs17[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness17;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 4;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 18
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa18[0], grgKa18[1], grgKa18[2], grgKa18[3]);
			constantBuffer.Kd = XMVectorSet(grgKd18[0], grgKd18[1], grgKd18[2], grgKd18[3]);
			constantBuffer.Ks = XMVectorSet(grgKs18[0], grgKs18[1], grgKs18[2], grgKs18[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness18;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 5;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 19
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa19[0], grgKa19[1], grgKa19[2], grgKa19[3]);
			constantBuffer.Kd = XMVectorSet(grgKd19[0], grgKd19[1], grgKd19[2], grgKd19[3]);
			constantBuffer.Ks = XMVectorSet(grgKs19[0], grgKs19[1], grgKs19[2], grgKs19[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness19;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = left;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 20
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa20[0], grgKa20[1], grgKa20[2], grgKa20[3]);
			constantBuffer.Kd = XMVectorSet(grgKd20[0], grgKd20[1], grgKd20[2], grgKd20[3]);
			constantBuffer.Ks = XMVectorSet(grgKs20[0], grgKs20[1], grgKs20[2], grgKs20[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness20;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 21
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa21[0], grgKa21[1], grgKa21[2], grgKa21[3]);
			constantBuffer.Kd = XMVectorSet(grgKd21[0], grgKd21[1], grgKd21[2], grgKd21[3]);
			constantBuffer.Ks = XMVectorSet(grgKs21[0], grgKs21[1], grgKs21[2], grgKs21[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness21;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 2;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 22
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa22[0], grgKa22[1], grgKa22[2], grgKa22[3]);
			constantBuffer.Kd = XMVectorSet(grgKd22[0], grgKd22[1], grgKd22[2], grgKd22[3]);
			constantBuffer.Ks = XMVectorSet(grgKs22[0], grgKs22[1], grgKs22[2], grgKs22[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness22;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 3;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 23
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa23[0], grgKa23[1], grgKa23[2], grgKa23[3]);
			constantBuffer.Kd = XMVectorSet(grgKd23[0], grgKd23[1], grgKd23[2], grgKd23[3]);
			constantBuffer.Ks = XMVectorSet(grgKs23[0], grgKs23[1], grgKs23[2], grgKs23[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness23;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 4;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 24
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa24[0], grgKa24[1], grgKa24[2], grgKa24[3]);
			constantBuffer.Kd = XMVectorSet(grgKd24[0], grgKd24[1], grgKd24[2], grgKd24[3]);
			constantBuffer.Ks = XMVectorSet(grgKs24[0], grgKs24[1], grgKs24[2], grgKs24[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness24;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 5;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);


	}
	else
	{
		translateMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);
		worldMatrix = translateMatrix;
		wvMatrix = worldMatrix * viewMatrix;
		CBUFFER constantBuffer;
		ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = gPerspectiveProjectionMatrix;

		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa1[0], grgKa1[1], grgKa1[2], grgKa1[3]);
			constantBuffer.Kd = XMVectorSet(grgKd1[0], grgKd1[1], grgKd1[2], grgKd1[3]);
			constantBuffer.Ks = XMVectorSet(grgKs1[0], grgKs1[1], grgKs1[2], grgKs1[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness1;
			constantBuffer.KeyPressed = 1;
			if (grgKeyPress == 1)
			{
				grglightPosition[0] = 0.0;
				grglightPosition[1] = (float)21 * sin(grflightAngleX);
				grglightPosition[2] = (float)21 * cos(grflightAngleX);
				grglightPosition[3] = 0.0;							// w for directional light
			}
			else if (grgKeyPress == 2)
			{
				grglightPosition[0] = (float)20 * sin(grflightAngleY);
				grglightPosition[1] = 0.0;
				grglightPosition[2] = (float)20 * cos(grflightAngleY);
				grglightPosition[3] = 0.0;							// w for directional light
			}

			else if (grgKeyPress == 3)
			{
				grglightPosition[0] = (float)20 * sin(grflightAngleZ);;
				grglightPosition[1] = (float)20 * cos(grflightAngleZ);
				grglightPosition[2] = 0.0f;
				grglightPosition[3] = 0.0;							// w for directional light
			}
			constantBuffer.LightPosition = XMVectorSet(grglightPosition[0], grglightPosition[1], grglightPosition[2], grglightPosition[3]);
			constantBuffer.La = XMVectorSet(grglightAmbientColor[0], grglightAmbientColor[1], grglightAmbientColor[2], grglightAmbientColor[3]);
			constantBuffer.Ld = XMVectorSet(grglightDiffuseColor[0], grglightDiffuseColor[1], grglightDiffuseColor[2], grglightDiffuseColor[3]);
			constantBuffer.Ls = XMVectorSet(grglightSpecularColor[0], grglightSpecularColor[1], grglightSpecularColor[2], grglightSpecularColor[3]);
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = left;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 2
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa2[0], grgKa2[1], grgKa2[2], grgKa2[3]);
			constantBuffer.Kd = XMVectorSet(grgKd2[0], grgKd2[1], grgKd2[2], grgKd2[3]);
			constantBuffer.Ks = XMVectorSet(grgKs2[0], grgKs2[1], grgKs2[2], grgKs2[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness2;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 3
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa3[0], grgKa3[1], grgKa3[2], grgKa3[3]);
			constantBuffer.Kd = XMVectorSet(grgKd3[0], grgKd3[1], grgKd3[2], grgKd3[3]);
			constantBuffer.Ks = XMVectorSet(grgKs3[0], grgKs3[1], grgKs3[2], grgKs3[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness3;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 2;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 4
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa4[0], grgKa4[1], grgKa4[2], grgKa4[3]);
			constantBuffer.Kd = XMVectorSet(grgKd4[0], grgKd4[1], grgKd4[2], grgKd4[3]);
			constantBuffer.Ks = XMVectorSet(grgKs4[0], grgKs4[1], grgKs4[2], grgKs4[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness4;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 3;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 5
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa5[0], grgKa5[1], grgKa5[2], grgKa5[3]);
			constantBuffer.Kd = XMVectorSet(grgKd5[0], grgKd5[1], grgKd5[2], grgKd5[3]);
			constantBuffer.Ks = XMVectorSet(grgKs5[0], grgKs5[1], grgKs5[2], grgKs5[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness5;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 4;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 6
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa6[0], grgKa6[1], grgKa6[2], grgKa6[3]);
			constantBuffer.Kd = XMVectorSet(grgKd6[0], grgKd6[1], grgKd6[2], grgKd6[3]);
			constantBuffer.Ks = XMVectorSet(grgKs6[0], grgKs6[1], grgKs6[2], grgKs6[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness6;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 5;
		d3dViewportDisplay.TopLeftY = bottom * 3;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 7
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa7[0], grgKa7[1], grgKa7[2], grgKa7[3]);
			constantBuffer.Kd = XMVectorSet(grgKd7[0], grgKd7[1], grgKd7[2], grgKd7[3]);
			constantBuffer.Ks = XMVectorSet(grgKs7[0], grgKs7[1], grgKs7[2], grgKs7[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness7;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = left;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 8
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa8[0], grgKa8[1], grgKa8[2], grgKa8[3]);
			constantBuffer.Kd = XMVectorSet(grgKd8[0], grgKd8[1], grgKd8[2], grgKd8[3]);
			constantBuffer.Ks = XMVectorSet(grgKs8[0], grgKs8[1], grgKs8[2], grgKs8[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness8;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 9
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa9[0], grgKa9[1], grgKa9[2], grgKa9[3]);
			constantBuffer.Kd = XMVectorSet(grgKd9[0], grgKd9[1], grgKd9[2], grgKd9[3]);
			constantBuffer.Ks = XMVectorSet(grgKs9[0], grgKs9[1], grgKs9[2], grgKs9[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness9;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 2;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 10
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa10[0], grgKa10[1], grgKa10[2], grgKa10[3]);
			constantBuffer.Kd = XMVectorSet(grgKd10[0], grgKd10[1], grgKd10[2], grgKd10[3]);
			constantBuffer.Ks = XMVectorSet(grgKs10[0], grgKs10[1], grgKs10[2], grgKs10[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness10;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 3;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 11
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa11[0], grgKa11[1], grgKa11[2], grgKa11[3]);
			constantBuffer.Kd = XMVectorSet(grgKd11[0], grgKd11[1], grgKd11[2], grgKd11[3]);
			constantBuffer.Ks = XMVectorSet(grgKs11[0], grgKs11[1], grgKs11[2], grgKs11[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness11;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 4;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 12
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa12[0], grgKa12[1], grgKa12[2], grgKa12[3]);
			constantBuffer.Kd = XMVectorSet(grgKd12[0], grgKd12[1], grgKd12[2], grgKd12[3]);
			constantBuffer.Ks = XMVectorSet(grgKs12[0], grgKs12[1], grgKs12[2], grgKs12[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness12;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 5;
		d3dViewportDisplay.TopLeftY = bottom * 2;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 13
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa13[0], grgKa13[1], grgKa13[2], grgKa13[3]);
			constantBuffer.Kd = XMVectorSet(grgKd13[0], grgKd13[1], grgKd13[2], grgKd13[3]);
			constantBuffer.Ks = XMVectorSet(grgKs13[0], grgKs13[1], grgKs13[2], grgKs13[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness13;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = left;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 14
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa14[0], grgKa14[1], grgKa14[2], grgKa14[3]);
			constantBuffer.Kd = XMVectorSet(grgKd14[0], grgKd14[1], grgKd14[2], grgKd14[3]);
			constantBuffer.Ks = XMVectorSet(grgKs14[0], grgKs14[1], grgKs14[2], grgKs14[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness14;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 15
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa15[0], grgKa15[1], grgKa15[2], grgKa15[3]);
			constantBuffer.Kd = XMVectorSet(grgKd15[0], grgKd15[1], grgKd15[2], grgKd15[3]);
			constantBuffer.Ks = XMVectorSet(grgKs15[0], grgKs15[1], grgKs15[2], grgKs15[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness15;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 2;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 16
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa16[0], grgKa16[1], grgKa16[2], grgKa16[3]);
			constantBuffer.Kd = XMVectorSet(grgKd16[0], grgKd16[1], grgKd16[2], grgKd16[3]);
			constantBuffer.Ks = XMVectorSet(grgKs16[0], grgKs16[1], grgKs16[2], grgKs16[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness16;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 3;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 17
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa17[0], grgKa17[1], grgKa17[2], grgKa17[3]);
			constantBuffer.Kd = XMVectorSet(grgKd17[0], grgKd17[1], grgKd17[2], grgKd17[3]);
			constantBuffer.Ks = XMVectorSet(grgKs17[0], grgKs17[1], grgKs17[2], grgKs17[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness17;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 4;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 18
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa18[0], grgKa18[1], grgKa18[2], grgKa18[3]);
			constantBuffer.Kd = XMVectorSet(grgKd18[0], grgKd18[1], grgKd18[2], grgKd18[3]);
			constantBuffer.Ks = XMVectorSet(grgKs18[0], grgKs18[1], grgKs18[2], grgKs18[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness18;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 5;
		d3dViewportDisplay.TopLeftY = bottom;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 19
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa19[0], grgKa19[1], grgKa19[2], grgKa19[3]);
			constantBuffer.Kd = XMVectorSet(grgKd19[0], grgKd19[1], grgKd19[2], grgKd19[3]);
			constantBuffer.Ks = XMVectorSet(grgKs19[0], grgKs19[1], grgKs19[2], grgKs19[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness19;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = left;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 20
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa20[0], grgKa20[1], grgKa20[2], grgKa20[3]);
			constantBuffer.Kd = XMVectorSet(grgKd20[0], grgKd20[1], grgKd20[2], grgKd20[3]);
			constantBuffer.Ks = XMVectorSet(grgKs20[0], grgKs20[1], grgKs20[2], grgKs20[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness20;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 21
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa21[0], grgKa21[1], grgKa21[2], grgKa21[3]);
			constantBuffer.Kd = XMVectorSet(grgKd21[0], grgKd21[1], grgKd21[2], grgKd21[3]);
			constantBuffer.Ks = XMVectorSet(grgKs21[0], grgKs21[1], grgKs21[2], grgKs21[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness21;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 2;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 22
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa22[0], grgKa22[1], grgKa22[2], grgKa22[3]);
			constantBuffer.Kd = XMVectorSet(grgKd22[0], grgKd22[1], grgKd22[2], grgKd22[3]);
			constantBuffer.Ks = XMVectorSet(grgKs22[0], grgKs22[1], grgKs22[2], grgKs22[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness22;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 3;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 23
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa23[0], grgKa23[1], grgKa23[2], grgKa23[3]);
			constantBuffer.Kd = XMVectorSet(grgKd23[0], grgKd23[1], grgKd23[2], grgKd23[3]);
			constantBuffer.Ks = XMVectorSet(grgKs23[0], grgKs23[1], grgKs23[2], grgKs23[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness23;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 4;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);

		// ********************************* 24
		if (gbLight == true)
		{
			constantBuffer.Ka = XMVectorSet(grgKa24[0], grgKa24[1], grgKa24[2], grgKa24[3]);
			constantBuffer.Kd = XMVectorSet(grgKd24[0], grgKd24[1], grgKd24[2], grgKd24[3]);
			constantBuffer.Ks = XMVectorSet(grgKs24[0], grgKs24[1], grgKs24[2], grgKs24[3]);
			constantBuffer.MaterialShinyness = grgMaterialShinyness24;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}
		d3dViewportDisplay.TopLeftX = right * 5;
		d3dViewportDisplay.TopLeftY = top;
		d3dViewportDisplay.Width = right;
		d3dViewportDisplay.Height = bottom;
		d3dViewportDisplay.MinDepth = 0.0f;
		d3dViewportDisplay.MaxDepth = 1.0f;
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewportDisplay);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
		gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);
	}






	// push transformation matrices into shaders
	/*
	if (gbLight == true)
	{
		constantBuffer.KeyPressed = 1;

		if (grgbXKeyPressed == true)
		{
			grLight[0].lightPosition[0] = 0.0;
			grLight[0].lightPosition[1] = (float)5.5 * sin(grflightAngleX);
			grLight[0].lightPosition[2] = (float)5.5 * cos(grflightAngleX);
			grLight[0].lightPosition[3] = 1.0;							// w for positional light
		}
		if (grgbYKeyPressed == true)
		{
			grLight[1].lightPosition[0] = (float)4.5 * sin(grflightAngleX);;
			grLight[1].lightPosition[1] = 0.0;
			grLight[1].lightPosition[2] = (float)4.5 * cos(grflightAngleX);
			grLight[1].lightPosition[3] = 1.0;
		}

		if (grgbZKeyPressed == true)
		{
			grLight[2].lightPosition[0] = (float)6.5 * sin(grflightAngleX);;
			grLight[2].lightPosition[1] = (float)6.5 * cos(grflightAngleX);
			grLight[2].lightPosition[2] = 0.0f;
			grLight[2].lightPosition[3] = 1.0;
		}

		for (i = 0; i < 3; i++)
		{
			constantBuffer.La[i] = XMVectorSet(grLight[i].lightAmbientColor[0], grLight[i].lightAmbientColor[1], grLight[i].lightAmbientColor[2], grLight[i].lightAmbientColor[3]);
			constantBuffer.Ld[i] = XMVectorSet(grLight[i].lightDiffuseColor[0], grLight[i].lightDiffuseColor[1], grLight[i].lightDiffuseColor[2], grLight[i].lightDiffuseColor[3]);
			constantBuffer.Ls[i] = XMVectorSet(grLight[i].lightSpecularColor[0], grLight[i].lightSpecularColor[1], grLight[i].lightSpecularColor[2], grLight[i].lightSpecularColor[3]);
			constantBuffer.LightPosition[i] = XMVectorSet(grLight[i].lightPosition[0], grLight[i].lightPosition[1], grLight[i].lightPosition[2], grLight[i].lightPosition[3]);
		}

		constantBuffer.Ka = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], materialAmbient[3]);
		constantBuffer.Kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
		constantBuffer.Ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
		constantBuffer.MaterialShinyness = materialShinyness;
	}
	else
	{
		constantBuffer.KeyPressed = 0;
	}

	if (isVertexLighting)
	{
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPV, 0, NULL, &constantBuffer, 0, 0);
	}
	else
	{
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBufferPP, 0, NULL, &constantBuffer, 0, 0);
	}


	// draw
	gpID3D11DeviceContext->DrawIndexed(grgNumElements, 0, 0);
	*/
	Update();
	// render buffer - switch front and back
	gpIDXGISwapChain->Present(0, 0);			// 1st param - is it needed to synchronize monitors refresh rate, if yes then details, 2nd param - how many framebuffers
}

void Update()
{
	if (grgKeyPress == 1)
	{
		grflightAngleX = grflightAngleX + 0.003f;
		if (grflightAngleX >= 360.0f)
			grflightAngleX = 0.0f;

	}
	else if (grgKeyPress == 2)
	{
		grflightAngleY = grflightAngleY + 0.0035f;
		if (grflightAngleY >= 360.0f)
			grflightAngleY = 0.0f;
	}
	else if (grgKeyPress == 3)
	{
		grflightAngleZ = grflightAngleZ + 0.0028f;
		if (grflightAngleZ >= 360.0f)
			grflightAngleZ = 0.0f;
	}

}


void Initialize24Spheres()
{
	grgKa1[0] = 0.0215;
	grgKa1[1] = 0.1745;
	grgKa1[2] = 0.0215;
	grgKa1[3] = 1.0f;

	grgKd1[0] = 0.07568;
	grgKd1[1] = 0.61424;
	grgKd1[2] = 0.07568;
	grgKd1[3] = 1.0f;

	grgKs1[0] = 0.633;
	grgKs1[1] = 0.727811;
	grgKs1[2] = 0.633;
	grgKs1[3] = 1.0f;
	grgMaterialShinyness1 = 0.6 * 128;

	grgKa2[0] = 0.135;
	grgKa2[1] = 0.2225;
	grgKa2[2] = 0.1575;
	grgKa2[3] = 1.0f;

	grgKd2[0] = 0.54; // r
	grgKd2[1] = 0.89; // g
	grgKd2[2] = 0.63; // b
	grgKd2[3] = 1.0f;

	grgKs2[0] = 0.316228; // r
	grgKs2[1] = 0.316228; // g
	grgKs2[2] = 0.316228; // b
	grgKs2[3] = 1.0f;
	grgMaterialShinyness2 = 0.1 * 128;

	grgKa3[0] = 0.05375; // r
	grgKa3[1] = 0.05;    // g
	grgKa3[2] = 0.06625; // b
	grgKa3[3] = 1.0f;	//a

	grgKd3[0] = 0.18275; // r
	grgKd3[1] = 0.17;    // g
	grgKd3[2] = 0.22525; // b
	grgKd3[3] = 1.0f;    // a

	grgKs3[0] = 0.332741; // r
	grgKs3[1] = 0.328634; // g
	grgKs3[2] = 0.346435; // b
	grgKs3[3] = 1.0f;     // a	
	grgMaterialShinyness3 = 0.3 * 128;

	grgKa4[0] = 0.25;    // r
	grgKa4[1] = 0.20725; // g
	grgKa4[2] = 0.20725; // b
	grgKa4[3] = 1.0f;    // a

	grgKd4[0] = 1.0;   // r
	grgKd4[1] = 0.829; // g
	grgKd4[2] = 0.829; // b
	grgKd4[3] = 1.0f;  // a

	grgKs4[0] = 0.296648; // r
	grgKs4[1] = 0.296648; // g
	grgKs4[2] = 0.296648; // b
	grgKs4[3] = 1.0f;     // a
	grgMaterialShinyness4 = 0.088 * 128;

	grgKa5[0] = 0.1745;  // r
	grgKa5[1] = 0.01175; // g
	grgKa5[2] = 0.01175; // b
	grgKa5[3] = 1.0f;    // a

	grgKd5[0] = 0.61424; // r
	grgKd5[1] = 0.04136; // g
	grgKd5[2] = 0.04136; // b
	grgKd5[3] = 1.0f;    // a

	grgKs5[0] = 0.727811; // r
	grgKs5[1] = 0.626959; // g
	grgKs5[2] = 0.626959; // b
	grgKs5[3] = 1.0f;     // a	
	grgMaterialShinyness5 = 0.6 * 128;

	grgKa6[0] = 0.1;     // r
	grgKa6[1] = 0.18725; // g
	grgKa6[2] = 0.1745;  // b
	grgKa6[3] = 1.0f;    // a

	grgKd6[0] = 0.396;   // r
	grgKd6[1] = 0.74151; // g
	grgKd6[2] = 0.69102; // b
	grgKd6[3] = 1.0f;    // a

	grgKs6[0] = 0.297254; // r
	grgKs6[1] = 0.30829;  // g
	grgKs6[2] = 0.306678; // b
	grgKs6[3] = 1.0f;     // a
	grgMaterialShinyness6 = 0.1 * 128;

	grgKa7[0] = 0.329412; // r
	grgKa7[1] = 0.223529; // g
	grgKa7[2] = 0.027451; // b
	grgKa7[3] = 1.0f;     // a

	grgKd7[0] = 0.780392; // r
	grgKd7[1] = 0.568627; // g
	grgKd7[2] = 0.113725; // b
	grgKd7[3] = 1.0f;    // a

	grgKs7[0] = 0.992157; // r
	grgKs7[1] = 0.941176; // g
	grgKs7[2] = 0.807843; // b
	grgKs7[3] = 1.0f;     // a

	grgMaterialShinyness7 = 0.21794872 * 128;

	grgKa8[0] = 0.2125; // r
	grgKa8[1] = 0.1275; // g
	grgKa8[2] = 0.054;  // b
	grgKa8[3] = 1.0f;   // a

	grgKd8[0] = 0.714;   // r
	grgKd8[1] = 0.4284;  // g
	grgKd8[2] = 0.18144; // b
	grgKd8[3] = 1.0f;    // a

	grgKs8[0] = 0.393548; // r
	grgKs8[1] = 0.271906; // g
	grgKs8[2] = 0.166721; // b
	grgKs8[3] = 1.0f;     // a

	grgMaterialShinyness8 = 0.2 * 128;

	grgKa9[0] = 0.25; // r
	grgKa9[1] = 0.25; // g
	grgKa9[2] = 0.25; // b
	grgKa9[3] = 1.0f; // a

	grgKd9[0] = 0.4;  // r
	grgKd9[1] = 0.4;  // g
	grgKd9[2] = 0.4;  // b
	grgKd9[3] = 1.0f; // a

	grgKs9[0] = 0.774597; // r
	grgKs9[1] = 0.774597; // g
	grgKs9[2] = 0.774597; // b
	grgKs9[3] = 1.0f;     // a

	grgMaterialShinyness9 = 0.6 * 128;

	grgKa10[0] = 0.19125; // r
	grgKa10[1] = 0.0735;  // g
	grgKa10[2] = 0.0225;  // b
	grgKa10[3] = 1.0f;    // a

	grgKd10[0] = 0.7038;  // r
	grgKd10[1] = 0.27048; // g
	grgKd10[2] = 0.0828;  // b
	grgKd10[3] = 1.0f;    // a

	grgKs10[0] = 0.256777; // r
	grgKs10[1] = 0.137622; // g
	grgKs10[2] = 0.086014; // b
	grgKs10[3] = 1.0f;     // a
	grgMaterialShinyness10 = 0.1 * 128;

	grgKa11[0] = 0.24725; // r
	grgKa11[1] = 0.1995;  // g
	grgKa11[2] = 0.0745;  // b
	grgKa11[3] = 1.0f;    // a

	grgKd11[0] = 0.75164; // r
	grgKd11[1] = 0.60648; // g
	grgKd11[2] = 0.22648; // b
	grgKd11[3] = 1.0f;    // a

	grgKs11[0] = 0.628281; // r
	grgKs11[1] = 0.555802; // g
	grgKs11[2] = 0.366065; // b
	grgKs11[3] = 1.0f;     // a

	grgMaterialShinyness11 = 0.4 * 128;

	grgKa12[0] = 0.19225; // r
	grgKa12[1] = 0.19225; // g
	grgKa12[2] = 0.19225; // b
	grgKa12[3] = 1.0f;    // a

	grgKd12[0] = 0.50754; // r
	grgKd12[1] = 0.50754; // g
	grgKd12[2] = 0.50754; // b
	grgKd12[3] = 1.0f;    // a

	grgKs12[0] = 0.508273; // r
	grgKs12[1] = 0.508273; // g
	grgKs12[2] = 0.508273; // b
	grgKs12[3] = 1.0f;     // a

	grgMaterialShinyness12 = 0.4 * 128;

	grgKa13[0] = 0.0;  // r
	grgKa13[1] = 0.0;  // g
	grgKa13[2] = 0.0;  // b
	grgKa13[3] = 1.0f; // a

	grgKd13[0] = 0.01; // r
	grgKd13[1] = 0.01; // g
	grgKd13[2] = 0.01; // b
	grgKd13[3] = 1.0f; // a

	grgKs13[0] = 0.50; // r
	grgKs13[1] = 0.50; // g
	grgKs13[2] = 0.50; // b
	grgKs13[3] = 1.0f; // a

	grgMaterialShinyness13 = 0.25 * 128;

	grgKa14[0] = 0.0;  // r
	grgKa14[1] = 0.1;  // g
	grgKa14[2] = 0.06; // b
	grgKa14[3] = 1.0f; // a

	grgKd14[0] = 0.0;        // r
	grgKd14[1] = 0.50980392; // g
	grgKd14[2] = 0.50980392; // b
	grgKd14[3] = 1.0f;       // a

	grgKs14[0] = 0.50196078; // r
	grgKs14[1] = 0.50196078; // g
	grgKs14[2] = 0.50196078; // b
	grgKs14[3] = 1.0f;       // a

	grgMaterialShinyness14 = 0.25 * 128;

	grgKa15[0] = 0.0;  // r
	grgKa15[1] = 0.0;  // g
	grgKa15[2] = 0.0;  // b
	grgKa15[3] = 1.0f; // a

	grgKd15[0] = 0.1;  // r
	grgKd15[1] = 0.35; // g
	grgKd15[2] = 0.1;  // b
	grgKd15[3] = 1.0f; // a

	grgKs15[0] = 0.45; // r
	grgKs15[1] = 0.55; // g
	grgKs15[2] = 0.45; // b
	grgKs15[3] = 1.0f; // a

	grgMaterialShinyness15 = 0.25 * 128;

	grgKa16[0] = 0.0;  // r
	grgKa16[1] = 0.0;  // g
	grgKa16[2] = 0.0;  // b
	grgKa16[3] = 1.0f; // a

	grgKd16[0] = 0.5;  // r
	grgKd16[1] = 0.0;  // g
	grgKd16[2] = 0.0;  // b
	grgKd16[3] = 1.0f; // a

	grgKs16[0] = 0.7;  // r
	grgKs16[1] = 0.6;  // g
	grgKs16[2] = 0.6;  // b
	grgKs16[3] = 1.0f; // a

	grgMaterialShinyness16 = 0.25 * 128;

	grgKa17[0] = 0.0;  // r
	grgKa17[1] = 0.0;  // g
	grgKa17[2] = 0.0;  // b
	grgKa17[3] = 1.0f; // a

	grgKd17[0] = 0.55; // r
	grgKd17[1] = 0.55; // g
	grgKd17[2] = 0.55; // b
	grgKd17[3] = 1.0f; // a

	grgKs17[0] = 0.70; // r
	grgKs17[1] = 0.70; // g
	grgKs17[2] = 0.70; // b
	grgKs17[3] = 1.0f; // a

	grgMaterialShinyness17 = 0.25 * 128;

	grgKa18[0] = 0.0;  // r
	grgKa18[1] = 0.0;  // g
	grgKa18[2] = 0.0;  // b
	grgKa18[3] = 1.0f; // a

	grgKd18[0] = 0.5;  // r
	grgKd18[1] = 0.5;  // g
	grgKd18[2] = 0.0;  // b
	grgKd18[3] = 1.0f; // a

	grgKs18[0] = 0.60; // r
	grgKs18[1] = 0.60; // g
	grgKs18[2] = 0.50; // b
	grgKs18[3] = 1.0f; // a
	grgMaterialShinyness18 = 0.25 * 128;

	grgKa19[0] = 0.02; // r
	grgKa19[1] = 0.02; // g
	grgKa19[2] = 0.02; // b
	grgKa19[3] = 1.0f; // a

	grgKd19[0] = 0.01; // r
	grgKd19[1] = 0.01; // g
	grgKd19[2] = 0.01; // b
	grgKd19[3] = 1.0f; // a

	grgKs19[0] = 0.4;  // r
	grgKs19[1] = 0.4;  // g
	grgKs19[2] = 0.4;  // b
	grgKs19[3] = 1.0f; // a
	grgMaterialShinyness19 = 0.078125 * 128;

	grgKa20[0] = 0.0;  // r
	grgKa20[1] = 0.05; // g
	grgKa20[2] = 0.05; // b
	grgKa20[3] = 1.0f; // a

	grgKd20[0] = 0.4;  // r
	grgKd20[1] = 0.5;  // g
	grgKd20[2] = 0.5;  // b
	grgKd20[3] = 1.0f; // a

	grgKs20[0] = 0.04; // r
	grgKs20[1] = 0.7;  // g
	grgKs20[2] = 0.7;  // b
	grgKs20[3] = 1.0f; // a	
	grgMaterialShinyness20 = 0.078125 * 128;

	grgKa21[0] = 0.0;  // r
	grgKa21[1] = 0.05; // g
	grgKa21[2] = 0.0;  // b
	grgKa21[3] = 1.0f; // a

	grgKd21[0] = 0.4;  // r
	grgKd21[1] = 0.5;  // g
	grgKd21[2] = 0.4;  // b
	grgKd21[3] = 1.0f; // a

	grgKs21[0] = 0.04; // r
	grgKs21[1] = 0.7;  // g
	grgKs21[2] = 0.04; // b
	grgKs21[3] = 1.0f; // a

	grgMaterialShinyness21 = 0.078125 * 128;

	grgKa22[0] = 0.05; // r
	grgKa22[1] = 0.0;  // g
	grgKa22[2] = 0.0;  // b
	grgKa22[3] = 1.0f; // a

	grgKd22[0] = 0.5;  // r
	grgKd22[1] = 0.4;  // g
	grgKd22[2] = 0.4;  // b
	grgKd22[3] = 1.0f; // a

	grgKs22[0] = 0.7;  // r
	grgKs22[1] = 0.04; // g
	grgKs22[2] = 0.04; // b
	grgKs22[3] = 1.0f; // a
	grgMaterialShinyness22 = 0.078125 * 128;

	grgKa23[0] = 0.05; // r
	grgKa23[1] = 0.05; // g
	grgKa23[2] = 0.05; // b
	grgKa23[3] = 1.0f; // a

	grgKd23[0] = 0.5;  // r
	grgKd23[1] = 0.5;  // g
	grgKd23[2] = 0.5;  // b
	grgKd23[3] = 1.0f; // a

	grgKs23[0] = 0.7;  // r
	grgKs23[1] = 0.7;  // g
	grgKs23[2] = 0.7;  // b
	grgKs23[3] = 1.0f; // a
	grgMaterialShinyness23 = 0.078125 * 128;

	grgKa24[0] = 0.05; // r
	grgKa24[1] = 0.05; // g
	grgKa24[2] = 0.0;  // b
	grgKa24[3] = 1.0f; // a

	grgKd24[0] = 0.5;  // r
	grgKd24[1] = 0.5;  // g
	grgKd24[2] = 0.4;  // b
	grgKd24[3] = 1.0f; // a

	grgKs24[0] = 0.7;  // r
	grgKs24[1] = 0.7;  // g
	grgKs24[2] = 0.04; // b
	grgKs24[3] = 1.0f; // a

	grgMaterialShinyness24 = 0.078125 * 128;

}

void Uninitialize(void)
{
	// code
	if (gpiD3D11RasterizerState)
	{
		gpiD3D11RasterizerState->Release();
		gpiD3D11RasterizerState = NULL;
	}
	if (gpID3D11Buffer_ConstantBufferPV)
	{
		gpID3D11Buffer_ConstantBufferPV->Release();
		gpID3D11Buffer_ConstantBufferPV = NULL;
	}
	if (gpID3D11Buffer_ConstantBufferPP)
	{
		gpID3D11Buffer_ConstantBufferPP->Release();
		gpID3D11Buffer_ConstantBufferPP = NULL;
	}
	if (gpID3D11InputLayoutPV)
	{
		gpID3D11InputLayoutPV->Release();
		gpID3D11InputLayoutPV = NULL;
	}
	if (gpID3D11InputLayoutPP)
	{
		gpID3D11InputLayoutPP->Release();
		gpID3D11InputLayoutPP = NULL;
	}

	if (gpiD3D11Buffer_VertexBuffer_Indexbuffer)
	{
		gpiD3D11Buffer_VertexBuffer_Indexbuffer->Release();
		gpiD3D11Buffer_VertexBuffer_Indexbuffer = NULL;
	}
	if (gpiD3D11Buffer_VertexBuffer_NormalRectangle)
	{
		gpiD3D11Buffer_VertexBuffer_NormalRectangle->Release();
		gpiD3D11Buffer_VertexBuffer_NormalRectangle = NULL;
	}
	if (gpiD3D11Buffer_VertexBuffer_PositionRectangle)
	{
		gpiD3D11Buffer_VertexBuffer_PositionRectangle->Release();
		gpiD3D11Buffer_VertexBuffer_PositionRectangle = NULL;
	}
	if (gpID3D11PixelShaderPV)
	{
		gpID3D11PixelShaderPV->Release();
		gpID3D11PixelShaderPV = NULL;
	}
	if (gpID3D11VertexShaderPV)
	{
		gpID3D11VertexShaderPV->Release();
		gpID3D11VertexShaderPV = NULL;
	}
	if (gpID3D11PixelShaderPP)
	{
		gpID3D11PixelShaderPP->Release();
		gpID3D11PixelShaderPP = NULL;
	}
	if (gpID3D11VertexShaderPP)
	{
		gpID3D11VertexShaderPP->Release();
		gpID3D11VertexShaderPP = NULL;
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









