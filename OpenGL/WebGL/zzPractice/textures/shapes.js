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
var grshaderProgramObject;

var grvaoPyramid;
var grvboPyramidPosition;
var grvboPyramidColors;

var grvaoCube;
var grvboCubePosition;
var grvboCubeColor;

var grperspectiveMatrix;
var grangle = 0.0;
var gviewMatrix;
var gmodelMatrix;
var gprojectionMatrix;
var transX = -2.0;
var zoomFactor = 1.0;
var isZoom = false;
var clipX, clipY;

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
    window.addEventListener("wheel", onWheel, false)
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
    console.log(event.keyCode);
   switch(event.keyCode)
   {
       case 70 :
            toggleFullscreen();
            // as there is no repaint event in javascript
           
            break;

        case 65 :
            transX = transX + 0.1;
            break;
   }
}

function mouseDown()
{
    // code
   
}

function onWheel(event)
{
    isZoom = true;
    console.log("wheel - "+ (event.deltaY));
    zoomFactor = zoomFactor + 0.1;
    console.log("\n zoomFactor = " + zoomFactor);
    console.log("\n X before= " + event.clientX);
    event.clientX+= event.deltaY;
    event.clientY+= event.deltaY;
    console.log("\n X after= " + event.clientX);


    const mainImage = canvas.getBoundingClientRect();
    const cssX = event.clientX - mainImage.left;
    const cssY = event.clientY - mainImage.top;

    console.log("x position : ", cssX);
    console.log("y position : ", cssY);

    getClipSpaceMousePosition(event);

}

function getClipSpaceMousePosition(e) {
    // get canvas relative css position
    const rect = canvas.getBoundingClientRect();
    const cssX = e.clientX - rect.left;
    const cssY = e.clientY - rect.top;
    
    // get normalized 0 to 1 position across and down canvas
    const normalizedX = cssX / canvas.width;
    const normalizedY = cssY / canvas.height;
  
    // convert to clip space
    this.clipX = normalizedX *  2 - 1;
    this.clipY = normalizedY * -2 + 1;

    
    console.log("x pos : ", clipX);
    console.log("y pos : ", clipY);
    
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
    "out vec4 out_color;" +
    "uniform mat4 u_model_matrix;" +
    "uniform mat4 u_view_matrix;" +
    "uniform mat4 u_projection_matrix;" +
    "void main(void)" +
    "{" +
    "out_color = vColor;" +
    "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" +
    "}";

    grvertexShaderObject = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(grvertexShaderObject, grvertexShaderSourceCode);
    gl.compileShader(grvertexShaderObject);
    if(gl.getShaderParameter(grvertexShaderObject, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(grvertexShaderObject);
        if(error.length > 0)
        {
            alert(error);
            uninitialize();
        }
        alert("in compile vertex shader error");

    }

    var grfragmentShaderSourceCode = 
    "#version 300 es" +
    "\n" +
    "precision highp float;" +
    "in vec4 out_color;" +
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
            alert(error);
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
            alert(err);
            
        }
        
        alert("in shader program object error");
        alert(err);
       // uninitialize(); 
    }

    // mvp uniform binding
    grmodelUniform = gl.getUniformLocation(grshaderProgramObject, "u_model_matrix");
    grviewUniform = gl.getUniformLocation(grshaderProgramObject, "u_view_matrix");
    grprojectionUniform = gl.getUniformLocation(grshaderProgramObject, "u_projection_matrix");


    var grpyramidVertices = new Float32Array(
        [
            0.0, 1.0, 0.0,
            -1.0, -1.0, 1.0,
            1.0, -1.0, 1.0,
                                    //right face
            0.0, 1.0, 0.0,
            1.0, -1.0, -1.0,
            1.0, -1.0, 1.0,
                                    // back face
            0.0, 1.0, 0.0,
            -1.0, -1.0, -1.0,
            1.0, -1.0, -1.0,
                                    // left face
            0.0, 1.0, 0.0,
            -1.0, -1.0, -1.0,
            -1.0, -1.0, 1.0
        ]
    );
    var grpyramidColors = new Float32Array(
        [
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
    
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
    
            0.0, 0.0, 0.0,
            0.0, 0.0, 1.0,
            0.0, 1.0, 0.0,
    
            1.0, 0.0, 0.0,
            0.0, 0.0, 1.0,
            0.0, 1.0, 0.0
        ]
    );

    var grcubeColors = new Float32Array(
        [
            1.0, 0.0, 0.0,
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

    // triangle
    grvaoPyramid = gl.createVertexArray();
    gl.bindVertexArray(grvaoPyramid);

    grvboPyramidPosition = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, grvboPyramidPosition);
    gl.bufferData(gl.ARRAY_BUFFER, grpyramidVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.GR_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.GR_ATTRIBUTE_POSITION);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    grvboPyramidColors = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, grvboPyramidColors);
    gl.bufferData(gl.ARRAY_BUFFER, grpyramidColors, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.GR_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.GR_ATTRIBUTE_COLOR);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    
    gl.bindVertexArray(null);


    // square
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
    
    gl.bindVertexArray(null);


    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);
    grperspectiveMatrix = mat4.create();
    // blue color
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
     
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

    mat4.perspective(grperspectiveMatrix, 45.0, parseFloat(canvas.width) / parseFloat(canvas.height), 0.1, 100.0);

}

function draw()
{
    // variables
    var grtranslateMatrix;
    var grmodelMatrix = mat4.create();
    var grviewMatrix = mat4.create();
    var grprojectionMatrix = mat4.create();
    var grrotateMatrix = mat4.create();

    gl.clear(gl.COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT);

    gl.useProgram(grshaderProgramObject);
    /*
    grmodelViewMatrix = mat4.create();
    grmodelViewProjectionMatrix = mat4.create();
    grtranslateMatrix = mat4.create();

    mat4.translate(grtranslateMatrix, grtranslateMatrix, [transX, 0.0, -6.0]);
    mat4.rotateZ(grrotateMatrix, grrotateMatrix, deg2rad(grangle));
    mat4.multiply(grmodelMatrix, grtranslateMatrix, grrotateMatrix);
    mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

    gl.uniformMatrix4fv(grmodelUniform, false, grmodelMatrix);
    gl.uniformMatrix4fv(grviewUniform, false, grviewMatrix);
    gl.uniformMatrix4fv(grprojectionUniform, false, grprojectionMatrix);


    gl.bindVertexArray(grvaoPyramid);
    gl.drawArrays(gl.TRIANGLES, 0, 12);
    gl.bindVertexArray(null);
    */
    //*** Square ******
    grtranslateMatrix = mat4.create();
    grscaleMatrix = mat4.create();
    grmodelMatrix = mat4.create();
    grrotateMatrix - mat4.create();
    grprojectionMatrix = mat4.create();

    mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -6.0]);
    //mat4.rotateX(grrotateMatrix, grrotateMatrix, deg2rad(grangle));
    //mat4.rotateY(grrotateMatrix, grrotateMatrix, deg2rad(grangle));
    //mat4.rotateZ(grrotateMatrix, grrotateMatrix, deg2rad(grangle));
    //mat4.multiply(grmodelMatrix, grtranslateMatrix, grrotateMatrix);

    mat4.scale(grscaleMatrix, grscaleMatrix, [zoomFactor, zoomFactor, zoomFactor]);
    mat4.multiply(grmodelMatrix, grtranslateMatrix, grscaleMatrix);
    //mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
    mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

    if(isZoom == true)
    {
        /*
        // Get world coordinates of mouse (currently we have clipCoords)
        var viewProjInverse = mat4.create();
        var viewProjMatrix = mat4.create();
        
        mat4.multiply(viewProjMatrix, grprojectionMatrix, grviewMatrix);
        viewProjInverse = invert(viewProjMatrix);
        var point3D = [clipX, clipY, 0];
        mat4.translate(viewProjInverse, viewProjInverse, point3D);
        
        //var mouseCoords = vec3.create();
        //console.log("m X = ", mouseCoords[0], "m Y = ", mouseCoords[1]);
        //mat4.getTranslation(mouseCoords, viewProjInverse);
        //console.log("trans m X = ", mouseCoords[0], "m Y = ", mouseCoords[1]);
        

        const start = transformPoint(viewProjInverse, [clipX, clipY, -1]);
        const end   = transformPoint(viewProjInverse, [clipX, clipY,  1]);
        
        console.log("******************trans m X = ", start, "m Y = ", end);
        */
        // scale at fixed point
        //1 - move model to the point that we want to scale from, e.g - we want to scale it from (-2, 0)
        var mouseCoords = [-2.0, 0.0];
        mat4.translate(grmodelMatrix, grmodelMatrix, [-2.0, 0.0, 0.0]);
        // scale the matrix so that it will appear as if we are scaling it from origin
        mat4.scale(grmodelMatrix, grmodelMatrix, [zoomFactor, zoomFactor, zoomFactor]);
       // mat4.multiply(grmodelMatrix, grtranslateMatrix, grscaleMatrix);
        // put the model back to where it was
        mat4.translate(grmodelMatrix, grmodelMatrix, [2.0, 0.0, 0.0]);
        //mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
    }


    gl.uniformMatrix4fv(grmodelUniform, false, grmodelMatrix);
    gl.uniformMatrix4fv(grviewUniform, false, grviewMatrix);
    gl.uniformMatrix4fv(grprojectionUniform, false, grprojectionMatrix);

    gl.bindVertexArray(grvaoCube);
    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);
    gl.bindVertexArray(null);

    gl.useProgram(null);
    update();
    requestAnimationFrame(draw, canvas);
}


//function transformPoint(e,r,t){t=t||o.create();var n=r[0],u=r[1],i=r[2],a=n*e[3]+u*e[7]+i*e[11]+e[15];return t[0]=(n*e[0]+u*e[4]+i*e[8]+e[12])/a,t[1]=(n*e[1]+u*e[5]+i*e[9]+e[13])/a,t[2]=(n*e[2]+u*e[6]+i*e[10]+e[14])/a,t}

  /**
   * Takes a 4-by-4 matrix and a vector with 3 entries,
   * interprets the vector as a point, transforms that point by the matrix, and
   * returns the result as a vector with 3 entries.
   * @param {Matrix4} m The matrix.
   * @param {Vector3} v The point.
   * @param {Vector4} dst optional vector4 to store result
   * @return {Vector4} dst or new Vector4 if not provided
   * @memberOf module:webgl-3d-math
   */
   function transformPoint(m, v, dst) {
    dst = mat4.create();
    var v0 = v[0];
    var v1 = v[1];
    var v2 = v[2];
    var d = v0 * m[0 * 4 + 3] + v1 * m[1 * 4 + 3] + v2 * m[2 * 4 + 3] + m[3 * 4 + 3];

    dst[0] = (v0 * m[0 * 4 + 0] + v1 * m[1 * 4 + 0] + v2 * m[2 * 4 + 0] + m[3 * 4 + 0]) / d;
    dst[1] = (v0 * m[0 * 4 + 1] + v1 * m[1 * 4 + 1] + v2 * m[2 * 4 + 1] + m[3 * 4 + 1]) / d;
    dst[2] = (v0 * m[0 * 4 + 2] + v1 * m[1 * 4 + 2] + v2 * m[2 * 4 + 2] + m[3 * 4 + 2]) / d;

    return dst;
  }

function invert(a) 
{
    let a00 = a[0],
      a01 = a[1],
      a02 = a[2],
      a03 = a[3];
    let a10 = a[4],
      a11 = a[5],
      a12 = a[6],
      a13 = a[7];
    let a20 = a[8],
      a21 = a[9],
      a22 = a[10],
      a23 = a[11];
    let a30 = a[12],
      a31 = a[13],
      a32 = a[14],
      a33 = a[15];
    let b00 = a00 * a11 - a01 * a10;
    let b01 = a00 * a12 - a02 * a10;
    let b02 = a00 * a13 - a03 * a10;
    let b03 = a01 * a12 - a02 * a11;
    let b04 = a01 * a13 - a03 * a11;
    let b05 = a02 * a13 - a03 * a12;
    let b06 = a20 * a31 - a21 * a30;
    let b07 = a20 * a32 - a22 * a30;
    let b08 = a20 * a33 - a23 * a30;
    let b09 = a21 * a32 - a22 * a31;
    let b10 = a21 * a33 - a23 * a31;
    let b11 = a22 * a33 - a23 * a32;
    // Calculate the determinant
    let det =
      b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
    if (!det) {
      return null;
    }
    return det;
}


function update()
{
    if(grangle >= 360.0)
    {
        grangle = 0.0;
    }
    grangle = grangle + 0.1;

}

function deg2rad(degrees)
{
    var rad = degrees * Math.PI / 180.0;
    return rad;
}

function uninitialize()
{
    if(grvaoPyramid)
    {
        gl.deleteVertexArray(grvaoPyramid);
        grvaoPyramid = null;
    }
    if(grvboPyramidPosition)
    {
        gl.deleteBuffer(grvboPyramidPosition);
        grvboPyramidPosition = null;
    }
    if(grvboPyramidColors)
    {
        gl.deleteBuffer(grvboPyramidColors);
        grvboPyramidColors = null;
    }
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




