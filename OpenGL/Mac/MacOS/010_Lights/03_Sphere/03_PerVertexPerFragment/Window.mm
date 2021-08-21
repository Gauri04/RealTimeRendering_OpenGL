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
   GLuint grgVertexShaderObjectPV;
    GLuint grgFragmentShaderObjectPV;
    GLuint grgShaderProgramObjectPV;
    mat4 grgPerspectiveProjectionMatrix;
    
    GLuint grgNumVertices;
    GLuint grgNumElements;
    GLuint grgNumNormals;
    GLuint grgVboSpherePosition;
    GLuint grgVboSphereNormal;
    GLuint grgVboSphereElement;
    GLuint grgVao;

    // uniforms
    GLuint grgModelMarixUniformPV;
    GLuint grgViewMatrixUniformPV;
    GLuint grgProjectionMatrixUniformPV;
    GLuint grgLaUniformPV;                // light ambient
    GLuint grgLdUniformPV;                // light diffuse
    GLuint grgLsUniformPV;                // light specular
    GLuint grgLightPositionUniformPV;
    GLuint grgKdUniformPV;
    GLuint grgKaUniformPV;
    GLuint grgKsUniformPV;
    GLuint grgMaterialShinynessUniformPV;
    GLuint grgLKeyPressedUniformPV;

    // shader object for per fragment
    GLuint grgVertexShaderObjectPF;
    GLuint grgFragmentShaderObjectPF;
    GLuint grgShaderProgramObjectPF;

    // uniforms for per fragment
    GLuint grgModelMarixUniformPF;
    GLuint grgViewMatrixUniformPF;
    GLuint grgProjectionMatrixUniformPF;
    GLuint grgLaUniformPF;                // light ambient
    GLuint grgLdUniformPF;                // light diffuse
    GLuint grgLsUniformPF;                // light specular
    GLuint grgLightPositionUniformPF;
    GLuint grgKdUniformPF;
    GLuint grgKaUniformPF;
    GLuint grgKsUniformPF;
    GLuint grgMaterialShinynessUniformPF;
    GLuint grgLKeyPressedUniformPF;
    
    vec3 grgLa;
    vec3 grgLd;
    vec3 grgLs;
    vec4 grgLightPosition;
    vec3 grgKa;   // material ambient
    vec3 grgKd;// material diffuse
    vec3 grgKs;    // material specular
    GLfloat grgMaterialShinyness;
    bool grgbLKeyPressed;
    bool grgbIsVertexLighting;
    bool grgbIsFragmentLighting;
    
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
    
    // ******************** PF Shader Code **********************
    
    // shaders code will be written here
    grgVertexShaderObjectPF = glCreateShader(GL_VERTEX_SHADER);

    ///// Vertex Shader
    const GLchar* grvertexShaderSourceCodePF =
        "#version 410 core" \
        "\n" \
        "in vec4 vPosition;" \
        "in vec3 vNormal;" \
        "uniform mat4 u_model_matrix;" \
        "uniform mat4 u_view_matrix;" \
        "uniform mat4 u_projection_matrix;" \
        "uniform int u_l_key_pressed;" \
        "uniform vec4 u_light_position;" \
        "out vec3 transformed_normal;" \
        "out vec3 light_direction;" \
        "out vec3 view_vector;"
        "void main(void)" \
        "{" \
        "if(u_l_key_pressed == 1)" \
        "{" \
        "vec4 eye_coordinates =  u_view_matrix * u_model_matrix * vPosition;" \
        "transformed_normal = mat3(u_view_matrix * u_model_matrix) * vNormal;" \
        "light_direction = vec3(u_light_position - eye_coordinates);" \
        "view_vector = -eye_coordinates.xyz;" \
        "}" \
        "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
        "}";
    
    glShaderSource(grgVertexShaderObjectPF, 1, (const GLchar **) &grvertexShaderSourceCodePF, NULL);

    // compile shader
    glCompileShader(grgVertexShaderObjectPF);
    // error check for compilation
    GLint griInfoLength = 0;
    GLint griShaderCompileStatus = 0;
    char* grszInfoLog = NULL;

    glGetShaderiv(grgVertexShaderObjectPF, GL_COMPILE_STATUS, &griShaderCompileStatus);
    if (griShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(grgVertexShaderObjectPF, GL_INFO_LOG_LENGTH, &griInfoLength);
        if (griInfoLength > 0)
        {
            grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
            if (grszInfoLog != NULL)
            {
                GLsizei grwritten;
                glGetShaderInfoLog(grgVertexShaderObjectPF, griInfoLength, &grwritten, grszInfoLog);
                fprintf(grgpFile, "\n Vertex Shader PF Compilation Log : %s", grszInfoLog);
                free(grszInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }

    ////// Fragment Shader
    grgFragmentShaderObjectPF = glCreateShader(GL_FRAGMENT_SHADER);

    // source code of shader
    const GLchar* grfragmentShaderSourceCodePF =
        "#version 410 core" \
        "\n" \
        "in vec3 transformed_normal;" \
        "in vec3 light_direction;" \
        "in vec3 view_vector;" \
        "uniform vec3 u_la;" \
        "uniform vec3 u_ld;" \
        "uniform vec3 u_ls;" \
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform float u_material_shinyness;" \
        "uniform int u_l_key_pressed;" \
        "vec3 fong_ads_light;"
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "if(u_l_key_pressed == 1)" \
        "{" \
        "vec3 normalized_transform_normal = normalize(transformed_normal);" \
        "vec3 normalized_light_direction = normalize(light_direction);" \
        "vec3 normalized_view_vector = normalize(view_vector);"
        "vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transform_normal);" \
        "vec3 ambient = u_la * u_ka;" \
        "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transform_normal), 0.0);" \
        "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector, normalized_view_vector), 0.0), u_material_shinyness);" \
        "fong_ads_light = ambient + diffuse + specular;" \
        "}" \
        "else" \
        "{" \
        "fong_ads_light = vec3(1.0, 1.0, 1.0);"
        "}" \
        "FragColor = vec4(fong_ads_light, 1.0);" \
        "}";

    glShaderSource(grgFragmentShaderObjectPF, 1, (const GLchar **) &grfragmentShaderSourceCodePF, NULL);

    // compile shader
    glCompileShader(grgFragmentShaderObjectPF);
    // error check for compiation
    glGetShaderiv(grgFragmentShaderObjectPF, GL_COMPILE_STATUS, &griShaderCompileStatus);
    if (griShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(grgFragmentShaderObjectPF, GL_INFO_LOG_LENGTH, &griInfoLength);
        if (griInfoLength > 0)
        {
            grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
            if (grszInfoLog != NULL)
            {
                GLsizei grwritten;
                glGetShaderInfoLog(grgFragmentShaderObjectPF, griInfoLength, &grwritten, grszInfoLog);
                fprintf(grgpFile, "\n Fragment Shader PF Compilation Log : %s", grszInfoLog);
                free(grszInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }

    //****** Shader Program *****//
    // create
    grgShaderProgramObjectPF = glCreateProgram();

    // attach vertex shader to shader program
    glAttachShader(grgShaderProgramObjectPF, grgVertexShaderObjectPF);

    // attach fragment shader to shader program
    glAttachShader(grgShaderProgramObjectPF, grgFragmentShaderObjectPF);

    // pre-link our attribute enum with shader's attributes
    glBindAttribLocation(grgShaderProgramObjectPF, GR_ATTRIBUTE_POSITION, "vPosition");
    glBindAttribLocation(grgShaderProgramObjectPF, GR_ATTRIBUTE_NORMAL, "vNormal");

    // link shader
    glLinkProgram(grgShaderProgramObjectPF);
    // error check for linking
    GLint griShaderProgramLinkStatus = 0;
    glGetProgramiv(grgShaderProgramObjectPF, GL_LINK_STATUS, &griShaderProgramLinkStatus);
    if (griShaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(grgShaderProgramObjectPF, GL_INFO_LOG_LENGTH, &griInfoLength);
        if (griInfoLength > 0)
        {
            grszInfoLog = (char*)malloc(sizeof(griInfoLength) * sizeof(char));
            if (grszInfoLog != NULL)
            {
                GLsizei grwritten;
                glGetProgramInfoLog(grgShaderProgramObjectPF, griInfoLength, &grwritten, grszInfoLog);
                fprintf(grgpFile, "\n Shader Program PF Link Log : %s", grszInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }

    grgModelMarixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_model_matrix");
    grgViewMatrixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_view_matrix");
    grgProjectionMatrixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_projection_matrix");
    grgLaUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_la");
    grgLdUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_ld");
    grgLsUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_ls");
    grgKaUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_ka");
    grgKdUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_kd");
    grgKsUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_ks");
    grgLightPositionUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_light_position");
    grgLKeyPressedUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_l_key_pressed");
    grgMaterialShinynessUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_material_shinyness");
    
    // ********************** PV Shader code **************************
    grgVertexShaderObjectPV = glCreateShader(GL_VERTEX_SHADER);

    ///// Vertex Shader
    const GLchar* grvertexShaderSourceCodePV =
        "#version 410 core" \
        "\n" \
        "in vec4 vPosition;" \
        "in vec3 vNormal;" \
        "uniform mat4 u_model_matrix;" \
        "uniform mat4 u_view_matrix;" \
        "uniform mat4 u_projection_matrix;" \
        "uniform int u_l_key_pressed;" \
        "uniform vec3 u_la;" \
        "uniform vec3 u_ld;" \
        "uniform vec3 u_ls;" \
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform float u_material_shinyness;" \
        "uniform vec4 u_light_position;" \
        "out vec3 fong_ads_light;" \
        "void main(void)" \
        "{" \
        "if(u_l_key_pressed == 1)" \
        "{" \
        "vec4 eye_coordinates =  u_view_matrix * u_model_matrix * vPosition;" \
        "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
        "vec3 light_direction = normalize(vec3(u_light_position - eye_coordinates));" \
        "vec3 reflection_vector = reflect(-light_direction, transformed_normal);" \
        "vec3 view_vector = normalize(-eye_coordinates.xyz);" \
        "vec3 ambient = u_la * u_ka;" \
        "vec3 diffuse = u_ld * u_kd * max(dot(light_direction, transformed_normal), 0.0);" \
        "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector, view_vector), 0.0), u_material_shinyness);" \
        "fong_ads_light = ambient + diffuse + specular;" \
        "}" \
        "else" \
        "{" \
        "fong_ads_light = vec3(1.0, 1.0, 1.0);" \
        "}" \
        "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
        "}";
    
    glShaderSource(grgVertexShaderObjectPV, 1, (const GLchar **) &grvertexShaderSourceCodePV, NULL);

    // compile shader
    glCompileShader(grgVertexShaderObjectPV);
    // error check for compilation
     griInfoLength = 0;
     griShaderCompileStatus = 0;
     grszInfoLog = NULL;

    glGetShaderiv(grgVertexShaderObjectPV, GL_COMPILE_STATUS, &griShaderCompileStatus);
    if (griShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(grgVertexShaderObjectPV, GL_INFO_LOG_LENGTH, &griInfoLength);
        if (griInfoLength > 0)
        {
            grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
            if (grszInfoLog != NULL)
            {
                GLsizei grwritten;
                glGetShaderInfoLog(grgVertexShaderObjectPV, griInfoLength, &grwritten, grszInfoLog);
                fprintf(grgpFile, "\n Vertex Shader PV Compilation Log : %s", grszInfoLog);
                free(grszInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }

    ////// Fragment Shader
    grgFragmentShaderObjectPV = glCreateShader(GL_FRAGMENT_SHADER);

    // source code of shader
    const GLchar* grfragmentShaderSourceCodePV =
        "#version 410 core" \
        "\n" \
        "in vec3 fong_ads_light;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "FragColor = vec4(fong_ads_light, 1.0);" \
        "}";

    glShaderSource(grgFragmentShaderObjectPV, 1, (const GLchar **) &grfragmentShaderSourceCodePV, NULL);

    // compile shader
    glCompileShader(grgFragmentShaderObjectPV);
    // error check for compiation
    glGetShaderiv(grgFragmentShaderObjectPV, GL_COMPILE_STATUS, &griShaderCompileStatus);
    if (griShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(grgFragmentShaderObjectPV, GL_INFO_LOG_LENGTH, &griInfoLength);
        if (griInfoLength > 0)
        {
            grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
            if (grszInfoLog != NULL)
            {
                GLsizei grwritten;
                glGetShaderInfoLog(grgFragmentShaderObjectPV, griInfoLength, &grwritten, grszInfoLog);
                fprintf(grgpFile, "\n Fragment Shader PV Compilation Log : %s", grszInfoLog);
                free(grszInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }

    //****** Shader Program *****//
    // create
    grgShaderProgramObjectPV = glCreateProgram();

    // attach vertex shader to shader program
    glAttachShader(grgShaderProgramObjectPV, grgVertexShaderObjectPV);

    // attach fragment shader to shader program
    glAttachShader(grgShaderProgramObjectPV, grgFragmentShaderObjectPV);

    // pre-link our attribute enum with shader's attributes
    glBindAttribLocation(grgShaderProgramObjectPV, GR_ATTRIBUTE_POSITION, "vPosition");
    glBindAttribLocation(grgShaderProgramObjectPV, GR_ATTRIBUTE_NORMAL, "vNormal");

    // link shader
    glLinkProgram(grgShaderProgramObjectPV);
    // error check for linking
     griShaderProgramLinkStatus = 0;
    glGetProgramiv(grgShaderProgramObjectPV, GL_LINK_STATUS, &griShaderProgramLinkStatus);
    if (griShaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(grgShaderProgramObjectPV, GL_INFO_LOG_LENGTH, &griInfoLength);
        if (griInfoLength > 0)
        {
            grszInfoLog = (char*)malloc(sizeof(griInfoLength) * sizeof(char));
            if (grszInfoLog != NULL)
            {
                GLsizei grwritten;
                glGetProgramInfoLog(grgShaderProgramObjectPV, griInfoLength, &grwritten, grszInfoLog);
                fprintf(grgpFile, "\n Shader Program PF Link Log : %s", grszInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }

    grgModelMarixUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_model_matrix");
    grgViewMatrixUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_view_matrix");
    grgProjectionMatrixUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_projection_matrix");
    grgLaUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_la");
    grgLdUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_ld");
    grgLsUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_ls");
    grgKaUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_ka");
    grgKdUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_kd");
    grgKsUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_ks");
    grgLightPositionUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_light_position");
    grgLKeyPressedUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_l_key_pressed");
    grgMaterialShinynessUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_material_shinyness");
    
    
    glGenVertexArrays(1, &grgVao);
    glBindVertexArray(grgVao);

    glGenBuffers(1, &grgVboSpherePosition);
    glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grsphereVertices), grsphereVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // normal for cube
    glGenBuffers(1, &grgVboSphereNormal);
    glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grsphereNormals), grsphereNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // element vbo
    glGenBuffers(1, &grgVboSphereElement);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphereElements), grsphereElements, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    grgPerspectiveProjectionMatrix = mat4::identity();
    
    // initialize variables
    grgbLKeyPressed = false;
    grgbIsVertexLighting = true;
    grgbIsFragmentLighting =false;
    grgKa = vec3(0.0f, 0.0f, 0.0f);
    grgKd = vec3(1.0f, 1.0f, 1.0f);
    grgKs = vec3(1.0f, 1.0f, 1.0f);
    grgMaterialShinyness = 128.0f;
    grgLa= vec3(0.0f, 0.0, 0.0);
    grgLd= vec3(0.5f, 0.2f, 0.7f);
    grgLs = vec3(0.7f, 0.7f, 0.7f);
    grgLightPosition = vec4(100.0f, 100.0f, 100.0f, 1.0f);
    
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
    
    // OpenGL drawing code will start here
    mat4 grmodelMatrix = mat4::identity();
    mat4 grviewMatrix = mat4::identity();
    mat4 grprojectionMatrix = mat4::identity();
    mat4 grrotateMatrix = mat4::identity();
    mat4 grtranslateMatrix = mat4::identity();
    mat4 grscaleMatrix = mat4::identity();


    // start use of shader program
    if (grgbIsVertexLighting)
    {
        glUseProgram(grgShaderProgramObjectPV);
        
        grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
        grmodelMatrix = grtranslateMatrix;

        grprojectionMatrix = grgPerspectiveProjectionMatrix;


        if (grgbLKeyPressed == true)
        {
            glUniform1i(grgLKeyPressedUniformPV, 1);
            glUniform3fv(grgLdUniformPV, 1, grgLd);
            glUniform3fv(grgLaUniformPV, 1, grgLa);
            glUniform3fv(grgLsUniformPV, 1, grgLs);
            glUniform3fv(grgKdUniformPV, 1, grgKd);
            glUniform3fv(grgKaUniformPV, 1, grgKa);
            glUniform3fv(grgKsUniformPV, 1, grgKs);
            glUniform4fv(grgLightPositionUniformPV, 1, grgLightPosition);
            glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness);
        }
        else
        {
            glUniform1i(grgLKeyPressedUniformPV, 0);
        }


        glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
        glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
        glUniformMatrix4fv(grgModelMarixUniformPV, 1, GL_FALSE, grmodelMatrix);

        // bind vao of square
        glBindVertexArray(grgVao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement);
        glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

        // unbind vao
        glBindVertexArray(0);


        // stop use of shader program
        glUseProgram(0);
    }
    else if(grgbIsFragmentLighting)
    {
        glUseProgram(grgShaderProgramObjectPF);
        grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
        grmodelMatrix = grtranslateMatrix;

        // multiply modelview and orthographic matrix to get modelviewprojectionmatrix
        grprojectionMatrix = grgPerspectiveProjectionMatrix;

        if (grgbLKeyPressed == true)
        {
            glUniform1i(grgLKeyPressedUniformPF, 1);
            glUniform3fv(grgLdUniformPF, 1, grgLd);
            glUniform3fv(grgLaUniformPF, 1, grgLa);
            glUniform3fv(grgLsUniformPF, 1, grgLs);
            glUniform3fv(grgKdUniformPF, 1, grgKd);
            glUniform3fv(grgKaUniformPF, 1, grgKa);
            glUniform3fv(grgKsUniformPF, 1, grgKs);
            glUniform4fv(grgLightPositionUniformPF, 1, grgLightPosition);
            glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness);
        }
        else
        {
            glUniform1i(grgLKeyPressedUniformPF, 0);
        }

        glUniformMatrix4fv(grgModelMarixUniformPF, 1, GL_FALSE, grmodelMatrix);
        glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
        glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);

        // bind vao
        glBindVertexArray(grgVao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement);
        glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

        // unbind vao
        glBindVertexArray(0);
    }

    //[self updateFn];
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
        case 'q':
        case 'Q':
            [self release];
            [NSApp terminate:self];
            break;
            
        case 27:                            // escape key
            [[self window]toggleFullScreen:self];
            break;
            
        case 'l':
        case 'L':
            grgbLKeyPressed = !grgbLKeyPressed;
            break;
            
        case 'v':
        case 'V':
            grgbIsVertexLighting = !grgbIsVertexLighting;
            break;
            
        case 'f':
        case 'F':
            grgbIsFragmentLighting = !grgbIsFragmentLighting;
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
    
    if (grgVboSpherePosition)
    {
        glDeleteBuffers(1, &grgVboSpherePosition);
        grgVboSpherePosition = 0;
    }
    if (grgVboSphereNormal)
    {
        glDeleteBuffers(1, &grgVboSphereNormal);
        grgVboSphereNormal = 0;
    }

    // delete square vao and vbo
    if (grgVao)
    {
        glDeleteVertexArrays(1, &grgVao);
        grgVao = 0;
    }
    
    
    glDetachShader(grgShaderProgramObjectPF, grgVertexShaderObjectPF);
    // detach fragment shader
    glDetachShader(grgShaderProgramObjectPF, grgFragmentShaderObjectPF);

    // delete vertex object
    glDeleteShader(grgVertexShaderObjectPF);
    grgVertexShaderObjectPF = 0;


    // delete fragment shader object
    glDeleteShader(grgFragmentShaderObjectPF);
    grgFragmentShaderObjectPF = 0;

    // unlink shader program
    glUseProgram(0);
    // delete fragment shader
    glDetachShader(grgShaderProgramObjectPF, grgVertexShaderObjectPF);
    // detach fragment shader
    glDetachShader(grgShaderProgramObjectPF, grgFragmentShaderObjectPF);

    // delete vertex object
    glDeleteShader(grgVertexShaderObjectPF);
    grgVertexShaderObjectPF = 0;


    // delete fragment shader object
    glDeleteShader(grgFragmentShaderObjectPF);
    grgFragmentShaderObjectPF = 0;

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









