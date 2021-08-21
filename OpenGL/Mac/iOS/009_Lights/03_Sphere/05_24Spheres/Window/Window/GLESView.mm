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
    
   mat4 grgPerspectiveProjectionMatrix;
        
         GLuint grgModelMatrixUniformPV;
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

     GLuint grgVertexShaderObjectPV;
     GLuint grgFragmentShaderObjectPV;
     GLuint grgShaderProgramObjectPV;


     // per fragment
     // uniforms
     GLuint grgModelMatrixUniformPF;
     GLuint grgViewMatrixUniformPF;
     GLuint grgProjectionMatrixUniformPF;
     GLuint grgLaUniformPF;
     GLuint grgLdUniformPF;
     GLuint grgLsUniformPF;
     GLuint grgLightPositionUniformPF;
     GLuint grgKdUniformPF;                    // material diffuse
     GLuint grgKaUniformPF;                    // material ambient
     GLuint grgKsUniformPF;                    // material specular
     GLuint grgMaterialShinynessUniformPF;
     GLuint grgLKeyPressedUniformPF;

     GLuint grgVertexShaderObjectPF;
     GLuint grgFragmentShaderObjectPF;
     GLuint grgShaderProgramObjectPF;

         
         GLuint grgVao1;
     GLuint grgVboSpherePosition1;
     GLuint grgVboSphereNormal1;
     GLuint grgVboSphereElement1;
     GLuint grgVao2;
     GLuint grgVboSpherePosition2;
     GLuint grgVboSphereNormal2;
     GLuint grgVboSphereElement2;
     GLuint grgVao3;
     GLuint grgVboSpherePosition3;
     GLuint grgVboSphereNormal3;
     GLuint grgVboSphereElement3;
     GLuint grgVao4;
     GLuint grgVboSpherePosition4;
     GLuint grgVboSphereNormal4;
     GLuint grgVboSphereElement4;
     GLuint grgVao5;
     GLuint grgVboSpherePosition5;
     GLuint grgVboSphereNormal5;
     GLuint grgVboSphereElement5;
     GLuint grgVao6;
     GLuint grgVboSpherePosition6;
     GLuint grgVboSphereNormal6;
     GLuint grgVboSphereElement6;
     GLuint grgVao7;
     GLuint grgVboSpherePosition7;
     GLuint grgVboSphereNormal7;
     GLuint grgVboSphereElement7;
     GLuint grgVao8;
     GLuint grgVboSpherePosition8;
     GLuint grgVboSphereNormal8;
     GLuint grgVboSphereElement8;
     GLuint grgVao9;
     GLuint grgVboSpherePosition9;
     GLuint grgVboSphereNormal9;
     GLuint grgVboSphereElement9;
     GLuint grgVao10;
     GLuint grgVboSpherePosition10;
     GLuint grgVboSphereNormal10;
     GLuint grgVboSphereElement10;
     GLuint grgVao11;
     GLuint grgVboSpherePosition11;
     GLuint grgVboSphereNormal11;
     GLuint grgVboSphereElement11;
     GLuint grgVao12;
     GLuint grgVboSpherePosition12;
     GLuint grgVboSphereNormal12;
     GLuint grgVboSphereElement12;
     GLuint grgVao13;
     GLuint grgVboSpherePosition13;
     GLuint grgVboSphereNormal13;
     GLuint grgVboSphereElement13;
     GLuint grgVao14;
     GLuint grgVboSpherePosition14;
     GLuint grgVboSphereNormal14;
     GLuint grgVboSphereElement14;
     GLuint grgVao15;
     GLuint grgVboSpherePosition15;
     GLuint grgVboSphereNormal15;
     GLuint grgVboSphereElement15;
     GLuint grgVao16;
     GLuint grgVboSpherePosition16;
     GLuint grgVboSphereNormal16;
     GLuint grgVboSphereElement16;
     GLuint grgVao17;
     GLuint grgVboSpherePosition17;
     GLuint grgVboSphereNormal17;
     GLuint grgVboSphereElement17;
     GLuint grgVao18;
     GLuint grgVboSpherePosition18;
     GLuint grgVboSphereNormal18;
     GLuint grgVboSphereElement18;
     GLuint grgVao19;
     GLuint grgVboSpherePosition19;
     GLuint grgVboSphereNormal19;
     GLuint grgVboSphereElement19;
     GLuint grgVao20;
     GLuint grgVboSpherePosition20;
     GLuint grgVboSphereNormal20;
     GLuint grgVboSphereElement20;
     GLuint grgVao21;
     GLuint grgVboSpherePosition21;
     GLuint grgVboSphereNormal21;
     GLuint grgVboSphereElement21;
     GLuint grgVao22;
     GLuint grgVboSpherePosition22;
     GLuint grgVboSphereNormal22;
     GLuint grgVboSphereElement22;
     GLuint grgVao23;
     GLuint grgVboSpherePosition23;
     GLuint grgVboSphereNormal23;
     GLuint grgVboSphereElement23;
     GLuint grgVao24;
     GLuint grgVboSpherePosition24;
     GLuint grgVboSphereNormal24;
     GLuint grgVboSphereElement24;
         
     vec3 grgKa1;
     vec3 grgKd1;
     vec3 grgKs1;
     GLfloat grgMaterialShinyness1;

     vec3 grgKa2;
     vec3 grgKd2;
     vec3 grgKs2;
     GLfloat grgMaterialShinyness2;

     vec3 grgKa3;
     vec3 grgKd3;
     vec3 grgKs3;
     GLfloat grgMaterialShinyness3;

     vec3 grgKa4;
     vec3 grgKd4;
     vec3 grgKs4;
     GLfloat grgMaterialShinyness4;

     vec3 grgKa5;
     vec3 grgKd5;
     vec3 grgKs5;
     GLfloat grgMaterialShinyness5;

     vec3 grgKa6;
     vec3 grgKd6;
     vec3 grgKs6;
     GLfloat grgMaterialShinyness6;

     vec3 grgKa7;
     vec3 grgKd7;
     vec3 grgKs7;
     GLfloat grgMaterialShinyness7;

     vec3 grgKa8;
     vec3 grgKd8;
     vec3 grgKs8;
     GLfloat grgMaterialShinyness8;

     vec3 grgKa9;
     vec3 grgKd9;
     vec3 grgKs9;
     GLfloat grgMaterialShinyness9;

     vec3 grgKa10;
     vec3 grgKd10;
     vec3 grgKs10;
     GLfloat grgMaterialShinyness10;

     vec3 grgKa11;
     vec3 grgKd11;
     vec3 grgKs11;
     GLfloat grgMaterialShinyness11;

     vec3 grgKa12;
     vec3 grgKd12;
     vec3 grgKs12;
     GLfloat grgMaterialShinyness12;

     vec3 grgKa13;
     vec3 grgKd13;
     vec3 grgKs13;
     GLfloat grgMaterialShinyness13;

     vec3 grgKa14;
     vec3 grgKd14;
     vec3 grgKs14;
     GLfloat grgMaterialShinyness14;

     vec3 grgKa15;
     vec3 grgKd15;
     vec3 grgKs15;
     GLfloat grgMaterialShinyness15;

     vec3 grgKa16;
     vec3 grgKd16;
     vec3 grgKs16;
     GLfloat grgMaterialShinyness16;

     vec3 grgKa17;
     vec3 grgKd17;
     vec3 grgKs17;
     GLfloat grgMaterialShinyness17;

     vec3 grgKa18;
     vec3 grgKd18;
     vec3 grgKs18;
     GLfloat grgMaterialShinyness18;

     vec3 grgKa19;
     vec3 grgKd19;
     vec3 grgKs19;
     GLfloat grgMaterialShinyness19;

     vec3 grgKa20;
     vec3 grgKd20;
     vec3 grgKs20;
     GLfloat grgMaterialShinyness20;

     vec3 grgKa21;
     vec3 grgKd21;
     vec3 grgKs21;
     GLfloat grgMaterialShinyness21;

     vec3 grgKa22;
     vec3 grgKd22;
     vec3 grgKs22;
     GLfloat grgMaterialShinyness22;

     vec3 grgKa23;
     vec3 grgKd23;
     vec3 grgKs23;
     GLfloat grgMaterialShinyness23;

     vec3 grgKa24;
     vec3 grgKd24;
     vec3 grgKs24;
     GLfloat grgMaterialShinyness24;

         vec3 grglightAmbientColor;
         vec3 grglightDiffuseColor;
         vec3 grglightSpecularColor;
         vec4 grglightPosition;
         GLfloat grgMaterialShinyness;
     bool grgbPerVertexLighting;
     bool grgbPerFragmentLighting;
         bool grgbLKeyPressed;
         int grgKeyPress;
         GLfloat grflightAngleX;
     GLfloat grflightAngleY;
     GLfloat grflightAngleZ;
     int i;
         float left, right, top, bottom;
         
         int grgNumElements;
         int grgNumVertices;
         float grsphere_vertices[1146];
         float grsphere_normals[1146];
         float grsphere_textures[764];
         unsigned short grsphere_elements[2280];
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
    
     [grSphere getSphereVertexData:grsphere_vertices :grsphere_normals :grsphere_textures :grsphere_elements];
    grgNumVertices = [grSphere getNumberOfSphereVertice];
    grgNumElements = [grSphere getNumberOfSphereElements];
       
    printf("\n shereVertices[1] : %f \t sphereNormals[1] : %f", grsphere_vertices[1], grsphere_normals[1]);
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
    "out vec3 view_vector;" \
    "int i;" \
    "void main(void)" \
    "{" \
        "if(u_l_key_pressed == 1)" \
        "{" \
            "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;" \
            "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
            "view_vector = normalize(-eye_coordinates.xyz);" \
            "light_direction = normalize(vec3(u_light_position - eye_coordinates));" \
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
    "vec3 fong_ads_light;" \
    "out vec4 FragColor;" \
    "int i;" \
    "void main(void)" \
    "{" \
        "if(u_l_key_pressed == 1)" \
        "{" \
            "vec3 reflection_vector = reflect(-light_direction, transformed_normal);" \
            "vec3 ambient = u_la *u_ka;" \
            "vec3 diffuse = u_ld * u_kd * max(dot(light_direction, transformed_normal), 0.0);" \
            "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector, view_vector), 0.0), u_material_shinyness);" \
            "fong_ads_light = ambient + diffuse + specular;" \
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
    
    grgModelMatrixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_model_matrix");
    grgViewMatrixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_view_matrix");
    grgProjectionMatrixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_projection_matrix");

    grgModelMatrixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_model_matrix");
    grgViewMatrixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_view_matrix");
    grgProjectionMatrixUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_projection_matrix");

    grgLdUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_ld");
    grgLaUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_la");
    grgLsUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_ls");
    grgLightPositionUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_light_position");


    grgKaUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_ka");
    grgKdUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_kd");
    grgKsUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_ks");

    grgLKeyPressedUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_l_key_pressed");
    grgMaterialShinynessUniformPF = glGetUniformLocation(grgShaderProgramObjectPF, "u_material_shinyness");
    
    // **** Per Vertex Shader ********
    grgVertexShaderObjectPV = glCreateShader(GL_VERTEX_SHADER);
    
    ///// Vertex Shader
    const GLchar* grvertexShaderSourceCodePV =
    "#version 300 es" \
    "\n" \
    "in vec4 vPosition;" \
         "in vec3 vNormal;"  \
         "uniform mat4 u_model_matrix;" \
         "uniform mat4 u_view_matrix;" \
         "uniform mat4 u_projection_matrix;" \
         "uniform mediump int u_l_key_pressed;" \
         "uniform vec3 u_ld;" \
         "uniform vec3 u_la;" \
         "uniform vec3 u_ls;" \
         "uniform vec3 u_ka;" \
         "uniform vec3 u_kd;" \
         "uniform vec3 u_ks;" \
         "uniform float u_material_shinyness;" \
         "uniform vec4 u_light_position;" \
         "out vec3 out_phong_light;" \
         "void main(void)" \
         "{" \
         "if(u_l_key_pressed == 1)" \
         "{" \
         "vec4 eye_coordinates = u_model_matrix * u_view_matrix * vPosition;" \
         "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
         "vec3 light_direction = normalize(vec3(u_light_position - eye_coordinates));" \
         "vec3 reflection_vector = reflect(-light_direction, transformed_normal);" \
         "vec3 view_vector = normalize(-eye_coordinates.xyz);" \
         "vec3 ambient = u_la * u_ka;" \
         "vec3 diffuse = u_ld * u_kd * max(dot(light_direction, transformed_normal), 0.0);" \
         "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector, view_vector), 0.0), u_material_shinyness);" \
         "out_phong_light = ambient + diffuse + specular;" \
         "}" \
         "else" \
         "{" \
         "out_phong_light = vec3(1.0, 1.0, 1.0);" \
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
    "in vec3 out_phong_light;" \
    "out vec4 FragColor;" \
    "void main(void)" \
    "{" \
    "FragColor = vec4(out_phong_light, 1.0);" \
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
    
   grgModelMatrixUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_model_matrix");
    grgViewMatrixUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_view_matrix");
    grgProjectionMatrixUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_projection_matrix");

    grgLdUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_ld");
    grgLaUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_la");
    grgLsUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_ls");
    grgLightPositionUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_light_position");
    grgKaUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_ka");
    grgKdUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_kd");
    grgKsUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_ks");

    grgLKeyPressedUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_l_key_pressed");
    grgMaterialShinynessUniformPV = glGetUniformLocation(grgShaderProgramObjectPV, "u_material_shinyness");
    
    [self initialize24Spheres];
    
    
   // **** VAO and VBO for 24 SPHERES ***********************
   
   ////// vao for sphere 1
   glGenVertexArrays(1, &grgVao1);
   glBindVertexArray(grgVao1);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition1);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition1);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal1);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal1);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement1);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement1);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);


   /////////// vao for sphere 2
   glGenVertexArrays(1, &grgVao2);
   glBindVertexArray(grgVao2);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition2);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition2);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal2);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal2);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement2);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement2);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 3
   glGenVertexArrays(1, &grgVao3);
   glBindVertexArray(grgVao3);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition3);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition3);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal3);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal3);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement3);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement3);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 5
   glGenVertexArrays(1, &grgVao4);
   glBindVertexArray(grgVao4);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition4);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition4);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal4);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal4);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement4);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement4);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 5
   glGenVertexArrays(1, &grgVao5);
   glBindVertexArray(grgVao5);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition5);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition5);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal5);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal5);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement5);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement5);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 6
   glGenVertexArrays(1, &grgVao6);
   glBindVertexArray(grgVao6);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition6);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition6);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal6);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal6);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement6);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement6);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 7
   glGenVertexArrays(1, &grgVao7);
   glBindVertexArray(grgVao7);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition7);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition7);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal7);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal7);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement7);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement7);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 8
   glGenVertexArrays(1, &grgVao8);
   glBindVertexArray(grgVao8);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition8);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition8);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal8);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal8);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement8);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement8);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);


   /////////// vao for sphere 9
   glGenVertexArrays(1, &grgVao9);
   glBindVertexArray(grgVao9);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition9);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition9);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal9);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal9);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement9);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement9);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 10
   glGenVertexArrays(1, &grgVao10);
   glBindVertexArray(grgVao10);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition10);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition10);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal10);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal10);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement10);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement10);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 11
   glGenVertexArrays(1, &grgVao11);
   glBindVertexArray(grgVao11);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition11);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition11);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal11);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal11);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement11);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement11);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);


   /////////// vao for sphere 12
   glGenVertexArrays(1, &grgVao12);
   glBindVertexArray(grgVao12);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition12);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition12);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal12);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal12);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement12);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement12);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 13
   glGenVertexArrays(1, &grgVao13);
   glBindVertexArray(grgVao13);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition13);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition13);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal13);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal13);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement13);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement13);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 14
   glGenVertexArrays(1, &grgVao14);
   glBindVertexArray(grgVao14);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition14);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition14);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal14);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal14);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement14);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement14);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);


   /////////// vao for sphere 15
   glGenVertexArrays(1, &grgVao15);
   glBindVertexArray(grgVao15);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition15);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition15);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal15);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal15);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement15);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement15);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);


   /////////// vao for sphere 16
   glGenVertexArrays(1, &grgVao16);
   glBindVertexArray(grgVao16);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition16);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition16);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal16);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal16);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement16);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement16);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);


   /////////// vao for sphere 17
   glGenVertexArrays(1, &grgVao17);
   glBindVertexArray(grgVao17);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition17);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition17);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal17);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal17);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement17);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement17);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 18
   glGenVertexArrays(1, &grgVao18);
   glBindVertexArray(grgVao18);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition18);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition18);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal18);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal18);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement18);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement18);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 19
   glGenVertexArrays(1, &grgVao19);
   glBindVertexArray(grgVao19);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition19);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition19);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal19);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal19);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement19);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement19);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 20
   glGenVertexArrays(1, &grgVao20);
   glBindVertexArray(grgVao20);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition20);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition20);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal20);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal20);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement20);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement20);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);


   /////////// vao for sphere 21
   glGenVertexArrays(1, &grgVao21);
   glBindVertexArray(grgVao21);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition21);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition21);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal21);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal21);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement21);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement21);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 22
   glGenVertexArrays(1, &grgVao22);
   glBindVertexArray(grgVao22);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition22);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition22);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal22);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal22);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement22);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement22);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);


   /////////// vao for sphere 23
   glGenVertexArrays(1, &grgVao23);
   glBindVertexArray(grgVao23);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition23);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition23);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal23);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal23);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement23);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement23);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);

   /////////// vao for sphere 24
   glGenVertexArrays(1, &grgVao24);
   glBindVertexArray(grgVao24);
   // position vbo
   glGenBuffers(1, &grgVboSpherePosition24);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition24);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_vertices), grsphere_vertices, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // normal vbo
   glGenBuffers(1, &grgVboSphereNormal24);
   glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal24);
   glBufferData(GL_ARRAY_BUFFER, sizeof(grsphere_normals), grsphere_normals, GL_STATIC_DRAW);
   glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
   glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // element vbo
   glGenBuffers(1, &grgVboSphereElement24);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement24);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphere_elements), grsphere_elements, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // unbind vao
   glBindVertexArray(0);
    
    
    // initialize variables
    grgbLKeyPressed = false;
    grgbPerVertexLighting = false;
    grgbPerFragmentLighting =false;
    grgKeyPress = 0;
    grglightAmbientColor = vec3(0.0f, 0.0f, 0.0f);
    grglightDiffuseColor = vec3(1.0f, 1.0f, 1.0f);
    grglightSpecularColor = vec3(1.0f, 1.0f, 1.0f);
    grglightPosition = vec4(0.0f, 0.0f, 100.0f, 0.0f);
    
    
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glClearColor(0.25, 0.25, 0.25, 1.0);
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
    
    left = 0.0f;
    right = ((GLfloat)width / (GLfloat)6);
    top = (GLfloat)0.0f;
    bottom = ((GLfloat)height / (GLfloat)4);
    
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
       mat4 grtranslateMatrix = mat4::identity();

       if(grgbPerFragmentLighting == true)
       {
           grmodelMatrix = mat4::identity();
           grviewMatrix = mat4::identity();
           grprojectionMatrix = mat4::identity();
           grtranslateMatrix = mat4::identity();
           
           // start use of shader program
           glUseProgram(grgShaderProgramObjectPF);
           
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grmodelMatrix = grtranslateMatrix;

           grprojectionMatrix = grgPerspectiveProjectionMatrix;

           if (grgbLKeyPressed == true)
           {

               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd1);
               glUniform3fv(grgKaUniformPF, 1, grgKa1);
               glUniform3fv(grgKsUniformPF, 1, grgKs1);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness1);

               if (grgKeyPress == 1)
               {
                   grglightPosition[0] = 0.0;
                   grglightPosition[1] = (GLfloat) 21 * sin(grflightAngleX);
                   grglightPosition[2] = (GLfloat) 21 * cos(grflightAngleX);
                   grglightPosition[3] = 0.0;                            // w for directional light
               }
               else if (grgKeyPress == 2)
               {
                   grglightPosition[0] = (GLfloat)20 * sin(grflightAngleY);
                   grglightPosition[1] = 0.0;
                   grglightPosition[2] = (GLfloat)20 * cos(grflightAngleY);
                   grglightPosition[3] = 0.0;                            // w for directional light
               }

               else if (grgKeyPress == 3)
               {
                   grglightPosition[0] = (GLfloat)20 * sin(grflightAngleZ);;
                   grglightPosition[1] = (GLfloat)20 * cos(grflightAngleZ);
                   grglightPosition[2] = 0.0f;
                   grglightPosition[3] = 0.0;                            // w for directional light
               }

               glUniform3fv(grgLdUniformPF, 1, grglightDiffuseColor);
               glUniform3fv(grgLaUniformPF, 1, grglightAmbientColor);
               glUniform3fv(grgLsUniformPF, 1, grglightSpecularColor);
               glUniform4fv(grgLightPositionUniformPF, 1, grglightPosition);

           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(left, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);

           // bind vao
           glBindVertexArray(grgVao1);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement1);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);
           glBindVertexArray(0);
           
           
           //////////// sphere 2
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd2);
               glUniform3fv(grgKaUniformPF, 1, grgKa2);
               glUniform3fv(grgKsUniformPF, 1, grgKs2);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness2);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }

           glViewport(right, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao2);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement2);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);
           

           //////////// sphere 3
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd3);
               glUniform3fv(grgKaUniformPF, 1, grgKa3);
               glUniform3fv(grgKsUniformPF, 1, grgKs3);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness3);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 2, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao3);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement3);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////// sphere 4
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd4);
               glUniform3fv(grgKaUniformPF, 1, grgKa4);
               glUniform3fv(grgKsUniformPF, 1, grgKs4);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness4);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 3, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao4);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement4);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////// sphere 5
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           glViewport(right * 2, bottom * 3, right, bottom);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd5);
               glUniform3fv(grgKaUniformPF, 1, grgKa5);
               glUniform3fv(grgKsUniformPF, 1, grgKs5);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness5);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 4, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao5);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement5);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////// sphere 6
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd6);
               glUniform3fv(grgKaUniformPF, 1, grgKa6);
               glUniform3fv(grgKsUniformPF, 1, grgKs6);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness6);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 5, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao6);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement6);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 7
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd7);
               glUniform3fv(grgKaUniformPF, 1, grgKa7);
               glUniform3fv(grgKsUniformPF, 1, grgKs7);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness7);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(left, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao7);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement7);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 8
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd8);
               glUniform3fv(grgKaUniformPF, 1, grgKa8);
               glUniform3fv(grgKsUniformPF, 1, grgKs8);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness8);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao8);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement8);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 9
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd9);
               glUniform3fv(grgKaUniformPF, 1, grgKa9);
               glUniform3fv(grgKsUniformPF, 1, grgKs9);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness9);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 2, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao9);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement9);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 10
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd10);
               glUniform3fv(grgKaUniformPF, 1, grgKa10);
               glUniform3fv(grgKsUniformPF, 1, grgKs10);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness10);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 3, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao10);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement10);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 11
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd11);
               glUniform3fv(grgKaUniformPF, 1, grgKa11);
               glUniform3fv(grgKsUniformPF, 1, grgKs11);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness11);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 4, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao11);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement11);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 12
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd12);
               glUniform3fv(grgKaUniformPF, 1, grgKa12);
               glUniform3fv(grgKsUniformPF, 1, grgKs12);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness12);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 5, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao12);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement12);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////// sphere 13
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd13);
               glUniform3fv(grgKaUniformPF, 1, grgKa13);
               glUniform3fv(grgKsUniformPF, 1, grgKs13);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness13);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(left, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao13);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement13);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ///////////// sphere 14
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd14);
               glUniform3fv(grgKaUniformPF, 1, grgKa14);
               glUniform3fv(grgKsUniformPF, 1, grgKs14);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness14);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao14);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement14);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ///////////// sphere 15
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd15);
               glUniform3fv(grgKaUniformPF, 1, grgKa15);
               glUniform3fv(grgKsUniformPF, 1, grgKs15);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness15);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 2, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao15);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement15);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ///////////// sphere 16
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd16);
               glUniform3fv(grgKaUniformPF, 1, grgKa16);
               glUniform3fv(grgKsUniformPF, 1, grgKs16);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness16);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 3, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao16);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement16);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ///////////// sphere 17
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd17);
               glUniform3fv(grgKaUniformPF, 1, grgKa17);
               glUniform3fv(grgKsUniformPF, 1, grgKs17);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness17);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 4, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao17);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement17);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ///////////// sphere 18
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd18);
               glUniform3fv(grgKaUniformPF, 1, grgKa18);
               glUniform3fv(grgKsUniformPF, 1, grgKs18);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness18);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 5, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao18);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement18);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);
           
           ////////////////////////////////////////////////////// sphere 19
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd19);
               glUniform3fv(grgKaUniformPF, 1, grgKa19);
               glUniform3fv(grgKsUniformPF, 1, grgKs19);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness19);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(left, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao19);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement19);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////////////////////////////////////////////////////// sphere 20
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd20);
               glUniform3fv(grgKaUniformPF, 1, grgKa20);
               glUniform3fv(grgKsUniformPF, 1, grgKs20);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness20);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao20);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement20);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////////////////////////////////////////////////////// sphere 21
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd21);
               glUniform3fv(grgKaUniformPF, 1, grgKa21);
               glUniform3fv(grgKsUniformPF, 1, grgKs21);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness21);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 2, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao21);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement21);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////////////////////////////////////////////////////// sphere 22
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd22);
               glUniform3fv(grgKaUniformPF, 1, grgKa22);
               glUniform3fv(grgKsUniformPF, 1, grgKs22);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness22);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 3, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao22);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement22);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////////////////////////////////////////////////////// sphere 23
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd23);
               glUniform3fv(grgKaUniformPF, 1, grgKa23);
               glUniform3fv(grgKsUniformPF, 1, grgKs23);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness23);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 4, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao23);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement23);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////////////////////////////////////////////////////// sphere 24
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPF, 1);
               glUniform3fv(grgKdUniformPF, 1, grgKd24);
               glUniform3fv(grgKaUniformPF, 1, grgKa24);
               glUniform3fv(grgKsUniformPF, 1, grgKs24);
               glUniform1f(grgMaterialShinynessUniformPF, grgMaterialShinyness24);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPF, 0);
           }
           glViewport(right * 5, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPF, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPF, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPF, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao24);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement24);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);
           
          [self updateFn];
           // stop use of shader program
           glUseProgram(0);
       }
       else 
       {
           grmodelMatrix = mat4::identity();
           grviewMatrix = mat4::identity();
           grprojectionMatrix = mat4::identity();
           grtranslateMatrix = mat4::identity();
           
           // start use of shader program
           glUseProgram(grgShaderProgramObjectPV);
           
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grmodelMatrix = grtranslateMatrix;

           grprojectionMatrix = grgPerspectiveProjectionMatrix;

           if (grgbLKeyPressed == true)
           {

               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd1);
               glUniform3fv(grgKaUniformPV, 1, grgKa1);
               glUniform3fv(grgKsUniformPV, 1, grgKs1);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness1);

               if (grgKeyPress == 1)
               {
                   grglightPosition[0] = 0.0;
                   grglightPosition[1] = (GLfloat) 21 * sin(grflightAngleX);
                   grglightPosition[2] = (GLfloat) 21 * cos(grflightAngleX);
                   grglightPosition[3] = 0.0;                            // w for directional light
               }
               else if (grgKeyPress == 2)
               {
                   grglightPosition[0] = (GLfloat)20 * sin(grflightAngleY);
                   grglightPosition[1] = 0.0;
                   grglightPosition[2] = (GLfloat)20 * cos(grflightAngleY);
                   grglightPosition[3] = 0.0;                            // w for directional light
               }

               else if (grgKeyPress == 3)
               {
                   grglightPosition[0] = (GLfloat)20 * sin(grflightAngleZ);;
                   grglightPosition[1] = (GLfloat)20 * cos(grflightAngleZ);
                   grglightPosition[2] = 0.0f;
                   grglightPosition[3] = 0.0;                            // w for directional light
               }

               glUniform3fv(grgLdUniformPV, 1, grglightDiffuseColor);
               glUniform3fv(grgLaUniformPV, 1, grglightAmbientColor);
               glUniform3fv(grgLsUniformPV, 1, grglightSpecularColor);
               glUniform4fv(grgLightPositionUniformPV, 1, grglightPosition);

           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(left, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);

           // bind vao
           glBindVertexArray(grgVao1);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement1);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);
           glBindVertexArray(0);
           
           
           //////////// sphere 2
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd2);
               glUniform3fv(grgKaUniformPV, 1, grgKa2);
               glUniform3fv(grgKsUniformPV, 1, grgKs2);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness2);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }

           glViewport(right, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao2);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement2);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);
           

           //////////// sphere 3
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd3);
               glUniform3fv(grgKaUniformPV, 1, grgKa3);
               glUniform3fv(grgKsUniformPV, 1, grgKs3);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness3);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 2, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao3);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement3);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////// sphere 4
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd4);
               glUniform3fv(grgKaUniformPV, 1, grgKa4);
               glUniform3fv(grgKsUniformPV, 1, grgKs4);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness4);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 3, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao4);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement4);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////// sphere 5
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           glViewport(right * 2, bottom * 3, right, bottom);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd5);
               glUniform3fv(grgKaUniformPV, 1, grgKa5);
               glUniform3fv(grgKsUniformPV, 1, grgKs5);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness5);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 4, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao5);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement5);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////// sphere 6
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd6);
               glUniform3fv(grgKaUniformPV, 1, grgKa6);
               glUniform3fv(grgKsUniformPV, 1, grgKs6);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness6);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 5, bottom * 3, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao6);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement6);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 7
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd7);
               glUniform3fv(grgKaUniformPV, 1, grgKa7);
               glUniform3fv(grgKsUniformPV, 1, grgKs7);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness7);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(left, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao7);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement7);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 8
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd8);
               glUniform3fv(grgKaUniformPV, 1, grgKa8);
               glUniform3fv(grgKsUniformPV, 1, grgKs8);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness8);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao8);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement8);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 9
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd9);
               glUniform3fv(grgKaUniformPV, 1, grgKa9);
               glUniform3fv(grgKsUniformPV, 1, grgKs9);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness9);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 2, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao9);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement9);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 10
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd10);
               glUniform3fv(grgKaUniformPV, 1, grgKa10);
               glUniform3fv(grgKsUniformPV, 1, grgKs10);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness10);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 3, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao10);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement10);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 11
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd11);
               glUniform3fv(grgKaUniformPV, 1, grgKa11);
               glUniform3fv(grgKsUniformPV, 1, grgKs11);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness11);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 4, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao11);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement11);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////////////////////////////////////////////////////// sphere 12
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd12);
               glUniform3fv(grgKaUniformPV, 1, grgKa12);
               glUniform3fv(grgKsUniformPV, 1, grgKs12);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness12);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 5, bottom * 2, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao12);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement12);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ////////////// sphere 13
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd13);
               glUniform3fv(grgKaUniformPV, 1, grgKa13);
               glUniform3fv(grgKsUniformPV, 1, grgKs13);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness13);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(left, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao13);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement13);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ///////////// sphere 14
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd14);
               glUniform3fv(grgKaUniformPV, 1, grgKa14);
               glUniform3fv(grgKsUniformPV, 1, grgKs14);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness14);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao14);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement14);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ///////////// sphere 15
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd15);
               glUniform3fv(grgKaUniformPV, 1, grgKa15);
               glUniform3fv(grgKsUniformPV, 1, grgKs15);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness15);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 2, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao15);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement15);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ///////////// sphere 16
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd16);
               glUniform3fv(grgKaUniformPV, 1, grgKa16);
               glUniform3fv(grgKsUniformPV, 1, grgKs16);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness16);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 3, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao16);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement16);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ///////////// sphere 17
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd17);
               glUniform3fv(grgKaUniformPV, 1, grgKa17);
               glUniform3fv(grgKsUniformPV, 1, grgKs17);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness17);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 4, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao17);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement17);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           ///////////// sphere 18
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd18);
               glUniform3fv(grgKaUniformPV, 1, grgKa18);
               glUniform3fv(grgKsUniformPV, 1, grgKs18);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness18);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 5, bottom, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao18);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement18);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);
           
           ////////////////////////////////////////////////////// sphere 19
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd19);
               glUniform3fv(grgKaUniformPV, 1, grgKa19);
               glUniform3fv(grgKsUniformPV, 1, grgKs19);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness19);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(left, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao19);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement19);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////////////////////////////////////////////////////// sphere 20
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd20);
               glUniform3fv(grgKaUniformPV, 1, grgKa20);
               glUniform3fv(grgKsUniformPV, 1, grgKs20);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness20);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao20);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement20);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////////////////////////////////////////////////////// sphere 21
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd21);
               glUniform3fv(grgKaUniformPV, 1, grgKa21);
               glUniform3fv(grgKsUniformPV, 1, grgKs21);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness21);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 2, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao21);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement21);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////////////////////////////////////////////////////// sphere 22
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd22);
               glUniform3fv(grgKaUniformPV, 1, grgKa22);
               glUniform3fv(grgKsUniformPV, 1, grgKs22);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness22);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 3, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao22);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement22);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////////////////////////////////////////////////////// sphere 23
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd23);
               glUniform3fv(grgKaUniformPV, 1, grgKa23);
               glUniform3fv(grgKsUniformPV, 1, grgKs23);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness23);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 4, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao23);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement23);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);

           //////////////////////////////////////////////////////////// sphere 24
           grtranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
           grprojectionMatrix = grgPerspectiveProjectionMatrix;
           grmodelMatrix = grtranslateMatrix;

           if (grgbLKeyPressed == true)
           {
               glUniform1i(grgLKeyPressedUniformPV, 1);
               glUniform3fv(grgKdUniformPV, 1, grgKd24);
               glUniform3fv(grgKaUniformPV, 1, grgKa24);
               glUniform3fv(grgKsUniformPV, 1, grgKs24);
               glUniform1f(grgMaterialShinynessUniformPV, grgMaterialShinyness24);
           }
           else
           {
               glUniform1i(grgLKeyPressedUniformPV, 0);
           }
           glViewport(right * 5, top, right, bottom);
           glUniformMatrix4fv(grgModelMatrixUniformPV, 1, GL_FALSE, grmodelMatrix);
           glUniformMatrix4fv(grgViewMatrixUniformPV, 1, GL_FALSE, grviewMatrix);
           glUniformMatrix4fv(grgProjectionMatrixUniformPV, 1, GL_FALSE, grprojectionMatrix);
           // bind vao
           glBindVertexArray(grgVao24);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement24);
           glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);

           // unbind vao
           glBindVertexArray(0);
           
            [self updateFn];
           // stop use of shader program
           glUseProgram(0);
       }
    
    [self updateFn];
    
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

-(void) updateFn
{
    // code
     if (grgKeyPress == 1)
       {
           grflightAngleX = grflightAngleX + 0.03f;
           if (grflightAngleX >= 360.0f)
               grflightAngleX = 0.0f;

       }
       else if (grgKeyPress == 2)
       {
           grflightAngleY = grflightAngleY + 0.035f;
           if (grflightAngleY >= 360.0f)
               grflightAngleY = 0.0f;
       }
       else if (grgKeyPress == 3)
       {
           grflightAngleZ = grflightAngleZ + 0.028f;
           if (grflightAngleZ >= 360.0f)
               grflightAngleZ = 0.0f;
       }
}

-(void)onSingleTap:(UIGestureRecognizer *)gr
{
    // code
    grgKeyPress++;
    
    if(grgKeyPress >= 4)
    {
        grgKeyPress = 0;
    }
    
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gr
{
    // code
     grgbPerFragmentLighting = !grgbPerFragmentLighting;
    //grgbPerVertexLighting = !grgbPerVertexLighting;
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

-(void) initialize24Spheres
{
    grgKa1 = vec3(0.0215,
        0.1745,
        0.0215
        );
    grgKd1 = vec3(0.07568,
        0.61424,
        0.07568
        );
    grgKs1 = vec3(0.633,
        0.727811,
        0.633
        );
    grgMaterialShinyness1 = 0.6 * 128;

    grgKa2 = vec3(0.135,
        0.2225,
        0.1575
        );
    grgKd2 = vec3(0.54, // r
        0.89, // g
        0.63
    );
    grgKs2 = vec3(0.316228, // r
        0.316228, // g
        0.316228
    );
    grgMaterialShinyness2 = 0.1 * 128;

    grgKa3 = vec3(0.05375, // r
        0.05,    // g
        0.06625
    );
    grgKd3 = vec3(0.18275, // r
        0.17,    // g
        0.22525
    );
    grgKs3 = vec3(0.332741, // r
        0.328634, // g
        0.346435
    );
    grgMaterialShinyness3 = 0.3 * 128;

    grgKa4 = vec3(0.25,    // r
        0.20725, // g
        0.20725
    );
    grgKd4 = vec3(1.0,   // r
        0.829, // g
        0.829
    );
    grgKs4 = vec3(0.296648, // r
        0.296648, // g
        0.296648
    );
    grgMaterialShinyness4 = 0.088 * 128;

    grgKa5 = vec3(0.1745,  // r
        0.01175, // g
        0.01175
    );
    grgKd5 = vec3(0.61424, // r
        0.04136, // g
        0.04136   // a
    );
    grgKs5 = vec3(0.727811, // r
        0.626959, // g
        0.626959
    );
    grgMaterialShinyness5 = 0.6 * 128;

    grgKa6 = vec3(0.1,     // r
        0.18725, // g
        0.1745
    );
    grgKd6 = vec3(0.396,   // r
        0.74151, // g
        0.69102
    );
    grgKs6 = vec3(0.297254, // r
        0.30829,  // g
        0.306678
    );
    grgMaterialShinyness6 = 0.1 * 128;

    grgKa7 = vec3(0.329412, // r
        0.223529, // g
        0.027451
    );
    grgKd7 = vec3(0.780392, // r
        0.568627, // g
        0.113725
    );
    grgKs7 = vec3(0.992157, // r
        0.941176, // g
        0.807843
    );
    grgMaterialShinyness7 = 0.21794872 * 128;

    grgKa8 = vec3(0.2125, // r
        0.1275, // g
        0.054
    );
    grgKd8 = vec3(0.714,   // r
        0.4284,  // g
        0.18144
    );
    grgKs8 = vec3(0.393548, // r
        0.271906, // g
        0.166721
    );
    grgMaterialShinyness8 = 0.2 * 128;

    grgKa9 = vec3(0.25, // r
        0.25, // g
        0.25
    );
    grgKd9 = vec3(0.4,  // r
        0.4,  // g
        0.4
    );
    grgKs9 = vec3(0.774597, // r
        0.774597, // g
        0.774597
    );
    grgMaterialShinyness9 = 0.6 * 128;

    grgKa10 = vec3(0.19125, // r
        0.0735,  // g
        0.0225
    );
    grgKd10 = vec3(0.7038,  // r
        0.27048, // g
        0.0828
    );
    grgKs10 = vec3(0.256777, // r
        0.137622, // g
        0.086014
    );
    grgMaterialShinyness10 = 0.1 * 128;

    grgKa11 = vec3(0.24725, // r
        0.1995,  // g
        0.0745
    );
    grgKd11 = vec3(0.75164, // r
        0.60648, // g
        0.22648
    );
    grgKs11 = vec3(
        0.628281, // r
        0.555802, // g
        0.366065
    );
    grgMaterialShinyness11 = 0.4 * 128;

    grgKa12 = vec3(0.19225, // r
        0.19225, // g
        0.19225
    );
    grgKd12 = vec3(0.50754, // r
        0.50754, // g
        0.50754
    );
    grgKs12 = vec3(0.508273, // r
        0.508273, // g
        0.508273
    );
    grgMaterialShinyness12 = 0.4 * 128;

    grgKa13 = vec3(0.0,  // r
        0.0,  // g
        0.0
    );
    grgKd13 = vec3(0.01, // r
        0.01, // g
        0.01
    );
    grgKs13 = vec3(0.50, // r
        0.50, // g
        0.50
    );
    grgMaterialShinyness13 = 0.25 * 128;

    grgKa14 = vec3(0.0,  // r
        0.1,  // g
        0.06
    );
    grgKd14 = vec3(0.0,        // r
        0.50980392, // g
        0.50980392
    );
    grgKs14 = vec3(0.50196078, // r
        0.50196078, // g
        0.50196078
    );
    grgMaterialShinyness14 = 0.25 * 128;

    grgKa15 = vec3(0.0,  // r
        0.0,  // g
        0.0
    );
    grgKd15 = vec3(0.1,  // r
        0.35, // g
        0.1
    );
    grgKs15 = vec3(0.45, // r
        0.55, // g
        0.45
    );
    grgMaterialShinyness15 = 0.25 * 128;

    grgKa16 = vec3(0.0,  // r
        0.0,  // g
        0.0
    );
    grgKd16 = vec3(0.5,  // r
        0.0,  // g
        0.0
    );
    grgKs16 = vec3(0.7,  // r
        0.6,  // g
        0.6
    );
    grgMaterialShinyness16 = 0.25 * 128;

    grgKa17 = vec3(0.0,  // r
        0.0,  // g
        0.0
    );
    grgKd17 = vec3(0.55, // r
        0.55, // g
        0.55
    );
    grgKs17 = vec3(0.70, // r
        0.70, // g
        0.70
    );
    grgMaterialShinyness17 = 0.25 * 128;

    grgKa18 = vec3(0.0,  // r
        0.0,  // g
        0.0
    );
    grgKd18 = vec3(0.5,  // r
        0.5,  // g
        0.0
    );
    grgKs18 = vec3(0.60, // r
        0.60, // g
        0.50
    );
    grgMaterialShinyness18 = 0.25 * 128;

    grgKa19 = vec3(0.02, // r
        0.02, // g
        0.02
    );
    grgKd19 = vec3(0.01, // r
        0.01, // g
        0.01
    );
    grgKs19 = vec3(0.4,  // r
        0.4,  // g
        0.4
    );
    grgMaterialShinyness19 = 0.078125 * 128;

    grgKa20 = vec3(0.0,  // r
        0.05, // g
        0.05
    );
    grgKd20 = vec3(0.4,  // r
        0.5,  // g
        0.5
    );
    grgKs20 = vec3(0.04, // r
        0.7,  // g
        0.7
    );
    grgMaterialShinyness20 = 0.078125 * 128;

    grgKa21 = vec3(0.0,  // r
        0.05, // g
        0.0
    );
    grgKd21 = vec3(0.4,  // r
        0.5,  // g
        0.4
    );
    grgKs21 = vec3(0.04, // r
        0.7,  // g
        0.04
    );
    grgMaterialShinyness21 = 0.078125 * 128;

    grgKa22 = vec3(0.05, // r
        0.0,  // g
        0.0
    );
    grgKd22 = vec3(0.5,  // r
        0.4,  // g
        0.4
    );
    grgKs22 = vec3(0.7,  // r
        0.04, // g
        0.04
    );
    grgMaterialShinyness22 = 0.078125 * 128;

    grgKa23 = vec3(0.05, // r
        0.05, // g
        0.05
    );
    grgKd23 = vec3(0.5,  // r
        0.5,  // g
        0.5
    );
    grgKs23 = vec3(0.7,  // r
        0.7,  // g
        0.7
    );
    grgMaterialShinyness23 = 0.078125 * 128;

    grgKa24 = vec3(0.05, // r
        0.05, // g
        0.0
    );
    grgKd24 = vec3(0.5,  // r
        0.5,  // g
        0.4
    );
    grgKs24 = vec3(0.7,  // r
        0.7,  // g
        0.04
    );
    grgMaterialShinyness24 = 0.078125 * 128;

}


-(void)uninitialize
{
    if (grgVao1)
    {
        glDeleteVertexArrays(1, &grgVao1);
        grgVao1 = 0;
    }

    if (grgVboSpherePosition1)
    {
        glDeleteBuffers(1, &grgVboSpherePosition1);
        grgVboSpherePosition1 = 0;
    }

    if (grgVboSphereNormal1)
    {
        glDeleteBuffers(1, &grgVboSphereNormal1);
        grgVboSphereNormal1 = 0;
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
    // PV shaders
    glDetachShader(grgShaderProgramObjectPV, grgVertexShaderObjectPV);
    // detach fragment shader
    glDetachShader(grgShaderProgramObjectPV, grgFragmentShaderObjectPV);
    
    // delete vertex object
    glDeleteShader(grgVertexShaderObjectPV);
    grgVertexShaderObjectPV = 0;
    
    
    // delete fragment shader object
    glDeleteShader(grgFragmentShaderObjectPV);
    grgFragmentShaderObjectPV = 0;
    
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
