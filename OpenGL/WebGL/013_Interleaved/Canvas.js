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

var grVaoCube;
var grVboCube;
var grAngleCube = 0.0;

var grmodelMatrixUniform;
var grviewMatrixUiform;
var grprojectionMatrixUniform;
var grlKeyPressedUniform;
var grlaUniform1;
var grldUniform1;
var grlsUniform1;
var grlightPositionUniform1;
var grlaUniform2;
var grldUniform2;
var grlsUniform2;
var grlightPositionUniform2;
var grkaUniform;
var grkdUniform;
var grksUniform;
var grmaterialShhinynessUniform;

var grperspectiveMatrix;
var grbAnimate = false;
var grbLight = false;
var i;
var grka = new Float32Array([0.0, 0.0, 0.0]);
var grkd = new Float32Array([1.0, 1.0, 1.0]);
var grks = new Float32Array([1.0, 1.0, 1.0]);
var grmaterialShinyness = 128.0;
var grlightAmbient1 = new Float32Array([0.0, 0.0, 0.0]);
var grlightDiffuse1 = new Float32Array([1.0, 1.0, 1.0]);
var grlightSpecular1 = new Float32Array([1.0, 1.0, 1.0]);
var grlightPosition1 = new Float32Array([100.0, 100.0, 100.0, 1.0]);
var grlightAmbient2 = new Float32Array([0.0, 0.0, 0.0]);
var grlightDiffuse2 = new Float32Array([1.0, 1.0, 1.0]);
var grlightSpecular2 = new Float32Array([1.0, 1.0, 1.0]);
var grlightPosition2 = new Float32Array([-100.0, -100.0, -100.0, 1.0]); 
var grtexture;
var grtextureSamplerUniform;

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

        case 27 :               //escape
            uninitialize();
            window.close();
            break;

        case 76 :                       // l
            grbLight = !grbLight;
            break;

        case 65 :                       // a
            grbAnimate = !grbAnimate;
            break;
        
   }
   console.log(event.keyCode);
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
    "in vec3 vNormal;" +
    "in vec2 vTexCoord;" +
    "in vec3 vColor;" +
    "uniform mat4 u_model_matrix;" +
    "uniform mat4 u_view_matrix;" +
    "uniform mat4 u_projection_matrix;" +
    "uniform mediump int u_l_key_pressed;" +
    "uniform vec3 u_la[2];" +
    "uniform vec3 u_ld[2];" +
    "uniform vec3 u_ls[2];" +
    "uniform vec3 u_ka;" +
    "uniform vec3 u_kd;" +
    "uniform vec3 u_ks;" +
    "uniform float u_material_shinyness;" +
    "uniform vec4 u_light_position[2];" +
    "out vec3 phong_ads_light;" +
    "out vec2 out_texcoord;" +
    "out vec3 out_color;" +
    "int i;" +
    "void main(void)" +
    "{" +
    "if(u_l_key_pressed == 1)" +
    "{" +
        "vec4 eye_coordinates =  u_view_matrix * u_model_matrix * vPosition;" +
        "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" +
        "vec3 view_vector = normalize(-eye_coordinates.xyz);" +
        "vec3 light_direction[2];" +
        "vec3 reflection_vector[2];" +
        "vec3 ambient[2];" +
        "vec3 diffuse[2];" +
        "vec3 specular[2];" +
        "for(i = 0; i < 2; i++)" +
        "{" +
            "light_direction[i] = normalize(vec3(u_light_position[i] - eye_coordinates));" +
            "reflection_vector[i] = reflect(-light_direction[i], transformed_normal);" +
            "ambient[i] = u_la[i] * u_ka;" +
            "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);" +
            "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflection_vector[i], view_vector), 0.0), u_material_shinyness);" +
            "phong_ads_light = phong_ads_light + ambient[i] + diffuse[i] + specular[i];" +
        "}" +
    "}" +
    "else" +
    "{" +
        "phong_ads_light = vec3(1.0, 1.0, 1.0);" +
    "}" +
    "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" +
    "out_texcoord = vTexCoord;" +
    "out_color = vColor;" +
    "}";

    grvertexShaderObject = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(grvertexShaderObject, grvertexShaderSourceCode);
    gl.compileShader(grvertexShaderObject);
    if(gl.getShaderParameter(grvertexShaderObject, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(grvertexShaderObject);
        if(error.length > 0)
        {
            alert("compilation error in vertex shader : " + error);
            uninitialize();
            window.close();
        }
      

    }

    var grfragmentShaderSourceCode = 
    "#version 300 es" +
    "\n" +
    "precision highp float;" +
    "vec4 color;" +
    "in vec3 phong_ads_light;" +
    "in vec2 out_texcoord;" +
    "in vec3 out_color;" +
    "uniform sampler2D u_texture_sampler;" +
    "uniform mediump int u_l_key_pressed;" +
    "out vec4 FragColor;" +
    "vec3 texture_color;" +
    "void main(void)" +
    "{" +
    "texture_color = vec3(texture(u_texture_sampler, out_texcoord));" +
    "if(u_l_key_pressed == 1)" +
    "{" +
        "color = vec4(vec3(out_color * phong_ads_light * texture_color), 1);" +
    "}" +
    "else" +
    "{" +
        "color = vec4(vec3(out_color * vec3(1.0) * texture_color), 1);"	+
    "}" +
    "FragColor = color;" +
    "}";

    grfragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(grfragmentShaderObject, grfragmentShaderSourceCode);
    gl.compileShader(grfragmentShaderObject);
    if(gl.getShaderParameter(grfragmentShaderObject, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(grfragmentShaderObject);
        if(error.length > 0)
        {
            alert("compilation error in fragment shader : " + error);
            uninitialize();
            window.close();
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
    gl.bindAttribLocation(grshaderProgramObject, WebGLMacros.GR_ATTRIBUTE_NORMAL, "vNormal");
    gl.bindAttribLocation(grshaderProgramObject, WebGLMacros.GR_ATTRIBUTE_TEXTURE, "vTexCoord");

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
    grmodelMatrixUniform = gl.getUniformLocation(grshaderProgramObject, "u_model_matrix");
    grviewMatrixUiform = gl.getUniformLocation(grshaderProgramObject, "u_view_matrix");
    grprojectionMatrixUniform = gl.getUniformLocation(grshaderProgramObject, "u_projection_matrix");

    grldUniform1 = gl.getUniformLocation(grshaderProgramObject, "u_ld[0]");
    grlaUniform1 = gl.getUniformLocation(grshaderProgramObject, "u_la[0]");
    grlsUniform1 = gl.getUniformLocation(grshaderProgramObject, "u_ls[0]");
    grldUniform2 = gl.getUniformLocation(grshaderProgramObject, "u_ld[1]");
    grlaUniform2 = gl.getUniformLocation(grshaderProgramObject, "u_la[1]");
    grlsUniform2 = gl.getUniformLocation(grshaderProgramObject, "u_ls[1]");
    grlightPositionUniform1 = gl.getUniformLocation(grshaderProgramObject, "u_light_position[0]");
    grlightPositionUniform2 = gl.getUniformLocation(grshaderProgramObject, "u_light_position[1]");

    grkaUniform = gl.getUniformLocation(grshaderProgramObject, "u_ka");
    grkdUniform = gl.getUniformLocation(grshaderProgramObject, "u_kd");
    grksUniform = gl.getUniformLocation(grshaderProgramObject, "u_ks");

    grlKeyPressedUniform = gl.getUniformLocation(grshaderProgramObject, "u_l_key_pressed");
    grmaterialShhinynessUniform = gl.getUniformLocation(grshaderProgramObject, "u_material_shinyness");
    grtextureSamplerUniform = gl.getUniformLocation(grshaderProgramObject, "u_texture_sampler");

    var data = new Float32Array(
        [
           //P					    //C					//N					//T
		    1.0, 1.0, 1.0,  	    1.0, 0.0, 0.0,	    0.0, 0.0, 1.0,	    0.0, 0.0,
		    -1.0, 1.0, 1.0,	        1.0, 0.0, 0.0,	    0.0, 0.0, 1.0,	    1.0, 0.0,
		    -1.0, -1.0, 1.0,	    1.0, 0.0, 0.0,	    0.0, 0.0, 1.0,	    1.0, 1.0,
		    1.0, -1.0, 1.0,	        1.0, 0.0, 0.0,	    0.0, 0.0, 1.0,	    0.0, 1.0,

		    1.0, 1.0, -1.0,	        0.0, 1.0, 0.0,	    1.0, 0.0, 0.0,	    1.0, 0.0,
		    1.0, 1.0, 1.0,	        0.0, 1.0, 0.0,	    1.0, 0.0, 0.0,	    1.0, 1.0,
		    1.0, -1.0, 1.0,	        0.0, 1.0, 0.0,	    1.0, 0.0, 0.0,	    0.0, 1.0,
		    1.0, -1.0, -1.0,	    0.0, 1.0, 0.0,	    1.0, 0.0, 0.0,	    0.0, 0.0,

		    -1.0, 1.0, -1.0,	    0.0, 0.0, 1.0,	    0.0, 0.0, -1.0,	    0.0, 1.0,
		    1.0, 1.0, -1.0,	        0.0, 0.0, 1.0,	    0.0, 0.0, -1.0,	    0.0, 0.0,
		    1.0, -1.0, -1.0,	    0.0, 0.0, 1.0,	    0.0, 0.0, -1.0,	    1.0, 0.0,
		    -1.0, -1.0, -1.0,       0.0,0.0, 1.0,	    0.0, 0.0, -1.0,	    1.0, 1.0,

		    -1.0, 1.0, 1.0,	        1.0, 0.0, 1.0,	    -1.0, 0.0, 0.0,	    0.0, 0.0,
		    -1.0, 1.0, -1.0,	    1.0, 0.0, 1.0,	    -1.0, 0.0, 0.0,	    1.0, 0.0,
		    -1.0, -1.0, -1.0,       1.0, 0.0, 1.0,	    -1.0, 0.0, 0.0,	    1.0, 1.0,
		    -1.0, -1.0, 1.0,	    1.0, 0.0, 1.0,	    -1.0, 0.0, 0.0,	    0.0, 1.0,

		    1.0, 1.0, -1.0,	        1.0, 1.0, 0.0,	    0.0, 1.0, 0.0,	    0.0, 1.0,
		    -1.0, 1.0, -1.0,	    1.0, 1.0, 0.0,	    0.0, 1.0, 0.0,	    0.0, 0.0,
		    -1.0, 1.0, 1.0,	        1.0, 1.0, 0.0,	    0.0, 1.0, 0.0,	    1.0, 0.0,
		    1.0, 1.0, 1.0,      	1.0, 1.0, 0.0,	    0.0, 1.0, 0.0,	    1.0, 1.0,

		    1.0, -1.0, -1.0,	    0.0, 1.0, 1.0,	    0.0, -1.0, 0.0,	    1.0, 0.0,
		    -1.0, -1.0, -1.0,       0.0, 1.0, 1.0,	    0.0, -1.0, 0.0,	    0.0, 0.0,
		    -1.0, -1.0, 1.0,	    0.0, 1.0, 1.0,	    0.0, -1.0, 0.0,	    0.0, 1.0,
		    1.0, -1.0, 1.0,	        0.0,1.0, 1.0,	    0.0, -1.0, 0.0,	    1.0, 1.0
        ]
    );
    
    console.log(data.byteLength);
    console.log("length : " + data.length);
    var bytes_per_float

    // triangle
    grVaoCube = gl.createVertexArray();
    gl.bindVertexArray(grVaoCube);

    grVboCube = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, grVboCube);
    gl.bufferData(gl.ARRAY_BUFFER, data, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.GR_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 11 * 4, 0);
    gl.vertexAttribPointer(WebGLMacros.GR_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 11 * 4, 3 * 4);
    gl.vertexAttribPointer(WebGLMacros.GR_ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, 11 * 4, 6 * 4);
    gl.vertexAttribPointer(WebGLMacros.GR_ATTRIBUTE_TEXTURE, 2, gl.FLOAT, false, 11 * 4, 9 * 4);
    gl.enableVertexAttribArray(WebGLMacros.GR_ATTRIBUTE_POSITION);
    gl.enableVertexAttribArray(WebGLMacros.GR_ATTRIBUTE_COLOR);
    gl.enableVertexAttribArray(WebGLMacros.GR_ATTRIBUTE_NORMAL);
    gl.enableVertexAttribArray(WebGLMacros.GR_ATTRIBUTE_TEXTURE);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    
    gl.bindVertexArray(null);

    
    // texture
    grtexture = gl.createTexture();
	grtexture.image = new Image();
	grtexture.image.src = "kundali.png";
	grtexture.image.onload = function()
	{
		gl.bindTexture(gl.TEXTURE_2D, grtexture);
		gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, grtexture.image);
        gl.bindTexture(gl.TEXTURE_2D, null);
	};


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
    var grmodelMatrix = mat4.create();
    var grviewMatrix = mat4.create();
    var grprojectionMatrix = mat4.create();
    var grtranslateMatrix = mat4.create();
    var grrotateMatrix = mat4.create();

    gl.clear(gl.COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT);

    gl.useProgram(grshaderProgramObject);
    mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -6.0]);
    mat4.rotateY(grrotateMatrix, grrotateMatrix, deg2rad(grAngleCube));
    mat4.rotateX(grrotateMatrix, grrotateMatrix, deg2rad(grAngleCube));
    mat4.rotateZ(grrotateMatrix, grrotateMatrix, deg2rad(grAngleCube));
    mat4.multiply(grmodelMatrix, grtranslateMatrix, grrotateMatrix);
    mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

    gl.uniformMatrix4fv(grmodelMatrixUniform, false, grmodelMatrix);
    gl.uniformMatrix4fv(grviewMatrixUiform, false, grviewMatrix);
    gl.uniformMatrix4fv(grprojectionMatrixUniform, false, grprojectionMatrix);

    if(grbLight)
    {
        
        gl.uniform1i(grlKeyPressedUniform, 1);
        
        gl.uniform3fv(grkdUniform, grkd);
        gl.uniform3fv(grkaUniform, grka);
        gl.uniform3fv(grksUniform, grks);
        gl.uniform1f(grmaterialShhinynessUniform, grmaterialShinyness);

        gl.uniform3fv(grldUniform1, grlightDiffuse1);
        gl.uniform3fv(grlaUniform1, grlightAmbient1);
        gl.uniform3fv(grlsUniform1, grlightSpecular1);
        gl.uniform4fv(grlightPositionUniform1, grlightPosition1);

        gl.uniform3fv(grldUniform2, grlightDiffuse2);
        gl.uniform3fv(grlaUniform2, grlightAmbient2);
        gl.uniform3fv(grlsUniform2, grlightSpecular2);
        gl.uniform4fv(grlightPositionUniform2, grlightPosition2);
        
    }
    else
    {
        gl.uniform1i(grlKeyPressedUniform, 0);
    }

    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, grtexture);
    gl.uniform1i(grtextureSamplerUniform, 0)

    gl.bindVertexArray(grVaoCube);
    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);
    gl.bindVertexArray(null);

    gl.useProgram(null);
    if(grbAnimate)
    {
        update();
    }
   
    requestAnimationFrame(draw, canvas);
}

function update()
{
    if(grAngleCube >= 360.0)
    {
        grAngleCube = 0.0;
    }
    grAngleCube = grAngleCube + 0.1;

}

function deg2rad(degrees)
{
    var rad = degrees * Math.PI / 180.0;
    return rad;
}

function uninitialize()
{
   
    if(grVboCube)
    {
        gl.deleteBuffer(grVboCube);
        grVboCube = null;
    }
    if(grVaoCube)
    {
        gl.deleteVertexArray(grVaoCube);
        grVaoCube = null;
    }
    if(grtexture)
    {
        gl.deleteTexture(grtexture);
        grtexture = null;
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




