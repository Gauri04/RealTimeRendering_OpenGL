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
GLuint grgTesControlShaderObject;
GLuint grgTesEvaluationShaderObject;

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
GLuint grgNumberOfSegmentsUniform;
GLuint grgNumberOfStripsUniform;
GLuint grgLineColorUniform;
unsigned int grgUiNumberOfCountSegments;
mat4 grgPerspectiveProjectionMatrix;

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

				case VK_UP :
					grgUiNumberOfCountSegments++;
					if (grgUiNumberOfCountSegments >= 30)
					{
						grgUiNumberOfCountSegments = 30;
					}
					break;

				case VK_DOWN :
					grgUiNumberOfCountSegments--;
					if (grgUiNumberOfCountSegments <= 0)
					{
						grgUiNumberOfCountSegments = 1;
					}
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
		"in vec2 vPosition;" \
		"void main(void)" \
		"{" \
		"gl_Position =  vec4(vPosition, 0.0, 1.0);" \
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
		"#version 450 core" \
		"\n" \
		"layout(vertices = 4)out;" \
		"uniform int numberOfSegments;" \
		"uniform int numberOfStrips;" \
		"void main(void)" \
		"{" \
		"gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;" \
		"gl_TessLevelOuter[0] = float(numberOfStrips);" \
		"gl_TessLevelOuter[1] = float(numberOfSegments);" \
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
		"#version 450 core" \
		"\n" \
		"layout(isolines)in;" \
		"uniform mat4 u_mvp_matrix;" \
		"void main(void)" \
		"{" \
		"float tessCoord = gl_TessCoord.x;" \
		"vec3 p0 = gl_in[0].gl_Position.xyz;" \
		"vec3 p1 = gl_in[1].gl_Position.xyz;" \
		"vec3 p2 = gl_in[2].gl_Position.xyz;" \
		"vec3 p3 = gl_in[3].gl_Position.xyz;" \
		"vec3 p = p0 * (1.0 - tessCoord) * (1.0 - tessCoord) * (1.0 - tessCoord) + p1 * 3.0 * tessCoord * (1.0 - tessCoord) * (1.0 - tessCoord) + p2 * 3.0 * tessCoord * tessCoord * (1.0 - tessCoord) + p3 * tessCoord * tessCoord * tessCoord;" \
		"gl_Position = u_mvp_matrix * vec4(p, 1.0);" \
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
				fprintf(grgpFile, "\n TessilationControl Shader Compilation Log : %s", grszInfoLog);
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
		"out vec4 FragColor;" \
		"uniform vec4 lineColor;" \
		"void main(void)" \
		"{" \
		"FragColor = lineColor;" \
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
	// attach fragment shader to shader program
	glAttachShader(grgShaderProgramObject, grgFragmentShadeerObject);

	// pre-link our attribute enum with shader's attributes
	glBindAttribLocation(grgShaderProgramObject, GR_ATTRIBUTE_POSITION, "vPosition");

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
	grgNumberOfSegmentsUniform = glGetUniformLocation(grgShaderProgramObject, "numberOfSegments");
	grgNumberOfStripsUniform = glGetUniformLocation(grgShaderProgramObject, "numberOfStrips");
	grgLineColorUniform = glGetUniformLocation(grgShaderProgramObject, "lineColor");

	const GLfloat grvertices[] =
	{
		-1.0f, -1.0f,
		-0.5f, 1.0f,
		0.5f, -1.0f,
		1.0f, 1.0f
	};

	glGenVertexArrays(1, &grgVao);
	glBindVertexArray(grgVao);

	glGenBuffers(1, &grgVbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grvertices), grvertices, GL_STATIC_DRAW);

	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);		// 2md param is two bcoz line has 2 vertices - x,y

	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Depth
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);

	grgUiNumberOfCountSegments = 1;
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
	// code
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// start use of shader program
	glUseProgram(grgShaderProgramObject);

	// OpenGL drawing code will start here
	mat4 grmodelViewMatrix = mat4::identity();
	mat4 grmodelViewProjectionMatrix = mat4::identity();
	mat4 grtranslateMatrix;

	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -4.0f);
	grmodelViewMatrix = grtranslateMatrix;

	// multiply modelview and orthographic matrix to get modelviewprojectionmatrix
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);
	glUniform1i(grgNumberOfSegmentsUniform, grgUiNumberOfCountSegments);
	glUniform1i(grgNumberOfStripsUniform, 1);
	glUniform4fv(grgLineColorUniform, 1, vmath::vec4(1.0, 1.0f, 0.0f, 1.0f));

	// bind vao
	glBindVertexArray(grgVao);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4);

	// unbind vao
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









