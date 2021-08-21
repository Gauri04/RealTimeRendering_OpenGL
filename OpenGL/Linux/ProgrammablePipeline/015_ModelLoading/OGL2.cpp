#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>
#include<GL/glew.h>
#include<GL/gl.h>		// for opengl api's 
#include<GL/glx.h>		// for glx apis
#include"vmath.h"


using namespace std;
using namespace vmath;


#define BUFFER_SIZE 256
#define S_EQUAL 0
#define NR_POINT_COORDS 3
#define NR_TEXTURE_COORDS 2
#define NR_NORMAL_COORDS 3
#define NR_FACE_TOKENS 3
#define NR_TRIANGLE_VERTICES 3

bool bFullscreen = false;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColormap;
Window gWindow;

GLXContext gglxContext;						// similar to HGLRC of windows (handle to rc)

int giWindowWidth = 800;
int giWindowHeight = 600;
static XFontStruct *grpXFontStruct = NULL;
XGCValues grgcValues;						// similar to PAINTSTRUCT ps
XColor grgreenColor;
int grstringLength;
int grstringWidth;
int grfontHeight;


// project specific global variables declaration
GLfloat grfangle;
typedef GLXContext(* glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int *);
glXCreateContextAttribsARBProc gglxCreateContextAttribsARB = NULL;
GLXFBConfig gglXFBConfig;

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

int main()
{
	void CreateWindow();
	void ToggleFullscreen();
	void Uninitialize();
	void Initialize(void);
	void Resize(int, int);
	void Draw(void);

	int winWidth = giWindowWidth;
	int winHeight = giWindowHeight;
	bool bDone = false;
	
	CreateWindow();
	Initialize();
	
	XEvent event;
	KeySym keysym;
	
	while(bDone == false)
	{
		while(XPending(gpDisplay))
		{
			XNextEvent(gpDisplay, &event);
			switch(event.type)
			{
				case MapNotify:
					grpXFontStruct = XLoadQueryFont(gpDisplay, "Fixed");
					break;
				
				case KeyPress:
					keysym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
					switch(keysym)
					{
						case XK_Escape :
							XUnloadFont(gpDisplay, grpXFontStruct->fid);
							bDone = true;
							break;
						
						case XK_F :
						case XK_f :
							if(bFullscreen == false)
							{
								ToggleFullscreen();
								bFullscreen = true;
							}
							else
							{
								ToggleFullscreen();
								bFullscreen = false;
								
							}
							break;

						
						default :
							break;
					}
					break;
				
				case ButtonPress :
					switch(event.xbutton.button)
					{
						case 1:
							break;
						case 2 :
							break;
						case 3 :
							break;
						default :
							break;
					}
					break;
				
				case MotionNotify:
					break;
				
				case ConfigureNotify:								// similar to WM_SIZE
					winWidth = event.xconfigure.width;
					winHeight = event.xconfigure.height;
					Resize(winWidth, winHeight);
					break;
				
				case Expose:										// similar to WM_PAINT
					break;
				
				case DestroyNotify:
					break;
				
				case 33:
					XUnloadFont(gpDisplay, grpXFontStruct->fid);
					bDone = true;

				default :
					break;
			}
		}
		Draw();
	}
	
	Uninitialize();
	return(0);
}


void CreateWindow()
{
	// function declaration
	void Uninitialize();
	
	XSetWindowAttributes winAttribs;
	int defaultScreen;
	int defaultDepth;
	int styleMask;
	static int framebufferAttributes[] = 				// similar to PixelFormatAttributes of windows (static declaration is conventional not compulsary)
	{
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,								// v4l recommentds bitsize of depth to be ketp around 24 or below (v4l = video for linux)
		GLX_STENCIL_SIZE, 8,
		GLX_DOUBLEBUFFER, True,
		None												// to indicate unix systems to set next attributes to default, insert 0 / None at end 
	};
	GLXFBConfig *pGLXFBConfig = NULL;
	GLXFBConfig bestGLXFBConfig;
	XVisualInfo *pTempXVisualInfo = NULL;
	int numFBConfigs = 0;
	int i;

	// code
	gpDisplay = XOpenDisplay(NULL);
	if(gpDisplay == NULL)
	{
		printf("\n Unable to open XDisplay.. Exiting now...");
		Uninitialize();
		exit(1);
	}
	
	defaultScreen = XDefaultScreen(gpDisplay);
	//defaultDepth =DefaultDepth(gpDisplay, defaultScreen);		// we will not be using defaultdepth of xVisual


	//gpXVisualInfo = (XVisualInfo *) malloc(sizeof(XVisualInfo));		// we will get visualInfo using opengl bridging api

	//gpXVisualInfo = glXChooseVisual(gpDisplay, defaultScreen, framebufferAttributes);	//opengl bridging api similar to wgl* from windows

	pGLXFBConfig = glXChooseFBConfig(gpDisplay, defaultScreen, framebufferAttributes, &numFBConfigs);
	printf("\n Total Framebuffer Configs : %d", numFBConfigs);

	int bestFramebufferConfig = -1;
	int worstFramebufferConfig = -1;
	int bestNumberOfSamples = -1;
	int worstNumberOfSamples = 999;

	for(i = 0; i < numFBConfigs; i++)
	{
		pTempXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, pGLXFBConfig[i]);
		if(pTempXVisualInfo != NULL)
		{
			int sampleBuffers, samples;
			glXGetFBConfigAttrib(gpDisplay, pGLXFBConfig[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
			glXGetFBConfigAttrib(gpDisplay, pGLXFBConfig[i], GLX_SAMPLES, &samples);

			if(bestFramebufferConfig < 0 || sampleBuffers && samples > bestNumberOfSamples)
			{
				bestFramebufferConfig = i;
				bestNumberOfSamples = samples;
			}
			if(worstFramebufferConfig < 0 || !sampleBuffers || samples < worstNumberOfSamples)
			{
				worstFramebufferConfig = i;
				worstNumberOfSamples = samples;
			}
			printf("\n When i : %d, sampleBuffers : %d, samples : %d", i, sampleBuffers, samples);
		}
		XFree(pTempXVisualInfo);

	}

	bestGLXFBConfig = pGLXFBConfig[bestFramebufferConfig];
	gglXFBConfig = bestGLXFBConfig;
	XFree(pGLXFBConfig);

	gpXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, bestGLXFBConfig);

	if(gpXVisualInfo == NULL)
	{
		printf("\n Unable to allocate mamory for visual info.. Exiting now..");
		Uninitialize();
		exit(1);
	}
	
	XMatchVisualInfo(gpDisplay, defaultScreen, defaultDepth, TrueColor, gpXVisualInfo);
	
	if(gpXVisualInfo == NULL)
	{
		printf("\n Unable to get a visual.. Exiting now..");
		Uninitialize();
		exit(1);
	}
	
	
	winAttribs.border_pixel = 0;
	winAttribs.background_pixmap = 0;
	winAttribs.colormap = XCreateColormap(gpDisplay, 
					RootWindow(gpDisplay, gpXVisualInfo->screen),
					gpXVisualInfo->visual,
					AllocNone);
	gColormap = winAttribs.colormap;
	winAttribs.background_pixel = BlackPixel(gpDisplay, defaultScreen);
	
	winAttribs.event_mask = ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;
	
	styleMask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;
	
	gWindow = XCreateWindow(gpDisplay,
					RootWindow(gpDisplay, gpXVisualInfo->screen),
					0,
					0,
					giWindowWidth,
					giWindowHeight,
					0,
					gpXVisualInfo->depth,
					InputOutput,
					gpXVisualInfo->visual,
					styleMask,
					&winAttribs);
					
	if(!gWindow)
	{
		printf("\n Error : Failed to create Main Window. Exiting Now ...");
		Uninitialize();
		exit(1);	
	}
	
	XStoreName(gpDisplay, gWindow, "OGL Blue Window");

	Atom windowManagerDelete = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(gpDisplay, gWindow, &windowManagerDelete, 1);
	XMapWindow(gpDisplay, gWindow);
	
}


void ToggleFullscreen()
{
	Atom wm_state;
	Atom fullscreen;
	XEvent xev = {0};
	
	wm_state = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
	memset(&xev, 0, sizeof(xev));
	
	xev.type = ClientMessage;
	xev.xclient.window = gWindow;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = bFullscreen ? 0 : 1;
	
	fullscreen = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
	xev.xclient.data.l[1] = fullscreen;
	
	XSendEvent(gpDisplay, 
			RootWindow(gpDisplay, gpXVisualInfo->screen),
			False,
			StructureNotifyMask,
			&xev);
}

void Initialize()
{
	// function declaration
	void Resize(int, int);
	void Uninitialize();
	void LoadMeshData(void);

	gglxCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB((GLubyte *) "glXCreateContextAttribsARB");

	const int attribs[] = 
	{
		GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
		GLX_CONTEXT_MINOR_VERSION_ARB, 5,
		GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		None
	};

	gglxContext = gglxCreateContextAttribsARB(gpDisplay, gglXFBConfig, 0, True, attribs);
	if(!gglxContext)
	{
		int attribsLow[] = 
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
			GLX_CONTEXT_MINOR_VERSION_ARB, 0,
			None
		};
		gglxContext = gglxCreateContextAttribsARB(gpDisplay, gglXFBConfig, 0, True, attribs);
		printf("\n Context for version 4.5 not found");
	}

	Bool bIsDirectContext = glXIsDirect(gpDisplay, gglxContext);
	if(bIsDirectContext == true)
	{
		printf("\n Context is direct hardware rendering context");
	}
	else
	{
		printf("\n Context is software rendering context");
	}

	glXMakeCurrent(gpDisplay, gWindow, gglxContext);		// similar to wglMakeColor()

	GLenum grglew_error = glewInit();
	if (grglew_error != GLEW_OK)
	{ 
		Uninitialize();
	}

	/********* Shader Code *********/

	// create shader
	grgVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	///// Vertex Shader
	const GLchar* grvertexShaderSourceCode =
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
				printf("\n Vertex Shader Compilation Log : %s", grszInfoLog);
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
				printf("\n Fragment Shader Compilation Log : %s", grszInfoLog);
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
				printf("\n Shader Program Link Log : %s", grszInfoLog);
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
			/*printf("\n vertices[%d] : %f \t vertices[%d] : %f \t vertices[%d] : %f", index + 0, vertices[index + 0],
					index + 1, vertices[index + 1], index + 2, vertices[index + 2]);
			*/
					
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
	/*
	glGenBuffers(1, &grgVboTexture);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboTexture);
	glBufferData(GL_ARRAY_BUFFER, gp_face_texture->size * 9 * sizeof(GLfloat), textures, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_TEXCOORD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/
	glBindVertexArray(0);

	free(vertices);
	vertices = NULL;
	free(textures);
	textures = NULL;

	// Depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	grgPerspectiveProjectionMatrix = mat4::identity();	
	// warm - up call to resize
	Resize(giWindowWidth, giWindowHeight);

}

void Resize(int width, int height)
{
	if(height ==0)
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

	g_fp_meshFile = fopen("monkey.obj", "r");
	if (g_fp_meshFile != 0)
	{
		printf("grgpFile, ""\n error while reading obj file");
		exit(1);
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

void Draw(void)
{
	// function declaration
	void Update();

	// variable declaration
	mat4 grmodelViewMatrix = mat4::identity();
	mat4 grmodelViewProjectionMatrix = mat4::identity();
	mat4 grrotateMatrix = mat4::identity();
	mat4 grtranslateMatrix = mat4::identity();

	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// start use of shader program
	glUseProgram(grgShaderProgramObject);

	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	grrotateMatrix = vmath::rotate(grgAnglePyramid, 0.0f, 1.0f, 0.0f);
	grmodelViewMatrix = grtranslateMatrix * grrotateMatrix;

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


	// stop use of shader program
	glUseProgram(0);
	Update();
	glXSwapBuffers(gpDisplay, gWindow);

}

void Update(void)
{
	grgAnglePyramid = grgAnglePyramid + 0.1f;
	if (grgAnglePyramid >= 360.0f)
		grgAnglePyramid = 0.0f;
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
		printf("\n Error in xcalloc, calloc fatal memory error");
		exit(1);
	}
	return(p);
}

void* xrealloc(void* p, size_t newSize)
{
	void* ptr = realloc(p, newSize);
	if (!ptr)
	{
		printf("\n Error in xrealloc, realloc out of memory error");
		exit(1);
	}
	return(ptr);
}

void Uninitialize()
{
	// function declaration
	void clean_vec_2d_float(vec_2d_float_t **ppVec);
	void clean_vec_2d_int(vec_2d_int_t **ppVec);

	int i;

	if (grgVboPosition)
	{
		glDeleteBuffers(1, &grgVboPosition);
		grgVboPosition = 0;
	}
	if (grgVao)
	{
		glDeleteVertexArrays(1, &grgVao);
		grgVao = 0;
	}
	
	// free shaders (safe release)
	if(grgShaderProgramObject)
	{
		glUseProgram(grgShaderProgramObject);
		GLsizei grshaderCount;
		glGetProgramiv(grgShaderProgramObject, GL_ATTACHED_SHADERS, &grshaderCount);
		GLuint *grpShaders = NULL;
		grpShaders = NULL;
		grpShaders = (GLuint *)malloc(sizeof(GLuint) * grshaderCount);
		glGetAttachedShaders(grgShaderProgramObject, grshaderCount, &grshaderCount, grpShaders);
		for(i = 0; i < grshaderCount; i++)
		{
			glDetachShader(grgShaderProgramObject, grpShaders[i]);
			glDeleteShader(grpShaders[i]);
			grpShaders[i] = 0;
		}
		// unlink shader program
		glUseProgram(0);
	}

	clean_vec_2d_float(&gp_vertices);
	clean_vec_2d_float(&gp_normals);
	clean_vec_2d_float(&gp_texture);

	clean_vec_2d_int(&gp_face_tri);
	clean_vec_2d_int(&gp_face_texture);
	clean_vec_2d_int(&gp_face_normals);

	GLXContext currentGLXContext = glXGetCurrentContext();			// in unix we can have multiple monitors, multiple screens hence check if we are uninitializing current context only
	if(currentGLXContext == gglxContext)
	{
		glXMakeCurrent(gpDisplay, 0, 0);
		if(gglxContext)
		{
			glXDestroyContext(gpDisplay, gglxContext);
		}

	}



	if(gWindow)
	{
		XDestroyWindow(gpDisplay, gWindow);
	}
	
	if(gColormap)
	{
		XFreeColormap(gpDisplay, gColormap);	
	}
	
	if(gpXVisualInfo)
	{
		free(gpXVisualInfo);
		gpXVisualInfo = NULL;
	}
	if(gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}

}

/* compile and linking command //
g++ -o OGL2 OGL2.cpp -lGL -lX11 -lGLEW
*/








































