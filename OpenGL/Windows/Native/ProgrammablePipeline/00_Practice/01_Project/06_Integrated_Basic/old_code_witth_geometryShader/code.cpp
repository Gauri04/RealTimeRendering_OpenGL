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

using namespace vmath;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

#pragma comment(lib, "assimp-vc142-mtd.lib")
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
GLuint grgMvpMatrixUniform;
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
GLuint grgRandPositionUniform[10];
GLfloat grgRandomX;
GLfloat grgRandomY;
GLfloat grgRandomZ;
vec3 grgRandPos[10];

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

#pragma region tri
GLuint grgVertexShaderObjectTri;
GLuint grgFragmentShadeerObjectTri;
GLuint grgShaderProgramObjectTri;
GLuint grgGeometryShaderObjectTri;
GLuint grgMvpMatrixUniform2;
Model* grgModel;
GLuint grgTextureSamplerTreeUniform;
#pragma endregion tri


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
			x2 = x2 - 1.1f;
			break;

		case 'X':
			x2 = x2 + 1.1f;
			break;

		case 'z':
			z2 = z2 - 1.1f;
			break;

		case 'Z':
			z2 = z2 + 1.1f;
			break;

		case 'y':
			// terrain's Y camera coordinate
			y2 = y2 - 1.1f;
			break;

		case 'Y':
			y2 = y2 + 1.1f;
			break;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		case 0x46:
			ToggleFullScreen();
			break;

		case VK_UP:
			// terrain depth (height)
			grgDepth = grgDepth + 1.2f;
			break;

		case VK_DOWN:
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

void Initialize()
{
	// function declaration
	void Resize(int, int);
	void Uninitialize(void);
	bool LoadGLTexture(GLuint*, TCHAR[]);

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

	glShaderSource(grgVertexShaderObject, 1, (const GLchar**)&grvertexShaderSourceCode, NULL);

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
		"uniform mat4 u_mvp_matrix;" \
		"int i = 0;" \
		"void main(void)" \
		"{" \
		"if(gl_InvocationID == 0)" \
		"{" \
		"vec4 p0 = u_mvp_matrix * gl_in[0].gl_Position;" \
		"vec4 p1 = u_mvp_matrix * gl_in[1].gl_Position;" \
		"vec4 p2 = u_mvp_matrix * gl_in[2].gl_Position;" \
		"vec4 p3 = u_mvp_matrix * gl_in[3].gl_Position;" \
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
		"uniform mat4 u_mvp_matrix;" \
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
		"gl_Position = u_mvp_matrix * p;" \
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

	glShaderSource(grgFragmentShadeerObject, 1, (const GLchar**)&grfragmentShaderSourceCode, NULL);

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

	grgMvpMatrixUniform = glGetUniformLocation(grgShaderProgramObject, "u_mvp_matrix");
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
		"uniform mat4 u_mvpMatrix;" \
		"uniform vec3 u_rand_pos[10];" \
		"out VS_OUT" \
		"{" \
		"vec3 out_rand_pos[10];" \
		"vec2 out_texcoord;" \
		"} vs_out;" \
		"int i = 0;" \
		"void main(void)" \
		"{" \
		"gl_Position = u_mvpMatrix * vPosition;" \
		"vs_out.out_texcoord = vTexCoord;" \
		"for(i = 0; i < 10; i++)" \
		"{" \
		"vs_out.out_rand_pos[i] = u_rand_pos[i];" \
		"}" \
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

	////// Geometry shader
	// create shader
	grgGeometryShaderObjectTri = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* grgeometryShaderSourceCodeTri =
		"#version 450 core" \
		"\n" \
		"layout(triangles, invocations = 10)in;" \
		"layout(triangle_strip,max_vertices=3)out;" \
		"uniform mat4 u_mvpMatrix;" \
		"in VS_OUT" \
		"{" \
		"vec3 out_rand_pos[10];" \
		"vec2 out_texcoord;" \
		"} gs_in[];" \
		"out GS_OUT" \
		"{" \
		"vec2 out_texcoord;" \
		"vec3 out_color;" \
		"} gs_out;" \
		"void main(void)" \
		"{" \
		"vec3 xyz_pos = vec3(1.0f, 0.0f, 0.0f) + gs_in[0].out_rand_pos[gl_InvocationID];"
		"gl_Position =  (gl_in[0].gl_Position + vec4(xyz_pos, 0.0));" \
		"gs_out.out_texcoord = gs_in[0].out_texcoord;" \
		"gs_out.out_color = vec3(1.0, 0.0, 0.0);" \
		"EmitVertex();" \
		"gl_Position =  (gl_in[1].gl_Position + vec4(xyz_pos, 0.0));" \
		"gs_out.out_texcoord = gs_in[0].out_texcoord;" \
		"gs_out.out_color = vec3(0.0, 1.0, 0.0);" \
		"EmitVertex();" \
		"gl_Position =  (gl_in[2].gl_Position + vec4(xyz_pos, 0.0));" \
		"gs_out.out_texcoord = gs_in[0].out_texcoord;" \
		"gs_out.out_color = vec3(0.0, 0.0, 1.0);" \
		"EmitVertex();" \
		"EndPrimitive();" \
		"}";

	glShaderSource(grgGeometryShaderObjectTri, 1, (const GLchar**)&grgeometryShaderSourceCodeTri, NULL);

	// compile shader
	glCompileShader(grgGeometryShaderObjectTri);
	// error check for compilation
	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	glGetShaderiv(grgGeometryShaderObjectTri, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgGeometryShaderObjectTri, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgGeometryShaderObjectTri, griInfoLength, &grwritten, grszInfoLog);
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
		"in GS_OUT" \
		"{" \
		"vec2 out_texcoord;" \
		"vec3 out_color;" \
		"} fs_in;" \
		"uniform sampler2D u_texture_tree;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"vec3 color = texture(u_texture_tree, fs_in.out_texcoord).rgb;" \
		/*"vec3 color = fs_in.out_color;"*/
		"FragColor = vec4(color, 1.0);" \
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

	grgMvpMatrixUniform2 = glGetUniformLocation(grgShaderProgramObjectTri, "u_mvpMatrix");
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
	grgTextureSamplerTreeUniform = glGetUniformLocation(grgShaderProgramObjectTri, "u_texture_tree");

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


	grgTextureTerragen1 = load("terragen1.ktx");
	//grgTextureTerragenColor = load("terragen_color.ktx");
	//LoadGLTexture(&grgTextureTerragen1, MAKEINTRESOURCE(GRTERRAIN_GEN));
	LoadGLTexture(&grgTextureTerragenColor, MAKEINTRESOURCE(GRTERRAIN_COLOR));
	LoadGLTexture(&grgTextureBlendMap, MAKEINTRESOURCE(GRTERRAIN_BLENDMAP));
	LoadGLTexture(&grgTextureGrass, MAKEINTRESOURCE(GRTERRAIN_GRASS));
	LoadGLTexture(&grgTextureDirt, MAKEINTRESOURCE(GRTERRAIN_DIRT));
	LoadGLTexture(&grgTextureGrassFlowers, MAKEINTRESOURCE(GRTERRAIN_GRSSSFLOWERS));
	LoadGLTexture(&grgTexturePath, MAKEINTRESOURCE(GRTERRAIN_PATH));

	//grgModel = new Model(FileSystem::getPath("model/Tree1/Tree2.obj"), grgpFile);
	grgModel = new Model(FileSystem::getPath("model/nanosuit/nanosuit.obj"), grgpFile);

	srand(1);

	grgRandPos[0] = vec3(7.0, 1.0, -10.0);
	grgRandPos[1] = vec3(10.0, -2.0, -15.0);
	grgRandPos[2] = vec3(-15.0, -10.0, -10.0);
	grgRandPos[3] = vec3(5.0, 0.0, -10.0);
	grgRandPos[4] = vec3(-15.0, -1.0, -10.0);
	grgRandPos[5] = vec3(-2.8, -1.0, -10.0);
	grgRandPos[6] = vec3(12.4, 0.0, -10.0);
	grgRandPos[7] = vec3(3.1, 0.0, -10.0);
	grgRandPos[8] = vec3(-8.1, -1.0, -10.0);
	grgRandPos[9] = vec3(5.0, -1.0, -13.0);
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
	if (height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	grgPerspectiveProjectionMatrix = vmath::perspective(60.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);


}

void Display(void)
{
	// function declaration
	void Update();
	void DrawMesh(int);

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
	grtranslateMatrix = vmath::translate(x2, y2, 5.0f + z2);
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grtranslateMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform2, 1, GL_FALSE, grmodelViewProjectionMatrix);

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

	for (unsigned int i = 0; i < grgModel->meshes.size(); i++)
	{
		DrawMesh(i);
	}

	glUseProgram(0);
#pragma endregion tri display

#pragma region terrain display
	glUseProgram(grgShaderProgramObject);
	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();

	grtranslateMatrix = vmath::translate(x2, y2, -20.0f + z2);
	//grrotateMatrix = vmath::rotate(180.0f, 0.0f, 0.0f, 1.0f);
	grmodelMatrix = grtranslateMatrix * grrotateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);
	//vmath::lookat(vec3(0.0f, 0.0f, 0.0f), vec3(x2, y2, z2), vec3(0.0f, 1.0f, 0.0f));
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



	Update();
	SwapBuffers(grghdc);
}

void Update()
{
	if (grgTime >= 360.0f)
		grgTime = 0.0f;
	grgTime = grgTime + 0.1f;
}

void DrawMesh(int i)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int j = 0; j < grgModel->meshes[i].textures.size(); j++)
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
		glUniform1i(grgTextureSamplerTreeUniform, j);
		glBindTexture(GL_TEXTURE_2D, grgModel->meshes[i].textures[j].id);
	}
	glBindVertexArray(grgModel->meshes[i].vao);
	glDrawElements(GL_TRIANGLES, grgModel->meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Uninitialize(void)
{
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

	if (grgpFile)
	{
		fprintf(grgpFile, "\n **** End ****\nLog File closed successfully. \n Program terminated successfully");
		fclose(grgpFile);
		grgpFile = NULL;
	}
}









