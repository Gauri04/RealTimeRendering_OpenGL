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

var grvertexShaderObjectPV;
var grfragmentShaderObjectPV;
var grshaderProgramObjectPV;

var grsphere1;
var grsphere2;
var grsphere3;
var grsphere4;
var grsphere5;
var grsphere6;
var grsphere7;
var grsphere8;
var grsphere9;
var grsphere10;
var grsphere11;
var grsphere12;
var grsphere13;
var grsphere14;
var grsphere15;
var grsphere16;
var grsphere17;
var grsphere18;
var grsphere19;
var grsphere20;
var grsphere21;
var grsphere22;
var grsphere23;
var grsphere24;

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

var grmodelMatrixUniformPV;
var grviewMatrixUiformPV;
var grprojectionMatrixUniformPV;
var grlKeyPressedUniformPV;
var grlaUniformPV;
var grldUniformPV;
var grlsUniformPV;
var grlightPositionUniformPV;
var grkaUniformPV;
var grkdUniformPV;
var grksUniformPV;
var grmaterialShhinynessUniformPV;

var grperspectiveMatrix;
var grbAnimate = false;
var grbIsVertexLighting = true;
var grbIsLight = false;
var i;
var x = 20;
var y;
var grfangle = 0.0;
var grla = new Float32Array([0.0, 0.0, 0.0]);
var grld = new Float32Array([1.0, 1.0, 1.0]);
var grls = new Float32Array([1.0, 1.0, 1.0]);
var grlightPosition = new Float32Array([0.0, 0.0, 100.0, 1.0]);

var grka1;
var grkd1;
var grks1;
var grmaterialShinyness1;
var grka2;
var grkd2;
var grks2;
var grmaterialShinyness2;
var grka3;
var grkd3;
var grks3;
var grmaterialShinyness3;
var grka4;
var grkd4;
var grks4;
var grmaterialShinyness4;
var grka5;
var grkd5;
var grks5;
var grmaterialShinyness5;
var grka6;
var grkd6;
var grks6;
var grmaterialShinyness6;
var grka7;
var grkd7;
var grks7;
var grmaterialShinyness7;
var grka8;
var grkd8;
var grks8;
var grmaterialShinyness8;
var grka9;
var grkd9;
var grks9;
var grmaterialShinyness9;
var grka10;
var grkd10;
var grks10;
var grmaterialShinyness10;
var grka11;
var grkd11;
var grks11;
var grmaterialShinyness11;
var grka12;
var grkd12;
var grks12;
var grmaterialShinyness12;
var grka13;
var grkd13;
var grks13;
var grmaterialShinyness13;
var grka14;
var grkd14;
var grks14;
var grmaterialShinyness14;
var grka15;
var grkd15;
var grks15;
var grmaterialShinyness15;
var grka16;
var grkd16;
var grks16;
var grmaterialShinyness16;
var grka17;
var grkd17;
var grks17;
var grmaterialShinyness17;
var grka18;
var grkd18;
var grks18;
var grmaterialShinyness18;
var grka19;
var grkd19;
var grks19;
var grmaterialShinyness19;
var grka20;
var grkd20;
var grks20;
var grmaterialShinyness20;
var grka21;
var grkd21;
var grks21;
var grmaterialShinyness21;
var grka22;
var grkd22;
var grks22;
var grmaterialShinyness1;
var grka23;
var grkd23;
var grks23;
var grmaterialShinyness23;
var grka24;
var grkd24;
var grks24;
var grmaterialShinyness24;

var grmaterialShinyness = 128.0;
var grkeyPressed = 0;
var left;
var right;
var top;
var bottom;

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

        case 88 :                       // x
            grkeyPressed = 1;
            break;

        case 89 :                       //y
            grkeyPressed = 2;
            break;

        case 90 :                       // z
            grkeyPressed = 3;
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
    "uniform mediump int u_l_key_pressed;" +
    "uniform vec3 u_ld;" +
    "uniform vec3 u_la;" +
    "uniform vec3 u_ls;" +
    "uniform vec3 u_ka;" +
    "uniform vec3 u_kd;" +
    "uniform vec3 u_ks;" +
    "uniform float u_material_shinyness;" +
    "uniform vec4 u_light_position;" +
    "out vec3 out_phong_light;" +
    "void main(void)" +
    "{" +
    "if(u_l_key_pressed == 1)" +
    "{" +
        "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;" +
        "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" +
        "vec3 view_vector = normalize(-eye_coordinates.xyz);" +
        "vec3 light_direction = normalize(vec3(u_light_position - eye_coordinates));" +
        "vec3 reflection_vector = reflect(-light_direction, transformed_normal);" +
        "vec3 ambient = u_la * u_ka;" +
        "vec3 diffuse = u_ld * u_kd * max(dot(light_direction, transformed_normal), 0.0);" +
        "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector, view_vector), 0.0), u_material_shinyness);" +
        "out_phong_light = ambient + diffuse + specular;" +
    "}" +
    "else" +
    "{" +
        "out_phong_light = vec3(1.0, 1.0, 1.0);" +
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
    "in vec3 out_phong_light;" +
    "out vec4 FragColor;" +
    "void main(void)" +
    "{" +
    "FragColor = vec4(out_phong_light, 1.0);" +
    "}"

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
    "out vec3 out_transformed_normal;" +
    "out vec3 out_light_direction;" +
    "out vec3 out_view_vector;" +
    "void main(void)" +
    "{" +
    "if(u_l_key_pressed == 1)" +
    "{" +
        "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;" +
        "out_transformed_normal = mat3(u_view_matrix * u_model_matrix) * vNormal;" +
        "out_view_vector = -eye_coordinates.xyz;" +
        "out_light_direction = vec3(u_light_position - eye_coordinates);" +
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
    "in vec3 out_transformed_normal;" +
    "in vec3 out_light_direction;" +
    "in vec3 out_view_vector;" +
    "uniform vec3 u_la;" +
    "uniform vec3 u_ld;" +
    "uniform vec3 u_ls;" +
    "uniform vec3 u_ka;" +
    "uniform vec3 u_kd;" +
    "uniform vec3 u_ks;" +
    "uniform float u_material_shinyness;" +
    "uniform mediump int u_l_key_pressed;" +
    "vec3 phong_ads_light;" +
    "out vec4 FragColor;" +
    "void main(void)" +
    "{" +
    "if(u_l_key_pressed == 1)" +
    "{" +
        "vec3 normalized_transform_normal = normalize(out_transformed_normal);" +
        "vec3 normalized_view_vector = normalize(out_view_vector);" +
        "vec3 normalized_light_direction = normalize(out_light_direction);" +
        "vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transform_normal);" +
        "vec3 ambient = u_la * u_ka;" +
        "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transform_normal), 0.0);" +
        "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector, normalized_view_vector), 0.0), u_material_shinyness);" +
        "phong_ads_light = ambient + diffuse + specular;" +
    "}" +
    "else" +
    "{" +
        "phong_ads_light = vec3(1.0, 1.0, 1.0);" +
    "}"	+
    "FragColor = vec4(phong_ads_light, 1.0);" +
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
            alert("Error in shader program PF : " + err);
            uninitialize(); 
            window.close();
        }
        
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

    initializeSphere();
    initializeSphereColors();


    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);
    grperspectiveMatrix = mat4.create();
    // blue color
    gl.clearColor(0.25, 0.25, 0.25, 1.0);
     
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
    
    left = 0;
    right = (canvas.width) / 6;
    top = 0;
    bottom = canvas.height / 4;
   
    console.log("X : " + x + " left : " + left + " right : " + right + " top : " + top + " bottom : " + bottom);
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
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
        
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd1);
            gl.uniform3fv(grkaUniformPV, grka1);
            gl.uniform3fv(grksUniformPV, grks1);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness1);
            
            if(grkeyPressed == 1)
            {
                grlightPosition = new Float32Array([0.0, 7.5 * Math.sin(grfangle), 7.5 * Math.cos(grfangle), 1.0]);
            }
            else if(grkeyPressed == 2)
            {
                grlightPosition = new Float32Array([7.5 * Math.sin(grfangle), 0.0,  7.5 * Math.cos(grfangle), 1.0]);
            }
            if(grkeyPressed == 3)
            {
                grlightPosition = new Float32Array([7.0 * Math.sin(grfangle), 7.0 * Math.cos(grfangle), 0.0,  1.0]);
            }

            gl.uniform3fv(grldUniformPV, grld);
            gl.uniform3fv(grlaUniformPV, grla);
            gl.uniform3fv(grlsUniformPV, grls);
            gl.uniform4fv(grlightPositionUniformPV, grlightPosition);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(left - x, bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        // 2
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();

        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd2);
            gl.uniform3fv(grkaUniformPV, grka2);
            gl.uniform3fv(grksUniformPV, grks2);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness2);

        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right - x, bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        // 3
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd3);
            gl.uniform3fv(grkaUniformPV, grka3);
            gl.uniform3fv(grksUniformPV, grks3);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness3);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 2 - x, bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //4
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd4);
            gl.uniform3fv(grkaUniformPV, grka4);
            gl.uniform3fv(grksUniformPV, grks4);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness4);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 3 -x , bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //5
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd5);
            gl.uniform3fv(grkaUniformPV, grka5);
            gl.uniform3fv(grksUniformPV, grks5);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness5);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 4 - x, bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //6
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd6);
            gl.uniform3fv(grkaUniformPV, grka6);
            gl.uniform3fv(grksUniformPV, grks6);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness6);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 5 - x, bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //7
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd7);
            gl.uniform3fv(grkaUniformPV, grka7);
            gl.uniform3fv(grksUniformPV, grks7);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness7);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(left -x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //8
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd8);
            gl.uniform3fv(grkaUniformPV, grka8);
            gl.uniform3fv(grksUniformPV, grks8);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness8);

        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right - x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //9
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd9);
            gl.uniform3fv(grkaUniformPV, grka9);
            gl.uniform3fv(grksUniformPV, grks9);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness9);
            
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 2 - x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();
        
        //10
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd10);
            gl.uniform3fv(grkaUniformPV, grka10);
            gl.uniform3fv(grksUniformPV, grks10);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness10);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 3 - x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //11
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd11);
            gl.uniform3fv(grkaUniformPV, grka11);
            gl.uniform3fv(grksUniformPV, grks11);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness11);
       }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 4 - x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //12
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd12);
            gl.uniform3fv(grkaUniformPV, grka12);
            gl.uniform3fv(grksUniformPV, grks12);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness12);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 5 - x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //13
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd13);
            gl.uniform3fv(grkaUniformPV, grka13);
            gl.uniform3fv(grksUniformPV, grks13);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness13);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(left - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //14
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd14);
            gl.uniform3fv(grkaUniformPV, grka14);
            gl.uniform3fv(grksUniformPV, grks14);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness14);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //15
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd15);
            gl.uniform3fv(grkaUniformPV, grka15);
            gl.uniform3fv(grksUniformPV, grks15);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness15);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 2 - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //16
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd16);
            gl.uniform3fv(grkaUniformPV, grka16);
            gl.uniform3fv(grksUniformPV, grks16);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness16);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 3 - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //17
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd17);
            gl.uniform3fv(grkaUniformPV, grka17);
            gl.uniform3fv(grksUniformPV, grks17);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness17);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 4 - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //18
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd18);
            gl.uniform3fv(grkaUniformPV, grka18);
            gl.uniform3fv(grksUniformPV, grks18);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness18);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 5 - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //19
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd19);
            gl.uniform3fv(grkaUniformPV, grka19);
            gl.uniform3fv(grksUniformPV, grks19);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness19);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(left - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //20
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd20);
            gl.uniform3fv(grkaUniformPV, grka20);
            gl.uniform3fv(grksUniformPV, grks20);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness20);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //21
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd21);
            gl.uniform3fv(grkaUniformPV, grka21);
            gl.uniform3fv(grksUniformPV, grks21);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness21);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 2 - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //22
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd22);
            gl.uniform3fv(grkaUniformPV, grka22);
            gl.uniform3fv(grksUniformPV, grks22);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness22);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 3 - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //23
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd23);
            gl.uniform3fv(grkaUniformPV, grka23);
            gl.uniform3fv(grksUniformPV, grks23);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness23);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 4 - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

        //24
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPV, 1);
        
            gl.uniform3fv(grkdUniformPV, grkd24);
            gl.uniform3fv(grkaUniformPV, grka24);
            gl.uniform3fv(grksUniformPV, grks24);
            gl.uniform1f(grmaterialShhinynessUniformPV, grmaterialShinyness24);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPV, 0);
        }
        gl.viewport(right * 5 - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPV, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPV, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPV, false, grprojectionMatrix);
        grsphere1.draw();

    }
    else
    {
        gl.useProgram(grshaderProgramObjectPF);
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
        
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd1);
            gl.uniform3fv(grkaUniformPF, grka1);
            gl.uniform3fv(grksUniformPF, grks1);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness1);
            
            if(grkeyPressed == 1)
            {
                grlightPosition = new Float32Array([0.0, 7.5 * Math.sin(grfangle), 7.5 * Math.cos(grfangle), 1.0]);
            }
            else if(grkeyPressed == 2)
            {
                grlightPosition = new Float32Array([7.5 * Math.sin(grfangle), 0.0,  7.5 * Math.cos(grfangle), 1.0]);
            }
            if(grkeyPressed == 3)
            {
                grlightPosition = new Float32Array([7.0 * Math.sin(grfangle), 7.0 * Math.cos(grfangle), 0.0,  1.0]);
            }

            gl.uniform3fv(grldUniformPF, grld);
            gl.uniform3fv(grlaUniformPF, grla);
            gl.uniform3fv(grlsUniformPF, grls);
            gl.uniform4fv(grlightPositionUniformPF, grlightPosition);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(left - x, bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        // 2
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();

        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd2);
            gl.uniform3fv(grkaUniformPF, grka2);
            gl.uniform3fv(grksUniformPF, grks2);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness2);

        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right - x, bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        // 3
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd3);
            gl.uniform3fv(grkaUniformPF, grka3);
            gl.uniform3fv(grksUniformPF, grks3);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness3);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 2 - x, bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //4
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd4);
            gl.uniform3fv(grkaUniformPF, grka4);
            gl.uniform3fv(grksUniformPF, grks4);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness4);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 3 -x , bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //5
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd5);
            gl.uniform3fv(grkaUniformPF, grka5);
            gl.uniform3fv(grksUniformPF, grks5);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness5);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 4 - x, bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //6
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd6);
            gl.uniform3fv(grkaUniformPF, grka6);
            gl.uniform3fv(grksUniformPF, grks6);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness6);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 5 - x, bottom * 3, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //7
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd7);
            gl.uniform3fv(grkaUniformPF, grka7);
            gl.uniform3fv(grksUniformPF, grks7);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness7);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(left -x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //8
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd8);
            gl.uniform3fv(grkaUniformPF, grka8);
            gl.uniform3fv(grksUniformPF, grks8);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness8);

        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right - x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //9
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd9);
            gl.uniform3fv(grkaUniformPF, grka9);
            gl.uniform3fv(grksUniformPF, grks9);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness9);
            
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 2 - x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();
        
        //10
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd10);
            gl.uniform3fv(grkaUniformPF, grka10);
            gl.uniform3fv(grksUniformPF, grks10);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness10);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 3 - x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //11
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd11);
            gl.uniform3fv(grkaUniformPF, grka11);
            gl.uniform3fv(grksUniformPF, grks11);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness11);
       }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 4 - x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //12
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd12);
            gl.uniform3fv(grkaUniformPF, grka12);
            gl.uniform3fv(grksUniformPF, grks12);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness12);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 5 - x, bottom * 2, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //13
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd13);
            gl.uniform3fv(grkaUniformPF, grka13);
            gl.uniform3fv(grksUniformPF, grks13);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness13);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(left - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //14
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd14);
            gl.uniform3fv(grkaUniformPF, grka14);
            gl.uniform3fv(grksUniformPF, grks14);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness14);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //15
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd15);
            gl.uniform3fv(grkaUniformPF, grka15);
            gl.uniform3fv(grksUniformPF, grks15);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness15);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 2 - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //16
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd16);
            gl.uniform3fv(grkaUniformPF, grka16);
            gl.uniform3fv(grksUniformPF, grks16);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness16);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 3 - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //17
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd17);
            gl.uniform3fv(grkaUniformPF, grka17);
            gl.uniform3fv(grksUniformPF, grks17);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness17);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 4 - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //18
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd18);
            gl.uniform3fv(grkaUniformPF, grka18);
            gl.uniform3fv(grksUniformPF, grks18);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness18);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 5 - x, bottom, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //19
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd19);
            gl.uniform3fv(grkaUniformPF, grka19);
            gl.uniform3fv(grksUniformPF, grks19);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness19);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(left - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //20
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd20);
            gl.uniform3fv(grkaUniformPF, grka20);
            gl.uniform3fv(grksUniformPF, grks20);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness20);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //21
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd21);
            gl.uniform3fv(grkaUniformPF, grka21);
            gl.uniform3fv(grksUniformPF, grks21);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness21);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 2 - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //22
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd22);
            gl.uniform3fv(grkaUniformPF, grka22);
            gl.uniform3fv(grksUniformPF, grks22);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness22);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 3 - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //23
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd23);
            gl.uniform3fv(grkaUniformPF, grka23);
            gl.uniform3fv(grksUniformPF, grks23);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness23);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 4 - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

        //24
        grmodelMatrix = mat4.create();
        grtranslateMatrix = mat4.create();
        grprojectionMatrix = mat4.create();
        
        mat4.translate(grtranslateMatrix, grtranslateMatrix, [0.0, 0.0, -2.5]);
        mat4.multiply(grmodelMatrix, grmodelMatrix, grtranslateMatrix);
        mat4.multiply(grprojectionMatrix, grprojectionMatrix, grperspectiveMatrix);

        if(grbIsLight)
        {
            
            gl.uniform1i(grlKeyPressedUniformPF, 1);
        
            gl.uniform3fv(grkdUniformPF, grkd24);
            gl.uniform3fv(grkaUniformPF, grka24);
            gl.uniform3fv(grksUniformPF, grks24);
            gl.uniform1f(grmaterialShhinynessUniformPF, grmaterialShinyness24);
        }
        else
        {
            gl.uniform1i(grlKeyPressedUniformPF, 0);
        }
        gl.viewport(right * 5 - x, top, right + x, bottom);
        gl.uniformMatrix4fv(grmodelMatrixUniformPF, false, grmodelMatrix);
        gl.uniformMatrix4fv(grviewMatrixUiformPF, false, grviewMatrix);
        gl.uniformMatrix4fv(grprojectionMatrixUniformPF, false, grprojectionMatrix);
        grsphere1.draw();

    }
    
    update();
    gl.useProgram(null); 
    requestAnimationFrame(draw, canvas);
}

function update()
{
    // code
    if(grkeyPressed != 0)
    {
        grfangle = grfangle + 0.04;
        if(grfangle >= 360.0)
        {
            grfangle = 0.0;
        }
    }
}

function initializeSphereColors()
{
    grka1 = new Float32Array([0.0215, 0.1745, 0.0215]);
    grkd1 = new Float32Array([0.07568, 0.61424, 0.07568]);
    grks1 = new Float32Array([0.633, 0.727811, 0.633]);
    grmaterialShinyness1 = 0.6 * 128.0;

    grka2 = new Float32Array([0.135, 0.2225, 0.1575]);
    grkd2 = new Float32Array([0.54, 0.89, 0.63]);
    grks2 = new Float32Array([0.316228,0.316228, 0.316228]);
    grmaterialShinyness2 = 0.1 * 128.0;

    grka3 = new Float32Array([0.05375, 0.05, 0.06625]);
    grkd3 = new Float32Array([0.18275, 0.17, 0.22525]);
    grks3 = new Float32Array([0.332741, 0.328634, 0.346435]);
    grmaterialShinyness3 = 0.3 * 128.0;

    grka4 = new Float32Array([0.25, 0.20725, 0.20725]);
    grkd4 = new Float32Array([1.0, 0.829, 0.829]);
    grks4 = new Float32Array([0.296648, 0.296648, 0.296648]);
    grmaterialShinyness4 = 0.088 * 128.0;

    grka5 = new Float32Array([0.1745, 0.01175, 0.01175]);
    grkd5 = new Float32Array([0.61424, 0.04136, 0.04136]);
    grks5 = new Float32Array([0.727811, 0.626959, 0.626959]);
    grmaterialShinyness5 = 0.6 * 128.0;

    grka6 = new Float32Array([0.1, 0.18725, 0.1745]);
    grkd6 = new Float32Array([0.396, 0.74151, 0.69102]);
    grks6 = new Float32Array([0.297254, 0.30829, 0.306678]);
    grmaterialShinyness6 = 0.1 * 128.0;

    grka7 = new Float32Array([0.329412, 0.223529, 0.027451]);
    grkd7 = new Float32Array([0.780392, 0.568627, 0.113725]);
    grks7 = new Float32Array([0.992157, 0.941176, 0.807843]);
    grmaterialShinyness7 = 0.21794872 * 128.0;

    grka8 = new Float32Array([0.2125, 0.1275, 0.054]);
    grkd8 = new Float32Array([0.714, 0.4284, 0.18144]);
    grks8 = new Float32Array([0.393548, 0.271906, 0.166721]);
    grmaterialShinyness8 = 0.2 * 128.0;

    grka9 = new Float32Array([0.25, 0.25, 0.25]);
    grkd9 = new Float32Array([0.4, 0.4, 0.4]);
    grks9 = new Float32Array([0.774597, 0.774597, 0.774597]);
    grmaterialShinyness9 = 0.6 * 128.0;

    grka10 = new Float32Array([0.19125, 0.0735, 0.0225]);
    grkd10 = new Float32Array([0.7038, 0.27048, 0.0828]);
    grks10 = new Float32Array([0.256777, 0.137622, 0.086014]);
    grmaterialShinyness10 = 0.1 * 128.0;

    grka11 = new Float32Array([0.24725, 0.1995, 0.0745]);
    grkd11 = new Float32Array([0.75164, 0.60648, 0.22648]);
    grks11 = new Float32Array([0.628281, 0.555802, 0.366065]);
    grmaterialShinyness11 = 0.4 * 128.0;

    grka12 = new Float32Array([0.19225, 0.19225, 0.19225]);
    grkd12 = new Float32Array([0.50754, 0.50754, 0.50754]);
    grks12 = new Float32Array([0.508273, 0.508273, 0.508273]);
    grmaterialShinyness12 = 0.4 * 128.0;

    grka13 = new Float32Array([0.0, 0.0, 0.0]);
    grkd13 = new Float32Array([0.01, 0.01, 0.01]);
    grks13 = new Float32Array([0.50, 0.50, 0.50]);
    grmaterialShinyness13 = 0.25 * 128.0;

    grka14 = new Float32Array([0.0, 0.1, 0.06]);
    grkd14 = new Float32Array([0.0, 0.50980392, 0.50980392]);
    grks14 = new Float32Array([0.50196078, 0.50196078, 0.50196078]);
    grmaterialShinyness14 = 0.25 * 128.0;

    grka15 = new Float32Array([0.0, 0.0, 0.0]);
    grkd15 = new Float32Array([0.1, 0.35, 0.1]);
    grks15 = new Float32Array([0.45, 0.55, 0.45]);
    grmaterialShinyness15 = 0.25 * 128.0;

    grka16 = new Float32Array([0.0, 0.0, 0.0]);
    grkd16 = new Float32Array([0.5, 0.0, 0.0]);
    grks16 = new Float32Array([0.7, 0.6, 0.6]);
    grmaterialShinyness16 = 0.25 * 128.0;

    grka17 = new Float32Array([0.0, 0.0, 0.0]);
    grkd17 = new Float32Array([0.55, 0.55, 0.55]);
    grks17 = new Float32Array([0.70, 0.70, 0.70]);
    grmaterialShinyness17 = 0.25 * 128.0;

    grka18 = new Float32Array([0.0, 0.0, 0.0]);
    grkd18 = new Float32Array([0.5, 0.5, 0.0]);
    grks18 = new Float32Array([0.60, 0.60, 0.50]);
    grmaterialShinyness18 = 0.25 * 128.0;

    grka19 = new Float32Array([0.02, 0.02, 0.02]);
    grkd19 = new Float32Array([0.01, 0.01, 0.01]);
    grks19 = new Float32Array([0.4, 0.4, 0.4]);
    grmaterialShinyness19 = 0.078125 * 128.0;

    grka20 = new Float32Array([0.0, 0.05, 0.05]);
    grkd20 = new Float32Array([0.4, 0.5, 0.5]);
    grks20= new Float32Array([0.04, 0.7, 0.7]);
    grmaterialShinyness20 = 0.078125 * 128.0;

    grka21 = new Float32Array([0.0, 0.05, 0.0]);
    grkd21 = new Float32Array([0.4, 0.5, 0.4]);
    grks21 = new Float32Array([0.04, 0.7, 0.04]);
    grmaterialShinyness21 = 0.078125 * 128.0;

    grka22 = new Float32Array([0.05, 0.0, 0.0]);
    grkd22 = new Float32Array([0.5, 0.4, 0.4]);
    grks22 = new Float32Array([0.7, 0.04, 0.04]);
    grmaterialShinyness22 = 0.078125 * 128.0;

    grka23 = new Float32Array([0.05, 0.05, 0.05]);
    grkd23 = new Float32Array([0.5, 0.5, 0.5]);
    grks23 = new Float32Array([0.7, 0.7, 0.7]);
    grmaterialShinyness23 = 0.078125 * 128.0;

    grka24 = new Float32Array([0.05, 0.05, 0.0]);
    grkd24 = new Float32Array([0.5, 0.5, 0.4]);
    grks24 = new Float32Array([0.7, 0.7, 0.04]);
    grmaterialShinyness24 = 0.078125 * 128.0;

}


function initializeSphere()
{
    grsphere1 = new Mesh();
    makeSphere(grsphere1, 1.0, 30, 30);
    grsphere2 = new Mesh();
    makeSphere(grsphere2, 1.0, 30, 30);
    grsphere3 = new Mesh();
    makeSphere(grsphere3, 1.0, 30, 30);
    grsphere4 = new Mesh();
    makeSphere(grsphere4, 1.0, 30, 30);
    grsphere5 = new Mesh();
    makeSphere(grsphere5, 1.0, 30, 30);
    grsphere6 = new Mesh();
    makeSphere(grsphere6, 1.0, 30, 30);
    grsphere7 = new Mesh();
    makeSphere(grsphere7, 1.0, 30, 30);
    grsphere8 = new Mesh();
    makeSphere(grsphere8, 1.0, 30, 30);
    grsphere9 = new Mesh();
    makeSphere(grsphere9, 1.0, 30, 30);
    grsphere10 = new Mesh();
    makeSphere(grsphere10, 1.0, 30, 30);
    grsphere11 = new Mesh();
    makeSphere(grsphere11, 1.0, 30, 30);
    grsphere12 = new Mesh();
    makeSphere(grsphere12, 1.0, 30, 30);
    grsphere13 = new Mesh();
    makeSphere(grsphere13, 1.0, 30, 30);
    grsphere14 = new Mesh();
    makeSphere(grsphere14, 1.0, 30, 30);
    grsphere15 = new Mesh();
    makeSphere(grsphere15, 1.0, 30, 30);
    grsphere16 = new Mesh();
    makeSphere(grsphere16, 1.0, 30, 30);
    grsphere17 = new Mesh();
    makeSphere(grsphere17, 1.0, 30, 30);
    grsphere18 = new Mesh();
    makeSphere(grsphere18, 1.0, 30, 30);
    grsphere19 = new Mesh();
    makeSphere(grsphere19, 1.0, 30, 30);
    grsphere20 = new Mesh();
    makeSphere(grsphere20, 1.0, 30, 30);
    grsphere21 = new Mesh();
    makeSphere(grsphere21, 1.0, 30, 30);
    grsphere22 = new Mesh();
    makeSphere(grsphere22, 1.0, 30, 30);
    grsphere23 = new Mesh();
    makeSphere(grsphere23, 1.0, 30, 30);
    grsphere24 = new Mesh();
    makeSphere(grsphere24, 1.0, 30, 30);

}

function deg2rad(degrees)
{
    var rad = degrees * Math.PI / 180.0;
    return rad;
}

function uninitialize()
{
    deallocateSpheres();
 
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

function deallocateSpheres()
{
    if(grsphere1)
    {
        grsphere1.deallocate();
    }
    if(grsphere2)
    {
        grsphere2.deallocate();
    }
    if(grsphere3)
    {
        grsphere3.deallocate();
    }
    if(grsphere4)
    {
        grsphere4.deallocate();
    }
    if(grsphere5)
    {
        grsphere5.deallocate();
    }
    if(grsphere6)
    {
        grsphere6.deallocate();
    }
    if(grsphere7)
    {
        grsphere7.deallocate();
    }
    if(grsphere8)
    {
        grsphere8.deallocate();
    }
    if(grsphere9)
    {
        grsphere9.deallocate();
    }
    if(grsphere10)
    {
        grsphere10.deallocate();
    }
    if(grsphere11)
    {
        grsphere11.deallocate();
    }
    if(grsphere12)
    {
        grsphere12.deallocate();
    }
    if(grsphere13)
    {
        grsphere13.deallocate();
    }
    if(grsphere14)
    {
        grsphere14.deallocate();
    }
    if(grsphere15)
    {
        grsphere15.deallocate();
    }
    if(grsphere16)
    {
        grsphere16.deallocate();
    }
    if(grsphere17)
    {
        grsphere17.deallocate();
    }
    if(grsphere18)
    {
        grsphere18.deallocate();
    }
    if(grsphere19)
    {
        grsphere19.deallocate();
    }
    if(grsphere20)
    {
        grsphere20.deallocate();
    }
    if(grsphere21)
    {
        grsphere21.deallocate();
    }
    if(grsphere22)
    {
        grsphere22.deallocate();
    }
    if(grsphere23)
    {
        grsphere23.deallocate();
    }
    if(grsphere24)
    {
        grsphere24.deallocate();
    }
}




