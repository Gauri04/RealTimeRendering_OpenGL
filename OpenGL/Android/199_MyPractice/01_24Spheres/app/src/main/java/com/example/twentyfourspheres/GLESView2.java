package com.example.twentyfourspheres;

import android.content.Context;

import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

import android.opengl.GLSurfaceView;				// OGL SurfaceView and related
import android.opengl.GLES32;						// for OpenGLES32
import android.opengl.Matrix;
import javax.microedition.khronos.opengles.GL10;	// OpenGLES 1.0 needed for param GL10
import javax.microedition.khronos.egl.EGLConfig;	// for EGLConfig

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

public class GLESView2 extends GLSurfaceView implements GLSurfaceView.Renderer,
	OnGestureListener, OnDoubleTapListener
{
	// variables declaration
	private GestureDetector grgestureDetector;
	private final Context grcontext;

	private int grvertexShaderObjectPF;
	private int grfragmentShaderObjectPF;
	private int grshaderProgramObjectPF;
	private int grvertexShaderObjectPV;
	private int grfragmentShaderObjectPV;
	private int grshaderProgramObjectPV;

	private int[] grvaoSphere = new int[1];
	private int[] grvboSpherePosition = new int[1];
	private int[] grvboSphereNormal = new int[1];
	private int[] grvboSphereElement = new int[1];
	private int grnumVertices;
	private int grnumElements;

	private int grmodelMatrixUniformPF;
	private int grviewMatrixUniformPF;
	private int grprojectionMatrixUniformPF;
	private int grlaUniformPF;
	private int grldUniformPF;
	private int grlsUniformPF;
	private int grlightPositionUniformPF;
	private int grkaUniformPF;
	private int grkdUniformPF;
	private int grksUniformPF;
	private int grmaterialShinynessUniformPF;
	private int grlKeyPressedUniformPF;

	private int grmodelMatrixUniformPV;
	private int grviewMatrixUniformPV;
	private int grprojectionMatrixUniformPV;
	private int grlaUniformPV;
	private int grldUniformPV;
	private int grlsUniformPV;
	private int grlightPositionUniformPV;
	private int grkaUniformPV;
	private int grkdUniformPV;
	private int grksUniformPV;
	private int grmaterialShinynessUniformPV;
	private int grlKeyPressedUniformPV;

	private float grla[] = new float[]{0.0f, 0.0f, 0.0f, 1.0f};
	private float grld[] = new float[]{1.0f, 1.0f, 1.0f, 1.0f};
	private float grls[] = new float[]{1.0f, 1.0f, 1.0f, 1.0f};
	private float grlightPosition[] = new float[]{0.0f, 0.0f, 100.0f, 1.0f};
	Light[] grlight = new Light[3];

	// vao and vbos of 24 sphere
	private int[] grvaoSphere1 = new int[1];
	private int[] grvboSpherePosition1 = new int[1];
	private int[] grvboSphereNormal1 = new int[1];
	private int[] grvboSphereElement1 = new int[1];
	private int[] grvaoSphere2 = new int[1];
	private int[] grvboSpherePosition2 = new int[1];
	private int[] grvboSphereNormal2 = new int[1];
	private int[] grvboSphereElement2 = new int[1];
	private int[] grvaoSphere3 = new int[1];
	private int[] grvboSpherePosition3 = new int[1];
	private int[] grvboSphereNormal3 = new int[1];
	private int[] grvboSphereElement3 = new int[1];
	private int[] grvaoSphere4 = new int[1];
	private int[] grvboSpherePosition4 = new int[1];
	private int[] grvboSphereNormal4 = new int[1];
	private int[] grvboSphereElement4 = new int[1];
	private int[] grvaoSphere5 = new int[1];
	private int[] grvboSpherePosition5 = new int[1];
	private int[] grvboSphereNormal5 = new int[1];
	private int[] grvboSphereElement5 = new int[1];
	private int[] grvaoSphere6 = new int[1];
	private int[] grvboSpherePosition6 = new int[1];
	private int[] grvboSphereNormal6 = new int[1];
	private int[] grvboSphereElement6 = new int[1];
	private int[] grvaoSphere7 = new int[1];
	private int[] grvboSpherePosition7 = new int[1];
	private int[] grvboSphereNormal7 = new int[1];
	private int[] grvboSphereElement7 = new int[1];
	private int[] grvaoSphere8 = new int[1];
	private int[] grvboSpherePosition8 = new int[1];
	private int[] grvboSphereNormal8 = new int[1];
	private int[] grvboSphereElement8 = new int[1];
	private int[] grvaoSphere9 = new int[1];
	private int[] grvboSpherePosition9 = new int[1];
	private int[] grvboSphereNormal9 = new int[1];
	private int[] grvboSphereElement9 = new int[1];
	private int[] grvaoSphere10 = new int[1];
	private int[] grvboSpherePosition10 = new int[1];
	private int[] grvboSphereNormal10 = new int[1];
	private int[] grvboSphereElement10 = new int[1];
	private int[] grvaoSphere11 = new int[1];
	private int[] grvboSpherePosition11 = new int[1];
	private int[] grvboSphereNormal11 = new int[1];
	private int[] grvboSphereElement11 = new int[1];
	private int[] grvaoSphere12 = new int[1];
	private int[] grvboSpherePosition12 = new int[1];
	private int[] grvboSphereNormal12 = new int[1];
	private int[] grvboSphereElement12 = new int[1];
	private int[] grvaoSphere13 = new int[1];
	private int[] grvboSpherePosition13 = new int[1];
	private int[] grvboSphereNormal13 = new int[1];
	private int[] grvboSphereElement13 = new int[1];
	private int[] grvaoSphere14 = new int[1];
	private int[] grvboSpherePosition14 = new int[1];
	private int[] grvboSphereNormal14 = new int[1];
	private int[] grvboSphereElement14 = new int[1];
	private int[] grvaoSphere15 = new int[1];
	private int[] grvboSpherePosition15 = new int[1];
	private int[] grvboSphereNormal15 = new int[1];
	private int[] grvboSphereElement15 = new int[1];
	private int[] grvaoSphere16 = new int[1];
	private int[] grvboSpherePosition16 = new int[1];
	private int[] grvboSphereNormal16 = new int[1];
	private int[] grvboSphereElement16 = new int[1];
	private int[] grvaoSphere17 = new int[1];
	private int[] grvboSpherePosition17 = new int[1];
	private int[] grvboSphereNormal17 = new int[1];
	private int[] grvboSphereElement17 = new int[1];
	private int[] grvaoSphere18 = new int[1];
	private int[] grvboSpherePosition18 = new int[1];
	private int[] grvboSphereNormal18 = new int[1];
	private int[] grvboSphereElement18 = new int[1];
	private int[] grvaoSphere19 = new int[1];
	private int[] grvboSpherePosition19 = new int[1];
	private int[] grvboSphereNormal19 = new int[1];
	private int[] grvboSphereElement19 = new int[1];
	private int[] grvaoSphere20 = new int[1];
	private int[] grvboSpherePosition20 = new int[1];
	private int[] grvboSphereNormal20 = new int[1];
	private int[] grvboSphereElement20 = new int[1];
	private int[] grvaoSphere21 = new int[1];
	private int[] grvboSpherePosition21 = new int[1];
	private int[] grvboSphereNormal21 = new int[1];
	private int[] grvboSphereElement21 = new int[1];
	private int[] grvaoSphere22 = new int[1];
	private int[] grvboSpherePosition22 = new int[1];
	private int[] grvboSphereNormal22 = new int[1];
	private int[] grvboSphereElement22 = new int[1];
	private int[] grvaoSphere23 = new int[1];
	private int[] grvboSpherePosition23 = new int[1];
	private int[] grvboSphereNormal23 = new int[1];
	private int[] grvboSphereElement23 = new int[1];
	private int[] grvaoSphere24 = new int[1];
	private int[] grvboSpherePosition24 = new int[1];
	private int[] grvboSphereNormal24 = new int[1];
	private int[] grvboSphereElement24 = new int[1];

	// material for 24 spheres
	private float grka1[] = new float[4];
	private float grkd1[] = new float[4];
	private float grks1[] = new float[4];
	private float grmaterialShinyness1;
	private float grka2[] = new float[4];
	private float grkd2[] = new float[4];
	private float grks2[] = new float[4];
	private float grmaterialShinyness2;
	private float grka3[] = new float[4];
	private float grkd3[] = new float[4];
	private float grks3[] = new float[4];
	private float grmaterialShinyness3;
	private float grka4[] = new float[4];
	private float grkd4[] = new float[4];
	private float grks4[] = new float[4];
	private float grmaterialShinyness4;
	private float grka5[] = new float[4];
	private float grkd5[] = new float[4];
	private float grks5[] = new float[4];
	private float grmaterialShinyness5;
	private float grka6[] = new float[4];
	private float grkd6[] = new float[4];
	private float grks6[] = new float[4];
	private float grmaterialShinyness6;
	private float grka7[] = new float[4];
	private float grkd7[] = new float[4];
	private float grks7[] = new float[4];
	private float grmaterialShinyness7;
	private float grka8[] = new float[4];
	private float grkd8[] = new float[4];
	private float grks8[] = new float[4];
	private float grmaterialShinyness8;
	private float grka9[] = new float[4];
	private float grkd9[] = new float[4];
	private float grks9[] = new float[4];
	private float grmaterialShinyness9;
	private float grka10[] = new float[4];
	private float grkd10[] = new float[4];
	private float grks10[] = new float[4];
	private float grmaterialShinyness10;
	private float grka11[] = new float[4];
	private float grkd11[] = new float[4];
	private float grks11[] = new float[4];
	private float grmaterialShinyness11;
	private float grka12[] = new float[4];
	private float grkd12[] = new float[4];
	private float grks12[] = new float[4];
	private float grmaterialShinyness12;
	private float grka13[] = new float[4];
	private float grkd13[] = new float[4];
	private float grks13[] = new float[4];
	private float grmaterialShinyness13;
	private float grka14[] = new float[4];
	private float grkd14[] = new float[4];
	private float grks14[] = new float[4];
	private float grmaterialShinyness14;
	private float grka15[] = new float[4];
	private float grkd15[] = new float[4];
	private float grks15[] = new float[4];
	private float grmaterialShinyness15;
	private float grka16[] = new float[4];
	private float grkd16[] = new float[4];
	private float grks16[] = new float[4];
	private float grmaterialShinyness16;
	private float grka17[] = new float[4];
	private float grkd17[] = new float[4];
	private float grks17[] = new float[4];
	private float grmaterialShinyness17;
	private float grka18[] = new float[4];
	private float grkd18[] = new float[4];
	private float grks18[] = new float[4];
	private float grmaterialShinyness18;
	private float grka19[] = new float[4];
	private float grkd19[] = new float[4];
	private float grks19[] = new float[4];
	private float grmaterialShinyness19;
	private float grka20[] = new float[4];
	private float grkd20[] = new float[4];
	private float grks20[] = new float[4];
	private float grmaterialShinyness20;
	private float grka21[] = new float[4];
	private float grkd21[] = new float[4];
	private float grks21[] = new float[4];
	private float grmaterialShinyness21;
	private float grka22[] = new float[4];
	private float grkd22[] = new float[4];
	private float grks22[] = new float[4];
	private float grmaterialShinyness22;
	private float grka23[] = new float[4];
	private float grkd23[] = new float[4];
	private float grks23[] = new float[4];
	private float grmaterialShinyness23;
	private float grka24[] = new float[4];
	private float grkd24[] = new float[4];
	private float grks24[] = new float[4];
	private float grmaterialShinyness24;



	int i;
	boolean grbLight = false;
	private boolean grisPerVertexLight = true;
	private boolean grX = false;
	private boolean grY = false;
	private boolean grZ = false;
	private float grangle = 0.0f;
	private int grleft, grright, grtop, grbottom;
	private int x,y;
	private int grcount = 0;
	private float grperspectiveMatrix[] = new float[16];

	public GLESView2(Context drawingContext)
	{
		super(drawingContext);
		grcontext = drawingContext;

		// set EGLContext to current supported version of opengl
		setEGLContextClientVersion(3);
		// set rendered for drawing on GLSurfaceView
		setRenderer(this);

		// render the view when there is change in drawing data
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

		// gestureDetector is same as handler (who is going to handle events)
		grgestureDetector = new GestureDetector(grcontext, this, null, false);
		grgestureDetector.setOnDoubleTapListener(this);
	}

	// overridden method of GLSurfaceView.Renderer
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		// Opengles version check
		String version = gl.glGetString(GL10.GL_VERSION);
		String glslVersion = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
		System.out.println("OGL_GR : OGLES version : " + version);
		System.out.println("OGL_GR : GLSL Vrsion : " + glslVersion);

		initialize(gl);
	}

	// overridden method of GLSurfaceView.Renderer
	@Override
	public void onSurfaceChanged(GL10 unused, int width, int height)
	{
		System.out.println("OGL_GR : in onSurfaceChanged");
		resize(width, height);
	}

	// overridden method of GLSurfaceView.Renderer
	@Override
	public void onDrawFrame(GL10 unused)
	{
		display();
	}

	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		int eventAction = event.getAction();
		if(!grgestureDetector.onTouchEvent(event))
		{
			super.onTouchEvent(event);
		}
		return(true);
	}

	@Override
	public boolean onDoubleTap(MotionEvent e)
	{
		grisPerVertexLight = !grisPerVertexLight;
		return(true);
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent e)
	{
		return(true);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e) {
		grcount++;
		if (grcount == 1) {
			grX = true;
			grY = false;
			grZ = false;
		}

		if (grcount == 2)
		{
			grY = true;
			grX = false;
			grZ = false;
		}

		if(grcount == 3)
		{
			grZ = true;
			grX = false;
			grY = false;
		}

		if(grcount > 3)
			grcount = 0;

		if(grcount == 0)
		{
			grX = false;
			grY = false;
			grZ = false;
		}


		System.out.println("OGL_GR : count : " + grcount);
		return(true);
	}

	@Override
	public boolean onDown(MotionEvent e)
	{
		return(true);
	}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2,
		float distanceX, float distanceY)
	{
		return(true);
	}

	@Override
	public void onLongPress(MotionEvent e)
	{
		grbLight = !grbLight;
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2,
		float distanceX, float distanceY)
	{
		// exit the application if scrolled
		uninitialize();
		System.exit(0);
		return(true);
	}

	@Override
	public void onShowPress(MotionEvent e)
	{
	}

	@Override
	public boolean onSingleTapUp(MotionEvent e)
	{
		return(true);
	}

	// user defined methods

	private void initialize(GL10 gl)
	{
		System.out.println("OGL_GR : in initialize");

		//************************************************** Per Fragment - shaders **************************************
		//****************************************************************************************************************
		// Vertex Shader
		//create shader
		grvertexShaderObjectPF = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		// vertex shader source code
		final String grvertexShaderSourceCode = String.format(
				"#version 320 es" +
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
				"}"
		);

		// provide source code to shader
		GLES32.glShaderSource(grvertexShaderObjectPF, grvertexShaderSourceCode);

		// compile shader and check for error
		GLES32.glCompileShader(grvertexShaderObjectPF);
		int[] griShaderCompileStatus = new int[1];
		int[] griInfoLength = new int[1];
		String grszInfoLog = null;

		GLES32.glGetShaderiv(grvertexShaderObjectPF, GLES32.GL_COMPILE_STATUS,
			griShaderCompileStatus, 0);
		if(griShaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(grvertexShaderObjectPF, GLES32.GL_INFO_LOG_LENGTH,
				griInfoLength, 0);

			if(griInfoLength[0] > 0)
			{
				grszInfoLog = GLES32.glGetShaderInfoLog(grvertexShaderObjectPF);
				System.out.println("OGL_GR : PF Vertex shader compilation log : " + grszInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		////////// Fragment shader
		// create shader
		grfragmentShaderObjectPF = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		// fragment shader source code
		final String grfragmentShaderSourceCode = String.format(
				"#version 320 es" +
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
				"uniform int u_l_key_pressed;" +
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
				"}"
		);

		// provide source code to shader
		GLES32.glShaderSource(grfragmentShaderObjectPF, grfragmentShaderSourceCode);

		// compile shader
		GLES32.glCompileShader(grfragmentShaderObjectPF);
		griShaderCompileStatus[0] = 0;
		griInfoLength[0] = 0;
		grszInfoLog = null;
		GLES32.glGetShaderiv(grfragmentShaderObjectPF, GLES32.GL_COMPILE_STATUS,
			griShaderCompileStatus, 0);
		if(griShaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(grfragmentShaderObjectPF, GLES32.GL_INFO_LOG_LENGTH, griInfoLength, 0);

			if(griInfoLength[0] > 0)
			{
				grszInfoLog = GLES32.glGetShaderInfoLog(grfragmentShaderObjectPF);
				System.out.println("OGL_GR : PF Fragment shader compilation log : " + grszInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		// create shader program
		grshaderProgramObjectPF = GLES32.glCreateProgram();

		// attach vertex shader to shader program
		GLES32.glAttachShader(grshaderProgramObjectPF, grvertexShaderObjectPF);

		// attach fragment shader to shader program
		GLES32.glAttachShader(grshaderProgramObjectPF, grfragmentShaderObjectPF);

		// prelink binding of shader program object with vertex shader attributes
		GLES32.glBindAttribLocation(grshaderProgramObjectPF, GLESMacros.GR_ATTRIBUTE_POSITION,
			"vPosition");
		GLES32.glBindAttribLocation(grshaderProgramObjectPF, GLESMacros.GR_ATTRIBUTE_NORMAL,
			"vNormal");

		// link
		GLES32.glLinkProgram(grshaderProgramObjectPF);
		int[] griShaderProgramLinkStatus = new int[1];
		griInfoLength[0] = 0;
		grszInfoLog = null;
		GLES32.glGetProgramiv(grshaderProgramObjectPF, GLES32.GL_LINK_STATUS,
			griShaderProgramLinkStatus, 0);
		if(griShaderProgramLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(grshaderProgramObjectPF, GLES32.GL_INFO_LOG_LENGTH,
				griInfoLength, 0);

			if(griInfoLength[0] > 0)
			{
				grszInfoLog = GLES32.glGetProgramInfoLog(grshaderProgramObjectPF);
				System.out.println("OGL_GR : PF Shader program link log : " + grszInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		// get uniform location
		grmodelMatrixUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_model_matrix");
		grviewMatrixUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_view_matrix");
		grprojectionMatrixUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_projection_matrix");

		grlaUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_la");
		grldUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_ld");
		grlsUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_ls");
		grlightPositionUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_light_position");
		grkaUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_ka");
		grkdUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_kd");
		grksUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_ks");

		grlKeyPressedUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_l_key_pressed");
		grmaterialShinynessUniformPF = GLES32.glGetUniformLocation(grshaderProgramObjectPF, "u_material_shinyness");

		//************************************************** Per Fragment - shaders **************************************
		//****************************************************************************************************************
		// Vertex Shader
		//create shader
		System.out.println("OGL_GR : In initialize - per vertex shaders");
		grvertexShaderObjectPV = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		// vertex shader source code
		final String grvertexShaderSourceCodePV = String.format(
						"#version 320 es" +
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
						"}"
		);

		// provide source code to shader
		GLES32.glShaderSource(grvertexShaderObjectPV, grvertexShaderSourceCodePV);

		// compile shader and check for error
		GLES32.glCompileShader(grvertexShaderObjectPV);
		griShaderCompileStatus[0] = 0;
		griInfoLength[0] = 0;
		grszInfoLog = null;

		GLES32.glGetShaderiv(grvertexShaderObjectPV, GLES32.GL_COMPILE_STATUS,
				griShaderCompileStatus, 0);
		if(griShaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(grvertexShaderObjectPV, GLES32.GL_INFO_LOG_LENGTH,
					griInfoLength, 0);

			if(griInfoLength[0] > 0)
			{
				grszInfoLog = GLES32.glGetShaderInfoLog(grvertexShaderObjectPV);
				System.out.println("OGL_GR : PV Vertex shader compilation log : " + grszInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		////////// Fragment shader
		// create shader
		grfragmentShaderObjectPV = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		// fragment shader source code
		final String grfragmentShaderSourceCodePV = String.format(
						"#version 320 es" +
						"\n" +
						"precision highp float;" +
						"in vec3 out_phong_light;" +
						"out vec4 FragColor;" +
						"void main(void)" +
						"{" +
						"FragColor = vec4(out_phong_light, 1.0);" +
						"}"
		);

		// provide source code to shader
		GLES32.glShaderSource(grfragmentShaderObjectPV, grfragmentShaderSourceCodePV);

		// compile shader
		GLES32.glCompileShader(grfragmentShaderObjectPV);
		griShaderCompileStatus[0] = 0;
		griInfoLength[0] = 0;
		grszInfoLog = null;
		GLES32.glGetShaderiv(grfragmentShaderObjectPV, GLES32.GL_COMPILE_STATUS,
				griShaderCompileStatus, 0);
		if(griShaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(grfragmentShaderObjectPV, GLES32.GL_INFO_LOG_LENGTH, griInfoLength, 0);

			if(griInfoLength[0] > 0)
			{
				grszInfoLog = GLES32.glGetShaderInfoLog(grfragmentShaderObjectPV);
				System.out.println("OGL_GR : PV Fragment shader compilation log : " + grszInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		// create shader program
		grshaderProgramObjectPV = GLES32.glCreateProgram();

		// attach vertex shader to shader program
		GLES32.glAttachShader(grshaderProgramObjectPV, grvertexShaderObjectPV);

		// attach fragment shader to shader program
		GLES32.glAttachShader(grshaderProgramObjectPV, grfragmentShaderObjectPV);

		// prelink binding of shader program object with vertex shader attributes
		GLES32.glBindAttribLocation(grshaderProgramObjectPV, GLESMacros.GR_ATTRIBUTE_POSITION,
				"vPosition");
		GLES32.glBindAttribLocation(grshaderProgramObjectPV, GLESMacros.GR_ATTRIBUTE_NORMAL,
				"vNormal");

		// link
		GLES32.glLinkProgram(grshaderProgramObjectPV);
		griShaderProgramLinkStatus = new int[1];
		griInfoLength[0] = 0;
		grszInfoLog = null;
		GLES32.glGetProgramiv(grshaderProgramObjectPV, GLES32.GL_LINK_STATUS,
				griShaderProgramLinkStatus, 0);
		if(griShaderProgramLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(grshaderProgramObjectPV, GLES32.GL_INFO_LOG_LENGTH,
					griInfoLength, 0);

			if(griInfoLength[0] > 0)
			{
				grszInfoLog = GLES32.glGetProgramInfoLog(grshaderProgramObjectPV);
				System.out.println("OGL_GR : Shader program link log : " + grszInfoLog);
				uninitialize();
				System.exit(0);
			}
		}
		System.out.println("OGL_GR : PV before uniform location");
		// get uniform location
		grmodelMatrixUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_model_matrix");
		grviewMatrixUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_view_matrix");
		grprojectionMatrixUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_projection_matrix");

		grlaUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_la");
		grldUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_ld");
		grlsUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_ls");
		grlightPositionUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_light_position");
		grkaUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_ka");
		grkdUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_kd");
		grksUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_ks");

		grlKeyPressedUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_l_key_pressed");
		grmaterialShinynessUniformPV = GLES32.glGetUniformLocation(grshaderProgramObjectPV, "u_material_shinyness");
		System.out.println("OGL_GR : PV after uniform location");

		// initialize materials for 24 spheres
		initialize24Spheres();
		//***************** Initialize vao and vbo of 24 spheres
		initializeVaoVbo();

		x = 140;
		y = 30;
		// enable depth testing
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glEnable(GLES32.GL_LEQUAL);
		GLES32.glClearDepthf(1.0f);

		// set background color
		GLES32.glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

		// set projection matrix to identity matrix
		Matrix.setIdentityM(grperspectiveMatrix, 0);
		System.out.println("OGL_GR : initialize end");

	}

	private void resize(int width, int height)
	{
		GLES32.glViewport(0, 71, width, height - 71);
		grleft = 0;
		grright = width/4;
		grtop = 0;
		grbottom = height/6;
		System.out.println("x : " + grleft + " y : " + grtop + " width : " + grright + " height : " + grbottom);
		Matrix.perspectiveM(grperspectiveMatrix, 0, 45.0f, (float) width / (float) height, 0.1f, 100.0f);
	}

	private void display()
	{
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		float[] grmodelMatrix = new float[16];
		float[] grviewMatrix = new float[16];
		float[] grprojectionMatrix = new float[16];
		float[] grtranslateMatrix = new float[16];
		float[] grrotateMatrix = new float[16];
		float[] grscaleMatrix = new float[16];

		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grrotateMatrix, 0);
		Matrix.setIdentityM(grscaleMatrix, 0);
        Matrix.setIdentityM(grmodelMatrix, 0);
		Matrix.setIdentityM(grviewMatrix, 0);
		Matrix.setIdentityM(grprojectionMatrix, 0);

		if(grisPerVertexLight)
		{
			GLES32.glUseProgram(grshaderProgramObjectPV);
			Matrix.translateM(grtranslateMatrix, 0, 0.0f, 0.0f, -4.0f);
			Matrix.multiplyMM(grmodelMatrix, 0, grmodelMatrix, 0, grtranslateMatrix, 0);
			Matrix.multiplyMM(grprojectionMatrix, 0, grprojectionMatrix, 0, grperspectiveMatrix, 0);

			// Sphere 1
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd1, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka1, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks1, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness1);
				if(grX == true)
				{
					grlightPosition[0] = 0.0f;
					grlightPosition[1] = (float) (21.0 * Math.sin(grangle));
					grlightPosition[2] = (float) (21.0 * Math.cos(grangle));
					grlightPosition[3] = 0.0f;									// light direction - 1 for positional light
				}
				else if(grY == true)
				{
					grlightPosition[0] = (float) (20.0 * Math.sin(grangle));
					grlightPosition[1] = 0.0f;
					grlightPosition[2] = (float) (20.0 * Math.cos(grangle));
					grlightPosition[3] = 0.0f;
				}
				else if(grZ == true)
				{
					grlightPosition[0] = (float) (20.0 * Math.sin(grangle));
					grlightPosition[1] = (float) (20.0 * Math.cos(grangle));
					grlightPosition[2] = 0.0f;
					grlightPosition[3] = 0.0f;
				}

				GLES32.glUniform3fv(grldUniformPV, 1, grld, 0);
				GLES32.glUniform3fv(grlaUniformPV, 1, grla, 0);
				GLES32.glUniform3fv(grlsUniformPV, 1, grls, 0);
				GLES32.glUniform4fv(grlightPositionUniformPV, 1, grlightPosition, 0);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grleft, grtop - (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere1[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement1[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 2
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd2, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka2, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks2, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness2);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright, grtop- (x/ 2), grright, grbottom + x);
			//System.out.println("2nd sphere x : " + grright + " y : " + grtop + " width : " + grright + " height : " + grbottom);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere2[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement2[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 3
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd3, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka3, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks3, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness3);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 2, grtop- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere3[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement3[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 4
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd4, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka4, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks4, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness4);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 3, grtop- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere4[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement4[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 5
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd5, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka5, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks5, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness5);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}

			GLES32.glViewport(grleft, grbottom * 5 - (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere5[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement5[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 6
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd6, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka6, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks6, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness6);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright, grbottom * 5- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere6[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement6[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 7
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd7, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka7, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks7, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness7);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 2, grbottom * 5- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere7[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement7[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 8
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd8, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka8, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks8, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness8);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 3, grbottom * 5- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere8[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement8[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 9
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd9, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka9, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks9, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness9);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grleft, grbottom * 4- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere9[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement9[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 10
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd10, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka10, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks10, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness10);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright, grbottom * 4- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere10[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement10[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 11
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd11, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka11, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks11, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness11);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 2, grbottom * 4- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere11[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement11[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 12
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd12, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka12, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks12, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness12);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 3, grbottom * 4- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere12[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement12[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 13
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd13, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka13, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks13, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness13);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grleft, grbottom * 3- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere13[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement13[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 14
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd14, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka14, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks14, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness14);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright, grbottom * 3- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere14[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement14[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 15
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd15, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka15, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks15, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness15);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 2, grbottom * 3- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere15[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement15[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 16
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd16, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka16, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks16, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness16);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 3, grbottom * 3- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere16[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement16[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 17
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd17, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka17, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks17, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness17);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grleft, grbottom * 2- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere17[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement17[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 18
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd18, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka18, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks18, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness18);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright, grbottom * 2- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere18[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement18[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 19
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd19, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka19, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks19, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness19);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 2, grbottom * 2- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere19[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement19[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 20
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd20, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka20, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks20, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness20);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 3, grbottom * 2- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere20[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement20[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 21
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd21, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka21, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks21, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness21);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grleft, grbottom - (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere20[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement20[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 22
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd22, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka22, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks22, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness22);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright, grbottom- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere22[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement22[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 23
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd23, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka23, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks23, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness23);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 2, grbottom- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere23[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement23[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 24
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 1);
				GLES32.glUniform3fv(grkdUniformPV, 1, grkd24, 0);
				GLES32.glUniform3fv(grkaUniformPV, 1, grka24, 0);
				GLES32.glUniform3fv(grksUniformPV, 1, grks24, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPV, grmaterialShinyness24);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPV, 0);
			}
			GLES32.glViewport(grright * 3, grbottom- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPV, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPV, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPV, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere24[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement24[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

		}
		else
		{
			GLES32.glUseProgram(grshaderProgramObjectPF);
			Matrix.setIdentityM(grtranslateMatrix, 0);
			Matrix.setIdentityM(grmodelMatrix, 0);
			Matrix.setIdentityM(grprojectionMatrix, 0);

			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd1, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka1, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks1, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness1);

				if(grX == true)
				{
					grlightPosition[0] = 0.0f;
					grlightPosition[1] = (float) (21.0 * Math.sin(grangle));
					grlightPosition[2] = (float) (21.0 * Math.cos(grangle));
					grlightPosition[3] = 0.0f;									// light direction - 1 for positional light
				}
				else if(grY == true)
				{
					grlightPosition[0] = (float) (20.0 * Math.sin(grangle));
					grlightPosition[1] = 0.0f;
					grlightPosition[2] = (float) (20.0 * Math.cos(grangle));
					grlightPosition[3] = 0.0f;
				}
				else if(grZ == true)
				{
					grlightPosition[0] = (float) (20.0 * Math.sin(grangle));
					grlightPosition[1] = (float) (20.0 * Math.cos(grangle));
					grlightPosition[2] = 0.0f;
					grlightPosition[3] = 0.0f;
				}

				GLES32.glUniform3fv(grldUniformPF, 1, grld, 0);
				GLES32.glUniform3fv(grlaUniformPF, 1, grla, 0);
				GLES32.glUniform3fv(grlsUniformPF, 1, grls, 0);
				GLES32.glUniform4fv(grlightPositionUniformPF, 1, grlightPosition, 0);

			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			Matrix.translateM(grtranslateMatrix, 0, 0.0f, 0.0f, -4.0f);
			Matrix.multiplyMM(grmodelMatrix, 0, grmodelMatrix, 0, grtranslateMatrix, 0);
			Matrix.multiplyMM(grprojectionMatrix, 0, grprojectionMatrix, 0, grperspectiveMatrix, 0);
			GLES32.glViewport(grleft, grtop - (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere1[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement1[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 2
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd2, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka2, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks2, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness2);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glUniform3fv(grldUniformPF, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniformPF, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniformPF, 1, grls, 0);
			GLES32.glUniform4fv(grlightPositionUniformPF, 1, grlightPosition, 0);
			GLES32.glViewport(grright, grtop- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere2[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement2[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 3
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd3, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka3, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks3, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness3);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glUniform3fv(grldUniformPF, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniformPF, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniformPF, 1, grls, 0);
			GLES32.glUniform4fv(grlightPositionUniformPF, 1, grlightPosition, 0);
			GLES32.glViewport(grright * 2, grtop- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere3[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement3[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 4
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd4, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka4, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks4, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness4);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glUniform3fv(grldUniformPF, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniformPF, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniformPF, 1, grls, 0);
			GLES32.glUniform4fv(grlightPositionUniformPF, 1, grlightPosition, 0);
			GLES32.glViewport(grright * 3, grtop- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere4[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement4[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 5
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd5, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka5, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks5, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness5);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grleft, grbottom * 5 - (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere5[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement5[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 6
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd6, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka6, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks6, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness6);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright, grbottom * 5- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere6[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement6[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 7
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd7, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka7, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks7, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness7);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright * 2, grbottom * 5- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere7[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement7[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 8
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd8, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka8, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks8, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness8);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright * 3, grbottom * 5- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere8[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement8[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 9
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd9, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka9, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks9, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness9);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grleft, grbottom * 4- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere9[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement9[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 10
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd10, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka10, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks10, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness10);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright, grbottom * 4- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere10[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement10[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 11
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd11, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka11, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks11, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness11);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright * 2, grbottom * 4- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere11[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement11[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 12
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd12, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka12, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks12, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness12);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright * 3, grbottom * 4- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere12[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement12[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 13
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd13, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka13, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks13, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness13);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grleft, grbottom * 3- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere13[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement13[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 14
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd14, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka14, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks14, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness14);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright, grbottom * 3- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere14[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement14[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 15
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd15, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka15, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks15, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness15);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright * 2, grbottom * 3- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere15[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement15[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 16
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd16, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka16, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks16, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness16);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright * 3, grbottom * 3- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere16[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement16[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 17
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd17, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka17, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks17, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness17);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grleft, grbottom * 2- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere17[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement17[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 18
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd18, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka18, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks18, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness18);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright, grbottom * 2- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere18[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement18[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 19
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd19, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka19, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks19, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness19);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright * 2, grbottom * 2- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere19[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement19[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 20
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd20, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka20, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks20, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness20);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright * 3, grbottom * 2- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere20[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement20[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 21
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd21, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka21, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks21, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness21);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grleft, grbottom - (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere20[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement20[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 22
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd22, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka22, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks22, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness22);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright, grbottom- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere22[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement22[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 23
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd23, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka23, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks23, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness23);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright * 2, grbottom- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere23[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement23[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

			// sphere 24
			if(grbLight == true)
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 1);
				GLES32.glUniform3fv(grkdUniformPF, 1, grkd24, 0);
				GLES32.glUniform3fv(grkaUniformPF, 1, grka24, 0);
				GLES32.glUniform3fv(grksUniformPF, 1, grks24, 0);
				GLES32.glUniform1f(grmaterialShinynessUniformPF, grmaterialShinyness24);
			}
			else
			{
				GLES32.glUniform1i(grlKeyPressedUniformPF, 0);
			}
			GLES32.glViewport(grright * 3, grbottom- (x/ 2), grright, grbottom + x);
			GLES32.glUniformMatrix4fv(grmodelMatrixUniformPF, 1, false, grmodelMatrix, 0);
			GLES32.glUniformMatrix4fv(grviewMatrixUniformPF, 1, false, grviewMatrix, 0);
			GLES32.glUniformMatrix4fv(grprojectionMatrixUniformPF, 1, false, grprojectionMatrix, 0);

			GLES32.glBindVertexArray(grvaoSphere24[0]);
			GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement24[0]);
			GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
			GLES32.glBindVertexArray(0);

		}

		GLES32.glUseProgram(0);
		update();
		requestRender();
	}

	void update()
	{
		grangle = grangle + 0.05f;
		if(grangle >= 360.0f)
			grangle = 0.0f;
	}

	void initialize24Spheres()
	{
		grka1 = new float[] {0.0215f, 0.1745f, 0.0215f, 1.0f};
		grkd1 = new float[] {0.07568f, 0.61424f, 0.07568f, 1.0f};
		grks1 = new float[] {0.633f, 0.727811f, 0.633f, 1.0f};
		grmaterialShinyness1 = 0.6f * 128.0f;

		grka2 = new float[] {0.135f, 0.2225f, 0.1575f, 1.0f};
		grkd2 = new float[] {0.54f, 0.89f, 0.63f, 1.0f};
		grks2 = new float[] {0.316228f,0.316228f, 0.316228f, 1.0f};
		grmaterialShinyness2 = 0.1f * 128.0f;

		grka3 = new float[] {0.05375f, 0.05f, 0.06625f, 1.0f};
		grkd3 = new float[] {0.18275f, 0.17f, 0.22525f, 1.0f};
		grks3 = new float[] {0.332741f, 0.328634f, 0.346435f, 1.0f};
		grmaterialShinyness3 = 0.3f * 128.0f;

		grka4 = new float[] {0.25f, 0.20725f, 0.20725f, 1.0f};
		grkd4 = new float[] {1.0f, 0.829f, 0.829f, 1.0f};
		grks4 = new float[] {0.296648f, 0.296648f, 0.296648f, 1.0f};
		grmaterialShinyness4 = 0.088f * 128.0f;

		grka5 = new float[] {0.1745f, 0.01175f, 0.01175f, 1.0f};
		grkd5 = new float[] {0.61424f, 0.04136f, 0.04136f, 1.0f};
		grks5 = new float[] {0.727811f, 0.626959f, 0.626959f, 1.0f};
		grmaterialShinyness5 = 0.6f * 128.0f;

		grka6 = new float[] {0.1f, 0.18725f, 0.1745f, 1.0f};
		grkd6 = new float[] {0.396f, 0.74151f, 0.69102f, 1.0f};
		grks6 = new float[] {0.297254f, 0.30829f, 0.306678f, 1.0f};
		grmaterialShinyness6 = 0.1f * 128.0f;

		grka7 = new float[] {0.329412f, 0.223529f, 0.027451f, 1.0f};
		grkd7 = new float[] {0.780392f, 0.568627f, 0.113725f, 1.0f};
		grks7 = new float[] {0.992157f, 0.941176f, 0.807843f, 1.0f};
		grmaterialShinyness7 =  0.21794872f * 128.0f;

		grka8 = new float[] {0.2125f, 0.1275f, 0.054f, 1.0f};
		grkd8 = new float[] {0.714f, 0.4284f, 0.18144f, 1.0f};
		grks8 = new float[] {0.393548f, 0.271906f, 0.166721f, 1.0f};
		grmaterialShinyness8 = 0.2f * 128.0f;

		grka9 = new float[] {0.25f, 0.25f, 0.25f, 1.0f};
		grkd9 = new float[] {0.4f, 0.4f, 0.4f, 1.0f};
		grks9 = new float[] {0.774597f, 0.774597f, 0.774597f, 1.0f};
		grmaterialShinyness9 = 0.6f * 128.0f;

		grka10 = new float[] {0.19125f, 0.0735f, 0.0225f, 1.0f};
		grkd10 = new float[] {0.7038f, 0.27048f, 0.0828f, 1.0f};
		grks10 = new float[] {0.256777f, 0.137622f, 0.086014f, 1.0f};
		grmaterialShinyness10 = 0.1f * 128.0f;

		grka11 = new float[] {0.24725f, 0.1995f, 0.0745f, 1.0f};
		grkd11 = new float[] {0.75164f, 0.60648f, 0.22648f, 1.0f};
		grks11 = new float[] {0.628281f, 0.555802f, 0.366065f, 1.0f};
		grmaterialShinyness11 = 0.4f * 128.0f;

		grka12 = new float[] {0.19225f, 0.19225f, 0.19225f, 1.0f};
		grkd12 = new float[] {0.50754f, 0.50754f, 0.50754f, 1.0f};
		grks12 = new float[] {0.508273f, 0.508273f, 0.508273f, 1.0f};
		grmaterialShinyness12 = 0.4f * 128.0f;

		grka13 = new float[] {0.0f, 0.0f, 0.0f, 1.0f};
		grkd13 = new float[] {0.01f, 0.01f, 0.01f, 1.0f};
		grks13 = new float[] {0.50f, 0.50f, 0.50f, 1.0f};
		grmaterialShinyness13 = 0.25f * 128.0f;

		grka14 = new float[] {0.0f, 0.1f, 0.06f, 1.0f};
		grkd14 = new float[] {0.0f, 0.50980392f, 0.50980392f, 1.0f};
		grks14 = new float[] {0.50196078f, 0.50196078f, 0.50196078f, 1.0f};
		grmaterialShinyness14 = 0.25f * 128.0f;

		grka15 = new float[] {0.0f, 0.0f, 0.0f, 1.0f};
		grkd15 = new float[] {0.1f, 0.35f, 0.1f, 1.0f};
		grks15 = new float[] {0.45f, 0.55f, 0.45f, 1.0f};
		grmaterialShinyness15 = 0.25f * 128.0f;

		grka16 = new float[] {0.0f, 0.0f, 0.0f, 1.0f};
		grkd16 = new float[] {0.5f, 0.0f, 0.0f, 1.0f};
		grks16 = new float[] {0.7f, 0.6f, 0.6f, 1.0f};
		grmaterialShinyness16 = 0.25f * 128.0f;

		grka17 = new float[] {0.0f, 0.0f, 0.0f, 1.0f};
		grkd17 = new float[] {0.55f, 0.55f, 0.55f, 1.0f};
		grks17 = new float[] {0.70f, 0.70f, 0.70f, 1.0f};
		grmaterialShinyness17 = 0.25f * 128.0f;

		grka18 = new float[] {0.0f, 0.0f, 0.0f, 1.0f};
		grkd18 = new float[] {0.5f, 0.5f, 0.0f, 1.0f};
		grks18 = new float[] {0.60f, 0.60f, 0.50f, 1.0f};
		grmaterialShinyness18 = 0.25f * 128.0f;

		grka19 = new float[] {0.02f, 0.02f, 0.02f, 1.0f};
		grkd19 = new float[] {0.01f, 0.01f, 0.01f, 1.0f};
		grks19 = new float[] {0.4f, 0.4f, 0.4f, 1.0f};
		grmaterialShinyness19 = 0.078125f * 128.0f;

		grka20 = new float[] {0.0f, 0.05f, 0.05f, 1.0f};
		grkd20 = new float[] {0.4f, 0.5f, 0.5f, 1.0f};
		grks20 = new float[] {0.04f, 0.7f, 0.7f, 1.0f};
		grmaterialShinyness20 = 0.078125f * 128.0f;

		grka21 = new float[] {0.0f, 0.05f, 0.0f, 1.0f};
		grkd21 = new float[] {0.4f, 0.5f, 0.4f, 1.0f};
		grks21 = new float[] {0.04f, 0.7f, 0.04f, 1.0f};
		grmaterialShinyness21 = 0.078125f * 128.0f;

		grka22 = new float[] {0.05f, 0.0f, 0.0f, 1.0f};
		grkd22 = new float[] {0.5f, 0.4f, 0.4f, 1.0f};
		grks22 = new float[] {0.7f, 0.04f, 0.04f, 1.0f};
		grmaterialShinyness22 = 0.078125f * 128.0f;

		grka23 = new float[] {0.05f, 0.05f, 0.05f, 1.0f};
		grkd23 = new float[] {0.5f, 0.5f, 0.5f, 1.0f};
		grks23 = new float[] {0.7f, 0.7f, 0.7f, 1.0f};
		grmaterialShinyness23 = 0.078125f * 128.0f;

		grka24 = new float[] {0.05f, 0.05f, 0.0f, 1.0f};
		grkd24 = new float[] {0.5f, 0.5f, 0.4f, 1.0f};
		grks24 = new float[] {0.7f, 0.7f, 0.04f, 1.0f};
		grmaterialShinyness24 = 0.078125f * 128.0f;

	}

	void initializeVaoVbo()
	{
		Sphere grsphere = new Sphere();
		float grsphereVertices[] = new float[1146];
		float grsphereNormals[] = new float[1146];
		float grsphereTextures[] = new float[764];
		short grsphereElements[] = new short[2280];
		grsphere.getSphereVertexData(grsphereVertices, grsphereNormals, grsphereTextures, grsphereElements);
		grnumVertices = grsphere.getNumberOfSphereVertices();
		grnumElements = grsphere.getNumberOfSphereElements();
		System.out.println("OGL_GR : in initialize, after sphere elements, b4 shaders");

		// 1
		GLES32.glGenVertexArrays(1, grvaoSphere1, 0);
		GLES32.glBindVertexArray(grvaoSphere1[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition1, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition1[0]);
		ByteBuffer grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal1, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal1[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement1, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement1[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		ShortBuffer grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		// 2
		GLES32.glGenVertexArrays(1, grvaoSphere2, 0);
		GLES32.glBindVertexArray(grvaoSphere2[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition2, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition2[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal2, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal2[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement2, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement2[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//3
		GLES32.glGenVertexArrays(1, grvaoSphere3, 0);
		GLES32.glBindVertexArray(grvaoSphere3[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition3, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition3[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal3, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal3[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement3, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement3[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		// 4
		GLES32.glGenVertexArrays(1, grvaoSphere4, 0);
		GLES32.glBindVertexArray(grvaoSphere4[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition4, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition4[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal4, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal4[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement4, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement4[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		// 5
		GLES32.glGenVertexArrays(1, grvaoSphere5, 0);
		GLES32.glBindVertexArray(grvaoSphere5[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition5, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition5[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal5, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal5[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement5, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement5[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		// 6
		GLES32.glGenVertexArrays(1, grvaoSphere6, 0);
		GLES32.glBindVertexArray(grvaoSphere6[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition6, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition6[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal6, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal6[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement6, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement6[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		// 7
		GLES32.glGenVertexArrays(1, grvaoSphere7, 0);
		GLES32.glBindVertexArray(grvaoSphere7[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition7, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition7[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal7, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal7[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement7, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement7[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		// 8
		GLES32.glGenVertexArrays(1, grvaoSphere8, 0);
		GLES32.glBindVertexArray(grvaoSphere8[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition8, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition8[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal8, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal8[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement8, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement8[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		// 9
		GLES32.glGenVertexArrays(1, grvaoSphere9, 0);
		GLES32.glBindVertexArray(grvaoSphere9[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition9, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition9[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal9, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal9[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement9, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement9[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		// 10
		GLES32.glGenVertexArrays(1, grvaoSphere10, 0);
		GLES32.glBindVertexArray(grvaoSphere10[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition10, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition10[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal10, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal10[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement10, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement10[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		// 11
		GLES32.glGenVertexArrays(1, grvaoSphere11, 0);
		GLES32.glBindVertexArray(grvaoSphere11[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition11, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition2[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal11, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal11[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement11, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement11[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//12
		GLES32.glGenVertexArrays(1, grvaoSphere12, 0);
		GLES32.glBindVertexArray(grvaoSphere12[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition12, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition12[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal12, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal12[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement12, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement12[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//13
		GLES32.glGenVertexArrays(1, grvaoSphere13, 0);
		GLES32.glBindVertexArray(grvaoSphere13[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition13, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition13[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal13, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal13[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement13, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement13[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//14
		GLES32.glGenVertexArrays(1, grvaoSphere14, 0);
		GLES32.glBindVertexArray(grvaoSphere14[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition14, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition14[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal14, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal14[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement14, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement14[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		// 15
		GLES32.glGenVertexArrays(1, grvaoSphere15, 0);
		GLES32.glBindVertexArray(grvaoSphere15[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition15, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition15[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal15, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal15[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement15, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement15[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//16
		GLES32.glGenVertexArrays(1, grvaoSphere16, 0);
		GLES32.glBindVertexArray(grvaoSphere16[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition16, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition16[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal16, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal16[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement16, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement16[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//17
		GLES32.glGenVertexArrays(1, grvaoSphere17, 0);
		GLES32.glBindVertexArray(grvaoSphere17[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition17, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition17[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal17, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal17[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement17, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement17[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//18
		GLES32.glGenVertexArrays(1, grvaoSphere18, 0);
		GLES32.glBindVertexArray(grvaoSphere18[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition18, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition18[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal18, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal18[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement18, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement18[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//19
		GLES32.glGenVertexArrays(1, grvaoSphere19, 0);
		GLES32.glBindVertexArray(grvaoSphere19[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition19, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition19[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal19, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal19[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement19, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement19[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//20
		GLES32.glGenVertexArrays(1, grvaoSphere20, 0);
		GLES32.glBindVertexArray(grvaoSphere20[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition20, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition20[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal20, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal20[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement20, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement20[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//21
		GLES32.glGenVertexArrays(1, grvaoSphere21, 0);
		GLES32.glBindVertexArray(grvaoSphere21[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition21, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition21[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal21, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal21[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement21, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement21[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//22
		GLES32.glGenVertexArrays(1, grvaoSphere22, 0);
		GLES32.glBindVertexArray(grvaoSphere22[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition22, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition22[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal22, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal22[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement22, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement22[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//23
		GLES32.glGenVertexArrays(1, grvaoSphere23, 0);
		GLES32.glBindVertexArray(grvaoSphere23[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition23, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition23[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal23, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal23[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement23, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement23[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		//24
		GLES32.glGenVertexArrays(1, grvaoSphere24, 0);
		GLES32.glBindVertexArray(grvaoSphere24[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition24, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition24[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereVertices.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereVertices);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereVertices.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo normal
		GLES32.glGenBuffers(1, grvboSphereNormal24, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal24[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereNormals.length * 4);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grverticesBuffer = grbyteBuffer.asFloatBuffer();
		grverticesBuffer.put(grsphereNormals);
		grverticesBuffer.position(0);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grsphereNormals.length * 4, grverticesBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_NORMAL);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		// vbo element
		GLES32.glGenBuffers(1, grvboSphereElement24, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement24[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);
	}

	void uninitialize()
	{
		// destroy vao
		if(grvaoSphere1[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere1, 0);
			grvaoSphere1[0] = 0;
		}

		if(grvboSpherePosition1[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition1, 0);
			grvboSpherePosition1[0] = 0;
		}

		if(grvboSphereNormal1[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal1, 0);
			grvboSphereNormal1[0] = 0;
		}
		// 2
		if(grvaoSphere2[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere2, 0);
			grvaoSphere2[0] = 0;
		}

		if(grvboSpherePosition2[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition2, 0);
			grvboSpherePosition2[0] = 0;
		}

		if(grvboSphereNormal2[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal2, 0);
			grvboSphereNormal2[0] = 0;
		}
		//3
		if(grvaoSphere3[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere3, 0);
			grvaoSphere3[0] = 0;
		}

		if(grvboSpherePosition3[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition3, 0);
			grvboSpherePosition3[0] = 0;
		}

		if(grvboSphereNormal3[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal3, 0);
			grvboSphereNormal3[0] = 0;
		}
		//4
		if(grvaoSphere4[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere4, 0);
			grvaoSphere4[0] = 0;
		}

		if(grvboSpherePosition4[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition4, 0);
			grvboSpherePosition4[0] = 0;
		}

		if(grvboSphereNormal4[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal4, 0);
			grvboSphereNormal4[0] = 0;
		}
		//5
		if(grvaoSphere5[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere5, 0);
			grvaoSphere5[0] = 0;
		}

		if(grvboSpherePosition5[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition5, 0);
			grvboSpherePosition5[0] = 0;
		}

		if(grvboSphereNormal5[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal5, 0);
			grvboSphereNormal5[0] = 0;
		}
		//6
		if(grvaoSphere6[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere6, 0);
			grvaoSphere6[0] = 0;
		}

		if(grvboSpherePosition6[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition6, 0);
			grvboSpherePosition6[0] = 0;
		}

		if(grvboSphereNormal6[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal6, 0);
			grvboSphereNormal6[0] = 0;
		}
		//7
		if(grvaoSphere7[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere7, 0);
			grvaoSphere7[0] = 0;
		}

		if(grvboSpherePosition7[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition7, 0);
			grvboSpherePosition7[0] = 0;
		}

		if(grvboSphereNormal7[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal7, 0);
			grvboSphereNormal7[0] = 0;
		}
		//8
		if(grvaoSphere8[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere8, 0);
			grvaoSphere8[0] = 0;
		}

		if(grvboSpherePosition8[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition8, 0);
			grvboSpherePosition8[0] = 0;
		}

		if(grvboSphereNormal8[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal8, 0);
			grvboSphereNormal8[0] = 0;
		}
		//9
		if(grvaoSphere9[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere9, 0);
			grvaoSphere9[0] = 0;
		}

		if(grvboSpherePosition9[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition9, 0);
			grvboSpherePosition9[0] = 0;
		}

		if(grvboSphereNormal9[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal9, 0);
			grvboSphereNormal9[0] = 0;
		}
		//10
		if(grvaoSphere10[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere10, 0);
			grvaoSphere10[0] = 0;
		}

		if(grvboSpherePosition10[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition10, 0);
			grvboSpherePosition10[0] = 0;
		}

		if(grvboSphereNormal10[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal10, 0);
			grvboSphereNormal10[0] = 0;
		}
		//11
		if(grvaoSphere11[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere11, 0);
			grvaoSphere11[0] = 0;
		}

		if(grvboSpherePosition11[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition11, 0);
			grvboSpherePosition11[0] = 0;
		}

		if(grvboSphereNormal11[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal11, 0);
			grvboSphereNormal11[0] = 0;
		}
		//12
		if(grvaoSphere1[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere12, 0);
			grvaoSphere12[0] = 0;
		}

		if(grvboSpherePosition12[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition12, 0);
			grvboSpherePosition12[0] = 0;
		}

		if(grvboSphereNormal12[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal12, 0);
			grvboSphereNormal12[0] = 0;
		}
		//13
		if(grvaoSphere13[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere13, 0);
			grvaoSphere13[0] = 0;
		}

		if(grvboSpherePosition13[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition13, 0);
			grvboSpherePosition13[0] = 0;
		}

		if(grvboSphereNormal13[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal13, 0);
			grvboSphereNormal13[0] = 0;
		}
		//14
		if(grvaoSphere14[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere14, 0);
			grvaoSphere14[0] = 0;
		}

		if(grvboSpherePosition14[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition14, 0);
			grvboSpherePosition14[0] = 0;
		}

		if(grvboSphereNormal14[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal14, 0);
			grvboSphereNormal14[0] = 0;
		}
		//15
		if(grvaoSphere15[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere15, 0);
			grvaoSphere15[0] = 0;
		}

		if(grvboSpherePosition15[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition15, 0);
			grvboSpherePosition15[0] = 0;
		}

		if(grvboSphereNormal15[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal15, 0);
			grvboSphereNormal15[0] = 0;
		}
		//16
		if(grvaoSphere16[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere16, 0);
			grvaoSphere16[0] = 0;
		}

		if(grvboSpherePosition16[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition16, 0);
			grvboSpherePosition16[0] = 0;
		}

		if(grvboSphereNormal16[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal16, 0);
			grvboSphereNormal16[0] = 0;
		}
		//17
		if(grvaoSphere17[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere17, 0);
			grvaoSphere17[0] = 0;
		}

		if(grvboSpherePosition17[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition17, 0);
			grvboSpherePosition17[0] = 0;
		}

		if(grvboSphereNormal17[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal17, 0);
			grvboSphereNormal17[0] = 0;
		}
		//18
		if(grvaoSphere18[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere18, 0);
			grvaoSphere18[0] = 0;
		}

		if(grvboSpherePosition18[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition18, 0);
			grvboSpherePosition18[0] = 0;
		}

		if(grvboSphereNormal18[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal18, 0);
			grvboSphereNormal18[0] = 0;
		}
		//19
		if(grvaoSphere19[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere19, 0);
			grvaoSphere19[0] = 0;
		}

		if(grvboSpherePosition19[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition19, 0);
			grvboSpherePosition19[0] = 0;
		}

		if(grvboSphereNormal19[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal19, 0);
			grvboSphereNormal19[0] = 0;
		}
		//20
		if(grvaoSphere20[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere20, 0);
			grvaoSphere20[0] = 0;
		}

		if(grvboSpherePosition20[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition20, 0);
			grvboSpherePosition20[0] = 0;
		}

		if(grvboSphereNormal20[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal20, 0);
			grvboSphereNormal20[0] = 0;
		}
		//21
		if(grvaoSphere21[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere21, 0);
			grvaoSphere21[0] = 0;
		}

		if(grvboSpherePosition21[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition21, 0);
			grvboSpherePosition21[0] = 0;
		}

		if(grvboSphereNormal21[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal21, 0);
			grvboSphereNormal21[0] = 0;
		}
		//22
		if(grvaoSphere22[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere22, 0);
			grvaoSphere22[0] = 0;
		}

		if(grvboSpherePosition22[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition22, 0);
			grvboSpherePosition22[0] = 0;
		}

		if(grvboSphereNormal22[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal22, 0);
			grvboSphereNormal22[0] = 0;
		}
		//23
		if(grvaoSphere23[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere23, 0);
			grvaoSphere23[0] = 0;
		}

		if(grvboSpherePosition23[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition23, 0);
			grvboSpherePosition23[0] = 0;
		}

		if(grvboSphereNormal23[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal23, 0);
			grvboSphereNormal23[0] = 0;
		}
		//24
		if(grvaoSphere24[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere24, 0);
			grvaoSphere24[0] = 0;
		}

		if(grvboSpherePosition24[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition24, 0);
			grvboSpherePosition24[0] = 0;
		}

		if(grvboSphereNormal24[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal24, 0);
			grvboSphereNormal24[0] = 0;
		}

		// shader for per vertex
		if(grshaderProgramObjectPV != 0)
		{
			if(grvertexShaderObjectPV != 0)
			{
				// detach vertex shader from shader program
				GLES32.glDetachShader(grshaderProgramObjectPV, grvertexShaderObjectPV);
				// delete vertex shader
				GLES32.glDeleteShader(grvertexShaderObjectPV);
				grvertexShaderObjectPV = 0;
			}
			if(grfragmentShaderObjectPV != 0)
			{
				// detach vertex shader from shader program
				GLES32.glDetachShader(grshaderProgramObjectPV, grfragmentShaderObjectPV);
				// delete vertex shader
				GLES32.glDeleteShader(grfragmentShaderObjectPV);
				grfragmentShaderObjectPV = 0;
			}
		}
		// delete shader program object
		if(grshaderProgramObjectPV != 0)
		{
			GLES32.glDeleteProgram(grshaderProgramObjectPV);
			grshaderProgramObjectPV = 0;
		}

		// shaders for Per Fragment
		if(grshaderProgramObjectPF != 0)
		{
			if(grvertexShaderObjectPF != 0)
			{
				// detach vertex shader from shader program
				GLES32.glDetachShader(grshaderProgramObjectPF, grvertexShaderObjectPF);
				// delete vertex shader
				GLES32.glDeleteShader(grvertexShaderObjectPF);
				grvertexShaderObjectPF = 0;
			}
			if(grfragmentShaderObjectPF != 0)
			{
				// detach vertex shader from shader program
				GLES32.glDetachShader(grshaderProgramObjectPF, grfragmentShaderObjectPF);
				// delete vertex shader
				GLES32.glDeleteShader(grfragmentShaderObjectPF);
				grfragmentShaderObjectPF = 0;
			}
		}
		// delete shader program object
		if(grshaderProgramObjectPF != 0)
		{
			GLES32.glDeleteProgram(grshaderProgramObjectPF);
			grshaderProgramObjectPF = 0;
		}

	}
}






























