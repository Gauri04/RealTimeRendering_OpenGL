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
    mat4 grgPerspectiveProjectionMatrix;
    GLuint grgVao;
    GLuint grgVboSpherePosition;
    GLuint grgVboSphereNormal;
    GLuint grgVboSphereElement;
    
    GLuint grgModelMarixUniformPV;
GLuint grgViewMatrixUniformPV;
GLuint grgProjectionMatrixUniformPV;
GLuint grgLaUniformPV[3];                // light ambient
GLuint grgLdUniformPV[3];                // light diffuse
GLuint grgLsUniformPV[3];                // light specular
GLuint grgLightPositionUniformPV[3];
GLuint grgKdUniformPV;
GLuint grgKaUniformPV;
GLuint grgKsUniformPV;
GLuint grgMaterialShinynessUniformPV;
GLuint grgLKeyPressedUniformPV;

GLuint grgVertexShaderObjectPV;
GLuint grgFragmentShaderObjectPV;
GLuint grgShaderProgramObjectPV;


// per fragment
// uniforms
GLuint grgModelMatrixUniformPF;
GLuint grgViewMatrixUniformPF;
GLuint grgProjectionMatrixUniformPF;
GLuint grgLaUniformPF[3];
GLuint grgLdUniformPF[3];
GLuint grgLsUniformPF[3];
GLuint grgLightPositionUniformPF[3];
GLuint grgKdUniformPF;                    // material diffuse
GLuint grgKaUniformPF;                    // material ambient
GLuint grgKsUniformPF;                    // material specular
GLuint grgMaterialShinynessUniformPF;
GLuint grgLKeyPressedUniformPF;

GLuint grgVertexShaderObjectPF;
GLuint grgFragmentShaderObjectPF;
GLuint grgShaderProgramObjectPF;

struct Light
{
    vec3 lightAmbientColor;
    vec3 lightDiffuseColor;
    vec3 lightSpecularColor;
    vec4 lightPosition;
};
struct Light grlight[3];
    vec3 grgKa;   // material ambient
    vec3 grgKd;    // material diffuse
    vec3 grgKs;   // material specular
    GLfloat grgMaterialShinyness;
bool grgbPerVertexLighting;
bool grgbPerFragmentLighting;
    bool grgbLKeyPressed;
bool grgbXKeyPressed;
bool grgbYKeyPressed;
bool grgbZKeyPressed;
GLfloat grflightAngleX;
GLfloat grflightAngleY;
GLfloat grflightAngleZ;
int i;
    
    int grgNumElements;
    int grgNumVertices;
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
        "uniform vec4 u_light_position[3];" \
        "out vec3 transformed_normal;" \
        "out vec3 light_direction[3];" \
        "out vec3 view_vector;" \
        "int i;" \
        "void main(void)" \
        "{" \
        "if(u_l_key_pressed == 1)" \
        "{" \
        "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;" \
        "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
        "view_vector = normalize(-eye_coordinates.xyz);" \
        "for(i = 0; i < 3; i++)" \
        "{" \
        "light_direction[i] = normalize(vec3(u_light_position[i] - eye_coordinates));" \
        "}" \
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
        "in vec3 light_direction[3];" \
        "in vec3 view_vector;" \
        "uniform vec3 u_la[3];" \
        "uniform vec3 u_ld[3];" \
        "uniform vec3 u_ls[3];" \
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform float u_material_shinyness;" \
        "uniform int u_l_key_pressed;" \
        "vec3 fong_ads_light;" \
        "out vec4 FragColor;" \
        "int i;" \
        "void main(void)" \
        "{" \
        "if(u_l_key_pressed == 1)" \
        "{" \
        "vec3 reflection_vector[3];" \
        "vec3 ambient[3];" \
        "vec3 diffuse[3];" \
        "vec3 specular[3];" \
        "vec3 light1;" \
        "vec3 light2;" \
        "vec3 light3;" \
        "for(i = 0; i < 3; i++)" \
        "{" \
        "reflection_vector[i] = reflect(-light_direction[i], transformed_normal);" \
        "ambient[i] = u_la[i] *u_ka;" \
        "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);" \
        "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflection_vector[i], view_vector), 0.0), u_material_shinyness);" \
        "light1 = ambient[0] + diffuse[0] + specular[0];" \
        "light2 = ambient[1] + diffuse[1] + specular[1];" \
        "light3 = ambient[2] + diffuse[2] + specular[2];" \
        "fong_ads_light = light1 + light2 + light3;" \
        "}" \
        "}" \
        "else" \
        "{" \
        "fong_ads_light = vec3(1.0, 1.0, 1.0);" \
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

   grgModelMatrixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_model_matrix");
    grgViewMatrixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_view_matrix");
    grgProjectionMatrixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_projection_matrix");

    grgLdUniformPF[0] = glGetUniformLocation(grgShaderProgramObjectPF, "u_ld[0]");
    grgLaUniformPF[0] = glGetUniformLocation(grgShaderProgramObjectPF, "u_la[0]");
    grgLsUniformPF[0] = glGetUniformLocation(grgShaderProgramObjectPF, "u_ls[0]");
    grgLdUniformPF[1] = glGetUniformLocation(grgShaderProgramObjectPF, "u_ld[1]");
    grgLaUniformPF[1] = glGetUniformLocation(grgShaderProgramObjectPF, "u_la[1]");
    grgLsUniformPF[1] = glGetUniformLocation(grgShaderProgramObjectPF, "u_ls[1]");
    grgLdUniformPF[2] = glGetUniformLocation(grgShaderProgramObjectPF, "u_ld[2]");
    grgLaUniformPF[2] = glGetUniformLocation(grgShaderProgramObjectPF, "u_la[2]");
    grgLsUniformPF[2] = glGetUniformLocation(grgShaderProgramObjectPF, "u_ls[2]");
    grgLightPositionUniformPF[0] = glGetUniformLocation(grgShaderProgramObjectPF, "u_light_position[0]");
    grgLightPositionUniformPF[1] = glGetUniformLocation(grgShaderProgramObjectPF, "u_light_position[1]");
    grgLightPositionUniformPF[2] = glGetUniformLocation(grgShaderProgramObjectPF, "u_light_position[2]");

    grgKaUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_ka");
    grgKdUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_kd");
    grgKsUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_ks");

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
        "uniform vec3 u_la[3];" \
        "uniform vec3 u_ld[3];" \
        "uniform vec3 u_ls[3];" \
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform float u_material_shinyness;" \
        "uniform vec4 u_light_position[3];" \
        "out vec3 phong_ads_light;" \
        "int i;" \
        "void main(void)" \
        "{" \
        "if(u_l_key_pressed == 1)" \
        "{" \
        "vec4 eye_coordinates =  u_view_matrix * u_model_matrix * vPosition;" \
        "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
        "vec3 view_vector = normalize(-eye_coordinates.xyz);" \
        "vec3 light_direction[3];" \
        "vec3 reflection_vector[3];" \
        "vec3 ambient[3];" \
        "vec3 diffuse[3];" \
        "vec3 specular[3];" \
        "vec3 light1;" \
        "vec3 light2;" \
        "vec3 light3;" \
        "for(i = 0; i < 3; i++)" \
        "{" \
        "light_direction[i] = normalize(vec3(u_light_position[i] - eye_coordinates));" \
        "reflection_vector[i] = reflect(-light_direction[i], transformed_normal);" \
        "ambient[i] = u_la[i] * u_ka;" \
        "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);" \
        "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflection_vector[i], view_vector), 0.0), u_material_shinyness);" \
        "light1 = ambient[0] + diffuse[0] + specular[0];" \
        "light2 = ambient[1] + diffuse[1] + specular[1];" \
        "light3 = ambient[2] + diffuse[2] + specular[2];" \
        "phong_ads_light = light1 + light2 + light3;" \
        "}" \
        "}" \
        "else" \
        "{" \
        "phong_ads_light = vec3(1.0, 1.0, 1.0);" \
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
        "in vec3 phong_ads_light;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "FragColor = vec4(phong_ads_light, 1.0);" \
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

    grgLdUniformPV[0] = glGetUniformLocation(grgShaderProgramObjectPV, "u_ld[0]");
    grgLaUniformPV[0] = glGetUniformLocation(grgShaderProgramObjectPV, "u_la[0]");
    grgLsUniformPV[0] = glGetUniformLocation(grgShaderProgramObjectPV, "u_ls[0]");
    grgLdUniformPV[1] = glGetUniformLocation(grgShaderProgramObjectPV, "u_ld[1]");
    grgLaUniformPV[1] = glGetUniformLocation(grgShaderProgramObjectPV, "u_la[1]");
    grgLsUniformPV[1] = glGetUniformLocation(grgShaderProgramObjectPV, "u_ls[1]");
    grgLdUniformPV[2] = glGetUniformLocation(grgShaderProgramObjectPV, "u_ld[2]");
    grgLaUniformPV[2] = glGetUniformLocation(grgShaderProgramObjectPV, "u_la[2]");
    grgLsUniformPV[2] = glGetUniformLocation(grgShaderProgramObjectPV, "u_ls[2]");
    grgLightPositionUniformPV[0] = glGetUniformLocation(grgShaderProgramObjectPV, "u_light_position[0]");
    grgLightPositionUniformPV[1] = glGetUniformLocation(grgShaderProgramObjectPV, "u_light_position[1]");
    grgLightPositionUniformPV[2] = glGetUniformLocation(grgShaderProgramObjectPV, "u_light_position[2]");

    grgKaUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_ka");
    grgKdUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_kd");
    grgKsUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_ks");

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
    
    // initialize colors
    grlight[0].lightAmbientColor = vec3(0.0, 0.0, 0.0);
    grlight[0].lightDiffuseColor = vec3(1.0, 0.0, 0.0);
    grlight[0].lightSpecularColor = vec3(1.0, 0.0, 0.0);

    grlight[1].lightAmbientColor = vec3(0.0, 0.0, 0.0);
    grlight[1].lightDiffuseColor = vec3(0.0, 1.0, 0.0);
    grlight[1].lightSpecularColor = vec3(0.0, 1.0, 0.0);

    grlight[2].lightAmbientColor = vec3(0.0, 0.0, 0.0);
    grlight[2].lightDiffuseColor = vec3(0.0, 0.0, 1.0);
    grlight[2].lightSpecularColor = vec3(0.0, 0.0, 1.0);

    grlight[0].lightPosition = vec4(0.0, 0.0, 0.0, 1.0);
    grlight[1].lightPosition = vec4(0.0, 0.0, 0.0, 1.0);
    grlight[2].lightPosition = vec4(0.0, 0.0, 0.0, 1.0);
    
    grgbLKeyPressed = false;
    grgbPerVertexLighting = false;
    grgbPerFragmentLighting =false;
    grgbXKeyPressed = false;
    grgbYKeyPressed = false;
    grgbZKeyPressed = false;
    grgKa = vec3(0.0f, 0.0f, 0.0f);
    grgKd = vec3(1.0f, 1.0f, 1.0f);
    grgKs = vec3(1.0f, 1.0f, 1.0f);
    grgMaterialShinyness = 128.0f;

    
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
    mat4 grtranslateMatrix = mat4::identity();

    if (grgbPerVertexLighting == true)
    {
        // start use of shader program
        glUseProgram(grgShaderProgramObjectPV);

        grmodelMatrix = mat4::identity();
        grviewMatrix = mat4::identity();
        grprojectionMatrix = mat4::identity();
        grtranslateMatrix = mat4::identity();

        grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
        grmodelMatrix = grtranslateMatrix;

        // multiply modelview and orthographic matrix to get modelviewprojectionmatrix
        grprojectionMatrix = grgPerspectiveProjectionMatrix;

        if (grgbLKeyPressed == true)
        {
            glUniform1i(grgLKeyPressedUniformPV, 1);
            glUniform3fv(grgKdUniformPV, 1, grgKd);
            glUniform3fv(grgKaUniformPV, 1, grgKa);
            glUniform3fv(grgKsUniformPV, 1, grgKs);
            glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness);

            if (grgbXKeyPressed == true)
            {
                grlight[0].lightPosition[0] = 0.0;
                grlight[0].lightPosition[1] = (GLfloat)5.5 * sin(grflightAngleX);
                grlight[0].lightPosition[2] = (GLfloat)5.5 * cos(grflightAngleX);
                grlight[0].lightPosition[3] = 1.0;                            // w for positional light
                //grlight[0].lightPosition = vec4(0.0, (GLfloat)(1.2 * sin(grflightAngleX)), (GLfloat)(1.2 * cos(grflightAngleX)), 1.0);
            }
            if (grgbYKeyPressed == true)
            {
                grlight[1].lightPosition[0] = (GLfloat)4.5 * sin(grflightAngleX);;
                grlight[1].lightPosition[1] = 0.0;
                grlight[1].lightPosition[2] = (GLfloat)4.5 * cos(grflightAngleX);
                grlight[1].lightPosition[3] = 1.0;                            // w for positional light
                //grlight[0].lightPosition = vec4(0.0, (GLfloat)(1.2 * sin(grflightAngleX)), (GLfloat)(1.2 * cos(grflightAngleX)), 1.0);
            }

            if (grgbZKeyPressed == true)
            {
                grlight[2].lightPosition[0] = (GLfloat)6.5 * sin(grflightAngleX);;
                grlight[2].lightPosition[1] = (GLfloat)6.5 * cos(grflightAngleX);
                grlight[2].lightPosition[2] = 0.0f;
                grlight[2].lightPosition[3] = 1.0;                            // w for positional light
                //grlight[0].lightPosition = vec4(0.0, (GLfloat)(1.2 * sin(grflightAngleX)), (GLfloat)(1.2 * cos(grflightAngleX)), 1.0);
            }

            for (i = 0; i < 3; i++)
            {
                glUniform3fv(grgLdUniformPV[i], 1, grlight[i].lightDiffuseColor);
                glUniform3fv(grgLaUniformPV[i], 1, grlight[i].lightAmbientColor);
                glUniform3fv(grgLsUniformPV[i], 1, grlight[i].lightSpecularColor);
                glUniform4fv(grgLightPositionUniformPV[i], 1, grlight[i].lightPosition);
            }

        }
        else
        {
            glUniform1i(grgLKeyPressedUniformPV, 0);
        }

        glUniformMatrix4fv(grgModelMarixUniformPV, 1, GL_FALSE, grmodelMatrix);
        glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
        glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
        // bind vao
        glBindVertexArray(grgVao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement);
        glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

        // unbind vao
        glBindVertexArray(0);

        // stop use of shader program
        glUseProgram(0);
    }
    else if (grgbPerFragmentLighting == true)
    {
        // start use of shader program
        glUseProgram(grgShaderProgramObjectPF);

        grmodelMatrix = mat4::identity();
        grviewMatrix = mat4::identity();
        grprojectionMatrix = mat4::identity();
        grtranslateMatrix = mat4::identity();

        grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
        grmodelMatrix = grtranslateMatrix;

        // multiply modelview and orthographic matrix to get modelviewprojectionmatrix
        grprojectionMatrix = grgPerspectiveProjectionMatrix;

        if (grgbLKeyPressed == true)
        {
            glUniform1i(grgLKeyPressedUniformPF, 1);
            glUniform3fv(grgKdUniformPF, 1, grgKd);
            glUniform3fv(grgKaUniformPF, 1, grgKa);
            glUniform3fv(grgKsUniformPF, 1, grgKs);
            glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness);

            if (grgbXKeyPressed == true)
            {
                grlight[0].lightPosition[0] = 0.0;
                grlight[0].lightPosition[1] = (GLfloat)5.5 * sin(grflightAngleX);
                grlight[0].lightPosition[2] = (GLfloat)5.5 * cos(grflightAngleX);
                grlight[0].lightPosition[3] = 1.0;                            // w for positional light
                //grlight[0].lightPosition = vec4(0.0, (GLfloat)(1.2 * sin(grflightAngleX)), (GLfloat)(1.2 * cos(grflightAngleX)), 1.0);
            }
            if (grgbYKeyPressed == true)
            {
                grlight[1].lightPosition[0] = (GLfloat)4.5 * sin(grflightAngleX);;
                grlight[1].lightPosition[1] = 0.0;
                grlight[1].lightPosition[2] = (GLfloat)4.5 * cos(grflightAngleX);
                grlight[1].lightPosition[3] = 1.0;                            // w for positional light
                //grlight[0].lightPosition = vec4(0.0, (GLfloat)(1.2 * sin(grflightAngleX)), (GLfloat)(1.2 * cos(grflightAngleX)), 1.0);
            }

            if (grgbZKeyPressed == true)
            {
                grlight[2].lightPosition[0] = (GLfloat)6.5 * sin(grflightAngleX);;
                grlight[2].lightPosition[1] = (GLfloat)6.5 * cos(grflightAngleX);
                grlight[2].lightPosition[2] = 0.0f;
                grlight[2].lightPosition[3] = 1.0;                            // w for positional light
                //grlight[0].lightPosition = vec4(0.0, (GLfloat)(1.2 * sin(grflightAngleX)), (GLfloat)(1.2 * cos(grflightAngleX)), 1.0);
            }

            for (i = 0; i < 3; i++)
            {
                glUniform3fv(grgLdUniformPF[i], 1, grlight[i].lightDiffuseColor);
                glUniform3fv(grgLaUniformPF[i], 1, grlight[i].lightAmbientColor);
                glUniform3fv(grgLsUniformPF[i], 1, grlight[i].lightSpecularColor);
                glUniform4fv(grgLightPositionUniformPF[i], 1, grlight[i].lightPosition);
            }

        }
        else
        {
            glUniform1i(grgLKeyPressedUniformPF, 0);
        }

        glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
        glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
        glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);

        // bind vao
        glBindVertexArray(grgVao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement);
        glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

        // unbind vao
        glBindVertexArray(0);
        glUseProgram(0);
    }

    [self updateFn];
    // buffer 
    CGLFlushDrawable((CGLContextObj) [[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj) [[self openGLContext] CGLContextObj]);
}

-(void) updateFn
{
    // code
   if (grgbXKeyPressed)
    {
        grflightAngleX = grflightAngleX + 0.003f;
        if (grflightAngleX >= 360.0f)
            grflightAngleX = 0.0f;

    }
    if (grgbYKeyPressed)
    {
        grflightAngleY = grflightAngleY + 0.0035f;
        if (grflightAngleY >= 360.0f)
            grflightAngleY = 0.0f;
    }
    if (grgbZKeyPressed)
    {
        grflightAngleZ = grflightAngleZ + 0.0028f;
        if (grflightAngleZ >= 360.0f)
            grflightAngleZ = 0.0f;
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
            grgbPerVertexLighting = !grgbPerVertexLighting;
            break;
            
        case 'f':
        case 'F':
            grgbPerFragmentLighting = !grgbPerFragmentLighting;
            break;
            
        case 'x':
        case 'X':
            grgbXKeyPressed = !grgbXKeyPressed;
            break;
            
        case 'y':
        case 'Y':
            grgbYKeyPressed = !grgbYKeyPressed;
            break;
            
        case 'z':
        case 'Z':
            grgbZKeyPressed = !grgbZKeyPressed;
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









