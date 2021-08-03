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
    
    GLuint grgVbo_position_pyramid;
    GLuint grgVbo_normal_pyramid;
    mat4 grgPerspectiveMatrix;
    GLuint grgVao_pyramid;
    GLfloat grgAnglePyramid;

    GLuint grgModelMarixUniform;
    GLuint grgViewMatrixUniform;
    GLuint grgProjectionMatrixUniform;
    GLuint grgLKeyPressedUniform;
    GLuint grgLdUniform[2];                    // light diffuse
    GLuint grgLaUniform[2];
    GLuint grgLsUniform[2];
    GLuint grgLightPositionUniform[2];

    GLuint grgKdUniform;                    // material diffuse
    GLuint grgKaUniform;
    GLuint grgKsUniform;
    GLuint grgMaterialShinynessUniform;

    bool grbAnimate;
    bool grbLight;
    int i;

    struct Light
    {
        vec3 lightAmbientColor;
        vec3 lightDiffuseColor;
        vec3 lightSpecularColor;
        vec4 lightPosition;
    };
    vec3 grgKa;    // material ambient
    vec3 grgKd;    // material diffuse
    vec3 grgKs;    // material specular
    GLfloat grgMaterialShinyness;
    struct Light grlight[2];
    
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
        "in vec3 vNormal;" \
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
            "vec3 light;" \
            "for(i = 0; i < 2; i++)" \
            "{" \
                "light_direction[i] = normalize(vec3(u_light_position[i] - eye_coordinates));" \
                "reflection_vector[i] = reflect(-light_direction[i], transformed_normal);" \
                "ambient[i] = u_la[i] * u_ka;" \
                "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);" \
                "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflection_vector[i], view_vector), 0.0), u_material_shinyness);" \
                "light = ambient[0] + diffuse[0] + specular[0];" \
                "phong_ads_light =  ambient[1] + diffuse[1] + specular[1] + light;" \
            "}" \
        "}" \
        "else" \
        "{" \
            "phong_ads_light = vec3(1.0, 1.0, 1.0);" \
        "}" \
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
        "in vec3 phong_ads_light;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "FragColor = vec4(phong_ads_light, 1.0);" \
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
    glBindAttribLocation(grgShaderProgramObject, GR_ATTRIBUTE_NORMAL, "vNormal");

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
    
    const GLfloat grpyramidVertices[] =
    {
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
                                //right face
        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
                                // back face
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
                                // left face
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f
    };
    
    const GLfloat grpyramidNormals[] =
    {
        0.0f, 0.447214f, 0.894427f,
        0.0f, 0.447214f, 0.894427f,
        0.0f, 0.447214f, 0.894427f,

        0.894427f, 0.447214f, 0.0f,
        0.894427f, 0.447214f, 0.0f,
        0.894427f, 0.447214f, 0.0f,

        0.0f, 0.447214f, -0.894427f,
        0.0f, 0.447214f, -0.894427f,
        0.0f, 0.447214f, -0.894427f,

        -0.894427f, 0.447214f, 0.0f,
        -0.894427f, 0.447214f, 0.0f,
        -0.894427f, 0.447214f, 0.0f
    };
    
    glGenVertexArrays(1, &grgVao_pyramid);
    glBindVertexArray(grgVao_pyramid);

    glGenBuffers(1, &grgVbo_position_pyramid);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position_pyramid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grpyramidVertices), grpyramidVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // normals
    glGenBuffers(1, &grgVbo_normal_pyramid);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_normal_pyramid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grpyramidNormals), grpyramidNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    grgPerspectiveMatrix = mat4::identity();
    
    // initialize variables
    
    grgKa = vec3(0.0f, 0.0f, 0.0f);
    grgKd = vec3(1.0f, 1.0f, 1.0f);
    grgKs = vec3(1.0f, 1.0f, 1.0f);
    grgMaterialShinyness = 128.0f;
    grgAnglePyramid = 0.0f;
    grlight[0].lightAmbientColor = vec3(0.0, 0.0, 0.0);
    grlight[0].lightDiffuseColor = vec3(1.0, 0.0, 0.0);
    grlight[0].lightSpecularColor = vec3(1.0, 0.0, 0.0);
    grlight[1].lightAmbientColor = vec3(0.0, 0.0, 0.0);
    grlight[1].lightDiffuseColor = vec3(0.0, 0.0, 1.0);
    grlight[1].lightSpecularColor = vec3(0.0, 0.0, 1.0);
    grlight[0].lightPosition = vec4(-2.0, 0.0, 0.0, 1.0);
    grlight[1].lightPosition = vec4(2.0, 0.0, 0.0, 1.0);
    
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
    
    // OpenGL's drawing code will start from here
    glUseProgram(grgShaderProgramObject);

    // OpenGL drawing code will start here
    mat4 grviewMatrix = mat4::identity();
    mat4 grmodelMatrix = mat4::identity();
    mat4 grprojectionMatrix = mat4::identity();
    mat4 grrotateMatrix = mat4::identity();
    mat4 grtranslateMatrix = mat4::identity();
    mat4 grscaleMatrix = mat4::identity();
    
    grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    if (grbAnimate == true)
    {
        grrotateMatrix = vmath::rotate(grgAnglePyramid, 0.0f, 1.0f, 0.0f);
    }
    
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

    grmodelMatrix = grtranslateMatrix * grrotateMatrix;
    grprojectionMatrix = grgPerspectiveMatrix;

    glUniformMatrix4fv(grgProjectionMatrixUniform, 1, GL_FALSE, grprojectionMatrix);
    glUniformMatrix4fv(grgViewMatrixUniform, 1, GL_FALSE, grviewMatrix);
    glUniformMatrix4fv(grgModelMarixUniform, 1, GL_FALSE, grmodelMatrix);

    // bind vao
    glBindVertexArray(grgVao_pyramid);
    glDrawArrays(GL_TRIANGLES, 0, 12);

    // unbind vao
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
    grgAnglePyramid = grgAnglePyramid + 0.1f;
    if (grgAnglePyramid >= 360.0f)
        grgAnglePyramid = 0.0f;
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
            
        case 'a':
        case 'A':
            grbAnimate = !grbAnimate;
            break;
            
        case 'l':
        case 'L':
            grbLight = !grbLight;
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
    
    if (grgVbo_position_pyramid)
    {
        glDeleteBuffers(1, &grgVbo_position_pyramid);
        grgVbo_position_pyramid = 0;
    }
    if (grgVbo_normal_pyramid)
    {
        glDeleteBuffers(1, &grgVbo_normal_pyramid);
        grgVbo_normal_pyramid = 0;
    }
    if (grgVao_pyramid)
    {
        glDeleteVertexArrays(1, &grgVao_pyramid);
        grgVao_pyramid = 0;
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









