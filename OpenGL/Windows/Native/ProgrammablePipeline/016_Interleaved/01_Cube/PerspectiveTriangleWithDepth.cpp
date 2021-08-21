#include<windows.h>
#include<stdio.h>
#include<gl\glew.h> // glew.h must be included BEFORE gl.h
#include<gl\GL.h>
#include"vmath.h"
#include"GRWindow.h"

using namespace vmath;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")


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

enum
{
	GR_ATTRIBUTE_POSITION = 0,
	GR_ATTRIBUTE_COLOR,
	GR_ATTRIBUTE_NORMAL,
	GR_ATTRIBUTE_TEXCOORD
};


GLuint grgMvpMatrixUniform;
mat4 grgPerspectiveProjectionMatrix;

// project specific variables
GLuint grgVao_cube;
GLuint grgVboCube;
GLfloat grgAngleCube = 0.0f;
GLuint grgModelMarixUniform;
GLuint grgViewMatrixUniform;
GLuint grgProjectionMatrixUniform;
GLuint grgLKeyPressedUniform;
GLuint grgLdUniform[2];					// light diffuse
GLuint grgLaUniform[2];
GLuint grgLsUniform[2];
GLuint grgLightPositionUniform[2];
GLuint grgKdUniform;					// material diffuse
GLuint grgKaUniform;
GLuint grgKsUniform;
GLuint grgMaterialShinynessUniform;
struct Light
{
	vec4 lightAmbientColor;
	vec4 lightDiffuseColor;
	vec4 lightSpecularColor;
	vec4 lightPosition;
};
vec4 grgKa = { 0.0, 0.0, 0.0, 1.0 };	// material ambient
vec4 grgKd = { 1.0, 1.0, 1.0, 1.0 };	// material diffuse
vec4 grgKs = { 1.0, 1.0, 1.0, 1.0 };	// material specular
GLfloat grgMaterialShinyness = 128;
struct Light grlight[2];

int i;
bool grbAnimate;
bool grbLight;
GLuint grkundali_texture;
GLuint grgtextureSamplerUniform;

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

		case WM_CHAR :
			switch (wParam)
			{
				case 'a':
				case 'A':
					grbAnimate = !grbAnimate;
					break;

				case 'l':
				case 'L':
					grbLight = !grbLight;
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

	/********* Shader Code *********/

	// create shader
	grgVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	///// Vertex Shader
	const GLchar* grvertexShaderSourceCode =					// also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"in vec2 vTexCoord;" \
		"in vec3 vColor;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform int u_l_key_pressed;" \
		"uniform vec3 u_la[2];" \
		"uniform vec3 u_ld[2];" \
		"uniform vec3 u_ls[2];" \
		"uniform vec3 u_ka;" \
		"uniform vec3 u_kd;" \
		"uniform vec3 u_ks;" \
		"uniform float u_material_shinyness;" \
		"uniform vec4 u_light_position[2];" \
		"out vec3 phong_ads_light;" \
		"out vec2 out_texcoord;" \
		"out vec3 out_color;" \
		"int i;" \
		"void main(void)" \
		"{" \
		"if(u_l_key_pressed == 1)" \
		"{" \
			"vec4 eye_coordinates =  u_view_matrix * u_model_matrix * vPosition;" \
			"vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
			"vec3 view_vector = normalize(-eye_coordinates.xyz);" \
			"vec3 light_direction[2];" \
			"vec3 reflection_vector[2];" \
			"vec3 ambient[2];" \
			"vec3 diffuse[2];" \
			"vec3 specular[2];" \
			"for(i = 0; i < 2; i++)" \
			"{" \
				"light_direction[i] = normalize(vec3(u_light_position[i] - eye_coordinates));" \
				"reflection_vector[i] = reflect(-light_direction[i], transformed_normal);" \
				"ambient[i] = u_la[i] * u_ka;" \
				"diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);" \
				"specular[i] = u_ls[i] * u_ks * pow(max(dot(reflection_vector[i], view_vector), 0.0), u_material_shinyness);" \
				"phong_ads_light = phong_ads_light + ambient[i] + diffuse[i] + specular[i];" \
			"}" \
		"}" \
		"else" \
		"{" \
			"phong_ads_light = vec3(1.0, 1.0, 1.0);" \
		"}" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"out_texcoord = vTexCoord;" \
		"out_color = vColor;" \
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

	////// Fragment Shader
	grgFragmentShadeerObject = glCreateShader(GL_FRAGMENT_SHADER);

	// source code of shader
	const GLchar* grfragmentShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"vec4 color;" \
		"in vec3 phong_ads_light;" \
		"in vec2 out_texcoord;" \
		"in vec3 out_color;" \
		"uniform sampler2D u_texture_sampler;" \
		"uniform int u_l_key_pressed;" \
		"out vec4 FragColor;" \
		"vec3 texture_color;" \
		"void main(void)" \
		"{" \
		"texture_color = vec3(texture(u_texture_sampler, out_texcoord));" \
		"if(u_l_key_pressed == 1)" \
		"{" \
			"color = vec4(vec3(out_color * phong_ads_light * texture_color), 1);" \
		"}" \
		"else" \
		"{" \
			"color = vec4(vec3(out_color * vec3(1.0) * texture_color), 1);"	\
		"}" \
		"FragColor = color;" \
		"}";

	glShaderSource(grgFragmentShadeerObject, 1, (const GLchar **) &grfragmentShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgFragmentShadeerObject);
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
				
			}
		}
		Uninitialize();
	}

	//****** Shader Program *****//
	// create
	grgShaderProgramObject = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(grgShaderProgramObject, grgVertexShaderObject);

	// attach fragment shader to shader program
	glAttachShader(grgShaderProgramObject, grgFragmentShadeerObject);

	// pre-link our attribute enum with shader's attributes
	glBindAttribLocation(grgShaderProgramObject, GR_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(grgShaderProgramObject, GR_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(grgShaderProgramObject, GR_ATTRIBUTE_TEXCOORD, "vTexCoord");
	glBindAttribLocation(grgShaderProgramObject, GR_ATTRIBUTE_COLOR, "vColor");

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

	grgModelMarixUniform = glGetUniformLocation(grgShaderProgramObject, "u_model_matrix");
	grgViewMatrixUniform = glGetUniformLocation(grgShaderProgramObject, "u_view_matrix");
	grgProjectionMatrixUniform = glGetUniformLocation(grgShaderProgramObject, "u_projection_matrix");

	grgLdUniform[0] = glGetUniformLocation(grgShaderProgramObject, "u_ld[0]");
	grgLaUniform[0] = glGetUniformLocation(grgShaderProgramObject, "u_la[0]");
	grgLsUniform[0] = glGetUniformLocation(grgShaderProgramObject, "u_ls[0]");
	grgLdUniform[1] = glGetUniformLocation(grgShaderProgramObject, "u_ld[1]");
	grgLaUniform[1] = glGetUniformLocation(grgShaderProgramObject, "u_la[1]");
	grgLsUniform[1] = glGetUniformLocation(grgShaderProgramObject, "u_ls[1]");
	grgLightPositionUniform[0] = glGetUniformLocation(grgShaderProgramObject, "u_light_position[0]");
	grgLightPositionUniform[1] = glGetUniformLocation(grgShaderProgramObject, "u_light_position[1]");
	grgKaUniform = glGetUniformLocation(grgShaderProgramObject, "u_ka");
	grgKdUniform = glGetUniformLocation(grgShaderProgramObject, "u_kd");
	grgKsUniform = glGetUniformLocation(grgShaderProgramObject, "u_ks");
	grgLKeyPressedUniform = glGetUniformLocation(grgShaderProgramObject, "u_l_key_pressed");
	grgMaterialShinynessUniform = glGetUniformLocation(grgShaderProgramObject, "u_material_shinyness");
	grgtextureSamplerUniform = glGetUniformLocation(grgShaderProgramObject, "u_texture_sampler");

	const GLfloat grcubeVertices[] =
	{
											// front face
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
											// right face
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
											// back face
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
										    // left face
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
											// top face
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
											// bottom face
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};
	
	const GLfloat grcubeNormals[] =		
	{
		0.0f, 0.0f, 1.0f,					// one normal for single surface 
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
	};

	GLfloat data[] =
	{
		//P					//C					//N					//T
		1.0f, 1.0f, 1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,

		1.0f, 1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,

		-1.0f, 1.0f, -1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
		1.0f, 1.0f, -1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,0.0f, 0.0f, 1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f,

		-1.0f, 1.0f, 1.0f,	1.0f, 0.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-1.0f, 1.0f, -1.0f,	1.0f, 0.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,1.0f, 0.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,	1.0f, 0.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

		1.0f, 1.0f, -1.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,0.0f, 1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
		1.0f, -1.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	1.0f, 1.0f
	};
	

	//** initialize vao_cube**//
	glGenVertexArrays(1, &grgVao_cube);
	glBindVertexArray(grgVao_cube);

	glGenBuffers(1, &grgVboCube);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0);
	glVertexAttribPointer(GR_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glVertexAttribPointer(GR_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(9 * sizeof(GLfloat)));
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glEnableVertexAttribArray(GR_ATTRIBUTE_COLOR);
	glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_TEXCOORD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// initialize colors
	grlight[0].lightAmbientColor = vec4(0.0, 0.0, 0.0, 1.0);
	grlight[0].lightDiffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
	grlight[0].lightSpecularColor = vec4(1.0, 1.0, 1.0, 1.0);
	grlight[1].lightAmbientColor = vec4(0.0, 0.0, 0.0, 1.0);
	grlight[1].lightDiffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
	grlight[1].lightSpecularColor = vec4(1.0, 1.0, 1.0, 1.0);
	grlight[0].lightPosition = vec4(100.0, 100.0, 100.0, 1.0);
	grlight[1].lightPosition = vec4(-100.0, -100.0, -100.0, 1.0);

	LoadGLTexture(&grkundali_texture, MAKEINTRESOURCE(GRTEXTURE));
	// Depth
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// set clearcolor
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// initialize boolean variables
	grbAnimate = false;
	grbLight = false;

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

	grgPerspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 100.0f);

}

void Display(void)
{
	// function declaration
	void Update(void);

	// code
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// start use of shader program
	glUseProgram(grgShaderProgramObject);

	// OpenGL drawing code will start here
	mat4 grviewMatrix = mat4::identity();
	mat4 grmodelMatrix = mat4::identity();
	mat4 grprojectionMatrix = mat4::identity();
	mat4 grrotateMatrix = mat4::identity();
	mat4 grtranslateMatrix = mat4::identity();
	mat4 grscaleMatrix = mat4::identity();

	#pragma region cube

	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	grscaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	grrotateMatrix = vmath::rotate(grgAngleCube, 1.0f, 0.0f, 0.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grgAngleCube, 0.0f, 1.0f, 0.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grgAngleCube, 0.0f, 0.0f, 1.0f);
		
	grmodelMatrix = grtranslateMatrix * grscaleMatrix * grrotateMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrix;
	
	if (grbLight == true)
	{
		// enable lighting
		glUniform1i(grgLKeyPressedUniform, 1);
		glUniform3fv(grgKdUniform, 1, grgKd);
		glUniform3fv(grgKaUniform, 1, grgKa);
		glUniform3fv(grgKsUniform, 1, grgKs);
		glUniform1f(grgMaterialShinynessUniform, grgMaterialShinyness);
		// two lights
		for (i = 0; i < 2; i++)
		{
			glUniform3fv(grgLdUniform[i], 1, grlight[i].lightDiffuseColor);
			glUniform3fv(grgLaUniform[i], 1, grlight[i].lightAmbientColor);
			glUniform3fv(grgLsUniform[i], 1, grlight[i].lightSpecularColor);
			glUniform4fv(grgLightPositionUniform[i], 1, grlight[i].lightPosition);
		}
	}
	else
	{
		glUniform1i(grgLKeyPressedUniform, 0);
	}

	glUniformMatrix4fv(grgProjectionMatrixUniform, 1, GL_FALSE, grprojectionMatrix);
	glUniformMatrix4fv(grgViewMatrixUniform, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grgModelMarixUniform, 1, GL_FALSE, grmodelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grkundali_texture);
	glUniform1i(grgtextureSamplerUniform, 0);

	// bind vao of square
	glBindVertexArray(grgVao_cube);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);											
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
	// unbind vao
	glBindVertexArray(0);
	#pragma endregion cube 

	// stop use of shader program
	glUseProgram(0);
	if (grbAnimate)
	{
		Update();
	}
	
	SwapBuffers(grghdc);
}

void Update(void)
{
	grgAngleCube = grgAngleCube + 0.1f;
	if (grgAngleCube >= 360.0f)
		grgAngleCube = 0.0f;
}

void Uninitialize(void)
{
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
	//  delete vbo

	if (grgVboCube)
	{
		glDeleteBuffers(1, &grgVboCube);
		grgVboCube = 0;
	}

	// delete square vao and vbo
	if (grgVao_cube)
	{
		glDeleteVertexArrays(1, &grgVao_cube);
		grgVao_cube = 0;
	}
	if (grkundali_texture)
	{
		glDeleteTextures(1, &grkundali_texture);
		grkundali_texture = NULL;
	}

	// free shader objects
	// detach vertex shader
	glDetachShader(grgShaderProgramObject, grgVertexShaderObject);
	// detach fragment shader
	glDetachShader(grgShaderProgramObject, grgFragmentShadeerObject);

	// delete vertex object
	glDeleteShader(grgVertexShaderObject);
	grgVertexShaderObject = 0;


	// delete fragment shader object
	glDeleteShader(grgFragmentShadeerObject);
	grgFragmentShadeerObject = 0;

	// unlink shader program
	glUseProgram(0);


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









