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
    GLuint grgVao;
    GLuint grgVbo_position;
    GLuint grgMvpMatrixUniform;
    mat4 grgPerspectiveMatrix;
    GLfloat grHorizontalLineVertices1[80];
    GLfloat grHorizontalLineVertices2[80];
    GLfloat grHorizontalCenterLineVertices[4];
   
}

-(id)initWithFrame:(CGRect)frame
{
    // code
    self = [super initWithFrame:frame];
    if(self)
    {
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
        "in vec4 vColor;" \
        "out vec4 out_color;" \
        "uniform mat4 u_mvpMatrix;" \
        "void main(void)" \
        "{" \
        "out_color = vColor;" \
        "gl_Position = u_mvpMatrix * vPosition;" \
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
        "in vec4 out_color;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "FragColor = out_color;" \
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

    grgMvpMatrixUniform = glGetUniformLocation(grgShaderProgramObject, "u_mvpMatrix");
   
    GLfloat i;
    GLint index = 0;

    for (i = 0.060f; i <= 1.5f; i = i + 0.060f)
    {
        if (index < 80)
        {
            grHorizontalLineVertices1[index] = -3;
            grHorizontalLineVertices1[index + 1] = -i;
            grHorizontalLineVertices1[index + 2] = 3;
            grHorizontalLineVertices1[index + 3] = -i;
            index = index + 4;
        }
        
    }

    index = 0;
    for (i = 0.060f; i <= 1.5f; i = i + 0.060f)
    {
        if (index < 80)
        {
            grHorizontalLineVertices2[index] = -3;
            grHorizontalLineVertices2[index + 1] = i;
            grHorizontalLineVertices2[index + 2] = 3;
            grHorizontalLineVertices2[index + 3] = i;
            index = index + 4;
        }

    }
    
    glGenVertexArrays(1, &grgVao);
    glBindVertexArray(grgVao);

    glGenBuffers(1, &grgVbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
    glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 0.0f, 0.0f);
    glBindVertexArray(0);
    
    // initialize vars
    grHorizontalCenterLineVertices[0] = -3.0f;
    grHorizontalCenterLineVertices[1] = 0.0f;
    grHorizontalCenterLineVertices[2] = 3.0f;
    grHorizontalCenterLineVertices[3] = 0.0f;
    
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
    // OGL drawing code
    glUseProgram(grgShaderProgramObject);

   // OGL drawing  code will start here
    mat4 grmodelViewMatrix = mat4::identity();
    mat4 grmodelViewProjectionMatrix = mat4::identity();
    mat4 grtranslateMatrix = mat4::identity();
       
    glUseProgram(grgShaderProgramObject);
    //*************** Horizontal Line - (bottom) **************************
    grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
    grmodelViewMatrix = grtranslateMatrix;
    grmodelViewProjectionMatrix = grgPerspectiveMatrix * grmodelViewMatrix;

    glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

    // bind vao
    glBindVertexArray(grgVao);
    glGenBuffers(1, &grgVbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
    glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), grHorizontalLineVertices1, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
    glDrawArrays(GL_LINES, 0, 40);
    glBindVertexArray(0);

    //****************** Horizontal Line - (center) **********************
       
    grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
    grmodelViewMatrix = grtranslateMatrix;
    grmodelViewProjectionMatrix = grgPerspectiveMatrix * grmodelViewMatrix;

    glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

    // bind vao
    glBindVertexArray(grgVao);
    glGenBuffers(1, &grgVbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
    glBufferData(GL_ARRAY_BUFFER, 2 * 2 * sizeof(GLfloat), grHorizontalCenterLineVertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    //******************** Horizontal Line - (top) **************************
    grtranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
    grmodelViewMatrix = grtranslateMatrix;
    grmodelViewProjectionMatrix = grgPerspectiveMatrix * grmodelViewMatrix;

    glUniformMatrix4fv(grgMvpMatrixUniform, 1, GL_FALSE, grmodelViewProjectionMatrix);

    // bind vao
    glBindVertexArray(grgVao);
    glBindBuffer(GL_ARRAY_BUFFER, grgVbo_position);
    glBufferData(GL_ARRAY_BUFFER, 40 * 2 * sizeof(GLfloat), grHorizontalLineVertices2, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttrib3f(GR_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
    glDrawArrays(GL_LINES, 0, 40);
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
    if (grgVbo_position)
    {
        glDeleteBuffers(1, &grgVbo_position);
        grgVbo_position = 0;
    }
    
    if (grgVao)
    {
        glDeleteVertexArrays(1, &grgVao);
        grgVao = 0;
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
