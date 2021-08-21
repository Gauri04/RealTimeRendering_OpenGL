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

typedef struct Node
{
    mat4 matrix;
    struct Node* ptr;
}Node;

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
    
    grgVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* grvertexShaderSourceCode =
        "#version 300 es" \
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
    
    // initialize variables
     grgDay = 0;
    grgYear = 0;
    top = NULL;
    
    
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
    
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
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

-(void)onSingleTap:(UIGestureRecognizer *)gr
{
    // code
    grgDay = (grgDay - 6) % 360;
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gr
{
    // code
    grgYear = (grgYear + 3) % 360;
}

-(void)onLongPress:(UILongPressGestureRecognizer *)gr
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
