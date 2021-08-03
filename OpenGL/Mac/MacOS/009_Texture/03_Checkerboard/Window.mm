#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h> // analogous to windows.h or xlib.h
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>          // gl.h
#import"vmath.h"

using namespace vmath;
#define CHECK_IMAGE_WIDTH 64
#define CHECK_IMAGE_HEIGHT 64

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
    GLuint grgVao_square;
    GLuint grgVbo_position_square;
    GLuint grgVbo_texture_square;
    GLuint grgMvpMatrixUniform;
    mat4 grgPerspectiveMatrix;
    
    GLuint grcheckerboard_texture;
    GLuint grgtextureSamplerUniform;
    GLfloat grsquareOneVertices[12];
    GLfloat grsquareTwoVertices[12];
    GLubyte checkImage[CHECK_IMAGE_HEIGHT][CHECK_IMAGE_WIDTH][4];
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
    const GLchar* grvertexShaderSourceCode =                    // also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
        "#version 410 core" \
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
        "in vec2 out_texcoord;"
        "uniform sampler2D u_texture_sampler;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "FragColor = texture(u_texture_sampler, out_texcoord);" \
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
                [self release];
                [NSApp terminate:self];
            }
        }
    }

    grgMvpMatrixUniform = glGetUniformLocation(grgShaderProgramObject, "u_mvpMatrix");
    grgtextureSamplerUniform = glGetUniformLocation(grgShaderProgramObject, "u_texture_sampler");
    
    const GLfloat grsquareTexCoord[] =
    {
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };
    
    
    glGenVertexArrays(1, &grgVao_square);
    glBindVertexArray(grgVao_square);

    glGenBuffers(1, &grgVbo_position_square);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position_square);
    glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // color for square
   glGenBuffers(1, &grgVbo_texture_square);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_texture_square);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grsquareTexCoord), grsquareTexCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_TEXCOORD);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    grgPerspectiveMatrix = mat4::identity();
    
    // initialize variables
    //CHECK_IMAGE_HEIGHT = 64;
    //CHECK_IMAGE_WIDTH = 64;
    grcheckerboard_texture = [self loadGLTexture];
    
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

-(GLuint) loadGLTexture
{
    [self makeCheckImage];
    GLuint texture;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
   
    // setting of texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);            // wrap the texture around x axis (Texture's "S" = x axis)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);            // wrap the texture around y axis (Texture's "T" = x axis)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        // MAG - Magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);                // MIN - Minification
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECK_IMAGE_WIDTH, CHECK_IMAGE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
}

-(void) makeCheckImage
{
    int i, j, c;
    for (i = 0; i < CHECK_IMAGE_HEIGHT; i++)
    {
        for (j = 0; j < CHECK_IMAGE_WIDTH; j++)
        {
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;

            checkImage[i][j][0] = (GLubyte)c;
            checkImage[i][j][1] = (GLubyte)c;
            checkImage[i][j][2] = (GLubyte)c;
            checkImage[i][j][3] = 255;

        }
    }
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
    
    // OpenGL's drawing code will start from here
    glUseProgram(grgShaderProgramObject);

    // OpenGL drawing code will start here
    mat4 grmodelViewMatrix = mat4::identity();
    mat4 grmodelViewProjectionMatrix = mat4::identity();
    mat4 grtranslateMatrix = mat4::identity();
    mat4 grrotateMatrix = mat4::identity();
    

    //*** Square ***
    grtranslateMatrix = mat4::identity();
    grmodelViewMatrix = mat4::identity();
    grrotateMatrix = mat4::identity();
    grmodelViewProjectionMatrix = mat4::identity();

    grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
    grmodelViewMatrix = grtranslateMatrix;
    grmodelViewProjectionMatrix = grgPerspectiveMatrix * grmodelViewMatrix;

    glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grcheckerboard_texture);
    glUniform1i(grgtextureSamplerUniform, 0);
    
    grsquareOneVertices[0] = 0.0f;
    grsquareOneVertices[1] = 1.0;
    grsquareOneVertices[2] = 0.0;
    grsquareOneVertices[3] = -2.0;
    grsquareOneVertices[4] = 1.0;
    grsquareOneVertices[5] = 0.0;
    grsquareOneVertices[6] = -2.0;
    grsquareOneVertices[7] = -1.0;
    grsquareOneVertices[8] = 0.0;
    grsquareOneVertices[9] = 0.0;
    grsquareOneVertices[10] = -1.0;
    grsquareOneVertices[11] = 0.0;
    
    // bind vao of square
    glBindVertexArray(grgVao_square);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position_square);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grsquareOneVertices), grsquareOneVertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    
    
    // Square - 2
    grtranslateMatrix = mat4::identity();
     grmodelViewMatrix = mat4::identity();
     grrotateMatrix = mat4::identity();
     grmodelViewProjectionMatrix = mat4::identity();

     grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
     grmodelViewMatrix = grtranslateMatrix;
     grmodelViewProjectionMatrix = grgPerspectiveMatrix * grmodelViewMatrix;

     glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);
     
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, grcheckerboard_texture);
     glUniform1i(grgtextureSamplerUniform, 0);
     
    grsquareTwoVertices[0] = 2.41421;
    grsquareTwoVertices[1] = 1.0;
    grsquareTwoVertices[2] = -1.41421;
    grsquareTwoVertices[3] = 1.0;
    grsquareTwoVertices[4] = 1.0;
    grsquareTwoVertices[5] = 0.0;
    grsquareTwoVertices[6] = 1.0;
    grsquareTwoVertices[7] = -1.0;
    grsquareTwoVertices[8] = 0.0;
    grsquareTwoVertices[9] = 2.41421;
    grsquareTwoVertices[10] = -1.0;
    grsquareTwoVertices[11] = -1.41421;
     // bind vao of square
     glBindVertexArray(grgVao_square);
     glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position_square);
     glBufferData(GL_ARRAY_BUFFER, sizeof(grsquareTwoVertices), grsquareTwoVertices, GL_DYNAMIC_DRAW);
     glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
     glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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
    
    if (grgVbo_position_square)
    {
        glDeleteBuffers(1, &grgVbo_position_square);
        grgVbo_position_square = 0;
    }
    if (grgVbo_texture_square)
    {
        glDeleteBuffers(1, &grgVbo_texture_square);
        grgVbo_texture_square = 0;
    }
    if (grgVao_square)
    {
        glDeleteVertexArrays(1, &grgVao_square);
        grgVao_square = 0;
    }
    
    if (grcheckerboard_texture)
    {
        glDeleteTextures(1, &grcheckerboard_texture);
        grcheckerboard_texture = 0;
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









