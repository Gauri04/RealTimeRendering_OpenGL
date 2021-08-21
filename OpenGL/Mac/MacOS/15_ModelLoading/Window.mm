#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h> // analogous to windows.h or xlib.h
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>          // gl.h
#import"vmath.h"

using namespace vmath;

CVReturn myDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags, CVOptionFlags*, void*);

FILE *grgpFile = NULL;
#define BUFFER_SIZE 256
#define S_EQUAL 0
#define NR_POINT_COORDS 3
#define NR_TEXTURE_COORDS 2
#define NR_NORMAL_COORDS 3
#define NR_FACE_TOKENS 3
#define NR_TRIANGLE_VERTICES 3

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
    GLuint grgVboPosition;
    GLuint grgVboTexture;
GLuint grgVboElement;
GLuint grgMvpMatrixUniform;
mat4 grgPerspectiveProjectionMatrix;
GLuint grgVao;
GLfloat grgAnglePyramid;
    GLfloat grgAngleCube;



vec_2d_float_t* gp_vertices;
vec_2d_float_t* gp_texture;
vec_2d_float_t* gp_normals;
vec_2d_int_t* gp_face_tri;
vec_2d_int_t* gp_face_texture;
vec_2d_int_t* gp_face_normals;

FILE* g_fp_meshFile;
FILE* g_fp_logFile;
char g_line[BUFFER_SIZE];
GLuint grstone_texture;
GLuint grgtextureSamplerUniform;
    
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
    
    [self LoadMeshData];
    
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

    glBindVertexArray(0);
    
    free(vertices);
    vertices = NULL;
    free(textures);
    textures = NULL;
    
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    grgPerspectiveProjectionMatrix = mat4::identity();
    
    // initialize variables
    grgAnglePyramid = 0.0f;
    grgAngleCube = 0.0f;
    g_fp_meshFile = NULL;
    
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
    
    // OpenGL's drawing code will start from here
    glUseProgram(grgShaderProgramObject);

    // OpenGL drawing code will start here
    mat4 grmodelViewMatrix = mat4::identity();
    mat4 grmodelViewProjectionMatrix = mat4::identity();
    mat4 grrotateMatrix = mat4::identity();
    mat4 grtranslateMatrix = mat4::identity();

    //////////////// model loading //////////////
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
    //[self updateFn];
    // buffer 
    CGLFlushDrawable((CGLContextObj) [[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj) [[self openGLContext] CGLContextObj]);
}

-(void) updateFn
{
    // code
    grgAnglePyramid = grgAnglePyramid + 0.03f;
    if(grgAnglePyramid >= 360.0f)
        grgAnglePyramid = 0.0f;
    
}

-(void)LoadMeshData
{
    int lineCount = 0;;
    
    NSBundle *appBundle = [NSBundle mainBundle];
    NSString *appDirPath = [appBundle bundlePath];
    NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
    NSString *imageFileNameWithPath = [NSString stringWithFormat:@"%@/%s", parentDirPath, "monkey.obj"];
    
    const char* fileNameWithPath = [imageFileNameWithPath UTF8String];
    g_fp_meshFile = fopen(fileNameWithPath, "r");
    if (g_fp_meshFile == NULL)
    {
        fprintf(grgpFile, "\n error while reading obj file");
        fclose(grgpFile);
        exit(0);
    }
    
    gp_vertices = [self create_vec_2d_float];
    gp_texture = [self create_vec_2d_float];
    gp_normals = [self create_vec_2d_float];
    
    gp_face_tri = [self create_vec_2d_int];
    gp_face_texture = [self create_vec_2d_int];
    gp_face_normals = [self create_vec_2d_int];
    
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
            GLfloat* pvec_point_coord = (GLfloat*)[self xcalloc:NR_POINT_COORDS  :sizeof(GLfloat)];
            for (int i = 0; i != NR_POINT_COORDS; i++)
            {
                pvec_point_coord[i] = atof(strtok(NULL, sep_space));
                
            }
            [self push_back_vec_2d_float:gp_vertices :pvec_point_coord];
        }
        else if (strcmp(firstToken, "vt") == S_EQUAL)
        {
            GLfloat* pvec_texture_coord = (GLfloat*)[self xcalloc:NR_TEXTURE_COORDS :sizeof(GLfloat)];
            for (int i = 0; i != NR_TEXTURE_COORDS; i++)
            {
                pvec_texture_coord[i] = atof(strtok(NULL, sep_space));
            }
            [self push_back_vec_2d_float:gp_texture :pvec_texture_coord];
        }

        else if (strcmp(firstToken, "vn") == S_EQUAL)
        {
            GLfloat* pvec_normal_coord = (GLfloat*)[self xcalloc:NR_NORMAL_COORDS  :sizeof(GLfloat)];
            for (int i = 0; i != NR_NORMAL_COORDS; i++)
            {
                pvec_normal_coord[i] = atof(strtok(NULL, sep_space));
            }
            [self push_back_vec_2d_float:gp_normals :pvec_normal_coord];
        }
       
        else if (strcmp(firstToken, "f") == S_EQUAL)
        {
            GLint* pvecVertexIndices = (GLint*)[self xcalloc:3 :sizeof(GLint)];
            GLint* pvecTextureIndices = (GLint*)[self xcalloc:3 :sizeof(GLint)];
            GLint* pvecNormalIndices = (GLint*)[self xcalloc:3 :sizeof(GLint)];
            memset((void*)faceTokens, 0, NR_FACE_TOKENS);

            nrTokens = 0;
            token = strtok(NULL, sep_space);
            while (token != NULL)
            {
                
                if (strlen(token) < 3)
                    break;
                faceTokens[nrTokens] = token;
                nrTokens++;
                token = strtok(NULL, sep_space);
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
            [self push_back_vec_2d_int:gp_face_tri :pvecVertexIndices];
            [self push_back_vec_2d_int:gp_face_texture :pvecTextureIndices];
            [self push_back_vec_2d_int:gp_face_normals :pvecNormalIndices];
        }
        
        memset((void*)g_line, (int)'\0', BUFFER_SIZE);
        
        lineCount++;
    }

    fclose(g_fp_meshFile);
    g_fp_meshFile = NULL;
  

}

 -(vec_2d_int_t*) create_vec_2d_int
{
    vec_2d_int_t* ptr = (vec_2d_int_t*) [self xcalloc:1 :sizeof(vec_2d_int_t)];

    return(ptr);
}

-(vec_2d_float_t *)create_vec_2d_float
{
     vec_2d_float_t* ptr = (vec_2d_float_t*) [self xcalloc:1 :sizeof(vec_2d_float_t)];

    return(ptr);
}
  
-(void) push_back_vec_2d_int:(vec_2d_int_t*) pVec :(GLint*) pArr
{
    pVec->pp_arr = (GLint**)[self xrealloc:pVec->pp_arr :(pVec->size+1)*sizeof(int**)];
    pVec->size++;
    pVec->pp_arr[pVec->size - 1] = pArr;
}
                                                                 
-(void) push_back_vec_2d_float:(vec_2d_float_t*) pVec :(GLfloat*) pArr
{
     pVec->pp_arr = (GLfloat**)[self xrealloc:pVec->pp_arr :(pVec->size+1)*sizeof(GLfloat**)];
    pVec->size++;
    pVec->pp_arr[pVec->size - 1] = pArr;
}

-(void) clean_vec_2d_int:(vec_2d_int_t**) ppVec
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

-(void) clean_vec_2d_float:(vec_2d_float_t**) ppVec
{
    vec_2d_float_t* pVec = *ppVec;
    for (size_t i = 0; i != pVec->size; i++)
    {
        free(pVec->pp_arr[i]);
    }
    free(pVec);
    ppVec = NULL;
}
 
-(void*) xcalloc:(int) nrElements :(size_t) sizePerElement
{

    void* p = calloc(nrElements, sizePerElement);
    if (!p)
    {
        fprintf(grgpFile, "\n Error in xcalloc, calloc fatal memory error");
        exit(1);
    }
    return(p);
}
 
-(void*) xrealloc:(void*) p :(size_t) newSize
{
    void* ptr = realloc(p, newSize);
    if (!ptr)
    {
        fprintf(grgpFile, "\n Error in xrealloc, realloc out of memory error");
        exit(1);
    }
    return(ptr);
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
    
    if (grgVao)
    {
        glDeleteVertexArrays(1, &grgVao);
        grgVao = 0;
    }

    [self clean_vec_2d_float:&gp_vertices];
    [self clean_vec_2d_float:&gp_normals];
    [self clean_vec_2d_float:&gp_texture];

    [self clean_vec_2d_int:&gp_face_tri];
    [self clean_vec_2d_int:&gp_face_texture];
    [self clean_vec_2d_int:&gp_face_normals];
    
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









