var canvas = null;
var gl = null;
var canvas_original_width;
var canvas_original_height;
var bFullscreen = false;
var requestAnimationFrame = window.requestAnimationFrame || 
                            window.webkitRequestAnimationFrame ||
                            window.mozRequestAnimationFrame ||
                            window.oRequestAnimationFrame ||
                            window.msRequestAnimationFrame;
const WebGLMacros = 
{
    GR_ATTRIBUTE_POSITION:0,
    GR_ATTRIBUTE_COLOR:1,
    GR_ATTRIBUTE_TEXTURE:2,
    GR_ATTRIBUTE_NORMAL:3
};

var grvertexShaderObject;
var grfragmentShaderObject;
var grfragmentShaderObject2;
var grshaderProgramObject;
var grshaderProgramObject2;

var grvaoCube;
var grvboCubePosition;
var grvboCubeColor;
var grvboCubeTexCoords;

var grvaoCube1;
var grvboCubePosition1;
var grvboCubeColor1;
var grvaoCube2;
var grvboCurePosition2;
var grvboCubeColor2;
var grmvpUniform;
var grmvpUniform2;
var grperspectiveMatrix;
var SCREEN_WIDTH = 1024;
var SCREEN_HEIGHT = 1024;
var grfbo;
var grcolorTexture;
var grdepthTexture;
var grtexture;
var grcolorGreen = new Float32Array([0.0, 1.0, 0.0]);
var grcolorBlue = new Float32Array([0.0, 0.0, 1.0]);
var grdepthOne = new Float32Array([1]);
var grangle = 0.0;
var grwidth, grheight;

var cancelAnimationFrame = window.cancelAnimationFrame || 
                        window.webkitCancelRequestAnimationFrame || window.webkitCancelAnimationFrame ||
                        window.mozCancelRequestAnimationFrame || window.mozCancelAnimationFrame ||
                        window.onCancelRequestAnimationFrame || window.onCancelAnimationFrame ||
                        window.msCancelRequestAnimationFrame || window.msCancelAnimationFrame;

function main()
{
   
    // get  canvas from DOM
    canvas = document.getElementById("GR");
    if(!canvas)
        console.log("\n ontaining canvas failed");
    else
        console.log("\n ontaining canvas succeeded");

    canvas_original_width = canvas.width;
    canvas_original_height = canvas.height;

    // "keydown" and "click" are in-built events. keyDown and mouseDown are user defined functions
    // param false states that events are bubble events, not captured events 
    window.addEventListener("keydown", keyDown, false);         // window is in-build variable, just like document. window is inherited from document, its a DOM object.
    window.addEventListener("click", mouseDown, false);
    window.addEventListener("resize", resize, false);

    init();

    resize();                                   
    // till now in window, linux and android we did not have to call display() / draw() as it internally calls repaint. But here we have to call it as it doesnt do it 
    // internally
    // warm up call
    draw();
}




function toggleFullscreen()
{
    var fullscreen_element = document.fullscreenElement || 
                            document.webkitFullscreenElement ||         // apple - safari fullscreen
                            document.mozFullScreenElement ||            // fullscreen for mozilla
                            document.msFullscreenElement ||             // fullscreen for internet explorer / edge
                            null;                                       // fullscreen for browsers other than the above three

    if(fullscreen_element == null)
    {
         // set fullscreen
        if(canvas.requestFullscreen)                                        // check whether function pointer is not null
        {
            canvas.requestFullscreen();
        }
        else if(canvas.webkitRequestFullscreen)
        {
            canvas.webkitRequestFullscreen();
        }
        else if(canvas.mozRequestFullScreen)
        {
            canvas.mozRequestFullScreen();
        }
        else if(canvas.msRequestFullscreen)
        {
          canvas.msRequestFullscreen();
        }
        bFullscreen = true;   
    }
    else
    {
        if(document.exitFullscreen)
        {
            document.exitFullscreen();
        }
        else if(document.webkitExitFullscreen)
        {
            document.webkitExitFullscreen();
        }
        else if(document.mozCancelFullScreen)
        {
            document.mozCancelFullScreen();
        }
        else if(document.msExitFullscreen)
        {
            document.msExitFullscreen();
        }
        bFullscreen = false;
    }
}


// because of type inference, we dont need to write data type of any variable
function keyDown(event)
{
   switch(event.keyCode)
    {
        case 70 :
            toggleFullscreen();
            // as there is no repaint event in javascript
           
            break;

        case 27 :
            uninitialize();
            window.close();
            break;
    }
}

function mouseDown()
{
    // code
   
}

function init()
{
    // get drawing context from canvas
    // to maintain consistency, we will get context in init() instead of in main()
    gl = canvas.getContext("webgl2");

    if(!gl)
        console.log("webgl2 context failed");
    else
        console.log("webgl2 context succeeded");

    gl.viewportWidth = canvas.width;
    gl.viewportHeight = canvas.height;

    // vertex shader
    var grvertexShaderSourceCode = 
    "#version 300 es" +
    "\n" +
    "in vec4 vPosition;" +
    "in vec4 vColor;" +
    "in vec2 vTexCoord;" +
    "uniform mat4 u_mvpMatrix;" +
    "out vec4 out_color;" +
    "out vec2 out_texcoord;" +
    "void main(void)" +
    "{" +
    "gl_Position = u_mvpMatrix * vPosition;" +
    "out_color = vColor;" +
    "out_texcoord = vTexCoord;" +
    "}";

    grvertexShaderObject = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(grvertexShaderObject, grvertexShaderSourceCode);
    gl.compileShader(grvertexShaderObject);
    if(gl.getShaderParameter(grvertexShaderObject, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(grvertexShaderObject);
        if(error.length > 0)
        {
            alert("Vertex Shader : " + error);
            uninitialize();
        }

    }

    var grfragmentShaderSourceCode = 
    "#version 300 es" +
    "\n" +
    "precision highp float;" +
    "in vec4 out_color;" +
    "in vec2 out_texcoord;" +
    "out vec4 FragColor;" +
    "void main(void)" +
    "{" +
    "FragColor = out_color;" +
    "}";

    grfragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(grfragmentShaderObject, grfragmentShaderSourceCode);
    gl.compileShader(grfragmentShaderObject);
    if(gl.getShaderParameter(grfragmentShaderObject, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(grfragmentShaderObject);
        if(error.length > 0)
        {
            alert("Fragment Shader : " + error);
            uninitialize(); 
        }
        alert("in compile fragment shader error");
        
    }

    // shader program
    grshaderProgramObject = gl.createProgram();
    //attach shader object
    gl.attachShader(grshaderProgramObject, grvertexShaderObject);
    gl.attachShader(grshaderProgramObject, grfragmentShaderObject);
    // pre-linking
    gl.bindAttribLocation(grshaderProgramObject, WebGLMacros.GR_ATTRIBUTE_POSITION, "vPosition");
    gl.bindAttribLocation(grshaderProgramObject, WebGLMacros.GR_ATTRIBUTE_COLOR, "vColor");
    // linking
    gl.linkProgram(grshaderProgramObject);
    if(!gl.getProgramParameter(grshaderProgramObject, gl.LINK_STATUS))
    {
        var err = gl.getProgramInfoLog(grshaderProgramObject);
        if(err.length > 0)
        {
            alert("Shader Program : " + err);
            uninitialize(); 
        }
       // uninitialize(); 
    }

    grmvpUniform = gl.getUniformLocation(grshaderProgramObject, "u_mvp_matrix");


    // *********************** Shader Program 2
    /*
    var grfragmentShaderSourceCode2 = 
    "#version 300 es" +
    "\n" +
    "precision highp float;" +
    "in vec2 out_texcoord;" +
    "in vec4 out_color;" +
    "uniform sampler2D u_texture_sampler;" +
    "out vec4 FragColor;" +
    "void main(void)" +
    "{" +
    "FragColor = vec4(1.0, 0.0, 0.0, 1.0);" +
    "}";

    grfragmentShaderObject2 = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(grfragmentShaderObject2, grfragmentShaderSourceCode2);
    gl.compileShader(grfragmentShaderObject2);
    if(gl.getShaderParameter(grfragmentShaderObject2, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(grfragmentShaderObject2);
        if(error.length > 0)
        {
            alert("Fragment Shader 2 : " + error);
            uninitialize(); 
        }
        alert("in compile fragment shader 2 error");
        
    }

    // shader program
    grshaderProgramObject2 = gl.createProgram();
    //attach shader object
    gl.attachShader(grshaderProgramObject2, grvertexShaderObject);
    gl.attachShader(grshaderProgramObject2, grfragmentShaderObject2);
    // pre-linking
    gl.bindAttribLocation(grshaderProgramObject2, WebGLMacros.GR_ATTRIBUTE_POSITION, "vPosition");
    gl.bindAttribLocation(grshaderProgramObject2, WebGLMacros.GR_ATTRIBUTE_TEXTURE, "vTexCoord");
    // linking
    gl.linkProgram(grshaderProgramObject2);
    if(!gl.getProgramParameter(grshaderProgramObject2, gl.LINK_STATUS))
    {
        var err = gl.getProgramInfoLog(grshaderProgramObject2);
        if(err.length > 0)
        {
            alert("Shader Program 2 : " + err);
            uninitialize(); 
        }
       // uninitialize(); 
    }


    // mvp uniform binding
    grmvpUniform2 = gl.getUniformLocation(grshaderProgramObject2, "u_mvp_matrix");
    */

    var grcubeColors = new Float32Array(
        [
            1.0, 0.0, 0.0,					// one color or single surface 
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
    
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
    
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
    
            0.0, 1.0, 1.0,
            0.0, 1.0, 1.0,
            0.0, 1.0, 1.0,
            0.0, 1.0, 1.0,
    
            1.0, 0.0, 1.0,
            1.0, 0.0, 1.0,
            1.0, 0.0, 1.0,
            1.0, 0.0, 1.0,
    
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0
        ]
    );

    var grcubeTexCoords = new Float32Array(
        [
            0.0, 0.0,					
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,
    
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
    
            0.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
    
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,
    
            0.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
    
            1.0, 0.0,
            0.0, 0.0,
            0.0, 1.0,
            1.0, 1.0
        ]
    );

    var grcubeVertices = new Float32Array(
        [
            1.0, 1.0, 1.0,
            -1.0, 1.0, 1.0,
            -1.0, -1.0, 1.0,
            1.0, -1.0, 1.0,
                                                // right face
            1.0, 1.0, -1.0,
            1.0, 1.0, 1.0,
            1.0, -1.0, 1.0,
            1.0, -1.0, -1.0,
                                                // back face
            -1.0, 1.0, -1.0,
            1.0, 1.0, -1.0,
            1.0, -1.0, -1.0,
            -1.0, -1.0, -1.0,
                                                // left face
            -1.0, 1.0, 1.0,
            -1.0, 1.0, -1.0,
            -1.0, -1.0, -1.0,
            -1.0, -1.0, 1.0,
                                                // top face
            1.0, 1.0, -1.0,
            -1.0, 1.0, -1.0,
            -1.0, 1.0, 1.0,
            1.0, 1.0, 1.0,
                                                // bottom face
            1.0, -1.0, -1.0,
            -1.0, -1.0, -1.0,
            -1.0, -1.0, 1.0,
            1.0, -1.0, 1.0
        ]
    );

    

    // cube 
    grvaoCube = gl.createVertexArray();
    gl.bindVertexArray(grvaoCube);

    grvboCubePosition = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, grvboCubePosition);
    gl.bufferData(gl.ARRAY_BUFFER, grcubeVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.GR_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.GR_ATTRIBUTE_POSITION);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    grvboCubeColor = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, grvboCubeColor);
    gl.bufferData(gl.ARRAY_BUFFER, grcubeColors, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.GR_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.GR_ATTRIBUTE_COLOR);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    grvboCubeTexCoords = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, grvboCubeTexCoords);
    gl.bufferData(gl.ARRAY_BUFFER, grcubeTexCoords, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.GR_ATTRIBUTE_TEXTURE, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.GR_ATTRIBUTE_TEXTURE);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    
    gl.bindVertexArray(null);

    // *************************************************** FBO **************************************************
    /*
    grfbo = gl.createFramebuffer();
   

    grcolorTexture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, grcolorTexture);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, gl.RGBA, gl.UNSIGNED_BYTE, null);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
   

    grdepthTexture = gl.createRenderbuffer();
    gl.bindRenderbuffer(gl.RENDERBUFFER, grdepthTexture);
    gl.renderbufferStorage(gl.RENDERBUFFER, gl.DEPTH_COMPONENT16, SCREEN_WIDTH, SCREEN_HEIGHT);

    gl.bindFramebuffer(gl.FRAMEBUFFER, grfbo);
    gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, grcolorTexture, null);
    gl.framebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTAHCMENT, gl.RENDERBUFFER, grdepthTexture);

    var error = gl.checkFramebufferStatus(gl.FRAMEBUFFER);
    if(error != gl.FRAMEBUFFER_COMPLETE)
    {
        console.log("Error in framebuffer : "+ error);
    }
    else
        console.log("Framebuffer successful");

    gl.bindFramebuffer(gl.FRAMEBUFFER, null);   
    */
    grperspectiveMatrix = mat4.create();
    // blue color
    gl.clearColor(0.0, 1.0, 1.0, 1.0);
     
}

function resize()
{
    if(bFullscreen == true)
    {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    else
    {
        canvas.width = canvas_original_width;
        canvas.height = canvas_original_height;
    }

    gl.viewport(0, 0, canvas.width, canvas.height);
    grwidth = canvas.width;
    grheight = canvas.height;
    mat4.perspective(grperspectiveMatrix, 45.0, parseFloat(canvas.width) / parseFloat(canvas.height), 0.1, 100.0);

}

function draw()
{
    // variables
    var grmodelViewMatrix;
    var grmodelViewProjectionMatrix;
    var grtranslateMatrix;
    var grrotateMatrix;

   // gl.clear(gl.COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT);

    
    gl.bindFramebuffer(gl.FRAMEBUFFER, null);
    gl.viewport(0, 0, 1024, 1024);
    gl.clearBufferfv(gl.COLOR, 0, new Float32Array([0.0, 0.0, 1.0, 1.0]));
    gl.clearBufferfv(gl.DEPTH, 0, new Float32Array([1.0]));


    gl.useProgram(grshaderProgramObject);
    grmodelViewMatrix = mat4.create();
    grmodelViewProjectionMatrix = mat4.create();
    grtranslateMatrix = mat4.create();
    grrotateMatrix = mat4.create();

    mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -6.0]);
   // mat4.rotateX(grrotateMatrix, grrotateMatrix, deg2rad(grangle));
   // mat4.rotateY(grrotateMatrix, grrotateMatrix, deg2rad(grangle));
   // mat4.rotateZ(grrotateMatrix, grrotateMatrix, deg2rad(grangle));
    mat4.multiply(grmodelViewMatrix, grtranslateMatrix, grrotateMatrix);
    mat4.multiply(grmodelViewProjectionMatrix, grperspectiveMatrix, grmodelViewMatrix);

    gl.uniformMatrix4fv(grmvpUniform, false, grmodelViewProjectionMatrix);

    gl.bindVertexArray(grvaoCube);
    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);
    gl.bindVertexArray(null);
    gl.useProgram(null);
  /*
    // ***** default framebuffer
    gl.bindFramebuffer(gl.FRAMEBUFFER, null);
    gl.viewport(0, 0, grwidth, grheight);
    gl.clearBufferfv(gl.COLOR, 0, grcolorBlue);
    gl.clearBufferfv(gl.DEPTH, 0, grdepthOne);


    gl.useProgram(grshaderProgramObject2);
    grmodelViewMatrix = mat4.create();
    grmodelViewProjectionMatrix = mat4.create();
    grtranslateMatrix = mat4.create();
    grrotateMatrix = mat4.create();

    mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -6.0]);
   // mat4.rotateX(grrotateMatrix, grrotateMatrix, deg2rad(grangle));
   // mat4.rotateY(grrotateMatrix, grrotateMatrix, deg2rad(grangle));
   // mat4.rotateZ(grrotateMatrix, grrotateMatrix, deg2rad(grangle));
    mat4.multiply(grmodelViewMatrix, grtranslateMatrix, grtranslateMatrix);
    mat4.multiply(grmodelViewProjectionMatrix, grperspectiveMatrix, grmodelViewMatrix);

    gl.uniformMatrix4fv(grmvpUniform2, false, grmodelViewProjectionMatrix);

    gl.bindVertexArray(grvaoCube);
    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);
    gl.bindVertexArray(null);
 */
    update();
    requestAnimationFrame(draw, canvas);
}


function update()
{
    if(grangle >= 360.0)
        grangle = 0.0;

    grangle = grangle + 0.1;
    
}

function deg2rad(degrees)
{
    var rad = degrees * Math.PI / 180.0;
    return rad;
}

function uninitialize()
{
    if(grvaoCube)
    {
        gl.deleteVertexArray(grvaoCube);
        grvaoCube = null;
    }
    if(grvboCubePosition)
    {
        gl.deleteBuffer(grvboCubePosition);
        grvboCubePosition = null;
    }
    if(grvboCubeColor)
    {
        gl.deleteBuffer(grvboCubeColor);
        grvboCubeColor = null;
    }

    if(grvaoCube1)
    {
        gl.deleteVertexArray(grvaoCube1);
        grvaoCube1 = null;
    }
    if(grvboCubePosition1)
    {
        gl.deleteBuffer(grvboCubePosition1);
        grvboCubePosition1 = null;
    }


    if(grshaderProgramObject)
    {
        if(grfragmentShaderObject)
        {
            gl.detachShader(grshaderProgramObject, grfragmentShaderObject);
            gl.deleteShader(grfragmentShaderObject);
            grfragmentShaderObject = null;
        }

        if(grfragmentShaderObject)
        {
            gl.detachShader(grshaderProgramObject, grvertexShaderObject);
            gl.deleteShader(grvertexShaderObject);
            grvertexShaderObject = null;
        }

        gl.deleteProgram(grshaderProgramObject);
        grshaderProgramObject = null;
    }

}




