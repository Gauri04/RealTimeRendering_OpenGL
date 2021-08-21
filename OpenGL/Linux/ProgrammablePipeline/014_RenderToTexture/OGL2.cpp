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

GLuint grgMvpMatrixUniform;
mat4 grgPerspectiveProjectionMatrix;

GLuint grgVao_pyramid;								// we will create as many Vbo's as glBegins, glEnds (one vbo for one glBegin glEnd)
GLuint grgVao_cube;
GLuint grgVbo_position_cube;
GLuint grgVbo_color_cube;
GLfloat grgAngleCube = 0.0f;

GLuint grgFbo;
GLuint grgColorTexture;
GLuint grgDepthTexture;
vec3 grgColorGreen = { 0.0f, 0.0f, 0.0f };
vec3 grgColorBlue = { 0.0f, 0.0f, 1.0f };
GLfloat grgOne = 1;

// shader for texture (for default framebuffer/ outer cube) 
GLuint grgVertexShaderObject2;
GLuint grgFragmentShadeerObject2;
GLuint grgShaderProgramObject2;

GLuint grgVao_cube2;
GLuint grgVbo_position_cube2;
GLuint grgVbo_texture_cube2;

GLuint grgtextureSamplerUniform2;
GLuint grgtexture2;

GLuint grgMvpMatrixUniform2;
int grgWidth, grgHeight;


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
		"in vec4 vColor;" \
		"in vec2 vTexCoord;" \
		"uniform mat4 u_mvpMatrix;" \
		"out vec4 out_color;" \
		"out vec2 out_texcoord;" \
		"void main(void)" \
		"{" \
		"gl_Position = u_mvpMatrix * vPosition;" \
		"out_color = vColor;" \
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
		"in vec4 out_color;"
		"in vec2 out_texcoord;"
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
				printf("\n Shader Program Link Log : %s", grszInfoLog);
				Uninitialize();
			}
		}
	}

	grgMvpMatrixUniform = glGetUniformLocation(grgShaderProgramObject, "u_mvpMatrix");


	// ****************************** Shader program for outer code **********************************************
	grgVertexShaderObject2 = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(grgVertexShaderObject2, 1, (const GLchar **) &grvertexShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgVertexShaderObject2);
	// error check for compilation
	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	glGetShaderiv(grgVertexShaderObject2, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgVertexShaderObject2, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgVertexShaderObject2, griInfoLength, &grwritten, grszInfoLog);
				printf("\n Vertex Shader 2 Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				Uninitialize();
			}
		}
	}

	////// Fragment Shader
	grgFragmentShadeerObject2 = glCreateShader(GL_FRAGMENT_SHADER);

	// source code of shader
	const GLchar* grfragmentShaderSourceCode2 =
		"#version 450 core" \
		"\n" \
		"in vec2 out_texcoord;" \
		"in vec4 out_color;" \
		"uniform sampler2D u_texture_sampler;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = texture(u_texture_sampler, out_texcoord);" \
		"}";

	glShaderSource(grgFragmentShadeerObject2, 1, (const GLchar **) &grfragmentShaderSourceCode2, NULL);

	// compile shader
	glCompileShader(grgFragmentShadeerObject2);
	// error check for compiation
	glGetShaderiv(grgFragmentShadeerObject2, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgFragmentShadeerObject2, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgFragmentShadeerObject2, griInfoLength, &grwritten, grszInfoLog);
				printf("\n Fragment Shader 2 Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				Uninitialize();
			}
		}
	}

	//****** Shader Program *****//
	// create
	grgShaderProgramObject2 = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(grgShaderProgramObject2, grgVertexShaderObject2);

	// attach fragment shader to shader program
	glAttachShader(grgShaderProgramObject2, grgFragmentShadeerObject2);

	// pre-link our attribute enum with shader's attributes
	glBindAttribLocation(grgShaderProgramObject2, GR_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(grgShaderProgramObject2, GR_ATTRIBUTE_TEXCOORD, "vTexCoord");

	// link shader
	glLinkProgram(grgShaderProgramObject2);
	// error check for linking
	griShaderProgramLinkStatus = 0;
	glGetProgramiv(grgShaderProgramObject2, GL_LINK_STATUS, &griShaderProgramLinkStatus);
	if (griShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(grgShaderProgramObject2, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(griInfoLength) * sizeof(char));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetProgramInfoLog(grgShaderProgramObject2, griInfoLength, &grwritten, grszInfoLog);
				printf("\n Shader Program 2 Link Log : %s", grszInfoLog);
				Uninitialize();
			}
		}
	}

	grgMvpMatrixUniform2 = glGetUniformLocation(grgShaderProgramObject2, "u_mvpMatrix");

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
	
	const GLfloat grcubeColors[] =		
	{
		1.0f, 0.0f, 0.0f,					// one color for single surface 
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	const GLfloat grcubeTexCoords[] =
	{
		0.0f, 0.0f,					// one color for single surface 
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};

	glGenVertexArrays(1, &grgVao_cube);
	glBindVertexArray(grgVao_cube);

	glGenBuffers(1, &grgVbo_position_cube);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grcubeVertices), grcubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &grgVbo_color_cube);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_color_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grcubeColors), grcubeColors, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &grgVbo_texture_cube2);
	glBindBuffer(GL_ARRAY_BUFFER, grgVbo_texture_cube2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grcubeTexCoords), grcubeTexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_TEXCOORD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	// ********************************************* FBO ***************************************
	glCreateFramebuffers(1, &grgFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, grgFbo);

	// create texture for color buffer
	glGenTextures(1, &grgColorTexture);
	glBindTexture(GL_TEXTURE_2D, grgColorTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// create texture for depth bufffer
	glGenTextures(1, &grgDepthTexture);
	glBindTexture(GL_TEXTURE_2D, grgDepthTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 512, 512);

	// attach color and depth texture to fbo
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, grgColorTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, grgDepthTexture, 0);

	static const GLenum grdrawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, grdrawBuffers);


	GLenum status;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		printf("\n framebuffer status successful %d", status);
	}
	glBindBuffer(GL_FRAMEBUFFER, 0);

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
	grgWidth = width;
	grgHeight = height;
	grgPerspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 100.0f);
}

void Draw(void)
{
	// function declaration
	void Update();

	// variables
	mat4 grmodelViewMatrix = mat4::identity();
	mat4 grmodelViewProjectionMatrix = mat4::identity();
	mat4 grrotateMatrix = mat4::identity();
	mat4 grtranslateMatrix = mat4::identity();
	mat4 grscaleMatrix = mat4::identity();

	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glBindFramebuffer(GL_FRAMEBUFFER, grgFbo);

	// set the viewport, clear depth and color buffers
	glViewport(0, 0, 512, 512);
	glClearBufferfv(GL_COLOR, 0, grgColorGreen);
	glClearBufferfv(GL_DEPTH, 0, &grgOne);

	glUseProgram(grgShaderProgramObject);

	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	//grscaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
	grrotateMatrix = vmath::rotate(grgAngleCube, 1.0f, 0.0f, 0.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grgAngleCube, 0.0f, 1.0f, 0.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grgAngleCube, 0.0f, 0.0f, 1.0f);
	grmodelViewMatrix = grtranslateMatrix  * grrotateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);
	// bind vao of square
	glBindVertexArray(grgVao_cube);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);												// In Programmable pipeline, there's no GL_QUADS, hence we have used GL_TRIANGLE_FAN
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
	// unbind vao
	glBindVertexArray(0);

	// *********** FBO ******************************************************
	// ************************************************************************8
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, grgWidth, grgHeight);
	glClearBufferfv(GL_COLOR, 0, grgColorBlue);
	glClearBufferfv(GL_DEPTH, 0, &grgOne);

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grgColorTexture);
	

	glUseProgram(grgShaderProgramObject2);

	grtranslateMatrix = mat4::identity();
	grrotateMatrix = mat4::identity();
	grmodelViewMatrix = mat4::identity();
	grmodelViewProjectionMatrix = mat4::identity();

	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	grrotateMatrix = vmath::rotate(grgAngleCube, 0.0f, 1.0f, 0.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grgAngleCube, 0.0f, 1.0f, 0.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grgAngleCube, 0.0f, 0.0f, 1.0f);
	grmodelViewMatrix = grtranslateMatrix * grrotateMatrix;
	grmodelViewProjectionMatrix = grgPerspectiveProjectionMatrix * grmodelViewMatrix;

	glUniformMatrix4fv(grgMvpMatrixUniform2, 1, GL_FALSE, grmodelViewProjectionMatrix);
	//glUniform1i(grgtextureSamplerUniform2, 0);
	// bind vao of square
	glBindVertexArray(grgVao_cube);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);												// In Programmable pipeline, there's no GL_QUADS, hence we have used GL_TRIANGLE_FAN
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
	// unbind vao
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glUseProgram(0);

	Update();
	glXSwapBuffers(gpDisplay, gWindow);

}


void Update()
{
	grgAngleCube = grgAngleCube + 0.02f;
	if (grgAngleCube >= 360.0f)
		grgAngleCube = 0.0f;
}

void Uninitialize()
{
	int i;

	
	if (grgVbo_color_cube)
	{
		glDeleteBuffers(1, &grgVbo_color_cube);
		grgVbo_color_cube = 0;
	}
	if (grgVbo_position_cube)
	{
		glDeleteBuffers(1, &grgVbo_position_cube);
		grgVbo_position_cube = 0;
	}
	if (grgVao_cube)
	{
		glDeleteVertexArrays(1, &grgVao_cube);
		grgVao_cube = 0;
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







































