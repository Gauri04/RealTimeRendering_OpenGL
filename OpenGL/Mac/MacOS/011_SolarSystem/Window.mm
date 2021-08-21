#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h> // analogous to windows.h or xlib.h
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>          // gl.h
#import"vmath.h"
#import"Sphere.h"

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

typedef struct Node
{
    mat4 matrix;
    struct Node* ptr;
}Node;

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
    struct Node* top;
    mat4 copyMatrix, mulMatrix, topMatrix;
    FILE* fp;
    
    GLuint grgVertexShaderObject;
    GLuint grgFragmentShadeerObject;
    GLuint grgShaderProgramObject;
    mat4 grgPerspectiveProjectionMatrix;
    
    GLuint grgNumVertices;
    GLuint grgNumElements;
    GLuint grgNumNormals;
    
    GLuint grgVboPositionEarth;
    GLuint grgVboElementEarth;
    GLuint grgVaoEarth;
    GLuint grgVboPositionSun;
    GLuint grgVboColorSun;
    GLuint grgVboElementSun;
    GLuint grgVaoSun;

    // uniforms
    GLuint grgModelMarixUniform;
    GLuint grgViewMatrixUniform;
    GLuint grgProjectionMatrixUniform;
    int grgDay;
    int grgYear;
    
    float grsphereVertices[1146];
    float grsphereNormals[1146];
    float grsphereTextures[764];
    unsigned short grsphereElements[2280];
    MySphere *grSphere;
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
    
    // init sphere data
    //getSphereVertexData(grsphereVertices, grsphereNormals, grsphereTextures, grsphereElements);
    //grgNumVertices = getNumberOfSphereVertices();
    //grgNumElements = getNumberOfSphereElements();
     //gropenGLView = [[MyOpenGLView alloc]initWithFrame:win_rect];
    grSphere = [[MySphere alloc]init];

    [grSphere getSphereVertexData:grsphereVertices :grsphereNormals :grsphereTextures :grsphereElements];
    grgNumVertices = [grSphere getNumberOfSphereVertice];
    grgNumElements = [grSphere getNumberOfSphereElements];
    
    fprintf(grgpFile, "\n shereVertices[1] : %f \t sphereNormals[1] : %f", grsphereVertices[1], grsphereNormals[1]);
    fprintf(grgpFile, "\n numVertices : %d, numElements : %d", grgNumVertices, grgNumElements);
    
    // shaders code will be written here
    grgVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    ///// Vertex Shader
    const GLchar* grvertexShaderSourceCode =
        "#version 410 core" \
        "\n" \
        "in vec4 vPosition;" \
        "in vec3 vColor;" \
        "uniform mat4 u_model_matrix;" \
        "uniform mat4 u_view_matrix;" \
        "uniform mat4 u_projection_matrix;" \
        "out vec3 out_color;" \
        "void main(void)" \
        "{" \
        "out_color = vColor;" \
        "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
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
       "in vec3 out_color;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "FragColor = vec4(out_color, 1.0);" \
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

    grgModelMarixUniform = glGetUniformLocation(grgShaderProgramObject, "u_model_matrix");
    grgViewMatrixUniform = glGetUniformLocation(grgShaderProgramObject, "u_view_matrix");
    grgProjectionMatrixUniform = glGetUniformLocation(grgShaderProgramObject, "u_projection_matrix");
    
    GLfloat *colors = (GLfloat *)malloc(grgNumElements * sizeof(GLfloat));

    for (int i = 0; i < grgNumElements; i = i + 3)
    {
        colors[i] = 1.0f;
        colors[i + 1] = 1.0f;
        colors[i + 2] = 0.0f;
    }
    
   // sun
    glGenVertexArrays(1, &grgVaoSun);
    glBindVertexArray(grgVaoSun);
    glGenBuffers(1, &grgVboPositionSun);
    glBindBuffer(GL_ARRAY_BUFFER, grgVboPositionSun);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grsphereVertices), grsphereVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // color vbo
    glGenBuffers(1, &grgVboColorSun);
    glBindBuffer(GL_ARRAY_BUFFER, grgVboColorSun);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grsphereVertices), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // element vbo
    glGenBuffers(1, &grgVboElementSun);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboElementSun);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphereElements), grsphereElements, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);

    //EARTH
    glGenVertexArrays(1, &grgVaoEarth);
    glBindVertexArray(grgVaoEarth);
    glGenBuffers(1, &grgVboPositionEarth);
    glBindBuffer(GL_ARRAY_BUFFER, grgVboPositionEarth);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grsphereVertices), grsphereVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 0.0f, 1.0f);
    // element vbo
    glGenBuffers(1, &grgVboElementEarth);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboElementEarth);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphereElements), grsphereElements, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 0.0f, 1.0f);

    glBindVertexArray(0);
    
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    grgPerspectiveProjectionMatrix = mat4::identity();
    
    // initialize variables
    grgDay = 0;
    grgYear = 0;
    top = NULL;
    
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
    grgPerspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)rect.size.width / (GLfloat)rect.size.height, 0.1f, 100.0f);
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
    
   glUseProgram(grgShaderProgramObject);

    // OpenGL drawing code will start here
    mat4 grmodelMatrix = mat4::identity();
    mat4 grviewMatrix = mat4::identity();
    mat4 grprojectionMatrix = mat4::identity();
    mat4 grrotateMatrix = mat4::identity();
    mat4 grtranslateMatrix = mat4::identity();
    mat4 grscaleMatrix = mat4::identity();

    // push matrix for whole geometry
    grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
    grmodelMatrix = grtranslateMatrix;
    [self PushToStack:grmodelMatrix];

    // sun
    grmodelMatrix = mat4::identity();
    grviewMatrix = mat4::identity();
    grprojectionMatrix = mat4::identity();
    grrotateMatrix = mat4::identity();
    grtranslateMatrix = mat4::identity();
    grscaleMatrix = mat4::identity();

    grrotateMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);        // rotate to match south-north pole to its right directions
    grmodelMatrix = grrotateMatrix;
    grmodelMatrix = [self PushToStack:grmodelMatrix];
    [self PopFromStack];

    grprojectionMatrix = grgPerspectiveProjectionMatrix;

    glUniformMatrix4fv(grgProjectionMatrixUniform, 1, GL_FALSE, grprojectionMatrix);
    glUniformMatrix4fv(grgViewMatrixUniform, 1, GL_FALSE, grviewMatrix);
    glUniformMatrix4fv(grgModelMarixUniform, 1, GL_FALSE, grmodelMatrix);

    // bind vao of square
    glBindVertexArray(grgVaoSun);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboElementSun);
    glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);
    //glDrawArrays(GL_LINE_STRIP, 0, grgNumVertices);
    // unbind vao
    glBindVertexArray(0);
    
    // Earth
    grmodelMatrix = mat4::identity();
    grviewMatrix = mat4::identity();
    grprojectionMatrix = mat4::identity();
    grrotateMatrix = mat4::identity();
    grtranslateMatrix = mat4::identity();
    grscaleMatrix = mat4::identity();


    grrotateMatrix = vmath::rotate((GLfloat)grgYear, 0.0f, 1.0f, 0.0f);
    grtranslateMatrix = vmath::translate(2.0f, 0.0f, 0.0f);
    grmodelMatrix = grrotateMatrix * grtranslateMatrix;
    grrotateMatrix = vmath::rotate(270.0f, 1.0f, 0.0f, 0.0f);
    grrotateMatrix = grrotateMatrix * vmath::rotate((GLfloat)grgDay, 0.0f, 0.0f, 1.0f);
    grmodelMatrix = grmodelMatrix * grrotateMatrix;
    grmodelMatrix = [self PushToStack:grmodelMatrix];
    [self PopFromStack];

    grprojectionMatrix = grgPerspectiveProjectionMatrix;

    glUniformMatrix4fv(grgProjectionMatrixUniform, 1, GL_FALSE, grprojectionMatrix);
    glUniformMatrix4fv(grgViewMatrixUniform, 1, GL_FALSE, grviewMatrix);
    glUniformMatrix4fv(grgModelMarixUniform, 1, GL_FALSE, grmodelMatrix);

    // bind vao of square
    glBindVertexArray(grgVaoEarth);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboElementEarth);
    glDrawElements(GL_LINE_LOOP, grgNumElements, GL_UNSIGNED_SHORT, 0);
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, grgNumVertices);
    // unbind vao
    glBindVertexArray(0);
    
    [self PopFromStack];

    // stop use of shader program
    glUseProgram(0);
    //[self updateFn];
    // buffer 
    CGLFlushDrawable((CGLContextObj) [[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj) [[self openGLContext] CGLContextObj]);
}

-(void) updateFn
{
    // code
   
}

-(Node*) CreateNode
{
    Node* temp = (Node*)malloc(sizeof(Node));
    return(temp);
}

-(int) isEmpty
{
    if (top == NULL)
        return(0);

    return(1);
}

-(mat4) PushToStack:(mat4)pushMatrix
{
    // variable declaration
    Node* temp;
    int i,j;
    mulMatrix = mat4::identity();
    topMatrix = mat4::identity();
    
    temp = [self CreateNode];

    if (top != NULL)
    {
        topMatrix = top->matrix;
        mulMatrix = topMatrix * pushMatrix;
    }
    else
    {
        mulMatrix = pushMatrix;
    }
    temp->matrix = mulMatrix;
    temp->ptr = top;
    top = temp;
    // return the current matrix which is at top of the stack
    return(mulMatrix);

}

-(mat4) PopFromStack
{
    Node* temp;
    mat4 tempMatrix;
    
    if ([self isEmpty] == 1)
    {
        temp = top;
        tempMatrix = top->matrix;
        top = top->ptr;
        temp->ptr = NULL;
        free(temp);
       
        if ([self isEmpty] == 1)
        {
            return(top->matrix);
        }
        else
        {
            tempMatrix = mat4::identity();
            return(tempMatrix);
        }
        
    }
    else
    {
        //printf("\n Stack empty");
        tempMatrix = mat4::identity();
        return(tempMatrix);
    }

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
            
        case 'd':
            grgDay = (grgDay - 6) % 360;
            break;
            
       case 'D':
            grgDay = (grgDay + 6) % 360;
            break;

    case 'Y':
            grgYear = (grgYear + 3) % 360;
            break;
            
    case 'y':
            grgYear = (grgYear - 3) % 360;
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
    if (grgVboPositionSun)
    {
        glDeleteBuffers(1, &grgVboPositionSun);
        grgVboPositionSun = 0;
    }
    if (grgVboElementSun)
    {
        glDeleteBuffers(1, &grgVboElementSun);
        grgVboElementSun = 0;
    }
    if (grgVboColorSun)
    {
        glDeleteBuffers(1, &grgVboColorSun);
        grgVboColorSun = 0;
    }

    if (grgVboPositionEarth)
    {
        glDeleteBuffers(1, &grgVboPositionEarth);
        grgVboPositionEarth = 0;
    }
    if (grgVboElementEarth)
    {
        glDeleteBuffers(1, &grgVboElementEarth);
        grgVboElementEarth = 0;
    }


    // delete square vao and vbo
    if (grgVaoSun)
    {
        glDeleteVertexArrays(1, &grgVaoSun);
        grgVaoSun = 0;
    }
    if (grgVaoEarth)
    {
        glDeleteVertexArrays(1, &grgVaoEarth);
        grgVaoEarth = 0;
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









