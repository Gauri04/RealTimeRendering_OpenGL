#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<gl\glew.h> // glew.h must be included BEFORE gl.h
#include<gl\GL.h>
#include"vmath.h"
#include"GRWindow.h"

using namespace vmath;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma warning(disable : 996)

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define BUFFER_SIZE 256
#define S_EQUAL 0
#define NR_POINT_COORDS 3
#define NR_TEXTURE_COORDS 2
#define NR_NORMAL_COORDS 3
#define NR_FACE_TOKENS 3
#define NR_TRIANGLE_VERTICES 3


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

GLuint grgVboPosition;
GLuint grgVboTexture;
GLuint grgVboElement;
GLuint grgMvpMatrixUniform;
mat4 grgPerspectiveProjectionMatrix;
GLuint grgVao;							
GLfloat grgAnglePyramid = 0.0f;
GLfloat grgAngleCube = 0.0f;

typedef struct vec_2d_int
{
	GLint** pp_arr;
	size_t size;
}vec_2d_int_t;

typedef struct vec_2d_float
{
	GLfloat** pp_arr;
	size_t size;
}vec_2d_float_t;

vec_2d_float_t* gp_vertices;
vec_2d_float_t* gp_texture;
vec_2d_float_t* gp_normals;
vec_2d_int_t* gp_face_tri;
vec_2d_int_t* gp_face_texture;
vec_2d_int_t* gp_face_normals;

FILE* g_fp_meshFile = NULL;
FILE* g_fp_logFile = NULL;
char g_line[BUFFER_SIZE];
GLuint grstone_texture;
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
	void LoadMeshData(void);
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

	fprintf(grgpFile, "\n test");
	/********* Shader Code *********/

	// create shader
	grgVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	///// Vertex Shader
	const GLchar* grvertexShaderSourceCode =					// also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec2 vTexCoord;" \
		"uniform mat4 u_mvpMatrix;" \
		"out vec2 out_texcoord;" \
		"void main(void)" \
		"{" \
		"gl_Position = u_mvpMatrix * vPosition;" \
		"out_texcoord = vTexCoord;" \
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
		"in vec2 out_texcoord;"
		"uniform sampler2D u_texture_sampler;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		/*"FragColor = texture(u_texture_sampler, out_texcoord);" \*/
		"FragColor = vec4(1.0, 1.0, 1.0, 1.0);" \
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
	glBindAttribLocation(grgShaderProgramObject, GR_ATTRIBUTE_TEXCOORD, "vTexCoord");

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
	grgtextureSamplerUniform = glGetUniformLocation(grgShaderProgramObject, "u_texture_sampler");

	
	LoadMeshData();
	
	GLfloat* vertices;
	int index = 0;
	int vi;
	vertices = (GLfloat*)malloc(gp_face_tri->size * 9 * sizeof(GLfloat));
	for (int i = 0; i != gp_face_tri->size; ++i)
	{
		for (int j = 0; j != NR_TRIANGLE_VERTICES; j++)
		{
			
			vi = gp_face_tri->pp_arr[i][j] - 1;
			vertices[index + 0] = gp_vertices->pp_arr[vi][0];
			vertices[index + 1] = gp_vertices->pp_arr[vi][1];
			vertices[index + 2] = gp_vertices->pp_arr[vi][2];
			fprintf(grgpFile, "\n vertices[%d] : %f \t vertices[%d] : %f \t vertices[%d] : %f", index + 0, vertices[index + 0],
					index + 1, vertices[index + 1], index + 2, vertices[index + 2]);
					
			index = index + 3;
		}
		
	}
	GLfloat* textures = NULL;
	index = 0;
	textures = (GLfloat*)malloc(gp_face_texture->size * 9 * sizeof(GLfloat));
	for (int i = 0; i != gp_face_texture->size; ++i)
	{
		for (int j = 0; j != NR_TEXTURE_COORDS; j++)
		{
			vi = gp_face_texture->pp_arr[i][j] - 1;
			textures[index + 0] = gp_texture->pp_arr[vi][0];
			textures[index + 1] = gp_texture->pp_arr[vi][1];
		}
		
	}
	
	glGenVertexArrays(1, &grgVao);
	glBindVertexArray(grgVao);

	glGenBuffers(1, &grgVboPosition);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboPosition);
	glBufferData(GL_ARRAY_BUFFER, gp_face_tri->size * 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &grgVboTexture);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboTexture);
	glBufferData(GL_ARRAY_BUFFER, gp_face_texture->size * 9 * sizeof(GLfloat), textures, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_TEXCOORD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	/*
	glGenBuffers(1, &grgVboElement);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboElement);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, gp_face_tri->size * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	*/
	glBindVertexArray(0);



	free(vertices);
	vertices = NULL;
	free(textures);
	textures = NULL;
	

	// Depth
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	LoadGLTexture(&grstone_texture, MAKEINTRESOURCE(GRTEXTURE));
	// set clearcolor
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);				// MIN - Minification

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

void LoadMeshData()
{
	int lineCount = 0;
	vec_2d_int_t* create_vec_2d_int(void);
	vec_2d_float_t* create_vec_2d_float(void);

	void push_back_vec_2d_int(vec_2d_int_t *, int *);
	void push_back_vec_2d_float(vec_2d_float_t*, float*);
	// wrapper for calloc
	void* xcalloc(int, size_t);

	if (fopen_s(&g_fp_meshFile, "monkey.obj", "r") != 0)
	{
		fprintf(grgpFile, "\n error while reading obj file");
		DestroyWindow(grghwnd);
	}
	gp_vertices = create_vec_2d_float();
	gp_texture = create_vec_2d_float();
	gp_normals = create_vec_2d_float();
	
	gp_face_tri = create_vec_2d_int();
	gp_face_texture = create_vec_2d_int();
	gp_face_normals = create_vec_2d_int();

	const char* sep_space = " ";
	const char* sep_fslash = "/";
	char* firstToken = NULL;
	char* token = NULL;
	char* faceTokens[NR_FACE_TOKENS];
	int nrTokens;
	char* tokenVertexIndex = NULL;
	char* tokenTextureIndex = NULL;
	char* tokenNormalIndex = NULL;

	while (fgets(g_line, BUFFER_SIZE, g_fp_meshFile) != NULL)
	{
		firstToken = strtok(g_line, sep_space);

		if (strcmp(firstToken, "v") == S_EQUAL)
		{
			GLfloat* pvec_point_coord = (GLfloat*)xcalloc(NR_POINT_COORDS, sizeof(GLfloat));
			for (int i = 0; i != NR_POINT_COORDS; i++)
			{
				pvec_point_coord[i] = atof(strtok(NULL, sep_space));
				
			}
			push_back_vec_2d_float(gp_vertices, pvec_point_coord);
		}

		else if (strcmp(firstToken, "vt") == S_EQUAL)
		{
			GLfloat* pvec_texture_coord = (GLfloat*)xcalloc(NR_TEXTURE_COORDS, sizeof(GLfloat));
			for (int i = 0; i != NR_TEXTURE_COORDS; i++)
			{
				pvec_texture_coord[i] = atof(strtok(NULL, sep_space));
			}
			push_back_vec_2d_float(gp_texture, pvec_texture_coord);
		}

		else if (strcmp(firstToken, "vn") == S_EQUAL)
		{
			GLfloat* pvec_normal_coord = (GLfloat*)xcalloc(NR_NORMAL_COORDS, sizeof(GLfloat));
			for (int i = 0; i != NR_NORMAL_COORDS; i++)
			{
				pvec_normal_coord[i] = atof(strtok(NULL, sep_space));
			}
			push_back_vec_2d_float(gp_normals, pvec_normal_coord);
		}

		else if (strcmp(firstToken, "f") == S_EQUAL)
		{
			GLint* pvecVertexIndices = (GLint*)xcalloc(3, sizeof(GLint));
			GLint* pvecTextureIndices = (GLint*)xcalloc(3, sizeof(GLint));
			GLint* pvecNormalIndices = (GLint*)xcalloc(3, sizeof(GLint));
			memset((void*)faceTokens, 0, NR_FACE_TOKENS);

			nrTokens = 0;
			while (token = strtok(NULL, sep_space))
			{
				if (strlen(token) < 3)
					break;
				faceTokens[nrTokens] = token;
				nrTokens++;
			}
		

			for (int i = 0; i != NR_FACE_TOKENS; ++i)
			{
				tokenVertexIndex = strtok(faceTokens[i], sep_fslash);
 				tokenTextureIndex = strtok(NULL, sep_fslash);
				tokenNormalIndex = strtok(NULL, sep_fslash);
				int tokenIndex = atoi(tokenVertexIndex);
				int textureIndex = atoi(tokenTextureIndex);
				int normalIndex = atoi(tokenNormalIndex);
				pvecVertexIndices[i] = tokenIndex;
				pvecTextureIndices[i] = textureIndex;
				pvecNormalIndices[i] = normalIndex;
				
			}
			push_back_vec_2d_int(gp_face_tri, pvecVertexIndices);
			push_back_vec_2d_int(gp_face_texture, pvecTextureIndices);
			push_back_vec_2d_int(gp_face_normals, pvecNormalIndices);
		}
		
		memset((void*)g_line, (int)'\0', BUFFER_SIZE);
		
		lineCount++;
	}

	fclose(g_fp_meshFile);
	g_fp_meshFile = NULL;
	
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
	mat4 grmodelViewMatrix = mat4::identity();
	mat4 grmodelViewProjectionMatrix = mat4::identity();
	mat4 grrotateMatrix = mat4::identity();
	mat4 grtranslateMatrix = mat4::identity();

	#pragma region pyramid
	//////////////// Triangle //////////////
	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	grrotateMatrix = vmath::rotate(grgAnglePyramid, 0.0f, 1.0f, 0.0f);
	grmodelViewMatrix = grtranslateMatrix * grrotateMatrix;

	// multiply modelview and orthographic matrix to get modelviewprojectionmatrix
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;
	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grstone_texture);
	glUniform1i(grgtextureSamplerUniform, 0);
	
	// bind vao
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(grgVao);
	glDrawArrays(GL_TRIANGLES, 0, gp_face_tri->size * 9);
	glBindVertexArray(0);
	#pragma endregion pyramid



	// stop use of shader program
	glUseProgram(0);

	//Update();
	SwapBuffers(grghdc);
}

void Update(void)
{
	// code
}

vec_2d_int_t* create_vec_2d_int(void)
{
	void* xcalloc(int, size_t);

	return((vec_2d_int_t*)xcalloc(1, sizeof(vec_2d_int_t)));
}

vec_2d_float_t *create_vec_2d_float(void)
{	
	void* xcalloc(int, size_t);

	return((vec_2d_float_t*)xcalloc(1, sizeof(vec_2d_float_t)));
}

void push_back_vec_2d_int(vec_2d_int_t* pVec, GLint* pArr)
{
	void* xrealloc(void *, size_t);

	pVec->pp_arr = (GLint**)xrealloc(pVec->pp_arr, (pVec->size + 1) * sizeof(int**));
	pVec->size++;
	pVec->pp_arr[pVec->size - 1] = pArr;
}

void push_back_vec_2d_float(vec_2d_float_t* pVec, GLfloat* pArr)
{
	void* xrealloc(void*, size_t);

	pVec->pp_arr = (GLfloat**)xrealloc(pVec->pp_arr, (pVec->size + 1) * sizeof(GLfloat**));
	pVec->size++;
	pVec->pp_arr[pVec->size - 1] = pArr;
}

void clean_vec_2d_int(vec_2d_int_t** ppVec)
{
	vec_2d_int_t* pVec = *ppVec;
	for (size_t i = 0; i != pVec->size; i++)
	{
		free(pVec->pp_arr[i]);
	}
	free(pVec->pp_arr);
	free(pVec);
	ppVec = NULL;
}

void clean_vec_2d_float(vec_2d_float_t** ppVec)
{
	vec_2d_float_t* pVec = *ppVec;
	for (size_t i = 0; i != pVec->size; i++)
	{
		free(pVec->pp_arr[i]);
	}
	free(pVec);
	ppVec = NULL;
}

void* xcalloc(int nrElements, size_t sizePerElement)
{

	void* p = calloc(nrElements, sizePerElement);
	if (!p)
	{
		fprintf(grgpFile, "\n Error in xcalloc, calloc fatal memory error");
		DestroyWindow(grghwnd);
	}
	return(p);
}

void* xrealloc(void* p, size_t newSize)
{
	void* ptr = realloc(p, newSize);
	if (!ptr)
	{
		fprintf(grgpFile, "\n Error in xrealloc, realloc out of memory error");
		DestroyWindow(grghwnd);
	}
	return(ptr);
}

void Uninitialize(void)
{
	void clean_vec_2d_float(vec_2d_float_t **ppVec);
	void clean_vec_2d_int(vec_2d_int_t **ppVec);

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
	// delete triangle vao and vbo
	if (grgVao)
	{
		glDeleteVertexArrays(1, &grgVao);
		grgVao = 0;
	}


	if (grgVboTexture)
	{
		glDeleteBuffers(1, &grgVboTexture);
		grgVboTexture = 0;
	}
	// delete square vao and vbo
	

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

	clean_vec_2d_float(&gp_vertices);
	clean_vec_2d_float(&gp_normals);
	clean_vec_2d_float(&gp_texture);

	clean_vec_2d_int(&gp_face_tri);
	clean_vec_2d_int(&gp_face_texture);
	clean_vec_2d_int(&gp_face_normals);

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









