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

#define CHECK_IMAGE_WIDTH 64
#define CHECK_IMAGE_HEIGHT 64

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
                printf("\n Shader Program Link Log : %s", grszInfoLog);
                [self uninitialize];
                [self release];
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
    
    // initialize variables
    grcheckerboard_texture = [self loadGLTexture];
    
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    grgPerspectiveMatrix = mat4::identity();
    
}

+(Class)layerClass
{
    return([CAEAGLLayer class]);
}

-(GLuint)loadGLTexture
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
    grgPerspectiveMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    [self drawView:nil];
}

-(void)drawView:(id)sender
{

    [EAGLContext setCurrentContext:eaglContext];
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    glUseProgram(grgShaderProgramObject);
    // OGL drawing code
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
    
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

-(void) updateFn
{
    // code
    
}

-(void)onSingleTap:(UIGestureRecognizer *)gr
{
    // code
   
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
