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
    
    GLuint grgVertexShaderObject;
     GLuint grgFragmentShadeerObject;
     GLuint grgShaderProgramObject;
     
    GLuint grgMvpMatrixUniform;
     mat4 grgPerspectiveProjectionMatrix;

     // project specific variables
     GLuint grgVao_cube;
     GLuint grgVboCube;
     GLfloat grgAngleCube;
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
     struct Light
     {
         vec4 lightAmbientColor;
         vec4 lightDiffuseColor;
         vec4 lightSpecularColor;
         vec4 lightPosition;
     };
     vec3 grgKa;    // material ambient
     vec3 grgKd;    // material diffuse
     vec3 grgKs;    // material specular
     GLfloat grgMaterialShinyness;
     struct Light grlight[2];
     int i;
     bool grbAnimate;
     bool grbLight;
     GLuint grkundali_texture;
     GLuint grgtextureSamplerUniform;
   
}

-(id)initWithFrame:(CGRect)frame
{
    // code
    self = [super initWithFrame:frame];
    if(self)
    {
        // register single tap and swipe recognition
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
    grgVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* grvertexShaderSourceCode =
        "#version 300 es" \
        "\n" \
        "in vec4 vPosition;" \
        "in vec3 vNormal;" \
        "in vec2 vTexCoord;" \
        "in vec3 vColor;" \
        "uniform mat4 u_model_matrix;" \
        "uniform mat4 u_view_matrix;" \
        "uniform mat4 u_projection_matrix;" \
        "uniform mediump int u_l_key_pressed;" \
        "uniform vec3 u_la[2];" \
        "uniform vec3 u_ld[2];" \
        "uniform vec3 u_ls[2];" \
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform float u_material_shinyness;" \
        "uniform vec4 u_light_position[2];" \
        "out vec3 phong_ads_light;" \
        "out vec2 out_texcoord;" \
        "out vec3 out_color;" \
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
            "for(i = 0; i < 2; i++)" \
            "{" \
                "light_direction[i] = normalize(vec3(u_light_position[i] - eye_coordinates));" \
                "reflection_vector[i] = reflect(-light_direction[i], transformed_normal);" \
                "ambient[i] = u_la[i] * u_ka;" \
                "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);" \
                "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflection_vector[i], view_vector), 0.0), u_material_shinyness);" \
                "phong_ads_light = ambient[0] + diffuse[0] + specular[0] + ambient[1] + diffuse[1] + specular[1];" \
            "}" \
        "}" \
        "else" \
        "{" \
            "phong_ads_light = vec3(1.0, 1.0, 1.0);" \
        "}" \
        "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
        "out_texcoord = vTexCoord;" \
        "out_color = vColor;" \
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
                [self uninitialize];
                
            }
        }
    }

    ////// Fragment Shader
    grgFragmentShadeerObject = glCreateShader(GL_FRAGMENT_SHADER);

    // source code of shader
    const GLchar* grfragmentShaderSourceCode =
        "#version 300 es" \
        "\n" \
        "precision highp float;" \
        "vec4 color;" \
        "in vec3 phong_ads_light;" \
        "in vec2 out_texcoord;" \
        "in vec3 out_color;" \
        "uniform sampler2D u_texture_sampler;" \
        "uniform int u_l_key_pressed;" \
        "out vec4 FragColor;" \
        "vec3 texture_color;" \
        "void main(void)" \
        "{" \
        "texture_color = vec3(texture(u_texture_sampler, out_texcoord));" \
        "if(u_l_key_pressed == 1)" \
        "{" \
            "color = vec4(vec3(out_color * phong_ads_light * texture_color), 1);" \
        "}" \
        "else" \
        "{" \
            "color = vec4(vec3(out_color * vec3(1.0) * texture_color), 1);"    \
        "}" \
        "FragColor = color;" \
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
                [self uninitialize];
                [self release];
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
    glBindAttribLocation(grgShaderProgramObject, GR_ATTRIBUTE_TEXCOORD, "vTexCoord");
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
                [self uninitialize];
                [self release];
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
    grgtextureSamplerUniform = glGetUniformLocation(grgShaderProgramObject, "u_texture_sampler");
    
    GLfloat data[] =
    {
        //P                    //C                    //N                    //T
        1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

        1.0f, 1.0f, -1.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,

        -1.0f, 1.0f, -1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, -1.0f,    0.0f, 1.0f,
        1.0f, 1.0f, -1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, -1.0f,    0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, -1.0f,    1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,0.0f, 0.0f, 1.0f,    0.0f, 0.0f, -1.0f,    1.0f, 1.0f,

        -1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,    1.0f, 0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,1.0f, 0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

        1.0f, 1.0f, -1.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,    0.0f, 1.0f, 1.0f,    0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,0.0f, 1.0f, 1.0f,    0.0f, -1.0f, 0.0f,    0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, -1.0f, 0.0f,    0.0f, 1.0f,
        1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, -1.0f, 0.0f,    1.0f, 1.0f
    };
    
    glGenVertexArrays(1, &grgVao_cube);
    glBindVertexArray(grgVao_cube);

    glGenBuffers(1, &grgVboCube);
    glBindBuffer(GL_ARRAY_BUFFER, grgVboCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0);
    glVertexAttribPointer(GR_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glVertexAttribPointer(GR_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(9 * sizeof(GLfloat)));
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glEnableVertexAttribArray(GR_ATTRIBUTE_COLOR);
    glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_TEXCOORD);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // initialize variables
    grgKa = vec3(0.0f, 0.0f, 0.0f);
    grgKd = vec3(1.0f, 1.0f, 1.0f);
    grgKs = vec3(1.0f, 1.0f, 1.0f);
    grgMaterialShinyness = 128.0f;
    grgAngleCube = 0.0f;
    grlight[0].lightAmbientColor = vec4(0.0, 0.0, 0.0, 1.0);
    grlight[0].lightDiffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
    grlight[0].lightSpecularColor = vec4(1.0, 1.0, 1.0, 1.0);
    grlight[1].lightAmbientColor = vec4(0.0, 0.0, 0.0, 1.0);
    grlight[1].lightDiffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
    grlight[1].lightSpecularColor = vec4(1.0, 1.0, 1.0, 1.0);
    grlight[0].lightPosition = vec4(100.0, 100.0, 100.0, 1.0);
    grlight[1].lightPosition = vec4(-100.0, -100.0, -100.0, 1.0);
    grkundali_texture = [self loadTextureFromBMP:@"Kundali" :@".bmp"];
    if(grkundali_texture == 0)
    {
        printf("\n Error in loadgltexture");
        [self uninitialize];
        [self release];
    }

    
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

-(GLuint)loadTextureFromBMP:(NSString *)imageFileName :(NSString *)extension
{
    NSString *imageFileNameWithPath = [[NSBundle mainBundle]pathForResource:imageFileName ofType:extension];
    
    // get NSImage representative of our img file and error check
    UIImage *bmpImage = [[UIImage alloc]initWithContentsOfFile:imageFileNameWithPath];
    // error check
    if(!bmpImage)
    {
        printf("\n UIImage conversion failed");
        return(0);
    }
     
    // get CGImage representation of NSImage
    CGImageRef cgImage = [bmpImage CGImage];
    // get width and height of cgimage
    int imageWidth = (int)CGImageGetWidth(cgImage);
    int imageHeight = (int)CGImageGetHeight(cgImage);
    // get corefoundation data represented by image pixels
    CFDataRef imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
     
    // convert cfdataref format data to void* generic format
    void *pixels = (void*)CFDataGetBytePtr(imageData);
    
    GLuint texture;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
     
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // setting of texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        // MAG - Magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);                // MIN - Minification
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    CFRelease(imageData);
    return texture;
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
    glUseProgram(grgShaderProgramObject);

    // OpenGL drawing code will start here
    // start use of shader program
    glUseProgram(grgShaderProgramObject);

    mat4 grviewMatrix = mat4::identity();
    mat4 grmodelMatrix = mat4::identity();
    mat4 grprojectionMatrix = mat4::identity();
    mat4 grrotateMatrix = mat4::identity();
    mat4 grtranslateMatrix = mat4::identity();
    mat4 grscaleMatrix = mat4::identity();

    grtranslateMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
    grscaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
    grrotateMatrix = vmath::rotate(grgAngleCube, 1.0f, 0.0f, 0.0f);
    grrotateMatrix = grrotateMatrix * vmath::rotate(grgAngleCube, 0.0f, 1.0f, 0.0f);
    grrotateMatrix = grrotateMatrix * vmath::rotate(grgAngleCube, 0.0f, 0.0f, 1.0f);
        
    grmodelMatrix = grtranslateMatrix * grscaleMatrix * grrotateMatrix;
    grprojectionMatrix = grgPerspectiveProjectionMatrix;
    
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

    glUniformMatrix4fv(grgProjectionMatrixUniform, 1, GL_FALSE, grprojectionMatrix);
    glUniformMatrix4fv(grgViewMatrixUniform, 1, GL_FALSE, grviewMatrix);
    glUniformMatrix4fv(grgModelMarixUniform, 1, GL_FALSE, grmodelMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grkundali_texture);
    glUniform1i(grgtextureSamplerUniform, 0);

    // bind vao of square
    glBindVertexArray(grgVao_cube);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
    // unbind vao
    glBindVertexArray(0);
    
    // stop use of shader program
    glUseProgram(0);
    
    [self updateFn];
    
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

-(void) updateFn
{
    // code
    grgAngleCube = grgAngleCube + 0.1f;
       if (grgAngleCube >= 360.0f)
           grgAngleCube = 0.0f;
}

-(void)onSingleTap:(UIGestureRecognizer *)gr
{
    // code
    grbLight = !grbLight;
}

-(void)onDoubleTap:(UIGestureRecognizer *)gr
{
    grbAnimate = !grbAnimate;
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
    if (grgVboCube)
    {
        glDeleteBuffers(1, &grgVboCube);
        grgVboCube = 0;
    }

    // delete square vao and vbo
    if (grgVao_cube)
    {
        glDeleteVertexArrays(1, &grgVao_cube);
        grgVao_cube = 0;
    }
    if (grkundali_texture)
    {
        glDeleteTextures(1, &grkundali_texture);
        grkundali_texture = 0;
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
