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

var grvertexShaderObjectPF;
var grfragmentShaderObjectPF;
var grshaderProgramObjectPF;

var grsphere;

var grmodelMatrixUniformPF;
var grviewMatrixUiformPF;
var grprojectionMatrixUniformPF;
var grlKeyPressedUniformPF;
var grlaUniformPF;
var grldUniformPF;
var grlsUniformPF;
var grlightPositionUniformPF;
var grkaUniformPF;
var grkdUniformPF;
var grksUniformPF;
var grmaterialShhinynessUniformPF;

var grperspectiveMatrix;
var grbAnimate = false;
var grbIsVertexLighting = true;
var grbIsLight = false;
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

        case 86 :                       // v
            grbIsVertexLighting = !grbIsVertexLighting;
            break;

        case 76 :                       // l
            grbIsLight = !grbIsLight;
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

    //***************************** Per Vertex ***********************
    var grvertexShaderSourceCodePV = 
    "#version 300 es" +
    "\n" +
    "in vec4 vPosition;" +
    "in vec3 vNormal;" +
    "uniform mat4 u_model_matrix;" +
    "uniform mat4 u_view_matrix;" +
    "uniform mat4 u_projection_matrix;" +
    "uniform int u_l_key_pressed;" +
    "uniform vec3 u_la;" +
    "uniform vec3 u_ld;" +
    "uniform vec3 u_ls;" +
    "uniform vec3 u_ka;" +
    "uniform vec3 u_kd;" +
    "uniform vec3 u_ks;" +
    "uniform float u_material_shinyness;" +
    "uniform vec4 u_light_position;" +
    "out vec3 fong_ads_light;" +
    "void main(void)" +
    "{" +
    "if(u_l_key_pressed == 1)" +
    "{" +
    "vec4 eye_coordinates =  u_view_matrix * u_model_matrix * vPosition;" +
    "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" +
    "vec3 light_direction = normalize(vec3(u_light_position - eye_coordinates));" +
    "vec3 reflection_vector = reflect(-light_direction, transformed_normal);" +
    "vec3 view_vector = normalize(-eye_coordinates.xyz);" +
    "vec3 ambient = u_la * u_ka;" +
    "vec3 diffuse = u_ld * u_kd * max(dot(light_direction, transformed_normal), 0.0);" +
    "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector, view_vector), 0.0), u_material_shinyness);" +
    "fong_ads_light = ambient + diffuse + specular;" +
    "}" +
    "else" +
    "{" +
    "fong_ads_light = vec3(1.0, 1.0, 1.0);" +
    "}" +
    "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" +
    "}";

    grvertexShaderObjectPV = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(grvertexShaderObjectPV, grvertexShaderSourceCodePV);
    gl.compileShader(grvertexShaderObjectPV);
    if(gl.getShaderParameter(grvertexShaderObjectPV, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(grvertexShaderObjectPV);
        if(error.length > 0)
        {
            alert("compilation error in PV vertex shader : " + error);
            uninitialize();
            window.close();
        }
      

    }

    var grfragmentShaderSourceCodePV = 
    "#version 300 es" +
    "\n" +
    "precision highp float;" +
    "in vec3 fong_ads_light;" +
    "out vec4 FragColor;" +
    "void main(void)" +
    "{" +
    "FragColor = vec4(fong_ads_light, 1.0);" +
    "}";
    "}";

    grfragmentShaderObjectPV = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(grfragmentShaderObjectPV, grfragmentShaderSourceCodePV);
    gl.compileShader(grfragmentShaderObjectPV);
    if(gl.getShaderParameter(grfragmentShaderObjectPV, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(grfragmentShaderObjectPV);
        if(error.length > 0)
        {
            alert("compilation error in PV fragment shader : " + error);
            uninitialize();
            window.close();
        }
        alert("in compile fragment shader error");
        
    }

    // shader program
    grshaderProgramObjectPV = gl.createProgram();
    //attach shader object
    gl.attachShader(grshaderProgramObjectPV, grvertexShaderObjectPV);
    gl.attachShader(grshaderProgramObjectPV, grfragmentShaderObjectPV);
    // pre-linking
    gl.bindAttribLocation(grshaderProgramObjectPV, WebGLMacros.GR_ATTRIBUTE_POSITION, "vPosition");
    gl.bindAttribLocation(grshaderProgramObjectPV, WebGLMacros.GR_ATTRIBUTE_NORMAL, "vNormal");

    // linking
    gl.linkProgram(grshaderProgramObjectPV);
    if(!gl.getProgramParameter(grshaderProgramObjectPV, gl.LINK_STATUS))
    {
        var err = gl.getProgramInfoLog(grshaderProgramObjectPV);
        if(err.length > 0)
        {
            alert("Per Vertex shader program object error : " + err);
            uninitialize();
            window.close();
        }
    }

    // mvp uniform binding
    grmodelMatrixUniformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_model_matrix");
    grviewMatrixUiformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_view_matrix");
    grprojectionMatrixUniformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_projection_matrix");

    grldUniformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_ld");
    grlaUniformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_la");
    grlsUniformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_ls");

    grlightPositionUniformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_light_position");

    grkaUniformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_ka");
    grkdUniformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_kd");
    grksUniformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_ks");

    grlKeyPressedUniformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_l_key_pressed");
    grmaterialShhinynessUniformPV = gl.getUniformLocation(grshaderProgramObjectPV, "u_material_shinyness");

    //*************************** Per Fragment *************************
    // vertex shader
    var grvertexShaderSourceCodePF = 
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


    grvertexShaderObjectPF = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(grvertexShaderObjectPF, grvertexShaderSourceCodePF);
    gl.compileShader(grvertexShaderObjectPF);
    if(gl.getShaderParameter(grvertexShaderObjectPF, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(grvertexShaderObjectPF);
        if(error.length > 0)
        {
            alert("compilation error in vertex shader : " + error);
            uninitialize();
            window.close();
        }
      

    }

    var grfragmentShaderSourceCodePF = 
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

    grfragmentShaderObjectPF = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(grfragmentShaderObjectPF, grfragmentShaderSourceCodePF);
    gl.compileShader(grfragmentShaderObjectPF);
    if(gl.getShaderParameter(grfragmentShaderObjectPF, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(grfragmentShaderObjectPF);
        if(error.length > 0)
        {
            alert("compilation error in fragment shader : " + error);
            uninitialize();
            window.close();
        }
        alert("in compile fragment shader error");
        
    }

    // shader program
    grshaderProgramObjectPF = gl.createProgram();
    //attach shader object
    gl.attachShader(grshaderProgramObjectPF, grvertexShaderObjectPF);
    gl.attachShader(grshaderProgramObjectPF, grfragmentShaderObjectPF);
    // pre-linking
    gl.bindAttribLocation(grshaderProgramObjectPF, WebGLMacros.GR_ATTRIBUTE_POSITION, "vPosition");
    gl.bindAttribLocation(grshaderProgramObjectPF, WebGLMacros.GR_ATTRIBUTE_NORMAL, "vNormal");

    // linking
    gl.linkProgram(grshaderProgramObjectPF);
    if(!gl.getProgramParameter(grshaderProgramObjectPF, gl.LINK_STATUS))
    {
        var err = gl.getProgramInfoLog(grshaderProgramObjectPF);
        if(err.length > 0)
        {
            alert(err);
            
        }
        
        alert("in shader program object error");
        alert(err);
       // uninitialize(); 
    }

    // mvp uniform binding
    grmodelMatrixUniformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_model_matrix");
    grviewMatrixUiformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_view_matrix");
    grprojectionMatrixUniformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_projection_matrix");

    grldUniformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_ld");
    grlaUniformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_la");
    grlsUniformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_ls");

    grlightPositionUniformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_light_position");

    grkaUniformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_ka");
    grkdUniformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_kd");
    grksUniformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_ks");

    grlKeyPressedUniformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_l_key_pressed");
    grmaterialShhinynessUniformPF = gl.getUniformLocation(grshaderProgramObjectPF, "u_material_shinyness");

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

    if(grbIsVertexLighting == true)
    {
        gl.useProgram(grshaderProgramObjectPV);
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -6.0]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);

        if(grbIsLight)
        {
        
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd);
            gl.uniform3fv(grkaUniformPV, grka);
            gl.uniform3fv(grksUniformPV, grks);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness);

            gl.uniform3fv(grldUniformPV, grlightDiffuse);
            gl.uniform3fv(grlaUniformPV, grlightAmbient);
            gl.uniform3fv(grlsUniformPV, grlightSpecular);
            gl.uniform4fv(grlightPositionUniformPV, grlightPosition);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }

        grsphere.draw();
    }
    else
    {
        gl.useProgram(grshaderProgramObjectPF);
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -6.0]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);
    
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
    
        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
            
            gl.uniform3fv(grkdUniformPF, grkd);
            gl.uniform3fv(grkaUniformPF, grka);
            gl.uniform3fv(grksUniformPF, grks);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness);
    
            gl.uniform3fv(grldUniformPF, grlightDiffuse);
            gl.uniform3fv(grlaUniformPF, grlightAmbient);
            gl.uniform3fv(grlsUniformPF, grlightSpecular);
            gl.uniform4fv(grlightPositionUniformPF, grlightPosition);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
    
        grsphere.draw();
    }
    
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
    
    if(grshaderProgramObjectPV)
    {
        if(grfragmentShaderObjectPV)
        {
            gl.detachShader(grshaderProgramObjectPV, grfragmentShaderObjectPV);
            gl.deleteShader(grfragmentShaderObjectPV);
            grfragmentShaderObjectPV = null;
        }

        if(grfragmentShaderObjectPV)
        {
            gl.detachShader(grshaderProgramObjectPV, grvertexShaderObjectPV);
            gl.deleteShader(grvertexShaderObjectPV);
            grvertexShaderObjectPV = null;
        }

        gl.deleteProgram(grshaderProgramObjectPV);
        grshaderProgramObjectPV = null;
    }

    if(grshaderProgramObjectPF)
    {
        if(grfragmentShaderObjectPF)
        {
            gl.detachShader(grshaderProgramObjectPF, grfragmentShaderObjectPF);
            gl.deleteShader(grfragmentShaderObjectPF);
            grfragmentShaderObjectPF = null;
        }

        if(grfragmentShaderObjectPF)
        {
            gl.detachShader(grshaderProgramObjectPF, grvertexShaderObjectPF);
            gl.deleteShader(grvertexShaderObjectPF);
            grvertexShaderObjectPF = null;
        }

        gl.deleteProgram(grshaderProgramObjectPF);
        grshaderProgramObjectPF = null;
    }

}




