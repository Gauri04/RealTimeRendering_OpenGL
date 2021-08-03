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

var grsphere;

var grmodelMatrixUniform;
var grviewMatrixUiform;
var grprojectionMatrixUniform;
var grlKeyPressedUniform;
var grlaUniform;
var grldUniform;
var grlsUniform;
var grlightPositionUniform;
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
var grlightAmbient = new Float32Array([0.0, 0.0, 0.0]);
var grlightDiffuse = new Float32Array([0.5, 0.2, 0.7]);
var grlightSpecular = new Float32Array([0.7, 0.7, 0.7]);
var grlightPosition = new Float32Array([100.0, 100.0, 100.0, 1.0]);

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
    "uniform mat4 u_model_matrix;" +
    "uniform mat4 u_view_matrix;" +
    "uniform mat4 u_projection_matrix;" +
    "uniform mediump int u_l_key_pressed;" +
    "uniform vec4 u_light_position;" +
    "out vec3 transformed_normal;" +
    "out vec3 light_direction;" +
    "out vec3 view_vector;" +
    "void main(void)" +
    "{" +
    "if(u_l_key_pressed == 1)" +
    "{" +
    "vec4 eye_coordinates =  u_view_matrix * u_model_matrix * vPosition;" +
    "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" +
    "light_direction = normalize(vec3(u_light_position - eye_coordinates));" +
    "view_vector = normalize(-eye_coordinates.xyz);" +
    "}" +
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
            alert("compilation error in vertex shader : " + error);
            uninitialize();
            window.close();
        }
      

    }

    var grfragmentShaderSourceCode = 
    "#version 300 es" +
    "\n" +
    "precision highp float;" +
    "in vec3 transformed_normal;" +
    "in vec3 light_direction;" +
    "in vec3 view_vector;" +
    "uniform vec3 u_la;" +
    "uniform vec3 u_ld;" +
    "uniform vec3 u_ls;" +
    "uniform vec3 u_ka;" +
    "uniform vec3 u_kd;" +
    "uniform vec3 u_ks;" +
    "uniform float u_material_shinyness;" +
    "uniform mediump int u_l_key_pressed;" +
    "vec3 fong_ads_light;" +
    "out vec4 FragColor;" +
    "void main(void)" +
    "{" +
    "if(u_l_key_pressed == 1)" +
    "{" +
    "vec3 normalized_transform_normal = normalize(transformed_normal);" +
    "vec3 normalized_light_direction = normalize(light_direction);" +
    "vec3 normalized_view_vector = normalize(view_vector);" +
    "vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transform_normal);" +
    "vec3 ambient = u_la * u_ka;" +
    "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transform_normal), 0.0);" +
    "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector, normalized_view_vector), 0.0), u_material_shinyness);" +
    "fong_ads_light = ambient + diffuse + specular;" +
    "}" +
    "else" +
    "{" +
    "fong_ads_light = vec3(1.0, 1.0, 1.0);" +
    "}" +
    "FragColor = vec4(fong_ads_light, 1.0);" +
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
    gl.bindAttribLocation(grshaderProgramObject, WebGLMacros.GR_ATTRIBUTE_NORMAL, "vNormal");

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

    grldUniform = gl.getUniformLocation(grshaderProgramObject, "u_ld");
    grlaUniform = gl.getUniformLocation(grshaderProgramObject, "u_la");
    grlsUniform = gl.getUniformLocation(grshaderProgramObject, "u_ls");

    grlightPositionUniform = gl.getUniformLocation(grshaderProgramObject, "u_light_position");

    grkaUniform = gl.getUniformLocation(grshaderProgramObject, "u_ka");
    grkdUniform = gl.getUniformLocation(grshaderProgramObject, "u_kd");
    grksUniform = gl.getUniformLocation(grshaderProgramObject, "u_ks");

    grlKeyPressedUniform = gl.getUniformLocation(grshaderProgramObject, "u_l_key_pressed");
    grmaterialShhinynessUniform = gl.getUniformLocation(grshaderProgramObject, "u_material_shinyness");

   grsphere = new Mesh();
   makeSphere(grsphere, 1.0, 30, 30);


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
    mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
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

        gl.uniform3fv(grldUniform, grlightDiffuse);
        gl.uniform3fv(grlaUniform, grlightAmbient);
        gl.uniform3fv(grlsUniform, grlightSpecular);
        gl.uniform4fv(grlightPositionUniform, grlightPosition);
    }
    else
    {
        gl.uniform1i(grlKeyPressedUniform, 0);
    }

    grsphere.draw();

    gl.useProgram(null); 
    requestAnimationFrame(draw, canvas);
}

function update()
{
    // code

}

function deg2rad(degrees)
{
    var rad = degrees * Math.PI / 180.0;
    return rad;
}

function uninitialize()
{
    if(grsphere)
    {
        grsphere.deallocate();
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




