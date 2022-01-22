#include<windows.h>
#include<stdio.h>
#include<gl\glew.h> // glew.h must be included BEFORE gl.h
#include<gl\GL.h>
#include"vmath.h"
#include"GRWindow.h"

#include"Sphere.h"
// assimp includes

#include"GRRoad.h"
#include"GRGrassQuad.h"
#include"GRGrass.h"
#include"GRTree.h"
#include"GRSky.h"
#include"GRTerrain.h"


using namespace vmath;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "assimp-vc141-mtd.lib")
#pragma comment(lib, "FreeImage.lib")
#define _CRT_SECURE_NO_WARNINGS


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



//grcommonviewX, grcommonViewY, grcommonViewZ
GLfloat grcommonviewX = 0.0f, grcommonviewY = 0.0f, grcommonviewZ = 0.0f;
GLfloat grcommonviewEyeX = 0.0f, grcommonviewEyeY = 0.0f, grcommonviewEyeZ = 0.0f;
/*
#pragma region grass vars
GLuint grshaderProgramObjectGrass;
vector< vector< glm::vec3> > grvertexDataGrass;
//
GLuint grtextureGrass;
int grwidthGrass, grheightGrass, grBPPGrass;
string grspathGrass;
bool grbMipMapGeneratedGrass;
GLuint grsamplerGrass;
int grtfMinificationGrass, grtfMagnificationGrass;
GLuint grtimePassedUniformGrass;
float grtim1 = 0;


glm::vec3 grvRenderScaleGrass;
int griNumGrassTrianglesGrass;
bool grbLoadedGrass;
int griRowsGrass;
int griColsGrass;
int griGrass;

GLuint grvboGrass;
GLuint grvaoGrass;


GLuint grviewMatrixUniformGrass;
GLuint greyeUniformGrass;
GLuint grmodelUniformGrass;
GLuint grcoloruniformGrass;
GLuint graplhaTestGrass;
GLuint gralphaMultiplierGrass;
GLuint grnormalMatrixUniformGrass;
GLuint grprojectionMatrixUniformGrass;
GLuint grtextureSamplerUniformGrass;

vector<BYTE> grdataGrass;
int grcurrentSizeGrass;


enum ETextureFilteringGrass
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};
#pragma endregion grass vars
*/
enum grmoveModel
{
	terrain = 0,
	road,
	grassQuad
};
grmoveModel grCurrentMoveModel = terrain;



void Uninitialize();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Function declaration
	void Initialize(void);
	void Display(void);
	void Uninitialize(void);

	// variables declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("OGL");
	int grDesktopWidth, grDesktopHeight;
	int grWndXPos, grWndYPos;
	bool grbDone = false;
	bool grgbEscapeKeyIsPressed = false;

	if (fopen_s(&grgpFile, "GRLog.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot open desired file"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf_s(grgpFile, "Log file created successfully. \n Program started successfully\n **** Logs ***** \n");
	}

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(GRICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(GRICON));

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
		TEXT("OpenGL"),
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

	Initialize();

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
				Display();
			}
		}

	}

	Uninitialize();

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declaration
	void ToggleFullScreen(void);
	void Resize(int, int);
	void Uninitialize(void);


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
		Resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CHAR:
		switch (wParam)
		{

		case 'x':
			// terrain's Y camera coordinate
			if (grCurrentMoveModel == terrain)
			{
				GRDecViewXTerrain();
			}
			else if (grCurrentMoveModel == road)
			{
				GRDecModelXRoad();
			}
			else if (grCurrentMoveModel == grassQuad)
			{
				GRDecModelXGrassQuad();
			}
			break;

		case 'X':
			if (grCurrentMoveModel == terrain)
			{
				GRIncViewXTerrain();
			}
			else if (grCurrentMoveModel == road)
			{
				GRIncModelXRoad();
			}
			else if (grCurrentMoveModel == grassQuad)
			{
				GRIncModelXGrassQuad();
			}
			break;

		case 'z':
			if (grCurrentMoveModel == terrain)
			{
				GRDecViewZTerrain();
			}
			else if (grCurrentMoveModel == road)
			{
				GRDecModelZRoad();
			}
			else if (grCurrentMoveModel == grassQuad)
			{
				GRDecModelZGrassQuad();
			}
			break;

		case 'Z':
			if (grCurrentMoveModel == terrain)
			{
				GRIncViewZTerrain();
			}
			else if (grCurrentMoveModel == road)
			{
				GRIncModelZRoad();
			}
			else if (grCurrentMoveModel == grassQuad)
			{
				GRIncModelZGrassQuad();
			}
			break;

		case 'y':
			if (grCurrentMoveModel == terrain)
			{
				// terrain's Y camera coordinate
				GRDecViewYTerrain();
			}
			else if (grCurrentMoveModel == road)
			{
				GRDecModelYRoad();
			}
			else if (grCurrentMoveModel == grassQuad)
			{
				GRDecModelYGrassQuad();
			}
			break;

		case 'Y':
			if (grCurrentMoveModel == terrain)
			{
				GRIncViewYTerrain();
			}
			else if (grCurrentMoveModel == road)
			{
				GRIncModelYRoad();
			}
			else if (grCurrentMoveModel == grassQuad)
			{
				GRIncModelYGrassQuad();
			}
			break;

		case 'r':
		case 'R':
			//grCurrentMoveModel = road;
			//grCurrentMoveModel = grassQuad;
			break;

		case 'a':
			if (grCurrentMoveModel == road)
			{
				GRIncAngleXRoad();
			}
			else if (grCurrentMoveModel == grassQuad)
			{
				GRIncAngleXGrassQuad();
			}
			break;

		case 'S':
			if (grCurrentMoveModel == road)
			{
				GRDecAngleYRoad();
			}
			else if (grCurrentMoveModel == grassQuad)
			{
				//GRDecAngleYGrassQuad();
			}
			break;

		case 's':
			if (grCurrentMoveModel == road)
			{
				GRIncAngleYRoad();
			}
			else if (grCurrentMoveModel == grassQuad)
			{
				GRIncAngleYGrassQuad();
			}
			break;

		case 'd':
			if (grCurrentMoveModel == road)
			{
				GRIncAngleZRoad();
			}
			else if (grCurrentMoveModel == grassQuad)
			{
				GRIncAngleZGrassQuad();
			}
			break;

		case 'p':
			GRIncModelXTree();
			GRIncModelXHouse();
			break;

		case 'P':
			GRDecModelXTree();
			GRDecModelXHouse();
			break;

		case 'o':
			GRIncModelYTree();
			GRIncModelYHouse();
			break;

		case 'O':
			GRDecModelYTree();
			GRDecModelYHouse();
			break;

		case 'i':
			GRIncModelZTree();
			GRIncModelZHouse();
			break;

		case 'I':
			GRDecModelZTree();
			GRDecModelZHouse();
			break;

		case 'm':
			//GRIncModelYGrass();
			GRIncAngleX();
			//grlightPosX = grlightPosX + 0.1f;
			break;

		case 'M':
			GRDecModelYGrass();
			//grlightPosX = grlightPosX - 0.1f;
			break;

		case 'n':
			GRIncAngleY();
			//GRIncModelXGrass();
			//grlightPosY = grlightPosY + 0.1f;
			break;

		case 'N':
			GRDecModelXGrass();
			//grlightPosY = grlightPosY - 0.1f;
			break;

		case 'b':
			GRIncAngleZ();
			//GRIncModelZGrass();
			//grlightPosZ = grlightPosZ + 0.1f;
			break;

		case 'B':
			//GRDecModelZGrass();
			//grlightPosZ = grlightPosZ - 0.1f;
			break;

		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			GRPrintVarsRoad();
			GRPrintVarsGrassQuad();
			GRPrintVarsGrass();
			GRPrintVarsTree();
			GRPrintVarsTerrain();
			DestroyWindow(hwnd);
			break;

		case 0x46:
			ToggleFullScreen();
			break;

		case VK_UP:
			// terrain depth (height)
			GRIncDepthTerrain();

			break;

		case VK_DOWN:
			// terrain depth (height)
			GRDecDepthTerrain();
			break;
			/*
			case VK_NUMPAD0:
				y2 = y2 - 0.1f;
				break;

			case VK_NUMPAD1 :
				x2 = x2 - 0.1f;
				break;

			case VK_NUMPAD5:
				y2 = y2 + 0.1f;
				break;

			case VK_NUMPAD6:
				x2 = x2 + 0.1f;
				break;

			case VK_SPACE:
				grswitch = !grswitch;
				break;
			*/

		case VK_MULTIPLY:
			// sky clouds scale
			GRIncScaleSky();
			break;

		case VK_DIVIDE:
			GRDecScaleSky();
			break;

		case VK_NUMPAD2:
			// sky - camera Z coord
			GRIncViewZSky();
			break;

		case VK_NUMPAD8:
			// sky - camera Z coord
			GRDecViewZSky();
			break;

		case VK_NUMPAD4:
			// sky - camera X coord
			GRDecViewXSky();
			break;

		case VK_NUMPAD6:
			GRIncViewXSky();
			break;

		case VK_NUMPAD0:
			// sky - camera X coord
			GRDecViewYSky();
			break;

		case VK_NUMPAD5:
			GRIncViewYSky();
			break;

		case VK_SPACE:
			grCurrentMoveModel = terrain;
			break;

		default:
			break;
		}
		break;

	case WM_CLOSE:
		Uninitialize();
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

void Initialize()
{
	// function declaration
	void Resize(int, int);
	void Uninitialize(void);
	//bool LoadGLTexture(GLuint*, TCHAR[]);


	//variable declarations
	PIXELFORMATDESCRIPTOR grpfd;
	int griPixelFormatIndex;

	//code
	grghdc = GetDC(grghwnd);

	ZeroMemory(&grpfd, sizeof(PIXELFORMATDESCRIPTOR));
	grpfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	grpfd.nVersion = 1;
	grpfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	grpfd.iPixelType = PFD_TYPE_RGBA;
	grpfd.cColorBits = 32;
	grpfd.cRedBits = 8;
	grpfd.cGreenBits = 8;
	grpfd.cBlueBits = 8;
	grpfd.cAlphaBits = 8;
	grpfd.cDepthBits = 32;

	griPixelFormatIndex = ChoosePixelFormat(grghdc, &grpfd);
	if (griPixelFormatIndex == 0)
	{
		fprintf(grgpFile, "ChoosePixelFormat() failed\n");
		Uninitialize();
	}

	if (SetPixelFormat(grghdc, griPixelFormatIndex, &grpfd) == FALSE)
	{
		fprintf(grgpFile, "SetPixelFormat() failed\n");
		Uninitialize();
	}

	grghrc = wglCreateContext(grghdc);
	if (grghrc == NULL)
	{
		fprintf(grgpFile, "wglCreateContext() failed\n");
		ReleaseDC(grghwnd, grghdc);
		grghdc = NULL;
	}

	if (wglMakeCurrent(grghdc, grghrc) == FALSE)
	{
		fprintf(grgpFile, "wglMakeCurrent() failed\n");
		wglDeleteContext(grghrc);
		grghrc = NULL;
		ReleaseDC(grghwnd, grghdc);
		grghdc = NULL;
	}

	GLenum grglew_error = glewInit();
	if (grglew_error != GLEW_OK)
	{
		wglDeleteContext(grghrc);
		grghrc = NULL;
		ReleaseDC(grghwnd, grghdc);
		grghdc = NULL;
	}

	fprintf(grgpFile, "\n test");

#pragma region terrain init
	//********* Shader Code ********
		// create shader

	/*
	grgRandPositionUniform[0] = glGetUniformLocation(grgShaderProgramObject, "u_rand_pos[0]");
	grgRandPositionUniform[1] = glGetUniformLocation(grgShaderProgramObject, "u_rand_pos[1]");
	grgRandPositionUniform[2] = glGetUniformLocation(grgShaderProgramObject, "u_rand_pos[2]");
	grgRandPositionUniform[3] = glGetUniformLocation(grgShaderProgramObject, "u_rand_pos[3]");
	grgRandPositionUniform[4] = glGetUniformLocation(grgShaderProgramObject, "u_rand_pos[4]");
	grgRandPositionUniform[5] = glGetUniformLocation(grgShaderProgramObject, "u_rand_pos[5]");
	grgRandPositionUniform[6] = glGetUniformLocation(grgShaderProgramObject, "u_rand_pos[6]");
	grgRandPositionUniform[7] = glGetUniformLocation(grgShaderProgramObject, "u_rand_pos[7]");
	grgRandPositionUniform[8] = glGetUniformLocation(grgShaderProgramObject, "u_rand_pos[8]");
	grgRandPositionUniform[9] = glGetUniformLocation(grgShaderProgramObject, "u_rand_pos[9]");
	*/
	GRInitializeTerrain();

#pragma endregion terrain init

#pragma region sky init
	GRInitializeSky();
#pragma endregion sky init


#pragma region tri init
	// create shader
	GRInitializeTree();

#pragma endregion tri init



	/*
	const GLfloat grtriangleVertices[] =
	{
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	glGenVertexArrays(1, &grgVao);
	glBindVertexArray(grgVao);

	glGenBuffers(1, &grgVbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grtriangleVertices), grtriangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	*/


#pragma region road init
	GRInitializeRoad();
#pragma endregion road init

#pragma region grass quad init
	GRInitializeGrassQuad();
#pragma endregion grass quad init

#pragma region grass init
	GRInitializeGrass();
#pragma endregion grass init

	
	

	/*
	for (int i = 0; i < 10; i++)
	{
		grgRandomX = rand() % 5;
		grgRandomY = 0.0f;
		grgRandomZ = rand() % 10;
		grgRandPos[i] = vec3(grgRandomX, grgRandomY, -grgRandomZ);
	}
	*/

	
	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// set clearcolor
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	

	// warm-up call to resize
	Resize(WIN_WIDTH, WIN_HEIGHT);
}




void Resize(int width, int height)
{
	if (height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

#pragma region terrain resize
	GRResizeTerrain(width, height);
#pragma endregion terrain resize
	

#pragma region road resize
	GRResizeRoad(width, height);
#pragma endregion road resize

#pragma region grass quad resize
	GRResizeGrassQuad(width, height);
#pragma endregion grass quad resize

#pragma region grass resize
	GRResizeGrass(width, height);
#pragma endregion frass resize
#pragma resize tree and house
	GRResizeTree(width, height);
#pragma endregion resize tree and house

#pragma resize tree and house
	GRResizeSky(width, height);
#pragma endregion resize tree and house
}

void Display(void)
{
	// function declaration
	void Update();
	void DrawMeshTree(int);
	void DrawMeshHouse(int);
	// code

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// OpenGL drawing code will start here

	mat4 grmodelMatrix = mat4::identity();
	mat4 grviewMatrix = mat4::identity();
	mat4 grprojectionMatrix = mat4::identity();
	mat4 grmodelViewProjectionMatrix = mat4::identity();
	mat4 grtranslateMatrix = mat4::identity();
	mat4 grrotateMatrix = mat4::identity();
	GRGetViewXYZTerrain(grcommonviewX, grcommonviewY, grcommonviewZ);
	GRGetViewEyeXYZTerrain(grcommonviewEyeX, grcommonviewEyeY, grcommonviewEyeZ);

#pragma region grass display
	GRSetViewMatrixGrass(
		vec3(grcommonviewX, grcommonviewY, grcommonviewZ),
		vec3(grcommonviewEyeX, grcommonviewEyeY, grcommonviewEyeZ),
		vec3(0.0f, 1.0f, 0.0f)
	);
	GRDisplayGrass();
#pragma endregion grass display
#pragma region tri display
	GRSetViewMatrixTree(vec3(grcommonviewX, grcommonviewY, grcommonviewZ), vec3(grcommonviewEyeX, grcommonviewEyeY, grcommonviewEyeZ), vec3(0.0f, 1.0f, 0.0f));
	GRDisplayTree();
#pragma endregion tri display

#pragma region terrain display
	GRDisplayTerrain();
	
#pragma endregion terrain display


#pragma region sky display
	
	//GRSetViewMatrixSky(vec3(x2, y2, z2), vec3(x2, y2, -6.0 + z2), vec3(0.0f, 1.0f, 0.0f));
	GRDisplaySky();
#pragma endregion sky display

#pragma region road display
	GRSetViewMatrixRoad(vec3(grcommonviewX, grcommonviewY, grcommonviewZ), vec3(grcommonviewEyeX, grcommonviewEyeY, grcommonviewEyeZ), vec3(0.0f, 1.0f, 0.0f));
	GRDisplayRoad();
#pragma endregion road display

#pragma region grass quad display
		GRSetViewMatrixGrassQuad(vec3(grcommonviewX, grcommonviewY, grcommonviewZ), vec3(grcommonviewEyeX, grcommonviewEyeY, grcommonviewEyeZ), vec3(0.0f, 1.0f, 0.0f));
	GRDisplayGrassQuad();
#pragma endregion grassQuad display



	Update();
	SwapBuffers(grghdc);
}

void Update()
{
	
}







void Uninitialize(void)
{
	void GRUnitializeGrass();

	//code
	if (grgbFullScreen == true)
	{
		grdwStyle = GetWindowLong(grghwnd, GWL_STYLE);
		SetWindowLong(grghwnd, GWL_STYLE, (grdwStyle | WS_OVERLAPPEDWINDOW));
		SetWindowPlacement(grghwnd, &grgwpPrev);
		SetWindowPos(grghwnd, HWND_TOP, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(true);

	}

#pragma region terrain uninit
	GRUninitializeTerrain();
#pragma endregion terrain uninit

#pragma region road uninit
	GRUninitializeRoad();
#pragma endregion road uninit

#pragma region grass quad uninit
	GRUninitializeGrassQuad();
#pragma endregion grass quad uninit

#pragma region grass uninit
	GRUnitializeGrass();
#pragma endregion grass uninit 

#pragma region tree uninit
	GRUninitializeTree();
#pragma endregion tree uninit 

#pragma region tree uninit
	GRUninitializeSky();
#pragma endregion tree uninit 

	if (wglGetCurrentContext() == grghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (grghrc)
	{
		wglDeleteContext(grghrc);
		grghrc = NULL;
	}

	if (grghdc)
	{
		ReleaseDC(grghwnd, grghdc);
		grghdc = NULL;
	}

	
}

void GRUnitializeGrass()
{

}








