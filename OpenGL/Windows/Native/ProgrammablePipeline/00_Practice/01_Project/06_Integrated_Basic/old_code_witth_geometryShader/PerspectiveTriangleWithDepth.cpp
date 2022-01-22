#include<windows.h>
#include<stdio.h>
#include<gl\glew.h> // glew.h must be included BEFORE gl.h
#include<gl\GL.h>
#include"vmath.h"
#include"GRWindow.h"
#include"ktx_loading_header.h"
//#include"Sphere.h"
// assimp includes
#include"assimp\mesh.h"
#include"assimp\scene.h"
#include"assimp\postprocess.h"
#include"filesystem.h"
#include"Model.h"
#include"GRRoad.h"
#include"GRGrassQuad.h"
#include"FreeImage.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

using namespace vmath;
#define FOR(q,n) for(int q=0;q<n;q++)

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
FILE *grgpFile = NULL;

GLuint grgVertexShaderObject;
GLuint grgFragmentShadeerObject;
GLuint grgShaderProgramObject;
GLuint grgTesControlShaderObject;
GLuint grgTesEvaluationShaderObject;
GLuint grgGeometryShaderObject;

/*
enum
{
	GR_ATTRIBUTE_POSITION = 0,
	GR_ATTRIBUTE_COLOR,
	GR_ATTRIBUTE_TEXCOORD,
	GR_ATTRIBUTE_NORMAL
};
*/

GLuint grgVao;
GLuint grgVbo_position;
GLuint grmodelMatrixUniformTerrain;
GLuint grviewMatrixUniformTerrain;
GLuint grprojectionMatrixUniformTerrain;
GLuint grgTextureTerragen1;
GLuint grgTextureTerragenColor;
GLuint tex_src;
GLuint tex_src2;
GLuint grgTextureSamplerUniformTexColor;
GLuint grgTextureSamplerUniformTexDisplacement;
GLuint grgDMapDepthUniform;
GLfloat grgDepth = 4.0f;
mat4 grgPerspectiveProjectionMatrix;
GLfloat x1 = 0.0f;
GLfloat gry1 = 0.0f;
GLfloat z1 = 0.0f;
GLfloat x2 = 0.0f;
GLfloat y2 = -1.0f;
GLfloat z2 = -6.0f;
bool grswitch;

GLuint grgTextureBlendMap;
GLuint grgTextureGrass;
GLuint grgTextureDirt;
GLuint grgTextureGrassFlowers;
GLuint grgTexturePath;

GLuint grgTextureSamplerUniformBlendMap;
GLuint grgTextureSamplerUniformGrass;
GLuint grgTextureSamplerUniformDirt;
GLuint grgTextureSamplerUniformGrassFlowers;
GLuint grgTextureSamplerUniformPath;
GLuint grgRandPositionUniform[20];
GLfloat grgRandomX;
GLfloat grgRandomY;
GLfloat grgRandomZ;
vec3 grgRandPos[20];

#pragma region sky vars
// sky vars
GLuint grgVertexShaderObjectSky;
GLuint grgFragmentShadeerObjectSky;
GLuint grgShaderProgramObjectSky;
float grsphereVertices[1146];
float grsphereNormals[1146];
float grsphereTextures[764];
unsigned short grsphereElements[2280];
GLuint grgNumVertices;
GLuint grgNumElements;
GLuint grgVboSpherePosition;
GLuint grgVboSphereNormal;
GLuint grgVboSphereElement;
GLuint grgVaoSphere;
GLuint grgModelMatrixUniform;
GLuint grgViewMatrixUniform;
GLuint grgProjectionMatrixUniform;
GLuint grgLightPositionUniform;
GLuint grgScaleUniform;
GLuint grgTextureSamplerUniform;
GLuint grgTextureSamplerNoiseUniform;
GLuint grgSkyColorUniform;
GLuint grgCloudColorUniform;
GLuint grgResolutionUniform;
GLuint grgTimeUniform;

float grgScale = 2.0f;
float grgTime = 0.0f;

GLfloat grgViewX = 0.0f, grgViewY = 0.0f, grgViewZ = -20.0f;
#pragma endregion sky vars

#pragma region tri vars
GLuint grgVertexShaderObjectTri;
GLuint grgFragmentShadeerObjectTri;
GLuint grgShaderProgramObjectTri;
GLuint grgGeometryShaderObjectTri;
GLuint grgModelMatrixUniformTri;
GLuint grgViewMatrixUniformTri;
GLuint grgProjectionMatrixUniformTri;
Model* grgModelTree;
Model* grgModelHouse;
GLuint grgKaUniformTree;
GLuint grgKdUniformTree;
GLuint grgKsUniformTree;
GLuint grgLaUniformTree;
GLuint grgLdUniformTree;
GLuint grgLsUniformTree;
GLuint grgLightPositionUniformTree;
GLuint grgMaterialShinynessUniformTree;
vec4 grgLaTree = { 0.0f, 0.0f, 0.0f, 1.0f };
vec4 grgLdTree = { 1.0f, 1.0f, 1.0f, 1.0f };
vec4 grgLsTree = { 1.0f, 1.0f, 1.0f, 1.0f };
vec4 grgLightPositionTree = { 7.0, 100.0, -100.0, 0.0f };
GLuint grgTextureSamplerUniformTree;
GLuint grgLightUniform;
#pragma endregion tri vars

#pragma region grass vars
GLuint G_gShaderProgramObject;
vector< vector< glm::vec3> > vVertexData;
//
GLuint uiTexture;
int iWidth, iHeight, iBPP;
string sPath;
bool bMipMapsGenerated;
GLuint uiSampler;
int tfMinification, tfMagnification;
GLuint fTimePassedUniform;
float tim1 = 0;


glm::vec3 vRenderScale;
int iNumGrassTriangles;
bool bLoaded;
int iRows;
int iCols;
int i;

GLuint uiBuffer;
GLuint uiGrassVAO;


GLuint G_viewUniform;
GLuint G_vEyeUniform;
GLuint G_modelUniform;
GLuint G_vColorUniform;
GLuint G_fAlphaTest;
GLuint G_fAlphaMultiplier;
GLuint G_NormalMatrixUniform;
GLuint G_projectionUniform;
GLuint G_gSampler;


float fAlphaTestVAR = 0.25f;
float fAlphaMultiplierVAR = 1.5f;


vector<BYTE> data1;
int iCurrentSize;


enum ETextureFiltering
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

enum grmoveModel
{
	terrain = 0,
	road,
	grassQuad
};
grmoveModel grCurrentMoveModel = terrain;


BOOL loadTexture(GLuint* texture_Smiley, TCHAR imageResourceID[]);
bool LoadHeightMapFromImageFree(string sImagePath);
void AddData(void* ptrData, UINT uiDataSize);
void LoadAllTextures();
bool LoadTexture2D(string a_sPath, bool bGenerateMipMaps);
void SetFiltering(int a_tfMagnification, int a_tfMinification);
void SetWrap();
void SetAnisotropyLevel(float fAnisotropyLevel);
void CreateFromData(BYTE* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps);
float GetHeightFromRealVector(glm::vec3 vRealPosition);
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
	
	if(fopen_s(&grgpFile, "GRLog.txt", "w") != 0)
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
			Resize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_CHAR:
			switch (wParam)
			{

				case 'x':
					// terrain's Y camera coordinate
					if (grCurrentMoveModel == terrain)
					{
						x2 = x2 - 1.1f;
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
						x2 = x2 + 1.1f;
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
						z2 = z2 - 1.1f;
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
						z2 = z2 + 1.1f;
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
						y2 = y2 - 1.1f;
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
						y2 = y2 + 1.1f;
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
				case 'R' :
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
			}
			break;
		
		case WM_KEYDOWN : 
			switch(wParam)
			{
				case VK_ESCAPE :
					GRPrintVarsRoad();
					GRPrintVarsGrassQuad();
					fprintf(grgpFile, "\n terrain X : %f, terrainY : %f, terrainZ : %f", x2, y2, z2);
					break;
				
				case 0x46 :
					ToggleFullScreen();
					break;

				case VK_UP :
					// terrain depth (height)
					grgDepth = grgDepth + 1.2f;
					
					break;

				case VK_DOWN :
					// terrain depth (height)
					grgDepth = grgDepth - 1.02f;
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
					grgScale = grgScale * 0.5f;
					break;

				case VK_DIVIDE:
					grgScale = grgScale / 0.5f;
					break;

				case VK_NUMPAD2:
					// sky - camera Z coord
					grgViewZ = grgViewZ + 1.1f;
					break;

				case VK_NUMPAD8:
					// sky - camera Z coord
					grgViewZ = grgViewZ - 1.1f;
					break;

				case VK_NUMPAD4:
					// sky - camera X coord
					grgViewX = grgViewX - 1.1f;
					break;

				case VK_NUMPAD6:
					grgViewX = grgViewX + 1.1f;
					break;

				case VK_NUMPAD0:
					// sky - camera X coord
					grgViewY = grgViewY - 1.1f;
					break;

				case VK_NUMPAD5:
					grgViewY = grgViewY + 1.1f;
					break;

				case VK_SPACE :
					grCurrentMoveModel = terrain;
					break;
				
				default : 
					break;
			}
			break;
			
		case WM_CLOSE :
			Uninitialize();
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

void Initialize()
{
	// function declaration
	void Resize(int, int);
	void Uninitialize(void);
	bool LoadGLTexture(GLuint*, TCHAR[]);
	void LoadGrassTexture();
	void initializeGrass();
	
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
	if(griPixelFormatIndex == 0)
	{
		fprintf(grgpFile, "ChoosePixelFormat() failed\n");
		Uninitialize();
	}
	
	if(SetPixelFormat(grghdc, griPixelFormatIndex, &grpfd) == FALSE)
	{
		fprintf(grgpFile, "SetPixelFormat() failed\n");
		Uninitialize();
	}
	
	grghrc = wglCreateContext(grghdc);
	if(grghrc == NULL)
	{
		fprintf(grgpFile, "wglCreateContext() failed\n");
		ReleaseDC(grghwnd, grghdc);
		grghdc = NULL;
	}
	
	if(wglMakeCurrent(grghdc, grghrc) == FALSE)
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
	grgVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	///// Vertex Shader
	const GLchar* grvertexShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"out VS_OUT" \
		"{" \
			"vec2 tc;" \
		"} vs_out;" \
		"void main(void)" \
		"{" \
		"const vec4 vertices[] = vec4[] (vec4(-1.5, -1.5, -0.5, 1.0)," \
									"vec4(1.5, -1.5, -0.5, 1.0)," \
									"vec4(-1.5, -1.5, 0.5, 1.0)," \
									"vec4(1.5, -1.5, 0.5, 1.0));" \
		"int x = gl_InstanceID & 127;" \
		"int y = gl_InstanceID >> 7;" \
		"vec2 offs = vec2(x, y);" \
		"vs_out.tc = (vertices[gl_VertexID].xz + offs + vec2(0.5)) / 128.0;" \
		"gl_Position = vertices[gl_VertexID] + vec4(float(x - 64), 0.0, float(y - 64), 0.0);" \
		"}";
	
	glShaderSource(grgVertexShaderObject, 1, (const GLchar **) &grvertexShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgVertexShaderObject);
	// error check for compilation
	GLint griInfoLength = 0;
	GLint griShaderCompileStatus = 0;
	char* grszInfoLog = NULL;

	glGetShaderiv(grgVertexShaderObject, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgVertexShaderObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgVertexShaderObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n Vertex Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				
			}
		}
		Uninitialize();
	}

	// Tescellation control shader
	// create shader
	grgTesControlShaderObject = glCreateShader(GL_TESS_CONTROL_SHADER);

	///// Tes control Shader
	const GLchar* grtesControlShaderSourceCode =					// also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
		"#version 430 core" \
		"\n" \
		"layout(vertices = 4) out;" \
		"in VS_OUT" \
		"{" \
			"vec2 tc;"
		"} tcs_in[];" \
		"out TCS_OUT" \
		"{" \
			"vec2 tc;" \
		"} tcs_out[];" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"int i = 0;" \
		"void main(void)" \
		"{" \
		"if(gl_InvocationID == 0)" \
		"{" \
			"mat4 vp_matrix = u_projection_matrix * u_view_matrix;" \
			"vec4 p0 = vp_matrix * gl_in[0].gl_Position;" \
			"vec4 p1 = vp_matrix * gl_in[1].gl_Position;" \
			"vec4 p2 = vp_matrix * gl_in[2].gl_Position;" \
			"vec4 p3 = vp_matrix * gl_in[3].gl_Position;" \
			"p0 /= p0.w;" \
			"p1 /= p1.w;" \
			"p2 /= p2.w;" \
			"p3 /= p3.w;" \
			"if(p0.z <= 0.0 || p1.z <= 0.0 || p2.z <= 0.0 || p3.z <= 0.0)" \
			"{" \
				"gl_TessLevelOuter[0] = 0.0;" \
				"gl_TessLevelOuter[1] = 0.0;" \
				"gl_TessLevelOuter[2] = 0.0;" \
				"gl_TessLevelOuter[3] = 0.0;" \
			"}" \
			"else" \
			"{" \
				"float l0 = length(p2.xy - p0.xy) * 16.0 + 1.0;" \
				"float l1 = length(p3.xy - p2.xy) * 16.0 + 1.0;" \
				"float l2 = length(p3.xy - p1.xy) * 16.0 + 1.0;" \
				"float l3 = length(p1.xy - p0.xy) * 16.0 + 1.0;" \
				"gl_TessLevelOuter[0] = l0;" \
				"gl_TessLevelOuter[1] = l1;" \
				"gl_TessLevelOuter[2] = l2;" \
				"gl_TessLevelOuter[3] = l3;" \
				"gl_TessLevelInner[0] = min(l1, l3);" \
				"gl_TessLevelInner[1] = min(l0, l2);" \
			"}" \
		"}" \
		"gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;" \
		"tcs_out[gl_InvocationID].tc = tcs_in[gl_InvocationID].tc;" \
		"}";


	glShaderSource(grgTesControlShaderObject, 1, (const GLchar**)&grtesControlShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgTesControlShaderObject);
	// error check for compilation
	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	glGetShaderiv(grgTesControlShaderObject, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgTesControlShaderObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgTesControlShaderObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n TessilationControl Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				Uninitialize();
			}
		}
	}

	////// Tessilation evaluation shader
	// create shader
	grgTesEvaluationShaderObject = glCreateShader(GL_TESS_EVALUATION_SHADER);

	///// Tes control Shader
	const GLchar* grtesEvaluationShaderSourceCode =					// also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
		"#version 430 core" \
		"\n" \
		"layout(quads, fractional_odd_spacing) in;" \
		"uniform sampler2D tex_displacement;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform float dmap_depth;" \
		"in TCS_OUT" \
		"{" \
		"vec2 tc;" \
		"} tes_in[];" \
		"out TES_OUT" \
		"{" \
			"vec2 tc;" \
		"} tes_out1;" \
		"int i = 0;" \
		"void main(void)" \
		"{" \
		"vec2 tc1 = mix(tes_in[0].tc, tes_in[1].tc, gl_TessCoord.x);" \
		"vec2 tc2 = mix(tes_in[2].tc, tes_in[3].tc, gl_TessCoord.x);" \
		"vec2 tc = mix(tc2, tc1, gl_TessCoord.y);" \
		"vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);" \
		"vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);" \
		"vec4 p = mix(p2, p1, gl_TessCoord.y);" \
		"p.y += texture(tex_displacement, tc).r * dmap_depth;" \
		"gl_Position = u_projection_matrix * u_view_matrix * p;" \
		"tes_out1.tc = tc;" \
		"}";

	glShaderSource(grgTesEvaluationShaderObject, 1, (const GLchar**)&grtesEvaluationShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgTesEvaluationShaderObject);
	// error check for compilation
	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	glGetShaderiv(grgTesEvaluationShaderObject, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgTesEvaluationShaderObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgTesEvaluationShaderObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n TessilationEval Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				Uninitialize();
			}
		}
	}

	/*
	/////// Geometry shader
	grgGeometryShaderObject = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* grgeometryShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"layout(triangles, invocations = 2)in;" \
		"layout(triangle_strip,max_vertices=3)out;" \
		"uniform mat4 u_mvpMatrix;" \
		"in TESS_OUT" \
		"{" \
		"vec2 tc;" \
		"vec3 out_rand_pos[2];" \
		"} gs_in[];" \
		"out GS_OUT" \
		"{" \
		"vec2 tc;" \
		"}gs_out;" \
		"void main(void)" \
		"{" \
		"gs_out.tc = gs_in[gl_InvocationID].tc;" \
		"vec3 xyz_pos = gs_in[gl_InvocationID].out_rand_pos[gl_InvocationID];"
		"gl_Position =  (gl_in[0].gl_Position + vec4(xyz_pos, 0.0));" \
		"EmitVertex();" \
		"gl_Position =  (gl_in[1].gl_Position + vec4(xyz_pos, 0.0));" \
		"EmitVertex();" \
		"gl_Position =  (gl_in[2].gl_Position + vec4(xyz_pos, 0.0));" \
		"EmitVertex();" \
		"EndPrimitive();" \
		"}";

	glShaderSource(grgGeometryShaderObject, 1, (const GLchar**)&grgeometryShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgGeometryShaderObject);
	// error check for compilation
	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	glGetShaderiv(grgGeometryShaderObject, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgGeometryShaderObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgGeometryShaderObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n Geometry Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				Uninitialize();
			}
		}
	}
	*/

	////// Fragment Shader
	grgFragmentShadeerObject = glCreateShader(GL_FRAGMENT_SHADER);

	// source code of shader
	const GLchar* grfragmentShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"out vec4 FragColor;" \
		"uniform sampler2D tex_color;" \
		"uniform sampler2D rtexture;" \
		"uniform sampler2D gtexture;" \
		"uniform sampler2D btexture;" \
		"uniform sampler2D blendMap;" \
		"in TES_OUT" \
		"{" \
			"vec2 tc;" \
		"} fs_in;" \
		"void main(void)" \
		"{" \
		"vec4 blendMapColor = texture(blendMap, fs_in.tc);" \
		"float backTextureAmount = 1 - (blendMapColor.r, blendMapColor.g, blendMapColor.b);" \
		"vec2 tileCoords = fs_in.tc * 5.0;" \
		"vec4 backgroundTextureColor = texture(tex_color, tileCoords) * backTextureAmount;" \
		"vec4 rTextureColor = texture(rtexture, tileCoords) * blendMapColor.r;" \
		"vec4 gTextureColor = texture(gtexture, tileCoords) * blendMapColor.g;" \
		"vec4 bTextureColor = texture(btexture, tileCoords) * blendMapColor.b;" \
		"vec4 totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;" \
		"FragColor = vec4(totalColor);" \
		/*"FragColor = texture(tex_color, fs_in.tc);" \*/
		"}";

	glShaderSource(grgFragmentShadeerObject, 1, (const GLchar **) &grfragmentShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgFragmentShadeerObject);

	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	// error check for compiation
	glGetShaderiv(grgFragmentShadeerObject, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgFragmentShadeerObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgFragmentShadeerObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n Fragment Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				Uninitialize();
			}
		}
	}

	//****** Shader Program *****//
	// create
	grgShaderProgramObject = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(grgShaderProgramObject, grgVertexShaderObject);
	glAttachShader(grgShaderProgramObject, grgTesControlShaderObject);
	glAttachShader(grgShaderProgramObject, grgTesEvaluationShaderObject);
	//glAttachShader(grgShaderProgramObject, grgGeometryShaderObject);
	// attach fragment shader to shader program
	glAttachShader(grgShaderProgramObject, grgFragmentShadeerObject);

	// pre-link our attribute enum with shader's attributes
	glBindAttribLocation(grgShaderProgramObjectTri, GR_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(grgShaderProgramObjectTri, GR_ATTRIBUTE_TEXCOORD, "vTexCoord");
	glBindAttribLocation(grgShaderProgramObjectTri, GR_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(grgShaderProgramObject);
	// error check for linking
	GLint griShaderProgramLinkStatus = 0;
	glGetProgramiv(grgShaderProgramObject, GL_LINK_STATUS, &griShaderProgramLinkStatus);
	if (griShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(grgShaderProgramObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(griInfoLength) * sizeof(char));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetProgramInfoLog(grgShaderProgramObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n Shader Program Link Log : %s", grszInfoLog);
				Uninitialize();
			}
		}
	}

	grmodelMatrixUniformTerrain = glGetUniformLocation(grgShaderProgramObject, "u_model_matrix");
	grviewMatrixUniformTerrain = glGetUniformLocation(grgShaderProgramObject, "u_view_matrix");
	grprojectionMatrixUniformTerrain = glGetUniformLocation(grgShaderProgramObject, "u_projection_matrix");
	grgTextureSamplerUniformTexColor = glGetUniformLocation(grgShaderProgramObject, "tex_color");
	grgTextureSamplerUniformTexDisplacement = glGetUniformLocation(grgShaderProgramObject, "tex_displacement");
	grgDMapDepthUniform = glGetUniformLocation(grgShaderProgramObject, "dmap_depth");
	grgTextureSamplerUniformGrass = glGetUniformLocation(grgShaderProgramObject, "gtexture");
	grgTextureSamplerUniformGrassFlowers = glGetUniformLocation(grgShaderProgramObject, "rtexture");
	grgTextureSamplerUniformPath = glGetUniformLocation(grgShaderProgramObject, "btexture");
	grgTextureSamplerUniformBlendMap = glGetUniformLocation(grgShaderProgramObject, "blendMap");
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

#pragma endregion terrain init

#pragma region sky shaders
	// *************************************** Sky Shaders *********************************************
	/*
	getSphereVertexData(grsphereVertices, grsphereNormals, grsphereTextures, grsphereElements);
	grgNumVertices = getNumberOfSphereVertices();
	grgNumElements = getNumberOfSphereElements();

	// create shader
	grgVertexShaderObjectSky = glCreateShader(GL_VERTEX_SHADER);

	///// Vertex Shader
	const GLchar* grvertexShaderSourceCodeSky =					// also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"in vec3 vTexcoord;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec3 u_light_position;" \
		"uniform float u_scale;" \
		"out vec2 out_position;" \
		"void main(void)" \
		"{" \
		"out_position = vPosition.xy * u_scale;" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";

	glShaderSource(grgVertexShaderObjectSky, 1, (const GLchar**)&grvertexShaderSourceCodeSky, NULL);

	// compile shader
	glCompileShader(grgVertexShaderObjectSky);
	// error check for compilation
	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	glGetShaderiv(grgVertexShaderObjectSky, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgVertexShaderObjectSky, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgVertexShaderObjectSky, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n Vertex Shader Compilation Log for sky : %s", grszInfoLog);
				free(grszInfoLog);
				Uninitialize();
			}
		}
	}

	////// Fragment Shader
	grgFragmentShadeerObjectSky = glCreateShader(GL_FRAGMENT_SHADER);

	// source code of shader
	const GLchar* grfragmentShaderSourceCodeSky =
		"#version 450 core" \
		"\n" \
		"in vec2 out_position;" \
		"uniform vec2 u_resolution;" \
		"uniform vec2 u_mouse;" \
		"uniform float u_time;" \
		"uniform float u_scale;" \
		"out vec4 FragColor;" \
		"int num_octaves = 5;" \
		"float random(vec2 st)" \
		"{" \
		"return(fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123));"
		"}" \
		"float noise(vec2 st)" \
		"{" \
		"vec2 i = floor(st);" \
		"vec2 f = fract(st);" \
		"float a = random(i);" \
		"float b = random(i + vec2(1.0, 0.0));" \
		"float c = random(i + vec2(0.0, 1.0));" \
		"float d = random(i + vec2(1.0, 1.0));" \
		"vec2 u = f * f * (3.0 - 2.0 * f);" \
		"return(mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y);" \
		"}" \
		"float fbm(vec2 st)" \
		"{" \
		"float v = 0.0;" \
		"float a = 0.5;" \
		"vec2 shift = vec2(100.0);" \
		"mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));" \
		"for(int i = 0; i < num_octaves; ++i)" \
		"{" \
		"v += a * noise(st);" \
		"st = rot * st * 2.0 + shift;" \
		"a *= 0.5;" \
		"}" \
		"return(v);" \
		"}" \
		"void main(void)" \
		"{" \
		"vec2 st = out_position;" \
		"vec3 color = vec3(0.0);"
		"vec2 q = vec2(0.0);" \
		"q.x = fbm(st + 0.0 * u_time);" \
		"q.y = fbm(st + vec2(1.0));" \
		"vec2 r = vec2(0.0);" \
		"r.x = fbm(st + 1.0 * q + vec2(1.7, 9.2) + 0.15 * u_time);" \
		"r.y = fbm(st + 1.0 * q + vec2(8.3, 2.8) + 0.126 * u_time);" \
		"float f = fbm(st + r);" \
		//*"color = mix(vec3(0.101961, 0.619608, 0.666667), vec3(0.666667, 0.666667, 0.498039), clamp((f * f) * 4.0, 0.0, 1.0));" \
		"color = mix(vec3(0.0, 0.0, 0.8), vec3(0.01, 0.01, 0.8), clamp((f * f) * 4.0, 0.0, 1.0));" \
		"color = mix(color, vec3(1.0, 1.0, 1.0), clamp(length(q), 0.0, 1.0));" \
		"color = mix(color, vec3(0.8, 0.8, 0.92), clamp(length(r.x), 0.0, 1.0));" \
		"FragColor = vec4(color, 1.0);" \
		"}";

	glShaderSource(grgFragmentShadeerObjectSky, 1, (const GLchar**)&grfragmentShaderSourceCodeSky, NULL);

	// compile shader
	glCompileShader(grgFragmentShadeerObjectSky);
	// error check for compiation
	glGetShaderiv(grgFragmentShadeerObjectSky, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgFragmentShadeerObjectSky, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgFragmentShadeerObjectSky, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n Fragment Shader Compilation Log for sky : %s", grszInfoLog);
				free(grszInfoLog);
				Uninitialize();
			}
		}
	}

	//****** Shader Program *****
	// create
	grgShaderProgramObjectSky = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(grgShaderProgramObjectSky, grgVertexShaderObjectSky);

	// attach fragment shader to shader program
	glAttachShader(grgShaderProgramObjectSky, grgFragmentShadeerObjectSky);

	// pre-link our attribute enum with shader's attributes
	glBindAttribLocation(grgShaderProgramObjectSky, GR_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(grgShaderProgramObjectSky, GR_ATTRIBUTE_TEXCOORD, "vTexcoord");
	glBindAttribLocation(grgShaderProgramObjectSky, GR_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(grgShaderProgramObjectSky);
	// error check for linking
	griShaderProgramLinkStatus = 0;
	glGetProgramiv(grgShaderProgramObjectSky, GL_LINK_STATUS, &griShaderProgramLinkStatus);
	if (griShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(grgShaderProgramObjectSky, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(griInfoLength) * sizeof(char));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetProgramInfoLog(grgShaderProgramObjectSky, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n Shader Program Link Log for sky : %s", grszInfoLog);
				Uninitialize();
			}
		}
	}

	grgModelMatrixUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_model_matrix");
	grgViewMatrixUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_view_matrix");
	grgProjectionMatrixUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_projection_matrix");
	grgLightPositionUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_light_position");
	grgScaleUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_scale");
	grgTextureSamplerNoiseUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_noise");
	grgSkyColorUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_sky_color");
	grgCloudColorUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_cloud_color");
	grgResolutionUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_resolution");
	grgTimeUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_time");
	*/
#pragma endregion sky shaders


#pragma region tri shaders
	// create shader
	grgVertexShaderObjectTri = glCreateShader(GL_VERTEX_SHADER);

	///// Vertex Shader
	const GLchar* grvertexShaderSourceCodeTri =					// also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec2 vTexCoord;" \
		"in vec3 vNormal;" \
		"in vec2 vPos;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec3 u_ka;" \
		"uniform vec3 u_kd;" \
		"uniform vec3 u_ks;" \
		"uniform vec3 u_la;" \
		"uniform vec3 u_ld;" \
		"uniform vec3 u_ls;" \
		"uniform float u_material_shinyness;" \
		"uniform vec4 u_light_position;" \
		"uniform int u_light;" \
		"uniform vec3 u_rand_pos[20];" \
		"out VS_OUT" \
		"{" \
		"vec3 out_phong_light;" \
		"vec2 out_texcoord;" \
		"} vs_out;" \
		"void main(void)" \
		"{" \
		// phong light
		"if(u_light == 1)" \
		"{" \
		"vec4 eye_coordinates =  u_view_matrix * u_model_matrix * vPosition;" \
		"vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
		"vec3 light_direction = normalize(vec3(u_light_position - eye_coordinates));" \
		"vec3 reflection_vector = reflect(-light_direction, transformed_normal);" \
		"vec3 view_vector = normalize(-eye_coordinates.xyz);" \
		"vec3 ambient = u_la * u_ka;" \
		"vec3 diffuse = u_ld * u_kd * max(dot(light_direction, transformed_normal), 0.0);" \
		"vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector, view_vector), 0.0), u_material_shinyness);" \
		"vs_out.out_phong_light = ambient + diffuse + specular;" \
		"}" \
		"else" \
		"{" \
		"vs_out.out_phong_light = vec3(1.0, 0.0f, 0.0f);" \
		"}" \
		// position
		"vec4 rand_pos = vec4(u_rand_pos[gl_InstanceID], 1.0);" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * (vPosition + rand_pos);" \
		"vs_out.out_texcoord = vTexCoord;" \
		"}";

	glShaderSource(grgVertexShaderObjectTri, 1, (const GLchar**)&grvertexShaderSourceCodeTri, NULL);

	// compile shader
	glCompileShader(grgVertexShaderObjectTri);
	// error check for compilation
	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	glGetShaderiv(grgVertexShaderObjectTri, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgVertexShaderObjectTri, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgVertexShaderObjectTri, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n Vertex Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				Uninitialize();
			}
		}
	}

	
	////// Fragment Shader
	grgFragmentShadeerObjectTri = glCreateShader(GL_FRAGMENT_SHADER);

	// source code of shader
	const GLchar* grfragmentShaderSourceCodeTri =
		"#version 450 core" \
		"\n" \
		"out vec4 FragColor;" \
		"uniform sampler2D u_texture_sampler;" \
		"in VS_OUT" \
		"{" \
		"vec3 out_phong_light;" \
		"vec2 out_texcoord;" \
		"} fs_in;" \
		"void main(void)" \
		"{" \
		"vec4 textureColor = texture(u_texture_sampler, fs_in.out_texcoord);" \
		"textureColor = textureColor * vec4(fs_in.out_phong_light, 1.0);" \
		"FragColor = textureColor;" \
		"}";

	glShaderSource(grgFragmentShadeerObjectTri, 1, (const GLchar**)&grfragmentShaderSourceCodeTri, NULL);

	// compile shader
	glCompileShader(grgFragmentShadeerObjectTri);
	// error check for compiation
	glGetShaderiv(grgFragmentShadeerObjectTri, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgFragmentShadeerObjectTri, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgFragmentShadeerObjectTri, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n Fragment Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				Uninitialize();
			}
		}
	}

	//****** Shader Program *****//
	// create
	grgShaderProgramObjectTri = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(grgShaderProgramObjectTri, grgVertexShaderObjectTri);
	glAttachShader(grgShaderProgramObjectTri, grgGeometryShaderObjectTri);
	// attach fragment shader to shader program
	glAttachShader(grgShaderProgramObjectTri, grgFragmentShadeerObjectTri);

	// pre-link our attribute enum with shader's attributes
	glBindAttribLocation(grgShaderProgramObjectTri, GR_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(grgShaderProgramObjectTri, GR_ATTRIBUTE_TEXCOORD, "vTexCoord");
	glBindAttribLocation(grgShaderProgramObjectTri, GR_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(grgShaderProgramObjectTri);
	// error check for linking
	griShaderProgramLinkStatus = 0;
	glGetProgramiv(grgShaderProgramObjectTri, GL_LINK_STATUS, &griShaderProgramLinkStatus);
	if (griShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(grgShaderProgramObjectTri, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(griInfoLength) * sizeof(char));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetProgramInfoLog(grgShaderProgramObjectTri, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFile, "\n Shader Program Link Log : %s", grszInfoLog);
				Uninitialize();
			}
		}
	}

	grgModelMatrixUniformTri = glGetUniformLocation(grgShaderProgramObjectTri, "u_model_matrix");
	grgViewMatrixUniformTri = glGetUniformLocation(grgShaderProgramObjectTri, "u_view_matrix");
	grgProjectionMatrixUniformTri = glGetUniformLocation(grgShaderProgramObjectTri, "u_projection_matrix");
	grgRandPositionUniform[0] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[0]");
	grgRandPositionUniform[1] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[1]");
	grgRandPositionUniform[2] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[2]");
	grgRandPositionUniform[3] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[3]");
	grgRandPositionUniform[4] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[4]");
	grgRandPositionUniform[5] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[5]");
	grgRandPositionUniform[6] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[6]");
	grgRandPositionUniform[7] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[7]");
	grgRandPositionUniform[8] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[8]");
	grgRandPositionUniform[9] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[9]");
	grgRandPositionUniform[10] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[10]");
	grgRandPositionUniform[11] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[11]");
	grgRandPositionUniform[12] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[12]");
	grgRandPositionUniform[13] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[13]");
	grgRandPositionUniform[14] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[14]");
	grgRandPositionUniform[15] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[15]");
	grgRandPositionUniform[16] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[16]");
	grgRandPositionUniform[17] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[17]");
	grgRandPositionUniform[18] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[18]");
	grgRandPositionUniform[19] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[19]");
	grgKaUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_ka");
	grgKdUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_kd");
	grgKsUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_ks");
	grgLaUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_la");
	grgLdUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_ld");
	grgLsUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_ls");
	grgLightPositionUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_light_position");
	grgLightUniform = glGetUniformLocation(grgShaderProgramObjectTri, "u_light");
	//u_texture_sampler
	grgTextureSamplerUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_texture_sampler");

#pragma endregion tri shaders




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
	/*
#pragma region sky vao
	// sphere vao, vbo
	glGenVertexArrays(1, &grgVaoSphere);
	glBindVertexArray(grgVaoSphere);

	glGenBuffers(1, &grgVboSpherePosition);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grsphereVertices), grsphereVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// normal for cube
	glGenBuffers(1, &grgVboSphereNormal);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grsphereNormals), grsphereNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// element vbo
	glGenBuffers(1, &grgVboSphereElement);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphereElements), grsphereElements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
#pragma endregion sky vao
*/

#pragma region road init
	GRInitializeRoad();
#pragma endregion road init

#pragma region grass quad init
	GRInitializeGrassQuad();
#pragma endregion grass quad init
	
	grgTextureTerragen1 = load("terragen1.ktx");
	//grgTextureTerragenColor = load("terragen_color.ktx");
	//LoadGLTexture(&grgTextureTerragen1, MAKEINTRESOURCE(GRTERRAIN_GEN));
	LoadGLTexture(&grgTextureTerragenColor, MAKEINTRESOURCE(GRTERRAIN_COLOR));
	LoadGLTexture(&grgTextureBlendMap, MAKEINTRESOURCE(GRTERRAIN_BLENDMAP));
	LoadGLTexture(&grgTextureGrass, MAKEINTRESOURCE(GRTERRAIN_GRASS));
	LoadGLTexture(&grgTextureDirt, MAKEINTRESOURCE(GRTERRAIN_DIRT));
	LoadGLTexture(&grgTextureGrassFlowers, MAKEINTRESOURCE(GRTERRAIN_GRSSSFLOWERS));
	LoadGLTexture(&grgTexturePath, MAKEINTRESOURCE(GRTERRAIN_PATH));

	grgModelTree = new Model(FileSystem::getPath("model/T/tree.obj"), grgpFile);
	//grgModelHouse = new Model(FileSystem::getPath("model/house/house2/house1.fbx"), grgpFile);
	
	grgRandPos[0] = vec3(7.0, 1.0, -10.0);
	grgRandPos[1] = vec3(20.0, 1.0, -15.0);
	grgRandPos[2] = vec3(15.0, 1.0, -5.0);
	grgRandPos[3] = vec3(20.0, 1.0, -18.0);
	grgRandPos[4] = vec3(20.0, 1.0, -13.0);
	grgRandPos[5] = vec3(12.8, 1.0, -8.0);
	grgRandPos[6] = vec3(12.4, 1.0, -10.0);
	grgRandPos[7] = vec3(3.1, 1.0, -10.0);
	grgRandPos[8] = vec3(18.1, 1.0, -10.0);
	grgRandPos[9] = vec3(21.0, 1.0, -13.0);
	
	grgRandPos[10] = vec3(-7.0, 1.0, -10.0);
	grgRandPos[11] = vec3(-10.0, -2.0, -15.0);
	grgRandPos[12] = vec3(-15.0, -10.0, -5.0);
	grgRandPos[13] = vec3(-20.0, 0.0, -18.0);
	grgRandPos[14] = vec3(-20.0, -1.0, -13.0);
	grgRandPos[15] = vec3(-12.8, 1.0, -8.0);
	grgRandPos[16] = vec3(-12.4, 0.0, -10.0);
	grgRandPos[17] = vec3(-3.1, 0.0, -10.0);
	grgRandPos[18] = vec3(-18.1, -1.0, -10.0);
	grgRandPos[19] = vec3(-21.0, -1.0, -13.0);
	
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
	grgPerspectiveProjectionMatrix = mat4::identity();
	
	// warm-up call to resize
	Resize(WIN_WIDTH, WIN_HEIGHT);
}


void initializeGrass()
{

	GLuint gVertexShaderObject;
	GLuint gFragmentShaderObject;
	GLuint gGeometryShaderObject;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	LoadAllTextures();
	//////////////////////////////// V E R T E X - S H A D E R //////////////////////////


	//Define Vertex Shader Object
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER); //This command will create the Shader Object
	//Now Write vertex shader code
	const GLchar** p;
	const GLchar* vertexShaderSourceCode =

		"#version 430 core" \
		"\n"
		"layout(location = 0) in vec3 inPosition;" \
		"void main(void)" \
		"{" \
		"gl_Position = vec4(inPosition,1.0);" \

		"}";
	// GPU will run the above code. And GPU WILL RUN FOR PER VERTEX. If there are 1000 vertex. Then GPU will run this shader for
	//1000 times. We are Multiplying each vertex with the Model View Matrix.
	//And how does the GPU gets to know about at what offset the array has to be taken . Go to glVertexAttribPointer() in Display.
	// in = Input. 

	//p = &vertexShaderSourceCode;
		//Specify above source code to the vertex shader object
	glShaderSource(gVertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	//Compile the vertex shader 
	glCompileShader(gVertexShaderObject);

	//////////////// Error Checking//////////////////
	//Code for catching the errors 
	GLint iShaderCompileStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar* szInfoLog = NULL;


	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
	if (iShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(grgpFile, "%s\n", szInfoLog);
				free(szInfoLog);
				exit(0);


			}
		}
	}


	////////// GEOMETRY SHADER ////////////////
	gGeometryShaderObject = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* geometryShaderSourceCode =         //Source code of Vertex shader
		"#version 430 core" \
		"\n" \
		"layout(points) in;" \
		"layout(triangle_strip) out;" \
		"layout(max_vertices = 12) out;" \

		"uniform struct Matrices" \
		"{" \
		"mat4 projMatrix;" \
		"mat4 modelMatrix;" \
		"mat4 viewMatrix;" \
		"mat4 normalMatrix;" \
		"} matrices;" \

		"smooth out vec2 vTexCoord;" \
		"smooth out vec3 vWorldPos;" \
		"smooth out vec4 vEyeSpacePos;" \

		"uniform float fTimePassed;" \

		"mat4 rotationMatrix(vec3 axis, float angle)" \
		"{" \
		"axis = normalize(axis);" \
		"float s = sin(angle);" \
		"float c = cos(angle);" \
		"float oc = 1.0 - c;" \

		"return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0," \
		"oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0," \
		"oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0," \
		"0.0, 0.0, 0.0, 1.0);" \
		"}" \

		"vec3 vLocalSeed;" \


		"float randZeroOne()" \
		"{" \
		"uint n = floatBitsToUint(vLocalSeed.y * 214013.0 + vLocalSeed.x * 2531011.0 + vLocalSeed.z * 141251.0);" \
		"n = n * (n * n * 15731u + 789221u);" \
		"n = (n >> 9u) | 0x3F800000u;" \

		"	float fRes = 2.0 - uintBitsToFloat(n);" \
		"vLocalSeed = vec3(vLocalSeed.x + 147158.0 * fRes, vLocalSeed.y*fRes + 415161.0 * fRes, vLocalSeed.z + 324154.0*fRes);" \
		"return fRes;" \
		"}" \

		"int randomInt(int min, int max)" \
		"{" \
		"float fRandomFloat = randZeroOne();" \
		"return int(float(min) + fRandomFloat * float(max - min));" \
		"}" \

		"void main()" \
		"{" \
		"mat4 mMV = matrices.viewMatrix*matrices.modelMatrix;" \
		"mat4 mMVP = matrices.projMatrix*matrices.viewMatrix*matrices.modelMatrix;" \

		"vec3 vGrassFieldPos = gl_in[0].gl_Position.xyz;" \

		"	float PIover180 = 3.1415 / 180.0;" \
		"vec3 vBaseDir[] =" \
		"{" \
		"vec3(1.0, 0.0, 0.0)," \
		"vec3(float(cos(45.0*PIover180)), 0.0f, float(sin(45.0*PIover180)))," \
		"vec3(float(cos(-45.0*PIover180)), 0.0f, float(sin(-45.0*PIover180)))" \
		"};" \

		"float fGrassPatchSize = 3.0;" \
		"float fWindStrength = 8.0;" \

		"vec3 vWindDirection = vec3(1.0, 0.0, 1.0);" \
		"vWindDirection = normalize(vWindDirection);" \

		"for (int i = 0; i < 3; i++)" \
		"{" \


		"vec3 vBaseDirRotated = (rotationMatrix(vec3(0, 1, 0), sin(fTimePassed*0.7f)*0.1f)*vec4(vBaseDir[i], 1.0)).xyz;" \

		"vLocalSeed = vGrassFieldPos * float(i);" \
		"int iGrassPatch = randomInt(0, 3);" \

		"float fGrassPatchHeight = 3.5 + randZeroOne()*2.0;" \

		"float fTCStartX = float(iGrassPatch)*0.25f;" \
		"float fTCEndX = fTCStartX + 0.25f;" \

		"float fWindPower = 0.5f + sin(vGrassFieldPos.x / 30 + vGrassFieldPos.z / 30 + fTimePassed * (1.2f + fWindStrength / 20.0f));" \
		"if (fWindPower < 0.0f)" \
		"fWindPower = fWindPower * 0.2f;" \
		"else fWindPower = fWindPower * 0.3f;" \

		"fWindPower *= fWindStrength;" \

		"vec3 vTL = vGrassFieldPos - vBaseDirRotated * fGrassPatchSize*0.5f + vWindDirection * fWindPower;" \
		"vTL.y += fGrassPatchHeight;" \
		"gl_Position = mMVP * vec4(vTL, 1.0);" \
		"vTexCoord = vec2(fTCStartX, 1.0);" \
		"vWorldPos = vTL;" \
		"vEyeSpacePos = mMV * vec4(vTL, 1.0);" \
		"EmitVertex();" \


		"vec3 vBL = vGrassFieldPos - vBaseDir[i] * fGrassPatchSize*0.5f;" \
		"gl_Position = mMVP * vec4(vBL, 1.0);" \
		"vTexCoord = vec2(fTCStartX, 0.0);" \
		"vWorldPos = vBL;" \
		"vEyeSpacePos = mMV * vec4(vBL, 1.0);" \
		"EmitVertex();" \


		"vec3 vTR = vGrassFieldPos + vBaseDirRotated * fGrassPatchSize*0.5f + vWindDirection * fWindPower;" \
		"vTR.y += fGrassPatchHeight;" \
		"gl_Position = mMVP * vec4(vTR, 1.0);" \
		"vTexCoord = vec2(fTCEndX, 1.0);" \
		"vWorldPos = vTR;" \
		"vEyeSpacePos = mMV * vec4(vTR, 1.0);" \
		"EmitVertex();" \


		"vec3 vBR = vGrassFieldPos + vBaseDir[i] * fGrassPatchSize*0.5f;" \
		"gl_Position = mMVP * vec4(vBR, 1.0);" \
		"vTexCoord = vec2(fTCEndX, 0.0);" \
		"vWorldPos = vBR;" \
		"vEyeSpacePos = mMV * vec4(vBR, 1.0);" \
		"EmitVertex();" \

		"EndPrimitive();" \
		"}" \

		"}";

	glShaderSource(gGeometryShaderObject, 1, (const GLchar**)&geometryShaderSourceCode, NULL); //NULL is for NULL terminated source code string

	//compile geometry shader
	glCompileShader(gGeometryShaderObject);
	//Shader compilation error checking goes here...
	iInfoLogLength = 0;
	iShaderCompileStatus = 0;
	szInfoLog = NULL;
	glGetShaderiv(gGeometryShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
	if (iShaderCompileStatus == GL_FALSE) {
		glGetShaderiv(gGeometryShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0) {
			szInfoLog = (char*)malloc(iInfoLogLength);
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(gGeometryShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(grgpFile, "GS%s\n", szInfoLog);
				free(szInfoLog);
				Uninitialize();
				exit(0);
			}
		}
	}
	/////////////////    F R A G M E N T S H A D E R            //////////////////////////
	//Define Vertex Shader Object
	//vec4(0.2, 1.0, 0.6, 1.0); 
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER); //This command will create the Shader Object
	//Now Write vertex shader code
	const GLchar* fragmentShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"	smooth in vec2 vTexCoord;" \


		"	smooth in vec3 vWorldPos;" \
		"smooth in vec4 vEyeSpacePos;" \

		"out vec4 outputColor;" \

		"uniform sampler2D gSampler;" \
		"uniform vec4 vColor = vec4(0.3, 1.0, 0.85,1.0);" \

		"uniform vec3 vEyePosition = vec3(0.0,9.0,85.0);" \



		"uniform float fAlphaTest = 0.05;" \
		"uniform float fAlphaMultiplier = 0.1;" \


		"void main(void)" \
		"{" \

		"vec4 vTexColor = texture2D(gSampler, vTexCoord);" \


		"float fNewAlpha = vTexColor.a*fAlphaMultiplier;" \
		"if (fNewAlpha < fAlphaTest)" \
		"discard;" \

		"if (fNewAlpha > 1.0f)" \
		"fNewAlpha = 0.5f;" \

		"vec4 vMixedColor = vTexColor * vColor;" \


		"outputColor = vec4(vMixedColor.zyx, fNewAlpha);" \

		"if(outputColor.r > 0.3)" \
		"discard;" \


		"}";

	//FragColor = vec4(1,1,1,1) = White Color
	//this means here we are giving color to the Triangle.




	//Specify above source code to the vertex shader object
	glShaderSource(gFragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	//Compile the vertex shader 
	glCompileShader(gFragmentShaderObject);
	//Code for catching the errors 
		   /*iShaderCompileStatus = 0;
		   iInfoLogLength = 0;*/
	szInfoLog = NULL;


	glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
	if (iShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written1;
				glGetShaderInfoLog(gFragmentShaderObject, iInfoLogLength, &written1, szInfoLog);
				fprintf(grgpFile, "%s\n", szInfoLog);
				free(szInfoLog);
				Uninitialize();
				exit(0);


			}
		}
	}
	// CREATE SHADER PROGRAM OBJECT
	G_gShaderProgramObject = glCreateProgram();
	//attach vertex shader to the gShaderProgramObject
	glAttachShader(G_gShaderProgramObject, gVertexShaderObject);
	glAttachShader(G_gShaderProgramObject, gGeometryShaderObject);
	//attach fragment shader to the gShaderProgramObject
	glAttachShader(G_gShaderProgramObject, gFragmentShaderObject);


	//Pre-Linking  binding to vertexAttributes
	//glBindAttribLocation(gShaderProgramObject, AMC_ATTRIBUTE_POSITION, "vPosition");
	//Here the above line means that we are linking the GPU's variable vPosition with the CPU's  enum member  i.e AMC_ATTRIBUTE_POSITION .
	//So whatever changes will be done in AMC_ATTRIBUTE_POSITION , those will also reflect in vPosition

	//RULE : ALWAYS BIND THE ATTRIBUTES BEFORE LINKING AND BIND THE UNIFORM AFTER LINKING.

	//Link the shader program 
	glLinkProgram(G_gShaderProgramObject);

	//Code for catching the errors 
	GLint iProgramLinkStatus = 0;



	glGetProgramiv(G_gShaderProgramObject, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(G_gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written3;
				glGetProgramInfoLog(G_gShaderProgramObject, iInfoLogLength, &written3, szInfoLog);
				fprintf(grgpFile, "%s\n", szInfoLog);
				free(szInfoLog);
				Uninitialize();
				exit(0);


			}
		}
	}


	//POST Linking
	//Retrieving uniform locations 
	G_modelUniform = glGetUniformLocation(G_gShaderProgramObject, "matrices.modelMatrix");
	G_viewUniform = glGetUniformLocation(G_gShaderProgramObject, "matrices.viewMatrix");
	G_vEyeUniform = glGetUniformLocation(G_gShaderProgramObject, "vEyePosition");
	G_projectionUniform = glGetUniformLocation(G_gShaderProgramObject, "matrices.projMatrix");
	G_NormalMatrixUniform = glGetUniformLocation(G_gShaderProgramObject, "matrices.normalMatrix");
	G_vColorUniform = glGetUniformLocation(G_gShaderProgramObject, "vColor");
	fTimePassedUniform = glGetUniformLocation(G_gShaderProgramObject, "fTimePassed");
	G_fAlphaTest = glGetUniformLocation(G_gShaderProgramObject, "fAlphaTest");
	G_fAlphaMultiplier = glGetUniformLocation(G_gShaderProgramObject, "fAlphaMultiplier");
	G_gSampler = glGetUniformLocation(G_gShaderProgramObject, "gSampler");

	//Here we have done all the preparations of data transfer from CPU to GPU

	const GLfloat triangleVertices[] =
	{
		0.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f
	};
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);

	glActiveTexture(GL_TEXTURE5);
	LoadHeightMapFromImageFree("heightmap.bmp");
}



BOOL loadTexture(GLuint* texture_Smiley, TCHAR imageResourceID[])
{
	HBITMAP hBitmap = NULL;
	BITMAP bmp;
	BOOL bStatus = FALSE;

	//Win32API
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap != NULL)
	{
		bStatus = TRUE;
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		//Now OpenGL part for texture starts from here
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, texture_Smiley);
		glBindTexture(GL_TEXTURE_2D, *texture_Smiley);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


		//gluBuild2DMipMaps = glTexImage2D + glGenerateMipMap
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		//2ndParam = MipMap Level. By passing zero we say that,I am not giving any level. You(OPENGL) decide by yourself.
		//Valid Numbers to be expected over there are - 2,4,6

		glGenerateMipmap(GL_TEXTURE_2D);

		DeleteObject(hBitmap);//As hBitmap posseses Global memory and it is always better to free it after the work has been done.
		//glBindTexture(GL_TEXTURE_2D, 0);
	}
	return bStatus;


}
bool LoadHeightMapFromImageFree(string sImagePath)
{
	vRenderScale = glm::vec3(255.0f, 25.0f, 300.0f);

	if (bLoaded)
	{
		bLoaded = false;
		//	ReleaseHeightmap();
	}
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(sImagePath.c_str(), 0); // Check the file signature and deduce its format

	if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(sImagePath.c_str());

	if (fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, sImagePath.c_str());
	if (!dib)
		return false;

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data
	iRows = FreeImage_GetHeight(dib);
	iCols = FreeImage_GetWidth(dib);

	fprintf_s(grgpFile, "iRows : %d\n", iRows);
	fprintf_s(grgpFile, "iCols : %d\n", iCols);
	// We also require our image to be either 24-bit (classic RGB) or 8-bit (luminance)
	if (bDataPointer == NULL || iRows == 0 || iCols == 0 || (FreeImage_GetBPP(dib) != 24 && FreeImage_GetBPP(dib) != 8))
		return false;

	// How much to increase data pointer to get to next pixel data
	unsigned int ptr_inc = FreeImage_GetBPP(dib) == 24 ? 3 : 1;
	// Length of one row in data
	unsigned int row_step = ptr_inc * iCols;

	//vboHeightmapData.CreateVBO();
	// All vertex data are here (there are iRows*iCols vertices in this heightmap), we will get to normals later
	vVertexData = vector< vector< glm::vec3> >(iRows, vector<glm::vec3>(iCols));
	vector< vector< glm::vec2> > vCoordsData(iRows, vector<glm::vec2>(iCols));

	float fTextureU = float(iCols) * 0.1f;
	float fTextureV = float(iRows) * 0.1f;
	int m = 0;
	FOR(i, iRows)
	{
		FOR(j, iCols)
		{
			float fScaleC = float(j) / float(iCols - 1);
			float fScaleR = float(i) / float(iRows - 1);
			float fVertexHeight = float(*(bDataPointer + row_step * i + j * ptr_inc)) / 255.0f;
			vVertexData[i][j] = glm::vec3(-0.5f + fScaleC, fVertexHeight, -0.5f + fScaleR);
			vCoordsData[i][j] = glm::vec2(fTextureU * fScaleC, fTextureV * fScaleR);
			//fprintf_s(grgpFile, "vVertexData : %f \t %f\n", vVertexData[i][j]);
		//	fprintf(grgpFile, "I am in loop %d", m);
			//m++;
		}
	}

	// Normals are here - the heightmap contains ( (iRows-1)*(iCols-1) quads, each one containing 2 triangles, therefore array of we have 3D array)
	vector< vector<glm::vec3> > vNormals[2];
	FOR(i, 2)vNormals[i] = vector< vector<glm::vec3> >(iRows - 1, vector<glm::vec3>(iCols - 1));

	FOR(i, iRows - 1)
	{
		FOR(j, iCols - 1)
		{
			glm::vec3 vTriangle0[] =
			{
				vVertexData[i][j],
				vVertexData[i + 1][j],
				vVertexData[i + 1][j + 1]
			};
			glm::vec3 vTriangle1[] =
			{
				vVertexData[i + 1][j + 1],
				vVertexData[i][j + 1],
				vVertexData[i][j]
			};

			glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0] - vTriangle0[1], vTriangle0[1] - vTriangle0[2]);
			glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0] - vTriangle1[1], vTriangle1[1] - vTriangle1[2]);

			vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
			vNormals[1][i][j] = glm::normalize(vTriangleNorm1);

		}
	}

	vector< vector<glm::vec3> > vFinalNormals = vector< vector<glm::vec3> >(iRows, vector<glm::vec3>(iCols));

	FOR(i, iRows)
		FOR(j, iCols)
	{
		// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
		// of all adjacent triangles' normals

		glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

		// Look for upper-left triangles
		if (j != 0 && i != 0)
			FOR(k, 2)vFinalNormal += vNormals[k][i - 1][j - 1];
		// Look for upper-right triangles
		if (i != 0 && j != iCols - 1)vFinalNormal += vNormals[0][i - 1][j];
		// Look for bottom-right triangles
		if (i != iRows - 1 && j != iCols - 1)
			FOR(k, 2)vFinalNormal += vNormals[k][i][j];
		// Look for bottom-left triangles
		if (i != iRows - 1 && j != 0)
			vFinalNormal += vNormals[1][i][j - 1];
		vFinalNormal = glm::normalize(vFinalNormal);

		vFinalNormals[i][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row

	}




	float fGrassPatchOffsetMin = 1.5f;
	float fGrassPatchOffsetMax = 2.5f;
	float fGrassPatchHeight = 5.0f;

	glm::vec3 vCurPatchPos(-vRenderScale.x * 0.5f + fGrassPatchOffsetMin, 0.0f, vRenderScale.z * 0.5f - fGrassPatchOffsetMin);
	iNumGrassTriangles = 0;

	while (vCurPatchPos.x < vRenderScale.x * 0.5f)
	{
		vCurPatchPos.z = vRenderScale.z * 0.5f - fGrassPatchOffsetMin;

		while (vCurPatchPos.z > -vRenderScale.z * 0.5f)
		{
			vCurPatchPos.y = GetHeightFromRealVector(vCurPatchPos) - 0.3f;
			AddData(&vCurPatchPos, sizeof(glm::vec3));

			iNumGrassTriangles += 1;

			vCurPatchPos.z -= fGrassPatchOffsetMin + (fGrassPatchOffsetMax - fGrassPatchOffsetMin) * float(rand() % 1000) * 0.001f;
		}

		vCurPatchPos.x += fGrassPatchOffsetMin + (fGrassPatchOffsetMax - fGrassPatchOffsetMin) * float(rand() % 1000) * 0.001f;

	}
	glGenVertexArrays(1, &uiGrassVAO);
	glBindVertexArray(uiGrassVAO);
	glGenBuffers(1, &uiBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uiBuffer);
	glBufferData(GL_ARRAY_BUFFER, data1.size(), &data1[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	bLoaded = true; // If get here, we succeeded with generating heightmap
	return true;
}
void AddData(void* ptrData, UINT uiDataSize)
{
	data1.insert(data1.end(), (BYTE*)ptrData, (BYTE*)ptrData + uiDataSize);
	iCurrentSize += uiDataSize;
}
void LoadAllTextures()
{
	std::string sTextureNames[] = { "grass.dds" };
	glActiveTexture(GL_TEXTURE6);
	LoadTexture2D("data\\textures\\grassPack6.dds", true);
	SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_TRILINEAR);
	SetWrap();
	SetAnisotropyLevel(4.0f);
}

void SetFiltering(int a_tfMagnification, int a_tfMinification)
{
	glBindSampler(0, uiSampler);

	// Set magnification filter
	if (a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if (a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if (a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	tfMinification = a_tfMinification;
	tfMagnification = a_tfMagnification;
}
void SetWrap()
{
	glBindSampler(0, uiSampler);
	glSamplerParameteri(uiSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(uiSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void SetAnisotropyLevel(float fAnisotropyLevel)
{
	glSamplerParameterf(uiSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, fAnisotropyLevel);
}
void CreateFromData(BYTE* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps)
{
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	if (format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	// We must handle this because of internal format parameter
	else if (format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	if (bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	glGenSamplers(1, &uiSampler);

	sPath = "";
	bMipMapsGenerated = bGenerateMipMaps;
	iWidth = a_iWidth;
	iHeight = a_iHeight;
	iBPP = a_iBPP;
}

float GetHeightFromRealVector(glm::vec3 vRealPosition)
{
	int iColumn = int((vRealPosition.x + vRenderScale.x * 0.5f) * float(iCols) / (vRenderScale.x));
	int iRow = int((vRealPosition.z + vRenderScale.z * 0.5f) * float(iRows) / (vRenderScale.z));

	iColumn = glm::min(iColumn, iCols - 1);
	iRow = glm::min(iRow, iRows - 1);

	iColumn = glm::max(iColumn, 0);
	iRow = glm::max(iRow, 0);

	return vVertexData[iRow][iColumn].y * vRenderScale.y;
}

bool LoadGLTexture(GLuint* texture, TCHAR resourceID[])
{
	// variable declarations
	bool bResult = false;
	HBITMAP hBitmap = NULL;
	BITMAP bmp;

	//code
	// OS dependent code starts from here
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), resourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);		// cx and cy  is 0,0 for bitmap img, for icon, give width and height
	if (hBitmap)
	{
		bResult = true;
		GetObject(hBitmap, sizeof(bmp), &bmp);

		// from here starts OpenGL actual code
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		// setting of texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// MAG - Magnification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);				// MIN - Minification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
		glGenerateMipmap(GL_TEXTURE_2D);

		DeleteObject(hBitmap);

	}

	return(bResult);

}

void Resize(int width, int height)
{
	if(height == 0)
		height = 1;
	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	grgPerspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 1000.0f);

#pragma region road resize
	GRResizeRoad(width, height);
#pragma endregion road resize

#pragma region grass quad resize
	GRResizeGrassQuad(width, height);
#pragma endregion grass quad resize
	
}

void Display(void)
{
	// function declaration
	void Update();
	void DrawMeshTree(int);
	void DrawMeshHouse(int);
	void GRDisplayGrass();
	// code
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

	// OpenGL drawing code will start here
	
	mat4 grmodelMatrix = mat4::identity();
	mat4 grviewMatrix = mat4::identity();
	mat4 grprojectionMatrix = mat4::identity();
	mat4 grmodelViewProjectionMatrix = mat4::identity();
	mat4 grtranslateMatrix = mat4::identity();
	mat4 grrotateMatrix = mat4::identity();
	
#pragma region tri display
	glUseProgram(grgShaderProgramObjectTri);

	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -20.0f);
	grmodelMatrix = grtranslateMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrix;
	grviewMatrix = vmath::lookat(vec3(x2, y2, z2), vec3(x2, y2, -6.0 + z2), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(grgModelMatrixUniformTri, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grgViewMatrixUniformTri, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grgProjectionMatrixUniformTri, 1, GL_FALSE, grprojectionMatrix);

	//for (int j = 0; j < 10; j++)
	//{
		//glUniform3fv(grgRandPositionUniform[j], 1, grgRandPos[j]);
	//}
	glUniform3fv(grgRandPositionUniform[0], 1, grgRandPos[0]);
	glUniform3fv(grgRandPositionUniform[1], 1, grgRandPos[1]);
	glUniform3fv(grgRandPositionUniform[2], 1, grgRandPos[2]);
	glUniform3fv(grgRandPositionUniform[3], 1, grgRandPos[3]);
	glUniform3fv(grgRandPositionUniform[4], 1, grgRandPos[4]);
	glUniform3fv(grgRandPositionUniform[5], 1, grgRandPos[5]);
	glUniform3fv(grgRandPositionUniform[6], 1, grgRandPos[6]);
	glUniform3fv(grgRandPositionUniform[7], 1, grgRandPos[7]);
	glUniform3fv(grgRandPositionUniform[8], 1, grgRandPos[8]);
	glUniform3fv(grgRandPositionUniform[9], 1, grgRandPos[9]);
	glUniform3fv(grgRandPositionUniform[10], 1, grgRandPos[10]);
	glUniform3fv(grgRandPositionUniform[11], 1, grgRandPos[11]);
	glUniform3fv(grgRandPositionUniform[12], 1, grgRandPos[12]);
	glUniform3fv(grgRandPositionUniform[13], 1, grgRandPos[13]);
	glUniform3fv(grgRandPositionUniform[14], 1, grgRandPos[14]);
	glUniform3fv(grgRandPositionUniform[15], 1, grgRandPos[15]);
	glUniform3fv(grgRandPositionUniform[16], 1, grgRandPos[16]);
	glUniform3fv(grgRandPositionUniform[17], 1, grgRandPos[17]);
	glUniform3fv(grgRandPositionUniform[18], 1, grgRandPos[18]);
	glUniform3fv(grgRandPositionUniform[19], 1, grgRandPos[19]);

	for (unsigned int i = 0; i < grgModelTree->meshes.size(); i++)
	{
		DrawMeshTree(i);
	}

	// house
	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();

	grtranslateMatrix = vmath::translate(-10.0f, 0.0f, -20.0f);
	grmodelMatrix = grtranslateMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrix;
	grviewMatrix = vmath::lookat(vec3(x2, y2, z2), vec3(x2, y2, -6.0 + z2), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(grgModelMatrixUniformTri, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grgViewMatrixUniformTri, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grgProjectionMatrixUniformTri, 1, GL_FALSE, grprojectionMatrix);
	glUniform3fv(grgRandPositionUniform[0], 1, grgRandPos[0]);
	for (unsigned int i = 0; i < grgModelTree->meshes.size(); i++)
	{
		//DrawMeshHouse(i);
	}


	/*
	// 2nd tres display
	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();
	grtranslateMatrix = vmath::translate(x2 - 20, y2, -10.0f + z2);
	grmodelMatrix = grtranslateMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrix;

	glUniformMatrix4fv(grgModelMatrixUniformTri, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grgViewMatrixUniformTri, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grgProjectionMatrixUniformTri, 1, GL_FALSE, grprojectionMatrix);

	//for (int j = 0; j < 10; j++)
	//{
		//glUniform3fv(grgRandPositionUniform[j], 1, grgRandPos[j]);
	//}
	glUniform3fv(grgRandPositionUniform[0], 1, grgRandPos[0]);
	glUniform3fv(grgRandPositionUniform[1], 1, grgRandPos[1]);
	glUniform3fv(grgRandPositionUniform[2], 1, grgRandPos[2]);
	glUniform3fv(grgRandPositionUniform[3], 1, grgRandPos[3]);
	glUniform3fv(grgRandPositionUniform[4], 1, grgRandPos[4]);
	glUniform3fv(grgRandPositionUniform[5], 1, grgRandPos[5]);
	glUniform3fv(grgRandPositionUniform[6], 1, grgRandPos[6]);
	glUniform3fv(grgRandPositionUniform[7], 1, grgRandPos[7]);
	glUniform3fv(grgRandPositionUniform[8], 1, grgRandPos[8]);
	glUniform3fv(grgRandPositionUniform[9], 1, grgRandPos[9]);
	glUniform3fv(grgRandPositionUniform[10], 1, grgRandPos[10]);
	glUniform3fv(grgRandPositionUniform[11], 1, grgRandPos[11]);
	glUniform3fv(grgRandPositionUniform[12], 1, grgRandPos[12]);
	glUniform3fv(grgRandPositionUniform[13], 1, grgRandPos[13]);
	glUniform3fv(grgRandPositionUniform[14], 1, grgRandPos[14]);
	glUniform3fv(grgRandPositionUniform[15], 1, grgRandPos[15]);
	glUniform3fv(grgRandPositionUniform[16], 1, grgRandPos[16]);
	glUniform3fv(grgRandPositionUniform[17], 1, grgRandPos[17]);
	glUniform3fv(grgRandPositionUniform[18], 1, grgRandPos[18]);
	glUniform3fv(grgRandPositionUniform[19], 1, grgRandPos[19]);

	for (unsigned int i = 0; i < grgModelTree->meshes.size(); i++)
	{
		DrawMeshTree(i);
	}

	// 3rd
	// 2nd tres display
	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();
	grtranslateMatrix = vmath::translate(x2 + 20, y2, 30.0f + z2);
	grmodelMatrix = grtranslateMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrix;

	glUniformMatrix4fv(grgModelMatrixUniformTri, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grgViewMatrixUniformTri, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grgProjectionMatrixUniformTri, 1, GL_FALSE, grprojectionMatrix);

	//for (int j = 0; j < 10; j++)
	//{
		//glUniform3fv(grgRandPositionUniform[j], 1, grgRandPos[j]);
	//}
	glUniform3fv(grgRandPositionUniform[0], 1, grgRandPos[0]);
	glUniform3fv(grgRandPositionUniform[1], 1, grgRandPos[1]);
	glUniform3fv(grgRandPositionUniform[2], 1, grgRandPos[2]);
	glUniform3fv(grgRandPositionUniform[3], 1, grgRandPos[3]);
	glUniform3fv(grgRandPositionUniform[4], 1, grgRandPos[4]);
	glUniform3fv(grgRandPositionUniform[5], 1, grgRandPos[5]);
	glUniform3fv(grgRandPositionUniform[6], 1, grgRandPos[6]);
	glUniform3fv(grgRandPositionUniform[7], 1, grgRandPos[7]);
	glUniform3fv(grgRandPositionUniform[8], 1, grgRandPos[8]);
	glUniform3fv(grgRandPositionUniform[9], 1, grgRandPos[9]);
	glUniform3fv(grgRandPositionUniform[10], 1, grgRandPos[10]);
	glUniform3fv(grgRandPositionUniform[11], 1, grgRandPos[11]);
	glUniform3fv(grgRandPositionUniform[12], 1, grgRandPos[12]);
	glUniform3fv(grgRandPositionUniform[13], 1, grgRandPos[13]);
	glUniform3fv(grgRandPositionUniform[14], 1, grgRandPos[14]);
	glUniform3fv(grgRandPositionUniform[15], 1, grgRandPos[15]);
	glUniform3fv(grgRandPositionUniform[16], 1, grgRandPos[16]);
	glUniform3fv(grgRandPositionUniform[17], 1, grgRandPos[17]);
	glUniform3fv(grgRandPositionUniform[18], 1, grgRandPos[18]);
	glUniform3fv(grgRandPositionUniform[19], 1, grgRandPos[19]);

	for (unsigned int i = 0; i < grgModelTree->meshes.size(); i++)
	{
		DrawMeshTree(i);
	}
	*/

	glUseProgram(0);
#pragma endregion tri display

#pragma region terrain display
	/*
	glUseProgram(grgShaderProgramObject);
	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();

	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -20.0f);
	//grrotateMatrix = vmath::rotate(180.0f, 0.0f, 0.0f, 1.0f);
	grmodelMatrix = grtranslateMatrix * grrotateMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrix;
	grviewMatrix = vmath::lookat(vec3(x2, y2, z2), vec3(x2, y2, -6.0 + z2), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(grmodelMatrixUniformTerrain, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grviewMatrixUniformTerrain, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grprojectionMatrixUniformTerrain, 1, GL_FALSE, grprojectionMatrix);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grgTextureTerragen1);
	glUniform1i(grgTextureSamplerUniformTexDisplacement, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grgTextureTerragenColor);
	glUniform1i(grgTextureSamplerUniformTexColor, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, grgTextureGrass);
	glUniform1i(grgTextureSamplerUniformGrass, 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, grgTextureGrassFlowers);
	glUniform1i(grgTextureSamplerUniformGrassFlowers, 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, grgTextureBlendMap);
	glUniform1i(grgTextureSamplerUniformBlendMap, 4);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, grgTexturePath);
	glUniform1i(grgTextureSamplerUniformPath, 5);



	glUniform1f(grgDMapDepthUniform, grgDepth);

	// bind vao
	//glBindVertexArray(grgVao);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 128 * 128);
	glBindTexture(GL_TEXTURE_2D, NULL);
	// unbind vao
	//glBindVertexArray(0);

	glUseProgram(0);
	*/
#pragma endregion terrain display


#pragma region sky display
	/*
	// **************************************** sky *****************************
	glUseProgram(grgShaderProgramObjectSky);
	vec3 greye = vec3(grgViewX, grgViewY, grgViewZ);
	vec3 grcenter = vec3(1.0f, 0.0f, 0.0f);
	vec3 grup = vec3(0.0f, 1.0f, 0.0f);
	vec3 grcloudColor = vec3(0.992f, 1.0f, 0.9608f);
	vec3 grskyColor = vec3(0.851f, 0.3765f, 0.2314f);
	mat4 grscaleMatrix = mat4::identity();
	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();
	grprojectionMatrix = mat4::identity();


	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -35.0f);
	//grrotateMatrix = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
	grviewMatrix = vmath::lookat(greye, grcenter, grup);
	grscaleMatrix = vmath::scale(200.0f, 200.0f, 200.0f);
	grmodelMatrix = grtranslateMatrix * grscaleMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrix;

	glUniform1f(grgTimeUniform, grgTime);
	glUniform1f(grgScaleUniform, grgScale);

	glUniformMatrix4fv(grgModelMatrixUniform, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grgViewMatrixUniform, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grgProjectionMatrixUniform, 1, GL_FALSE, grprojectionMatrix);


	// bind vao
	glBindVertexArray(grgVaoSphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement);
	glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	// stop use of shader program
	glUseProgram(0);
	*/
#pragma endregion sky display

#pragma region road display
	//GRSetViewMatrixRoad(vec3(x2, y2, z2), vec3(x2, y2, -6.0 + z2), vec3(0.0f, 1.0f, 0.0f));
	//GRDisplayRoad();
#pragma endregion road display

#pragma region grass quad display
	//GRSetViewMatrixGrassQuad(vec3(x2, y2, z2), vec3(x2, y2, -6.0 + z2), vec3(0.0f, 1.0f, 0.0f));
	//GRDisplayGrassQuad();
#pragma endregion grassQuad display

#pragma region grass display
	GRDisplayGrass();
#pragma endregion grass display

	Update();
	SwapBuffers(grghdc);
}

void Update()
{
	if (grgTime >= 360.0f)
		grgTime = 0.0f;
	grgTime = grgTime + 0.1f;
}

void GRDisplayGrass()
{
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();;
	mat4 modelViewProjectionMatrix = mat4::identity();;
	mat4 modelViewMatrix =  mat4::identity();;

	mat4 RotationMatrix= mat4::identity();;
	mat4 TranslateMatrix = mat4::identity();;
	mat4 scaleMatrix = mat4::identity();;
	glUseProgram(G_gShaderProgramObject);
	modelMatrix = mat4::identity();
	//viewMatrix = mat4::identity();
	modelViewProjectionMatrix = mat4::identity();
	modelViewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(0.0f, -7.0f, 0.0f);
	vmath::vec3 vEye(0.0, 10.0f, -20);
	//viewMatrix = lookat(vec3(xView, 10.0f, zView), vec3(xView, 2.0f, -100.0f), vec3(0.0f, 1.0f, 0.0f));
	glUniform1i(G_gSampler, 6);
	glUniformMatrix4fv(G_projectionUniform, 1, GL_FALSE, (GLfloat*)&grgPerspectiveProjectionMatrix);
	glUniformMatrix4fv(G_viewUniform, 1, GL_FALSE, (GLfloat*)&viewMatrix);
	glUniformMatrix4fv(G_modelUniform, 1, GL_FALSE, modelMatrix);
	glUniform1f(fTimePassedUniform, tim1);
	tim1 += 0.002f;
	glBindVertexArray(uiGrassVAO);
	glDrawArrays(GL_POINTS, 0, iNumGrassTriangles);
	//glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

	glUseProgram(0);
}

void DrawMeshTree(int i)
{
	glBindTexture(GL_TEXTURE_2D, NULL);
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	vec3 matColor = vec3(0.0f, 0.0f, 0.0f);
	for (unsigned int j = 0; j < grgModelTree->meshes[i].textures.size(); j++)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		
		/*std::string number;
		std::string name = grgModel->meshes[i].textures[j].type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);

		}
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);
		*/
		glBindTexture(GL_TEXTURE_2D, grgModelTree->meshes[i].textures[j].id);
	}

	// get ambient color
	matColor[0] = grgModelTree->meshes[i].colors[0];
	matColor[1] = grgModelTree->meshes[i].colors[1];
	matColor[2] = grgModelTree->meshes[i].colors[2];
	glUniform3fv(grgKaUniformTree, 1, matColor);
	// get diffuse color
	matColor[0] = grgModelTree->meshes[i].colors[3];
	matColor[1] = grgModelTree->meshes[i].colors[4];
	matColor[2] = grgModelTree->meshes[i].colors[5];
	glUniform3fv(grgKdUniformTree, 1, matColor);
	// get specular color
	matColor[0] = grgModelTree->meshes[i].colors[6];
	matColor[1] = grgModelTree->meshes[i].colors[7];
	matColor[2] = grgModelTree->meshes[i].colors[8];
	glUniform3fv(grgKsUniformTree, 1, matColor);

	glUniform3fv(grgLaUniformTree, 1, grgLaTree);
	glUniform3fv(grgLdUniformTree, 1, grgLdTree);
	glUniform3fv(grgLsUniformTree, 1, grgLsTree);
	glUniform4fv(grgLightPositionUniformTree, 1, grgLightPositionTree);
	glUniform1f(grgMaterialShinynessUniformTree, 50);
	glUniform1i(grgLightUniform, 1);
	glUniform1i(grgTextureSamplerUniformTree, 0);
	glBindVertexArray(grgModelTree->meshes[i].vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, grgModelTree->meshes[i].indices.size(), 3);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void DrawMeshHouse(int i)
{
	glBindTexture(GL_TEXTURE_2D, NULL);
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int j = 0; j < grgModelHouse->meshes[i].textures.size(); j++)
	{
		glActiveTexture(GL_TEXTURE0 + j);

		/*std::string number;
		std::string name = grgModel->meshes[i].textures[j].type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);

		}
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);
		*/
		glUniform1i(grgTextureSamplerUniformTree, j);
		glBindTexture(GL_TEXTURE_2D, grgModelHouse->meshes[i].textures[j].id);
	}
	glBindVertexArray(grgModelHouse->meshes[i].vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, grgModelHouse->meshes[i].indices.size(), 1);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Uninitialize(void)
{
	void GRUnitializeGrass();

	//code
	if(grgbFullScreen == true)
	{
		grdwStyle = GetWindowLong(grghwnd, GWL_STYLE);
		SetWindowLong(grghwnd, GWL_STYLE, (grdwStyle | WS_OVERLAPPEDWINDOW));
		SetWindowPlacement(grghwnd, &grgwpPrev);
		SetWindowPos(grghwnd, HWND_TOP, 0, 0, 0, 0, 
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		
		ShowCursor(true);
		
	}

	if (grgVao)
	{
		glDeleteVertexArrays(1, &grgVao);
		grgVao = 0;
	}

	if (grgVbo_position)
	{
		glDeleteBuffers(1, &grgVbo_position);
		grgVbo_position = 0;
	}
	
	// free shader objects
	// detach vertex shader
	glDetachShader(grgShaderProgramObject, grgVertexShaderObject);
	glDetachShader(grgShaderProgramObject, grgTesControlShaderObject);
	glDetachShader(grgShaderProgramObject, grgTesEvaluationShaderObject);
	// detach fragment shader
	glDetachShader(grgShaderProgramObject, grgFragmentShadeerObject);


	// delete vertex object
	glDeleteShader(grgVertexShaderObject);
	grgVertexShaderObject = 0;

	glDeleteShader(grgTesControlShaderObject);
	grgTesControlShaderObject = 0;

	glDeleteShader(grgTesEvaluationShaderObject);
	grgTesEvaluationShaderObject = 0;

	// delete fragment shader object
	glDeleteShader(grgFragmentShadeerObject);
	grgFragmentShadeerObject = 0;

	// unlink shader program
	glUseProgram(0);

#pragma region road uninit
	GRUninitializeRoad();
#pragma endregion road uninit

#pragma region grass quad uninit
	GRUninitializeGrassQuad();
#pragma endregion grass quad uninit

#pragma region uninit
	GRUnitializeGrass();
#pragma endregion uninit 

	if(wglGetCurrentContext() == grghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}
		
	if(grghrc)
	{
		wglDeleteContext(grghrc);
		grghrc = NULL;
	}
		
	if(grghdc)
	{
		ReleaseDC(grghwnd, grghdc);
		grghdc = NULL;
	}
		
	if(grgpFile)
	{
		fprintf(grgpFile, "\n **** End ****\nLog File closed successfully. \n Program terminated successfully");
		fclose(grgpFile);
		grgpFile = NULL;
	}
}

void GRUnitializeGrass()
{
	
}

bool LoadTexture2D(string a_sPath, bool bGenerateMipMaps)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);
	fif = FreeImage_GetFileType(a_sPath.c_str(), 0); // Check the file signature and deduce its format
	if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(a_sPath.c_str());

	if (fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, a_sPath.c_str());
	if (!dib)
		return false;
	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data
	// If somehow one of these failed (they shouldn't), return failure
	if (bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
		return false;

	GLenum format = 0;
	int bada = FreeImage_GetBPP(dib);
	if (FreeImage_GetBPP(dib) == 32)format = GL_RGBA;
	if (FreeImage_GetBPP(dib) == 24)format = GL_BGR;
	if (FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;
	if (format != 0)

		CreateFromData(bDataPointer, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, bGenerateMipMaps);

	FreeImage_Unload(dib);

	if (format == 0)
		return false;

	sPath = a_sPath;

	return true; // Success


}







