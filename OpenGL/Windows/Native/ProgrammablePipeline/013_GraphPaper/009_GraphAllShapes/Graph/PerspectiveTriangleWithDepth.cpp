#include<windows.h>
#include<stdio.h>
#include<math.h>
#include<gl\glew.h> // glew.h must be included BEFORE gl.h
#include<gl\GL.h>
#include"vmath.h"
#include<math.h>
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
	GR_ATTRIBUTE_TEXCOORD,
	GR_ATTRIBUTE_NORMAL
};

GLuint grgVao;
GLuint grgVbo_position;
GLuint grgMvpMatrixUniform;
mat4 grgPerspectiveProjectionMatrix;
GLfloat grVerticalLineVerts1[80];
GLfloat grVerticalLineVerts2[80];
GLfloat grVerticalCenterLineVerts[4] =
{
	0.0f, -2.0f,
	0.0f, 2.0f
};
GLfloat grHorizontalLineVerts1[80];
GLfloat grHorizontalLineVerts2[80];
GLfloat grHorizontalCenterLineVerts[4] =
{
	-3.0f, 0.0f,
	3.0f, 0.0f
};
GLuint grgVaoCircle;
GLuint grgVboPositionCircle;

GLuint grgVao2;
GLuint grgVboPosition2;
GLfloat grtriangleVerts[] =
{
	0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
};
GLfloat grsquareVerts[] =
{
	0.86f, 0.5f, 0.0f,
	-0.86f, 0.5f, 0.0f,
	-0.86f, -0.5f, 0.0f,
	0.86f, -0.5f, 0.0f
};

GLfloat grcircleVerts[1258];
GLdouble grs;
const GLdouble PI = 3.14159655359f;
GLdouble grlength1, grlength2, grlength3;
GLdouble grradius;
GLdouble grincenterX, grincenterY;
GLdouble grsqrt;
GLfloat grcircleVerts2[1258];


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
	/********* Shader Code *********/

	// create shader
	grgVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	///// Vertex Shader
	const GLchar* grvertexShaderSourceCode =					// also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec4 vColor;" \
		"out vec4 out_color;" \
		"uniform mat4 u_mvpMatrix;" \
		"void main(void)" \
		"{" \
		"out_color = vColor;" \
		"gl_Position = u_mvpMatrix * vPosition;" \
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
				Uninitialize();
			}
		}
	}

	////// Fragment Shader
	grgFragmentShadeerObject = glCreateShader(GL_FRAGMENT_SHADER);

	// source code of shader
	const GLchar* grfragmentShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec4 out_color;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = out_color;" \
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
				Uninitialize();
			}
		}
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

	grgMvpMatrixUniform = glGetUniformLocation(grgShaderProgramObject, "u_mvpMatrix");

	
	GLfloat i = 0.11f;
	GLint index = 0;
	// vertical lines
	for (index = 0; index < 80; index = index + 4)
	{
			grVerticalLineVerts1[index] = i;
			grVerticalLineVerts1[index + 1] = -2.0f;
			grVerticalLineVerts1[index + 2] = i;
			grVerticalLineVerts1[index + 3] = 2.0f;
			i = i + 0.11f;
		
	}

	i = 0.11f;
	for (index = 0; index < 80; index = index + 4)
	{
		grVerticalLineVerts2[index] = -i;
		grVerticalLineVerts2[index + 1] = -2.0f;
		grVerticalLineVerts2[index + 2] = -i;
		grVerticalLineVerts2[index + 3] = 2.0f;
			i = i + 0.11f;

	}

	// horizontal lines
	i = 0.060f;
	for (index = 0; index < 80; index = index + 4)
	{
		grHorizontalLineVerts1[index] = -3.0f;
		grHorizontalLineVerts1[index + 1] = i;
		grHorizontalLineVerts1[index + 2] = 3.0f;
		grHorizontalLineVerts1[index + 3] = i;
		i = i + 0.060f;
	}
	i = 0.05f;
	for (index = 0; index < 80; index = index + 4)
	{
		grHorizontalLineVerts2[index] = -3.0f;
		grHorizontalLineVerts2[index + 1] = -i;
		grHorizontalLineVerts2[index + 2] = 3.0f;
		grHorizontalLineVerts2[index + 3] = -i;
		i = i + 0.060f;
	}

	index = 0;
	for (i = 0.0f; i < 3.142f * 2.0f; i = i + 0.01f)
	{
		grcircleVerts[index] = cosf(i);
		grcircleVerts[index + 1] = sinf(i);
		index = index + 2;
	}

	/*** Formula for incircle ****/
	// calculate length between the coordinates of triangle
	// distance between ax,ay and bx, by : bx-ax, by-ay (bx = -1.0, by = -1.0) (ax = 0.0, ay = 1.0) VERTEX B
	grlength3 = sqrtl(pow((-0.5f - 0.0f), 2) + pow((-0.5f - 0.5f), 2));
	// distance between bx, by and cx, cy : cx-bx, cy- by (cx = 1.0, cy = -1.0) (bx = -1.0, by = -1.0) VERTEX A
	grlength1 = sqrtl(pow((0.5f + 0.5f), 2) + pow((-0.5f + 0.5f), 2));
	// distance between ax, ay and cx, cy : cx-ax, cy-ay (cx = 1.0, cy = -1.0) (ax = 0.0, ay = 1.0) VERTEX C
	grlength2 = sqrtl(pow((0.5 - 0.0), 2) + pow((-0.5 - 0.5), 2));


	// x = (ax*length1 + bx*length2 + cx*length3) / length1 + length2 + length3 
	grincenterX = ((0.0f * grlength1) + (-0.5f * grlength2) + (0.5f * grlength3)) / (grlength1 + grlength2 + grlength3);
	// y = (ay*length + by*length + cy*length) / 3*length
	grincenterY = ((0.5f * grlength1) + (-0.5f * grlength2) + (-0.5f * grlength3)) / (grlength1 + grlength2 + grlength3);

	// s = (length1 + length2 + length3) / 2
	grs = (grlength1 + grlength2 + grlength3) / 2;

	//commented the formula for radius from wikipedia
	// radius = (sqrt(s * (s - length1) * (s - length2) * (s - length3))) / s
	grsqrt = grs * (grs - grlength1) * (grs - grlength2) * (grs - grlength3);
	grsqrt = sqrtl(grsqrt);
	grradius = grsqrt / grs;
	
	index = 0;
	for (i = 0.0f; i < 3.142f * 2.0f; i = i + 0.01f)
	{
		grcircleVerts2[index] = (GLfloat) grincenterX + (grradius * cosf(i));
		grcircleVerts2[index + 1] = (GLfloat) grincenterY + (grradius * sinf(i));
		index = index + 2;
	}

	// Vao and Vbo initialization for lines
	glGenVertexArrays(1, &grgVao);
	glBindVertexArray(grgVao);

	glGenBuffers(1, &grgVbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
	glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 0.0f, 0.0f);

	glBindVertexArray(0);

	// vao and vbo for circle
	glGenVertexArrays(1, &grgVaoCircle);
	glBindVertexArray(grgVaoCircle);

	glGenBuffers(1, &grgVboPositionCircle);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboPositionCircle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grcircleVerts), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 1.0f, 0.0f);

	glBindVertexArray(0);

	// vao and vbo for triangle and rectangle
	glGenVertexArrays(1, &grgVao2);
	glBindVertexArray(grgVao2);

	glGenBuffers(1, &grgVboPosition2);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboPosition2);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 1.0f, 0.0f);

	glBindVertexArray(0);


	

	// Depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// set clearcolor
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	grgPerspectiveProjectionMatrix = mat4::identity();
	
	// warm-up call to resize
	Resize(WIN_WIDTH, WIN_HEIGHT);
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
	// variables declaration
	mat4 grmodelViewMatrix = mat4::identity();
	mat4 grmodelViewProjectionMatrix = mat4::identity();
	mat4 grtranslateMatrix;
	
	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	// start use of shader program
	glUseProgram(grgShaderProgramObject);

	//******************** VERTICAL Line - (bottom) **************************
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	grmodelViewMatrix = grtranslateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

	// bind vao
	glBindVertexArray(grgVao);
	glGenBuffers(1, &grgVbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
	glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), grVerticalLineVerts1, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINES, 0, 40);

	// unbind vao
	glBindVertexArray(0);

	//******************** VERTICAL Line - (center line) **************************
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	grmodelViewMatrix = grtranslateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

	// bind vao
	glBindVertexArray(grgVao);
	glGenBuffers(1, &grgVbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
	glBufferData(GL_ARRAY_BUFFER, 2 * 2 * sizeof(GLfloat), grVerticalCenterLineVerts, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_LINES, 0, 40);

	// unbind vao
	glBindVertexArray(0);
	
	//******************** VERTICAL Line - (top) **************************
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	grmodelViewMatrix = grtranslateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

	glBindVertexArray(grgVao);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
	glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), grVerticalLineVerts2, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINES, 0, 40);
	glBindVertexArray(0);

	//**************************************** HORIZONTAL LINES ****************************************
	//************************************************************************************************
	//******************** Horizontal Line - (top line) **************************
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	grmodelViewMatrix = grtranslateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

	// bind vao
	glBindVertexArray(grgVao);
	glGenBuffers(1, &grgVbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
	glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), grHorizontalLineVerts1, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINES, 0, 40);

	// unbind vao
	glBindVertexArray(0);

	//******************** Horizontal Line - (center line) **************************
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	grmodelViewMatrix = grtranslateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

	// bind vao
	glBindVertexArray(grgVao);
	glGenBuffers(1, &grgVbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
	glBufferData(GL_ARRAY_BUFFER, 2 * 2 * sizeof(GLfloat), grHorizontalCenterLineVerts, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
	glDrawArrays(GL_LINES, 0, 40);

	// unbind vao
	glBindVertexArray(0);

	//******************** Horizontal Line - (top) **************************
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	grmodelViewMatrix = grtranslateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

	glBindVertexArray(grgVao);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
	glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), grHorizontalLineVerts2, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINES, 0, 40);
	glBindVertexArray(0);

	//**************** Circle ****************************************
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	grmodelViewMatrix = grtranslateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

	glBindVertexArray(grgVaoCircle);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboPositionCircle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grcircleVerts), grcircleVerts, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 1.0f, 0.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 629);
	glBindVertexArray(0);

	//**************** Triangle ****************************************
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	grmodelViewMatrix = grtranslateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

	glBindVertexArray(grgVao2);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboPosition2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grtriangleVerts), grtriangleVerts, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 1.0f, 0.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
	glBindVertexArray(0);

	//**************** Rectangle ****************************************
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	grmodelViewMatrix = grtranslateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

	glBindVertexArray(grgVao2);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboPosition2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grsquareVerts), grsquareVerts, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 1.0f, 0.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glBindVertexArray(0);

	//**************** Incircle - circle ****************************************
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	grmodelViewMatrix = grtranslateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

	glBindVertexArray(grgVaoCircle);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboPositionCircle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grcircleVerts2), grcircleVerts2, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 1.0f, 0.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 629);
	glBindVertexArray(0);


	// stop use of shader program
	glUseProgram(0);
	SwapBuffers(grghdc);
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

	if (grgVboPositionCircle)
	{
		glDeleteBuffers(1, &grgVboPositionCircle);
		grgVboPositionCircle = 0;
	}
	if (grgVaoCircle)
	{
		glDeleteVertexArrays(1, &grgVaoCircle);
		grgVaoCircle = 0;
	}
	
	if (grgVbo_position)
	{
		glDeleteBuffers(1, &grgVbo_position);
		grgVbo_position = 0;
	}
	if (grgVao)
	{
		glDeleteVertexArrays(1, &grgVao);
		grgVao = 0;
	}

	if (grgVboPosition2)
	{
		glDeleteBuffers(1, &grgVboPosition2);
		grgVboPosition2 = 0;
	}
	if (grgVao2)
	{
		glDeleteVertexArrays(1, &grgVao2);
		grgVao2 = 0;
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









