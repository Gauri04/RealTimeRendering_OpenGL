//
//  MyView.m
//  Window
//
//  Created by user172418 on 7/4/21.
//  Copyright © 2021 com.student. All rights reserved.
//
#import<OpenGLES/ES3/gl.h>
#import<OpenGLES/ES3/glext.h>
#import "GLESView.h"
#import "vmath.h"
#import "Sphere.h"

using namespace vmath;

@implementation GLESView
{
@private
    NSString *centralText;
    EAGLContext *eaglContext;
    GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    NSInteger animationFrameInterval;
    BOOL isAnimating;
    id displayLink;
    
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

-(id)initWithFrame:(CGRect)frame
{
    // code
    self = [super initWithFrame:frame];
    if(self)
    {
        // register singletap, double tap, swipe recognizer
        UITapGestureRecognizer *singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        UISwipeGestureRecognizer *swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
        [swipeGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(onLongPress:)];
        [longPressGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:longPressGestureRecognizer];
        
        
        CAEAGLLayer *eaglLayer = (CAEAGLLayer*)[super layer];
        [eaglLayer setOpaque:YES];
        [eaglLayer setDrawableProperties:[NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil]];
        eaglContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
        if(eaglContext == nil)
        {
            printf("\n OpenGLES context creation failed");
            return(nil);
        }
        
        [EAGLContext setCurrentContext:eaglContext];
        glGenFramebuffers(1, &defaultFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        GLint backingWidth;
        GLint backingHeight;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
        
        // depth buffer
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("\n Framebuffer is not complete");
            [self uninitialize];
            return(nil);
        }
        
        animationFrameInterval = 60;    // iOS 8.2
        isAnimating = NO;
        
        // OGL code for shaders starts here
        [self initialize];
    }
    return self;
    
}

-(void)initialize
{
    grSphere = [[MySphere alloc]init];
    
    [grSphere getSphereVertexData:grsphereVertices :grsphereNormals :grsphereTextures :grsphereElements];
    grgNumVertices = [grSphere getNumberOfSphereVertice];
    grgNumElements = [grSphere getNumberOfSphereElements];
    
    printf("\n shereVertices[1] : %f \t sphereNormals[1] : %f", grsphereVertices[1], grsphereNormals[1]);
    printf("\n numVertices : %d, numElements : %d", grgNumVertices, grgNumElements);
    
    grgVertexShaderObjectPF = glCreateShader(GL_VERTEX_SHADER);
    
    ///// Vertex Shader
    const GLchar* grvertexShaderSourceCodePF =
    "#version 300 es" \
    "\n" \
    "in vec4 vPosition;" \
    "in vec3 vNormal;" \
    "uniform mat4 u_model_matrix;" \
    "uniform mat4 u_view_matrix;" \
    "uniform mat4 u_projection_matrix;" \
    "uniform mediump int u_l_key_pressed;" \
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
                printf("\n Vertex Shader PF Compilation Log : %s", grszInfoLog);
                free(grszInfoLog);
                [self uninitialize];
                
            }
        }
    }
    
    ////// Fragment Shader
    grgFragmentShaderObjectPF = glCreateShader(GL_FRAGMENT_SHADER);
    
    // source code of shader
    const GLchar* grfragmentShaderSourceCodePF =
    "#version 300 es" \
    "\n" \
    "precision highp float;" \
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
                printf( "\n Fragment Shader PF Compilation Log : %s", grszInfoLog);
                free(grszInfoLog);
                [self uninitialize];
                [self release];
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
                printf("rgpFile, ""\n Shader Program PF Link Log : %s", grszInfoLog);
                [self uninitialize];
                [self release];
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
    
    // **** Per Fragment Shader ********
    grgVertexShaderObjectPV = glCreateShader(GL_VERTEX_SHADER);
    
    ///// Vertex Shader
    const GLchar* grvertexShaderSourceCodePV =
    "#version 300 es" \
    "\n" \
    "in vec4 vPosition;" \
    "in vec3 vNormal;" \
    "uniform mat4 u_model_matrix;" \
    "uniform mat4 u_view_matrix;" \
    "uniform mat4 u_projection_matrix;" \
    "uniform mediump int u_l_key_pressed;" \
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
                printf( "\n Vertex Shader PV Compilation Log : %s", grszInfoLog);
                free(grszInfoLog);
                [self release];
                [self uninitialize];
            }
        }
    }
    
    ////// Fragment Shader
    grgFragmentShaderObjectPV = glCreateShader(GL_FRAGMENT_SHADER);
    
    // source code of shader
    const GLchar* grfragmentShaderSourceCodePV =
    "#version 300 es" \
    "\n" \
    "precision highp float;" \
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
                printf("\n Fragment Shader PV Compilation Log : %s", grszInfoLog);
                free(grszInfoLog);
                [self uninitialize];
                [self release];
                
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
                printf("\n Shader Program PF Link Log : %s", grszInfoLog);
                [self uninitialize];
                [self release];
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
    
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    grgPerspectiveProjectionMatrix = mat4::identity();
    
}

+(Class)layerClass
{
    return([CAEAGLLayer class]);
}

// method layoutSubviews is similar to resize
-(void)layoutSubviews
{
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)[self layer]];
    
    GLint width;
    GLint height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
    // depth buffer
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("\n In layoutSubViews Framebuffer is not complete");
    }
    
    if(height < 0)
    {
        height = 1;
    }
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    // here we will give ortho or perspective call
    grgPerspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    [self drawView:nil];
}

-(void)drawView:(id)sender
{
    
    [EAGLContext setCurrentContext:eaglContext];
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
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
        glUseProgram(0);
    }
        
    //[self updateFn];
    
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

-(void) updateFn
{
    // code
    
}

-(void)onSingleTap:(UIGestureRecognizer *)gr
{
    // code
    grgbIsVertexLighting = !grgbIsVertexLighting;
    
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gr
{
    // code
     grgbIsFragmentLighting = !grgbIsFragmentLighting;
}

-(void)onLongPress:(UILongPressGestureRecognizer *)gr
{
    // code
    grgbLKeyPressed = true;
}

-(void)onSwipe:(UISwipeGestureRecognizer *)gr
{
    // code
    [self uninitialize];
    [self release];
    exit(0);
}

-(void)dealloc
{
    [self uninitialize];
    [super dealloc];
}

-(void)uninitialize
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
    
    
    
    if(depthRenderbuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
    if(defaultFramebuffer)
    {
        glDeleteFramebuffers(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }
    if(colorRenderbuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
    if(eaglContext)
    {
        if([EAGLContext currentContext] == eaglContext)
        {
            [EAGLContext setCurrentContext:nil];
            [eaglContext release];
            eaglContext = 0;
        }
    }
}

-(void)startAnimation
{
    if(isAnimating == NO)
    {
        displayLink = [NSClassFromString(@"CADisplayLink")displayLinkWithTarget:self selector:@selector(drawView:)];
        [displayLink setPreferredFramesPerSecond:animationFrameInterval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop]forMode:NSDefaultRunLoopMode];
    isAnimating:YES;
    }
}

-(void)stopAnimation
{
    if(isAnimating == YES)
    {
        [displayLink invalidate];
        displayLink = nil;
        isAnimating = NO;
    }
}
@end
