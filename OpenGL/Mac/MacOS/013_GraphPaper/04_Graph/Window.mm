#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h> // analogous to windows.h or xlib.h
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>          // gl.h
#import"vmath.h"

using namespace vmath;

CVReturn myDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags, CVOptionFlags*, void*);

FILE *grgpFile = NULL;

@interface AppDelegate:NSObject <NSApplicationDelegate,NSWindowDelegate>
@end

int main (int argc, const char * argv[])
{
  // code
  NSAutoreleasePool *grpool = [[NSAutoreleasePool alloc] init]; // allocate memory and initialize
  NSApp = [NSApplication sharedApplication];                    // NSApp similar to HINSTANCE, get nsapp //by calling sharedApplication method
  [NSApp setDelegate : [[AppDelegate alloc] init]];     // setDelegate calls applicationDidFinishLaunching() which is similar to WM_CREATE
  
  // when application terminates, it calls NSApp's applicationWillTerminate() method
  
  // message loop
  [NSApp run];
  [grpool release];
  return 0;
}

@interface MyOpenGLView:NSOpenGLView            // declaration
@end

@implementation AppDelegate
{
    @private NSWindow *window;
    MyOpenGLView *gropenGLView;
}

-(void)applicationDidFinishLaunching:(NSNotification *) aNotification
{
    // code
    // log file
    NSBundle *appBundle = [NSBundle mainBundle];        // call static mainBundle() on NSBundle
    NSString *appDirPath = [appBundle bundlePath];      // get this application's directory(Window.app) path (path from route)
    NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
    NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/Log.txt", parentDirPath];
    // convert path to char*
    const char* pszLogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding :NSASCIIStringEncoding];
    // file creation
    grgpFile = fopen(pszLogFileNameWithPath, "w");
    if(grgpFile == NULL)
    {
        [self release];
        [NSApp terminate:self];
    }
    
    fprintf(grgpFile, "****Program started successfully****");
    
    NSRect win_rect = NSMakeRect(0.0, 0.0, 800.0, 600.0);   // NSRect is internally CGRect from CoreGraphics(C lang. library) and its a struct so no alloc
    window = [[NSWindow alloc] initWithContentRect:win_rect
                                styleMask:NSWindowStyleMaskTitled |
                                    NSWindowStyleMaskClosable |
                                    NSWindowStyleMaskMiniaturizable |
                                    NSWindowStyleMaskResizable
                                backing:NSBackingStoreBuffered
                                defer:NO];
    
    [window setTitle:@"GR OpenGL Window"];
    [window center];
    fprintf(grgpFile, "\n in init, window set to center");
    // create view
    gropenGLView = [[MyOpenGLView alloc]initWithFrame:win_rect];

    // apply view
    [window setContentView:gropenGLView];
        [window setDelegate:self];
    [window makeKeyAndOrderFront:self];         // same as setForegroundWindow, keep the window to front and set keyboard focus to my window
    
    
}

-(void)applicationWillTerminate:(NSNotification *) aNotification
{
    // code
    if(grgpFile)
    {
        fprintf(grgpFile, "\n Program terminated successfully");
        fclose(grgpFile);
        grgpFile = NULL;
    }
}

-(void)windowWillClose:(NSNotification *) aNotification
{
    // code
    [NSApp terminate:self];
}

-(void)dealloc
{
    // code
    [gropenGLView release];
    [window release];
    [super dealloc];
}
@end

@implementation MyOpenGLView
{
    @private
    CVDisplayLinkRef displayLink;
    
    enum
    {
        GR_ATTRIBUTE_POSITION = 0,
        GR_ATTRIBUTE_COLOR,
        GR_ATTRIBUTE_TEXCOORD,
        GR_ATTRIBUTE_NORMAL
    };
    
    GLuint grgVertexShaderObject;
    GLuint grgFragmentShadeerObject;
    GLuint grgShaderProgramObject;
    GLuint grgVao;
    GLuint grgVbo_position;
    GLuint grgMvpMatrixUniform;
    mat4 grgPerspectiveMatrix;
    GLfloat grVerticalLineVertices1[80];
    GLfloat grVerticalLineVertices2[80];
    GLfloat grVerticalCenterLineVertices[4];
    GLfloat grHorizontalLineVerts1[80];
    GLfloat grHorizontalLineVerts2[80];
    GLfloat grHorizontalCenterLineVerts[4];
    
}

-(id)initWithFrame:(NSRect) frame
{
    // code
    self = [super initWithFrame:frame];
    if(self)
    {
        NSOpenGLPixelFormatAttribute attributes[] =      // resembles with pfd or framebufferattributes from linux
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,                         // pfa = piexl format attrib , 4.1 core profile [here if we give profile = NSOpenGLProfileVersionLegacy, then we can run ffp programs in mac]
            NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
            NSOpenGLPFANoRecovery,
            NSOpenGLPFAAccelerated,                                                         // hw rendering
            NSOpenGLPFAColorSize, 24,                                                       // RGB - 8,8,8
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFADoubleBuffer,
            0                                                                           // similar to None in framebufferattributes
        };
        
        // get pixelFormat
        NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes]autorelease];
        if(pixelFormat == nil)
        {
            fprintf(grgpFile, "\n Cannot get pixel format. Exiting Now..");
            [self release];
            [NSApp terminate:self];
        }
        
        // get context from pixelformat
        NSOpenGLContext *glContext = [[[NSOpenGLContext alloc]initWithFormat:pixelFormat shareContext:nil]autorelease];        // same as sharing context null in xwindows
        
        // set pixelFormat
        [self setPixelFormat:pixelFormat];
        // set context
        [self setOpenGLContext:glContext];
    }
    return self;
}

-(CVReturn)getFrameForTime:(const CVTimeStamp*) outputTime
{
    // code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];         // create another thread (multithrading application)
    [self drawView];
    [pool release];
    return(kCVReturnSuccess);
}

-(void) prepareOpenGL
{
    // code
    [super prepareOpenGL];
    [[self openGLContext]makeCurrentContext];
    
    // swap interval
    GLint swapInt = 1;                                  // swapInterval
    [[self openGLContext]setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];   // set value of nsopengl's context parameters's swap interval to saapInt
    
    // shaders code will be written here
    grgVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    ///// Vertex Shader
    const GLchar* grvertexShaderSourceCode =
        "#version 410 core" \
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
                [self release];
                [NSApp terminate:self];
            }
        }
    }

    ////// Fragment Shader
    grgFragmentShadeerObject = glCreateShader(GL_FRAGMENT_SHADER);

    // source code of shader
    const GLchar* grfragmentShaderSourceCode =
        "#version 410 core" \
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
                [self release];
                [NSApp terminate:self];
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
                [self release];
                [NSApp terminate:self];
            }
        }
    }

    grgMvpMatrixUniform = glGetUniformLocation(grgShaderProgramObject, "u_mvpMatrix");
   
    GLfloat i = 0.11f;
    GLint index = 0;

    for (index = 0; index < 80; index = index + 4)
    {
            grVerticalLineVertices1[index] = i;
            grVerticalLineVertices1[index + 1] = -2.0f;
            grVerticalLineVertices1[index + 2] = i;
            grVerticalLineVertices1[index + 3] = 2.0f;
            i = i + 0.11f;
    }
    i = 0.11f;
    for (index = 0; index < 80; index = index + 4)
    {
            grVerticalLineVertices2[index] = -i;
            grVerticalLineVertices2[index + 1] = -2.0f;
            grVerticalLineVertices2[index + 2] = -i;
            grVerticalLineVertices2[index + 3] = 2.0f;
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
    
    
    // initialize vars
    grVerticalCenterLineVertices[0] = 0.0f;
    grVerticalCenterLineVertices[1] = -2.0f;
    grVerticalCenterLineVertices[2] = 0.0f;
    grVerticalCenterLineVertices[3] = 2.0f;
    grHorizontalCenterLineVerts[0] = -3.0f;
    grHorizontalCenterLineVerts[1] = 0.0f;
    grHorizontalCenterLineVerts[2] = 3.0f;
    grHorizontalCenterLineVerts[3] = 0.0f;
    
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    grgPerspectiveMatrix = mat4::identity();
    
    //*** CV and CG related code
    // create display link
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    // set callback of display link
    CVDisplayLinkSetOutputCallback(displayLink, &myDisplayLinkCallback, self);
    // take nsopengl's context to cgl context
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext]CGLContextObj];
    // nsopengl pixel format convert to cglpixel format
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat]CGLPixelFormatObj];
    //set cgl's display and cgl's current context
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    // start
    CVDisplayLinkStart(displayLink);
}

-(void) reshape
{
    // code
    [super reshape];
    CGLLockContext((CGLContextObj) [[self openGLContext] CGLContextObj]);
    // get window width and height
    NSRect rect = [self bounds];
    if(rect.size.height < 0)
    {
        rect.size.height = 1;
    }
    grgPerspectiveMatrix = vmath::perspective(45.0f, (GLfloat)rect.size.width / (GLfloat)rect.size.height, 0.1f, 100.0f);
    glViewport(0, 0, (GLsizei) rect.size.width, (GLsizei) rect.size.height);
    CGLUnlockContext((CGLContextObj) [[self openGLContext] CGLContextObj]);
}

-(void)drawRect:(NSRect) dirtyRect                          // simiar to WM_PAINT
{
    [self drawView];
}

-(void) drawView
{
    // code
    [[self openGLContext]makeCurrentContext];
    CGLLockContext((CGLContextObj) [[self openGLContext] CGLContextObj]);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // OpenGL drawing code will start here
    mat4 grmodelViewMatrix = mat4::identity();
    mat4 grmodelViewProjectionMatrix = mat4::identity();
    mat4 grtranslateMatrix = mat4::identity();
    
    glUseProgram(grgShaderProgramObject);
    //******************** **************************
    grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
    grmodelViewMatrix = grtranslateMatrix;
    grmodelViewProjectionMatrix = grgPerspectiveMatrix * grmodelViewMatrix;

    glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

    // bind vao
   glBindVertexArray(grgVao);
    glGenBuffers(1, &grgVbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
    glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), grVerticalLineVertices1, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
    glDrawArrays(GL_LINES, 0, 40);
    glBindVertexArray(0);

    //******************** Line - (center) **************************
    
    grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
    grmodelViewMatrix = grtranslateMatrix;
    grmodelViewProjectionMatrix = grgPerspectiveMatrix * grmodelViewMatrix;

    glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

    // bind vao
    glBindVertexArray(grgVao);
    glGenBuffers(1, &grgVbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
    glBufferData(GL_ARRAY_BUFFER, 2 * 2 * sizeof(GLfloat), grVerticalCenterLineVertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    //******************** Horizontal Line - (top) **************************
    grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
    grmodelViewMatrix = grtranslateMatrix;
    grmodelViewProjectionMatrix = grgPerspectiveMatrix * grmodelViewMatrix;

    glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

    // bind vao
    glBindVertexArray(grgVao);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
    glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), grVerticalLineVertices2, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
    glDrawArrays(GL_LINES, 0, 40);
    glBindVertexArray(0);

    //**** HORIZONTAL LINES
    //******************** **************************
     grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
     grmodelViewMatrix = grtranslateMatrix;
     grmodelViewProjectionMatrix = grgPerspectiveMatrix * grmodelViewMatrix;

     glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

     // bind vao
    glBindVertexArray(grgVao);
     glGenBuffers(1, &grgVbo_position);
     glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
     glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), grHorizontalLineVerts1, GL_DYNAMIC_DRAW);
     glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
     glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
     glDrawArrays(GL_LINES, 0, 40);
     glBindVertexArray(0);

     //******************** Line - (center) **************************
     
     grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
     grmodelViewMatrix = grtranslateMatrix;
     grmodelViewProjectionMatrix = grgPerspectiveMatrix * grmodelViewMatrix;

     glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

     // bind vao
     glBindVertexArray(grgVao);
     glGenBuffers(1, &grgVbo_position);
     glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
     glBufferData(GL_ARRAY_BUFFER, 2 * 2 * sizeof(GLfloat), grHorizontalCenterLineVerts, GL_DYNAMIC_DRAW);
     glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
     glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 0.0f, 0.0f);
     glDrawArrays(GL_LINES, 0, 4);
     glBindVertexArray(0);

     //******************** Horizontal Line - (top) **************************
     grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
     grmodelViewMatrix = grtranslateMatrix;
     grmodelViewProjectionMatrix = grgPerspectiveMatrix * grmodelViewMatrix;

     glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

     // bind vao
     glBindVertexArray(grgVao);
     glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
     glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), grHorizontalLineVerts2, GL_DYNAMIC_DRAW);
     glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
     glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
     glDrawArrays(GL_LINES, 0, 40);
     glBindVertexArray(0);
    // stop use of shader program
    glUseProgram(0);
    [self updateFn];
    // buffer 
    CGLFlushDrawable((CGLContextObj) [[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj) [[self openGLContext] CGLContextObj]);
}

-(void) updateFn
{
    // code
}

- (BOOL)acceptsFirstResponder
{
    [[self window]makeFirstResponder:nil];
    return YES;
}

-(void) keyDown:(NSEvent *) theEvent
{
    int key = [[theEvent characters]characterAtIndex:0];
    switch(key)
    {
        case 27:                    // escape key
            [self release];
            [NSApp terminate:self];
            break;
            
        case 'F':
        case 'f':
            [[self window]toggleFullScreen:self];
            break;
    }
}

-(void) mouseDown:(NSEvent *) theEvent
{
   
}

-(void) rightMouseDown:(NSEvent *) theEvent
{
    // code
  
}

-(void) otherMouseDown:(NSEvent *) theEvent
{
   
}

-(void) dealloc
{
    
    CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);
    [self uninitlialize];
    [super dealloc];
}

-(void) uninitlialize
{
    
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
}
@end

// as callback function is C-style function its written in global space
CVReturn myDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn,
    CVOptionFlags* flagsOut, void* displayLinkContext)                              // last param is myopenglview pointer
{
    // code
    CVReturn result = [(MyOpenGLView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}









