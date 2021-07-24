package com.example.gameone;

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

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer,
	OnGestureListener, OnDoubleTapListener
{
	// variables declaration
	private GestureDetector grgestureDetector;
	private final Context grcontext;

	private int grvertexShaderObject;
	private int grfragmentShaderObject;
	private int grshaderProgramObject;

	private int[] grvaoSphere = new int[1];
	private int[] grvboSpherePosition = new int[1];
	private int[] grvboSphereNormal = new int[1];
	private int[] grvboSphereElement = new int[1];
	private int grnumVertices;
	private int grnumElements;

	private int grmodelMatrixUniform;
	private int grviewMatrixUniform;
	private int grprojectionMatrixUniform;
	private int grlaUniform;
	private int grldUniform;
	private int grlsUniform;
	private int grlightPositionUniform;
	private int grkaUniform;
	private int grkdUniform;
	private int grksUniform;
	private int grmaterialShinynessUniform;
	private int grlKeyPressedUniform;
	

	private float grka[] = new float[]{0.0f, 0.0f, 0.0f, 1.0f};
	private float[][] grkd = {
			//{0.8314f, 0.6863f, 0.2157f, 1.0f},
			{0.0f, 0.545f, 0.545f, 1.0f},
			{0.933f, 0.51f, 0.933f, 1.0f},			// v
			{0.294f, 0.0f, 0.51f, 1.0f},			// i
			{0.0f, 0.0f, 1.0f, 1.0f},				// b
			{0.0f, 0.52f, 0.0f, 1.0f},				// g
			{1.0f, 0.984f, 0.0f, 1.0f},				// y
			{1.0f, 0.647f, 0.0f, 1.0f},				// o
			{1.0f, 0.0f, 0.0f, 1.0f}				// r
		};
	private float[][] grks =  {
			//{0.8314f, 0.6863f, 0.2157f, 1.0f},
			{0.0f, 0.545f, 0.545f, 1.0f},
			{0.933f, 0.51f, 0.933f, 1.0f},			// v
			{0.294f, 0.0f, 0.51f, 1.0f},			// i
			{0.0f, 0.0f, 1.0f, 1.0f},				// b
			{0.0f, 0.52f, 0.0f, 1.0f},				// g
			{1.0f, 0.984f, 0.0f, 1.0f},				// y
			{1.0f, 0.647f, 0.0f, 1.0f},				// o
			{1.0f, 0.0f, 0.0f, 1.0f}				// r
		};
	private float grla[] = new float[]{0.0f, 0.0f, 0.0f, 1.0f};
	private float grld[] = new float[]{1.0f, 1.0f, 1.0f, 1.0f};
	private float grls[] = new float[]{1.0f, 1.0f, 1.0f, 1.0f};
	float[] grlightPosition = new float[] {0.f, 0.0f, 100.0f, 1.0f};
	private float grmaterialShinyness = 128.0f;

	float i1 = 0.0f;
	float i2 = 0.0f;
	boolean grbLight = false;
	float x = 0.3925f;
	float y = 0.5925f;
	private float grperspectiveMatrix[] = new float[16];
	private float grmoveX = 0;
	private float grmoveY = 0;
	float[] grmodelMatrix = new float[16];
	float[] grviewMatrix = new float[16];
	float[] grprojectionMatrix = new float[16];
	int grwidth, grheight;

	public GLESView(Context drawingContext)
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
		return(true);
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent e)
	{
		return(true);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e)
	{
		float[] xy = new float[2];
	    System.out.println("X : " + e.getX() + " Y : " + e.getY());
		xy = convertSCtoWC(e.getX(), e.getY(), grwidth, grheight);
		grmoveX = xy[0];
		grmoveY = xy[1];
		System.out.println("WC coords : " + grmoveX+ ", " + grmoveY);
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
	private float[] convertSCtoWC(float touchX, float touchY, int screenWidth, int screenHeight)
	{
		float[] worldPos = new float[2];
		float[] invertedMatrix = new float[16];
		float[] transformMatrix = new float[16];
		float[] grmodelViewMatrix = new float[16];
		float[] normalizedInPoint = new float[4];
		float[] outPoint = new float[4];
		float[] modelMatrix = new float[16];

		Matrix.setIdentityM(modelMatrix, 0);

		int oglTouchY = (int)(screenHeight - touchY);
		normalizedInPoint[0] = (float) ((touchX) * 2.0f / screenWidth - 1.0f);
		normalizedInPoint[1] = (float) ((oglTouchY) * 2.0 / screenHeight - 1.0f);
		normalizedInPoint[2] = -1.0f;
		normalizedInPoint[3] = 1.0f;

		Matrix.multiplyMM(grmodelViewMatrix, 0, grviewMatrix, 0, modelMatrix, 0);
		Matrix.multiplyMM(transformMatrix, 0, grprojectionMatrix, 0, grmodelViewMatrix, 0);
		Matrix.invertM(invertedMatrix, 0, transformMatrix, 0);

		Matrix.multiplyMV(outPoint, 0, invertedMatrix, 0, normalizedInPoint, 0);

		if(outPoint[3] == 0.0)
		{
			System.out.println("Error in calculating SC to WC");
			return worldPos;
		}

		worldPos[0] = outPoint[0] / outPoint[3];
		worldPos[1] = outPoint[1] / outPoint[3];

		return worldPos;
	}
	private void initialize(GL10 gl)
	{
		System.out.println("OGL_GR : in initialize");
		Sphere grsphere = new Sphere();
		float grsphereVertices[] = new float[1146];
		float grsphereNormals[] = new float[1146];
		float grsphereTextures[] = new float[764];
		short grsphereElements[] = new short[2280];
		grsphere.getSphereVertexData(grsphereVertices, grsphereNormals, grsphereTextures, grsphereElements);
		grnumVertices = grsphere.getNumberOfSphereVertices();
		grnumElements = grsphere.getNumberOfSphereElements();

		///////////////// Vertex Shader
		//create shader
		grvertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

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
				"vec4 eye_coordinates = u_model_matrix * u_view_matrix * vPosition;" +
				"out_transformed_normal = mat3(u_view_matrix * u_model_matrix) * vNormal;" +
				"out_light_direction = vec3(u_light_position - eye_coordinates);" +
				"out_view_vector = -eye_coordinates.xyz;" +
				"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" +
				"}"
		);

		// provide source code to shader
		GLES32.glShaderSource(grvertexShaderObject, grvertexShaderSourceCode);

		// compile shader and check for error
		GLES32.glCompileShader(grvertexShaderObject);
		int[] griShaderCompileStatus = new int[1];
		int[] griInfoLength = new int[1];
		String grszInfoLog = null;

		GLES32.glGetShaderiv(grvertexShaderObject, GLES32.GL_COMPILE_STATUS,
			griShaderCompileStatus, 0);
		if(griShaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(grvertexShaderObject, GLES32.GL_INFO_LOG_LENGTH,
				griInfoLength, 0);

			if(griInfoLength[0] > 0)
			{
				grszInfoLog = GLES32.glGetShaderInfoLog(grvertexShaderObject);
				System.out.println("OGL_GR : Vertex shader compilation log : " + grszInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		////////// Fragment shader
		// create shader
		grfragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

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
					"vec3 normalized_light_direction = normalize(out_light_direction);" +
					"vec3 normalized_view_vector = normalize(out_view_vector);" +
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
		GLES32.glShaderSource(grfragmentShaderObject, grfragmentShaderSourceCode);

		// compile shader
		GLES32.glCompileShader(grfragmentShaderObject);
		griShaderCompileStatus[0] = 0;
		griInfoLength[0] = 0;
		grszInfoLog = null;
		GLES32.glGetShaderiv(grfragmentShaderObject, GLES32.GL_COMPILE_STATUS,
			griShaderCompileStatus, 0);
		if(griShaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(grfragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, griInfoLength, 0);

			if(griInfoLength[0] > 0)
			{
				grszInfoLog = GLES32.glGetShaderInfoLog(grfragmentShaderObject);
				System.out.println("OGL_GR : Fragment shader compilation log : " + grszInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		System.out.println("OGL_GR : initialize->after shaders");
		// create shader program
		grshaderProgramObject = GLES32.glCreateProgram();

		// attach vertex shader to shader program
		GLES32.glAttachShader(grshaderProgramObject, grvertexShaderObject);

		// attach fragment shader to shader program
		GLES32.glAttachShader(grshaderProgramObject, grfragmentShaderObject);

		// prelink binding of shader program object with vertex shader attributes
		GLES32.glBindAttribLocation(grshaderProgramObject, GLESMacros.GR_ATTRIBUTE_POSITION,
			"vPosition");
		GLES32.glBindAttribLocation(grshaderProgramObject, GLESMacros.GR_ATTRIBUTE_NORMAL,
			"vNormal");

		// link
		GLES32.glLinkProgram(grshaderProgramObject);
		int[] griShaderProgramLinkStatus = new int[1];
		griInfoLength[0] = 0;
		grszInfoLog = null;
		GLES32.glGetProgramiv(grshaderProgramObject, GLES32.GL_LINK_STATUS,
			griShaderProgramLinkStatus, 0);
		if(griShaderProgramLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(grshaderProgramObject, GLES32.GL_INFO_LOG_LENGTH,
				griInfoLength, 0);

			if(griInfoLength[0] > 0)
			{
				grszInfoLog = GLES32.glGetProgramInfoLog(grshaderProgramObject);
				System.out.println("OGL_GR : Shader program link log : " + grszInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		// get uniform location
		grmodelMatrixUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_model_matrix");
		grviewMatrixUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_view_matrix");
		grprojectionMatrixUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_projection_matrix");
		grlaUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_la");
		grldUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_ld");
		grlsUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_ls");
		grkaUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_ka");
		grkdUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_kd");
		grksUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_ks");
		grlightPositionUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_light_position");
		grlKeyPressedUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_l_key_pressed");
		grmaterialShinynessUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_material_shinyness");

		// vao
		GLES32.glGenVertexArrays(1, grvaoSphere, 0);
		GLES32.glBindVertexArray(grvaoSphere[0]);
		// vbo position
		GLES32.glGenBuffers(1, grvboSpherePosition, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSpherePosition[0]);
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
		GLES32.glGenBuffers(1, grvboSphereNormal, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvboSphereNormal[0]);
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
		GLES32.glGenBuffers(1, grvboSphereElement, 0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement[0]);
		grbyteBuffer = ByteBuffer.allocateDirect(grsphereElements.length * 2);
		grbyteBuffer.order(ByteOrder.nativeOrder());
		ShortBuffer grelementsBuffer = grbyteBuffer.asShortBuffer();
		grelementsBuffer.put(grsphereElements);
		grelementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, grsphereElements.length * 2, grelementsBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		System.out.println("OGL_GR : initialize->after vao of cube");
		System.out.println("OGL_GR : initialize->after shaders");

		// enable depth testing
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glEnable(GLES32.GL_LEQUAL);
		GLES32.glClearDepthf(1.0f);


		// set background color
		GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// set projection matrix to identity matrix
		Matrix.setIdentityM(grperspectiveMatrix, 0);
	}

	private void resize(int width, int height)
	{
		GLES32.glViewport(0, 0, width, height);
		grwidth = width;
		grheight = height;
		//Matrix.perspectiveM(grperspectiveMatrix, 0, 45.0f, (float) width / (float) height, 0.1f, 100.0f);
		if(width <= height)
		{
			Matrix.orthoM(grperspectiveMatrix, 0, -100.0f, 100.0f,
					(-100.0f * (height / width)), (100.0f * (height / width)),
					-100.0f, 100.0f);
		}
		else
		{
			Matrix.orthoM(grperspectiveMatrix, 0,
					(-100.0f * (width / height)),  (100.0f * (width / height)),
					-100.0f, 100.0f,
					-100.0f, 100.0f);
		}
	}

	private void display()
	{
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		GLES32.glUseProgram(grshaderProgramObject);

		float[] grtranslateMatrix = new float[16];
		float[] grrotateMatrix = new float[16];
		float[] grscaleMatrix = new float[16];
		float[] grkdzero = new float[]{0.8314f, 0.6863f, 0.2157f, 1.0f};
		float[] grkszero = new float[]{0.8314f, 0.6863f, 0.2157f, 1.0f};

		//******* Sphere 0 drawing
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grrotateMatrix, 0);
		Matrix.setIdentityM(grscaleMatrix, 0);
        Matrix.setIdentityM(grmodelMatrix, 0);
		Matrix.setIdentityM(grviewMatrix, 0);
		Matrix.setIdentityM(grprojectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, (float)( i1 + Math.sin(0.7)), (float)(i2 + Math.cos(0.7)), -8.0f);
		Matrix.rotateM(grrotateMatrix,0, 90.0f, 0.0f, 0.0f, 1.0f);
		if(grbLight == true)
        {
            GLES32.glUniform1i(grlKeyPressedUniform, 1);
            GLES32.glUniform3fv(grldUniform, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniform, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniform, 1, grls, 0);
			GLES32.glUniform3fv(grkdUniform, 1, grkdzero, 0);
			GLES32.glUniform3fv(grkaUniform, 1, grka, 0);
			GLES32.glUniform3fv(grksUniform, 1, grkszero, 0);
			GLES32.glUniform4fv(grlightPositionUniform, 1, grlightPosition, 0);
			GLES32.glUniform1f(grmaterialShinynessUniform, grmaterialShinyness);
        }
		else
        {
            GLES32.glUniform1i(grlKeyPressedUniform, 0);
        }

		Matrix.multiplyMM(grmodelMatrix, 0, grtranslateMatrix, 0, grrotateMatrix, 0);
		Matrix.multiplyMM(grprojectionMatrix, 0,
			grprojectionMatrix, 0, grperspectiveMatrix, 0);

		GLES32.glUniformMatrix4fv(grmodelMatrixUniform, 1, false, grmodelMatrix, 0);
		GLES32.glUniformMatrix4fv(grviewMatrixUniform, 1, false, grviewMatrix, 0);
        GLES32.glUniformMatrix4fv(grprojectionMatrixUniform, 1, false, grprojectionMatrix, 0);

		// bind vao
		GLES32.glBindVertexArray(grvaoSphere[0]);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement[0]);
		GLES32.glDrawElements(GLES32.GL_LINES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
		GLES32.glBindVertexArray(0);

		//******* Sphere 1 drawing
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grrotateMatrix, 0);
		Matrix.setIdentityM(grscaleMatrix, 0);
		Matrix.setIdentityM(grmodelMatrix, 0);
		Matrix.setIdentityM(grviewMatrix, 0);
		Matrix.setIdentityM(grprojectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, (float)(80 * Math.sin(x)), (float)(80 *Math.cos(x)), -8.0f);
		if(grbLight == true)
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 1);
			GLES32.glUniform3fv(grldUniform, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniform, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniform, 1, grls, 0);
			GLES32.glUniform3fv(grkdUniform, 1, grkd[0], 0);
			GLES32.glUniform3fv(grkaUniform, 1, grka, 0);
			GLES32.glUniform3fv(grksUniform, 1, grks[0], 0);
			GLES32.glUniform4fv(grlightPositionUniform, 1, grlightPosition, 0);
			GLES32.glUniform1f(grmaterialShinynessUniform, grmaterialShinyness);
		}
		else
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 0);
		}

		Matrix.multiplyMM(grmodelMatrix, 0, grmodelMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grprojectionMatrix, 0,
				grprojectionMatrix, 0, grperspectiveMatrix, 0);

		GLES32.glUniformMatrix4fv(grmodelMatrixUniform, 1, false, grmodelMatrix, 0);
		GLES32.glUniformMatrix4fv(grviewMatrixUniform, 1, false, grviewMatrix, 0);
		GLES32.glUniformMatrix4fv(grprojectionMatrixUniform, 1, false, grprojectionMatrix, 0);

		// bind vao
		GLES32.glBindVertexArray(grvaoSphere[0]);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
		GLES32.glBindVertexArray(0);

		//******* Sphere 2 drawing
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grrotateMatrix, 0);
		Matrix.setIdentityM(grscaleMatrix, 0);
		Matrix.setIdentityM(grmodelMatrix, 0);
		Matrix.setIdentityM(grviewMatrix, 0);
		Matrix.setIdentityM(grprojectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, (float) (80 * Math.sin(3.0 * x)), (float)(80 * Math.cos(3.0 * x)), -8.0f);
		if(grbLight == true)
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 1);
			GLES32.glUniform3fv(grldUniform, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniform, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniform, 1, grls, 0);
			GLES32.glUniform3fv(grkdUniform, 1, grkd[1], 0);
			GLES32.glUniform3fv(grkaUniform, 1, grka, 0);
			GLES32.glUniform3fv(grksUniform, 1, grks[1], 0);
			GLES32.glUniform4fv(grlightPositionUniform, 1, grlightPosition, 0);
			GLES32.glUniform1f(grmaterialShinynessUniform, grmaterialShinyness);
		}
		else
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 0);
		}

		Matrix.multiplyMM(grmodelMatrix, 0, grmodelMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grprojectionMatrix, 0,
				grprojectionMatrix, 0, grperspectiveMatrix, 0);

		GLES32.glUniformMatrix4fv(grmodelMatrixUniform, 1, false, grmodelMatrix, 0);
		GLES32.glUniformMatrix4fv(grviewMatrixUniform, 1, false, grviewMatrix, 0);
		GLES32.glUniformMatrix4fv(grprojectionMatrixUniform, 1, false, grprojectionMatrix, 0);

		// bind vao
		GLES32.glBindVertexArray(grvaoSphere[0]);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
		GLES32.glBindVertexArray(0);

		//******* Sphere 3 drawing
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grrotateMatrix, 0);
		Matrix.setIdentityM(grscaleMatrix, 0);
		Matrix.setIdentityM(grmodelMatrix, 0);
		Matrix.setIdentityM(grviewMatrix, 0);
		Matrix.setIdentityM(grprojectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, (float) (80 * Math.sin(5.1 * x)), (float)(80 * Math.cos(5.1 * x)), -8.0f);
		if(grbLight == true)
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 1);
			GLES32.glUniform3fv(grldUniform, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniform, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniform, 1, grls, 0);
			GLES32.glUniform3fv(grkdUniform, 1, grkd[2], 0);
			GLES32.glUniform3fv(grkaUniform, 1, grka, 0);
			GLES32.glUniform3fv(grksUniform, 1, grks[2], 0);
			GLES32.glUniform4fv(grlightPositionUniform, 1, grlightPosition, 0);
			GLES32.glUniform1f(grmaterialShinynessUniform, grmaterialShinyness);
		}
		else
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 0);
		}

		Matrix.multiplyMM(grmodelMatrix, 0, grmodelMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grprojectionMatrix, 0,
				grprojectionMatrix, 0, grperspectiveMatrix, 0);

		GLES32.glUniformMatrix4fv(grmodelMatrixUniform, 1, false, grmodelMatrix, 0);
		GLES32.glUniformMatrix4fv(grviewMatrixUniform, 1, false, grviewMatrix, 0);
		GLES32.glUniformMatrix4fv(grprojectionMatrixUniform, 1, false, grprojectionMatrix, 0);

		// bind vao
		GLES32.glBindVertexArray(grvaoSphere[0]);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
		GLES32.glBindVertexArray(0);

		//******* Sphere 4 drawing
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grrotateMatrix, 0);
		Matrix.setIdentityM(grscaleMatrix, 0);
		Matrix.setIdentityM(grmodelMatrix, 0);
		Matrix.setIdentityM(grviewMatrix, 0);
		Matrix.setIdentityM(grprojectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, (float) (80 * Math.sin(7.1 * x)), (float)(80 * Math.cos(7.1 * x)), -8.0f);
		if(grbLight == true)
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 1);
			GLES32.glUniform3fv(grldUniform, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniform, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniform, 1, grls, 0);
			GLES32.glUniform3fv(grkdUniform, 1, grkd[3], 0);
			GLES32.glUniform3fv(grkaUniform, 1, grka, 0);
			GLES32.glUniform3fv(grksUniform, 1, grks[3], 0);
			GLES32.glUniform4fv(grlightPositionUniform, 1, grlightPosition, 0);
			GLES32.glUniform1f(grmaterialShinynessUniform, grmaterialShinyness);
		}
		else
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 0);
		}

		Matrix.multiplyMM(grmodelMatrix, 0, grmodelMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grprojectionMatrix, 0, grprojectionMatrix, 0, grperspectiveMatrix, 0);
		GLES32.glUniformMatrix4fv(grmodelMatrixUniform, 1, false, grmodelMatrix, 0);
		GLES32.glUniformMatrix4fv(grviewMatrixUniform, 1, false, grviewMatrix, 0);
		GLES32.glUniformMatrix4fv(grprojectionMatrixUniform, 1, false, grprojectionMatrix, 0);

		GLES32.glBindVertexArray(grvaoSphere[0]);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
		GLES32.glBindVertexArray(0);

		//******* Sphere 5 drawing
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grrotateMatrix, 0);
		Matrix.setIdentityM(grscaleMatrix, 0);
		Matrix.setIdentityM(grmodelMatrix, 0);
		Matrix.setIdentityM(grviewMatrix, 0);
		Matrix.setIdentityM(grprojectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, (float) (80 * Math.sin(9.1 * x)), (float)(80 * Math.cos(9.1 * x)), -8.0f);
		if(grbLight == true)
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 1);
			GLES32.glUniform3fv(grldUniform, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniform, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniform, 1, grls, 0);
			GLES32.glUniform3fv(grkdUniform, 1, grkd[4], 0);
			GLES32.glUniform3fv(grkaUniform, 1, grka, 0);
			GLES32.glUniform3fv(grksUniform, 1, grks[4], 0);
			GLES32.glUniform4fv(grlightPositionUniform, 1, grlightPosition, 0);
			GLES32.glUniform1f(grmaterialShinynessUniform, grmaterialShinyness);
		}
		else
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 0);
		}

		Matrix.multiplyMM(grmodelMatrix, 0, grmodelMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grprojectionMatrix, 0, grprojectionMatrix, 0, grperspectiveMatrix, 0);
		GLES32.glUniformMatrix4fv(grmodelMatrixUniform, 1, false, grmodelMatrix, 0);
		GLES32.glUniformMatrix4fv(grviewMatrixUniform, 1, false, grviewMatrix, 0);
		GLES32.glUniformMatrix4fv(grprojectionMatrixUniform, 1, false, grprojectionMatrix, 0);

		GLES32.glBindVertexArray(grvaoSphere[0]);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
		GLES32.glBindVertexArray(0);

		//******* Sphere 6 drawing
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grrotateMatrix, 0);
		Matrix.setIdentityM(grscaleMatrix, 0);
		Matrix.setIdentityM(grmodelMatrix, 0);
		Matrix.setIdentityM(grviewMatrix, 0);
		Matrix.setIdentityM(grprojectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, (float) (80 * Math.sin(11.1 * x)), (float)(80 * Math.cos(11.1 * x)), -8.0f);
		if(grbLight == true)
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 1);
			GLES32.glUniform3fv(grldUniform, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniform, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniform, 1, grls, 0);
			GLES32.glUniform3fv(grkdUniform, 1, grkd[5], 0);
			GLES32.glUniform3fv(grkaUniform, 1, grka, 0);
			GLES32.glUniform3fv(grksUniform, 1, grks[5], 0);
			GLES32.glUniform4fv(grlightPositionUniform, 1, grlightPosition, 0);
			GLES32.glUniform1f(grmaterialShinynessUniform, grmaterialShinyness);
		}
		else
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 0);
		}

		Matrix.multiplyMM(grmodelMatrix, 0, grmodelMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grprojectionMatrix, 0, grprojectionMatrix, 0, grperspectiveMatrix, 0);
		GLES32.glUniformMatrix4fv(grmodelMatrixUniform, 1, false, grmodelMatrix, 0);
		GLES32.glUniformMatrix4fv(grviewMatrixUniform, 1, false, grviewMatrix, 0);
		GLES32.glUniformMatrix4fv(grprojectionMatrixUniform, 1, false, grprojectionMatrix, 0);

		GLES32.glBindVertexArray(grvaoSphere[0]);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
		GLES32.glBindVertexArray(0);

		//******* Sphere 7 drawing
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grrotateMatrix, 0);
		Matrix.setIdentityM(grscaleMatrix, 0);
		Matrix.setIdentityM(grmodelMatrix, 0);
		Matrix.setIdentityM(grviewMatrix, 0);
		Matrix.setIdentityM(grprojectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, (float) (80 * Math.sin(13.1 * x)), (float)(80 * Math.cos(13.1 * x)), -8.0f);
		if(grbLight == true)
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 1);
			GLES32.glUniform3fv(grldUniform, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniform, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniform, 1, grls, 0);
			GLES32.glUniform3fv(grkdUniform, 1, grkd[6], 0);
			GLES32.glUniform3fv(grkaUniform, 1, grka, 0);
			GLES32.glUniform3fv(grksUniform, 1, grks[6], 0);
			GLES32.glUniform4fv(grlightPositionUniform, 1, grlightPosition, 0);
			GLES32.glUniform1f(grmaterialShinynessUniform, grmaterialShinyness);
		}
		else
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 0);
		}

		Matrix.multiplyMM(grmodelMatrix, 0, grmodelMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grprojectionMatrix, 0, grprojectionMatrix, 0, grperspectiveMatrix, 0);
		GLES32.glUniformMatrix4fv(grmodelMatrixUniform, 1, false, grmodelMatrix, 0);
		GLES32.glUniformMatrix4fv(grviewMatrixUniform, 1, false, grviewMatrix, 0);
		GLES32.glUniformMatrix4fv(grprojectionMatrixUniform, 1, false, grprojectionMatrix, 0);

		GLES32.glBindVertexArray(grvaoSphere[0]);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
		GLES32.glBindVertexArray(0);

		//******* Sphere 8 drawing
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grrotateMatrix, 0);
		Matrix.setIdentityM(grscaleMatrix, 0);
		Matrix.setIdentityM(grmodelMatrix, 0);
		Matrix.setIdentityM(grviewMatrix, 0);
		Matrix.setIdentityM(grprojectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, (float) (80 * Math.sin(15.1 * x)), (float)(80 * Math.cos(15.1 * x)), -8.0f);
		if(grbLight == true)
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 1);
			GLES32.glUniform3fv(grldUniform, 1, grld, 0);
			GLES32.glUniform3fv(grlaUniform, 1, grla, 0);
			GLES32.glUniform3fv(grlsUniform, 1, grls, 0);
			GLES32.glUniform3fv(grkdUniform, 1, grkd[7], 0);
			GLES32.glUniform3fv(grkaUniform, 1, grka, 0);
			GLES32.glUniform3fv(grksUniform, 1, grks[7], 0);
			GLES32.glUniform4fv(grlightPositionUniform, 1, grlightPosition, 0);
			GLES32.glUniform1f(grmaterialShinynessUniform, grmaterialShinyness);
		}
		else
		{
			GLES32.glUniform1i(grlKeyPressedUniform, 0);
		}

		Matrix.multiplyMM(grmodelMatrix, 0, grmodelMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grprojectionMatrix, 0, grprojectionMatrix, 0, grperspectiveMatrix, 0);
		GLES32.glUniformMatrix4fv(grmodelMatrixUniform, 1, false, grmodelMatrix, 0);
		GLES32.glUniformMatrix4fv(grviewMatrixUniform, 1, false, grviewMatrix, 0);
		GLES32.glUniformMatrix4fv(grprojectionMatrixUniform, 1, false, grprojectionMatrix, 0);

		GLES32.glBindVertexArray(grvaoSphere[0]);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, grvboSphereElement[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, grnumElements, GLES32.GL_UNSIGNED_SHORT, 0);
		GLES32.glBindVertexArray(0);

		GLES32.glUseProgram(0);

		update();
		requestRender();
	}

	void update()
	{
		// code

		if(i1 <= grmoveX)
			i1 = i1 + 1.01f;
		else if(i1 >= grmoveX)
			i1 = i1 - 1.01f;
		if(i2 <= grmoveY)
			i2 = i2 + 1.01f;
		else if(i2 >= grmoveY)
			i2 = i2 - 1.01f;

		//i = i + 0.1f;
	}

	void uninitialize()
	{
		// destroy vao
		if(grvaoSphere[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvaoSphere, 0);
			grvaoSphere[0] = 0;
		}

		if(grvboSpherePosition[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSpherePosition, 0);
			grvboSpherePosition[0] = 0;
		}

		if(grvboSphereNormal[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvboSphereNormal, 0);
			grvboSphereNormal[0] = 0;
		}


		if(grshaderProgramObject != 0)
		{
			if(grvertexShaderObject != 0)
			{
				// detach vertex shader from shader program
				GLES32.glDetachShader(grshaderProgramObject, grvertexShaderObject);
				// delete vertex shader
				GLES32.glDeleteShader(grvertexShaderObject);
				grvertexShaderObject = 0;
			}
			if(grfragmentShaderObject != 0)
			{
				// detach vertex shader from shader program
				GLES32.glDetachShader(grshaderProgramObject, grfragmentShaderObject);
				// delete vertex shader
				GLES32.glDeleteShader(grfragmentShaderObject);
				grfragmentShaderObject = 0;
			}
		}
		// delete shader program object
		if(grshaderProgramObject != 0)
		{
			GLES32.glDeleteProgram(grshaderProgramObject);
			grshaderProgramObject = 0;
		}

	}
}






























