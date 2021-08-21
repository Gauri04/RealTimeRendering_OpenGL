package com.example.graph;

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

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer,
	OnGestureListener, OnDoubleTapListener
{
	// variables declaration
	private GestureDetector grgestureDetector;
	private final Context grcontext;

	private int grvertexShaderObject;
	private int grfragmentShaderObject;
	private int grshaderProgramObject;

	private int[] grvao = new int[1];
	private int[] grvbo = new int[1];
	private int grmvpUniform;

	private float grperspectiveProjectionMatrix[] = new float[16];
	float grverticalLineVertices1[] = new float[80];
	float grverticalLineVertices2[] = new float[80];
	float grhorizontalLineVertices1[] = new float[80];
	float grhorizontalLineVertices2[] = new float[80];
	float grverticalLineVertices[] = new float[]{0.0f, -2.0f, 0.0f, 2.0f};
	float grhorizontalLineVertices[] = new float[]{-3.0f, 0.0f, 3.0f, 0.0f};
	FloatBuffer grfloatBufferVerticalLineVerts1;
	FloatBuffer grfloatBufferVerticalLineVerts2;
	FloatBuffer grfloatBufferVerticalLineVerts;
	FloatBuffer grfloatBufferHorizontalLineVerts1;
	FloatBuffer grfloatBufferHorizontalLineVerts2;
	FloatBuffer grfloatBufferHorizontalLineVerts;

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
		///////////////// Vertex Shader
		//create shader
		grvertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		// vertex shader source code
		final String grvertexShaderSourceCode = String.format(
			"#version 320 es" +
			"\n" +
			"in vec4 vPosition;" +
			"in vec3 vColor;" +
			"out vec3 out_color;" +
			"uniform mat4 u_mvpMatrix;" +
			"void main(void)" +
			"{" +
			"out_color = vColor;" +
			"gl_Position = u_mvpMatrix * vPosition;" +
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
			"in vec3 out_color;" +
			"out vec4 FragColor;" +
			"void main(void)" +
			"{" +
			"FragColor = vec4(out_color, 1.0);" +
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
			GLES32.glGetShaderiv(grfragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH,
				griInfoLength, 0);

			if(griInfoLength[0] > 0)
			{
				grszInfoLog = GLES32.glGetShaderInfoLog(grfragmentShaderObject);
				System.out.println("OGL_GR : Fragment shader compilation log : " + grszInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		// create shader program
		grshaderProgramObject = GLES32.glCreateProgram();

		// attach vertex shader to shader program
		GLES32.glAttachShader(grshaderProgramObject, grvertexShaderObject);

		// attach fragment shader to shader program
		GLES32.glAttachShader(grshaderProgramObject, grfragmentShaderObject);

		// prelink binding of shader program object with vertex shader attributes
		GLES32.glBindAttribLocation(grshaderProgramObject, GLESMacros.GR_ATTRIBUTE_POSITION,
			"vPosition");
		GLES32.glBindAttribLocation(grshaderProgramObject, GLESMacros.GR_ATTRIBUTE_COLOR,
				"vColor");

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
		grmvpUniform = GLES32.glGetUniformLocation(grshaderProgramObject, "u_mvpMatrix");

		float i = 0.11f;
		int index = 0;

		for (index = 0; index < 80; index = index + 4)
		{
			grverticalLineVertices1[index] = i;
			grverticalLineVertices1[index + 1] = -2.0f;
			grverticalLineVertices1[index + 2] = i;
			grverticalLineVertices1[index + 3] = 2.0f;
			i = i + 0.11f;
		}

		i = 0.11f;
		for (index = 0; index < 80; index = index + 4)
		{
			grverticalLineVertices2[index] = -i;
			grverticalLineVertices2[index + 1] = -2.0f;
			grverticalLineVertices2[index + 2] = -i;
			grverticalLineVertices2[index + 3] = 2.0f;
			i = i + 0.11f;
		}
		i = 0.060f;
		for (index = 0; index < 80; index = index + 4)
		{
			grhorizontalLineVertices1[index] = -3.0f;
			grhorizontalLineVertices1[index + 1] = i;
			grhorizontalLineVertices1[index + 2] = 3.0f;
			grhorizontalLineVertices1[index + 3] = i;
			i = i + 0.060f;
		}
		i = 0.05f;
		for (index = 0; index < 80; index = index + 4)
		{
			grhorizontalLineVertices2[index] = -3.0f;
			grhorizontalLineVertices2[index + 1] = -i;
			grhorizontalLineVertices2[index + 2] = 3.0f;
			grhorizontalLineVertices2[index + 3] = -i;
			i = i + 0.060f;
		}

		GLES32.glGenVertexArrays(1, grvao, 0);
		GLES32.glBindVertexArray(grvao[0]);

		GLES32.glGenBuffers(1, grvbo, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvbo[0]);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, 40 * 2 * 4, null, GLES32.GL_DYNAMIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 2,
			GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		GLES32.glBindVertexArray(0);

		// bytebuffer for vertial line vertices - 1
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(grverticalLineVertices1.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		grfloatBufferVerticalLineVerts1 = byteBuffer.asFloatBuffer();
		grfloatBufferVerticalLineVerts1.put(grverticalLineVertices1);
		grfloatBufferVerticalLineVerts1.position(0);

		// bytebuffer for vertial line vertices - 2
		byteBuffer = ByteBuffer.allocateDirect(grverticalLineVertices2.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		grfloatBufferVerticalLineVerts2 = byteBuffer.asFloatBuffer();
		grfloatBufferVerticalLineVerts2.put(grverticalLineVertices2);
		grfloatBufferVerticalLineVerts2.position(0);

		// bytebuffer for vertial line vertices - center line
		byteBuffer = ByteBuffer.allocateDirect(grverticalLineVertices.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		grfloatBufferVerticalLineVerts = byteBuffer.asFloatBuffer();
		grfloatBufferVerticalLineVerts.put(grverticalLineVertices);
		grfloatBufferVerticalLineVerts.position(0);

		// bytebuffer for horizontal line vertices - 1
		byteBuffer = ByteBuffer.allocateDirect(grhorizontalLineVertices1.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		grfloatBufferHorizontalLineVerts1 = byteBuffer.asFloatBuffer();
		grfloatBufferHorizontalLineVerts1.put(grhorizontalLineVertices1);
		grfloatBufferHorizontalLineVerts1.position(0);

		// bytebuffer for horizontal line vertices - 2
		byteBuffer = ByteBuffer.allocateDirect(grhorizontalLineVertices2.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		grfloatBufferHorizontalLineVerts2 = byteBuffer.asFloatBuffer();
		grfloatBufferHorizontalLineVerts2.put(grhorizontalLineVertices2);
		grfloatBufferHorizontalLineVerts2.position(0);

		// bytebuffer for horizontal line vertices - center line
		byteBuffer = ByteBuffer.allocateDirect(grhorizontalLineVertices.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		grfloatBufferHorizontalLineVerts = byteBuffer.asFloatBuffer();
		grfloatBufferHorizontalLineVerts.put(grhorizontalLineVertices);
		grfloatBufferHorizontalLineVerts.position(0);

		// enable depth testing
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glEnable(GLES32.GL_LEQUAL);
		GLES32.glClearDepthf(1.0f);
		// set background color
		GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// set projection matrix to identity matrix
		Matrix.setIdentityM(grperspectiveProjectionMatrix, 0);
	}

	private void resize(int width, int height)
	{
		GLES32.glViewport(0, 0, width, height);

		Matrix.perspectiveM(grperspectiveProjectionMatrix, 0, 45.0f, (float) width / (float) height, 0.1f, 100.0f);
	}

	private void display()
	{
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		GLES32.glUseProgram(grshaderProgramObject);

		float grmodelViewMatrix[] = new float[16];
		float grmodelViewProjectionMatrix[] = new float[16];
		float grtranslateMatrix[] = new float[16];

		Matrix.setIdentityM(grmodelViewMatrix, 0);
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grmodelViewProjectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, 0.0f, 0.0f, -2.5f);
		Matrix.multiplyMM(grmodelViewMatrix, 0, grmodelViewMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grmodelViewProjectionMatrix, 0,
			grperspectiveProjectionMatrix, 0, grmodelViewMatrix, 0);

		GLES32.glUniformMatrix4fv(grmvpUniform, 1, false, grmodelViewProjectionMatrix, 0);

		// bind vao
		GLES32.glBindVertexArray(grvao[0]);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvbo[0]);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grverticalLineVertices1.length * 4,
				grfloatBufferVerticalLineVerts1, GLES32.GL_DYNAMIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 2,
				GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		GLES32.glVertexAttrib3f(GLESMacros.GR_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
		GLES32.glDrawArrays(GLES32.GL_LINES, 0, 40);
		GLES32.glBindVertexArray(0);

		// ********* vertical Line vertices - 2 ***********************88
		Matrix.setIdentityM(grmodelViewMatrix, 0);
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grmodelViewProjectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, 0.0f, 0.0f, -2.5f);
		Matrix.multiplyMM(grmodelViewMatrix, 0, grmodelViewMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grmodelViewProjectionMatrix, 0,
				grperspectiveProjectionMatrix, 0, grmodelViewMatrix, 0);

		GLES32.glUniformMatrix4fv(grmvpUniform, 1, false, grmodelViewProjectionMatrix, 0);

		// bind vao
		GLES32.glBindVertexArray(grvao[0]);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvbo[0]);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grverticalLineVertices2.length * 4,
				grfloatBufferVerticalLineVerts2, GLES32.GL_DYNAMIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 2,
				GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		GLES32.glVertexAttrib3f(GLESMacros.GR_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
		GLES32.glDrawArrays(GLES32.GL_LINES, 0, 40);
		GLES32.glBindVertexArray(0);

		// ********* vertical Line - center ***********************88
		Matrix.setIdentityM(grmodelViewMatrix, 0);
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grmodelViewProjectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, 0.0f, 0.0f, -2.5f);
		Matrix.multiplyMM(grmodelViewMatrix, 0, grmodelViewMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grmodelViewProjectionMatrix, 0,
				grperspectiveProjectionMatrix, 0, grmodelViewMatrix, 0);

		GLES32.glUniformMatrix4fv(grmvpUniform, 1, false, grmodelViewProjectionMatrix, 0);

		// bind vao
		GLES32.glBindVertexArray(grvao[0]);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvbo[0]);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grverticalLineVertices.length * 4,
				grfloatBufferVerticalLineVerts, GLES32.GL_DYNAMIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 2,
				GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		GLES32.glVertexAttrib3f(GLESMacros.GR_ATTRIBUTE_COLOR, 1.0f, 0.0f, 0.0f);
		GLES32.glDrawArrays(GLES32.GL_LINES, 0, 2);
		GLES32.glBindVertexArray(0);


		//** HORIZONTAL LINES **-----------------------------------------------------------------------------------
		// ********* Horizontal Line vertices - 1 ***********************88
		Matrix.setIdentityM(grmodelViewMatrix, 0);
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grmodelViewProjectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, 0.0f, 0.0f, -2.5f);
		Matrix.multiplyMM(grmodelViewMatrix, 0, grmodelViewMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grmodelViewProjectionMatrix, 0,
				grperspectiveProjectionMatrix, 0, grmodelViewMatrix, 0);

		GLES32.glUniformMatrix4fv(grmvpUniform, 1, false, grmodelViewProjectionMatrix, 0);

		// bind vao
		GLES32.glBindVertexArray(grvao[0]);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvbo[0]);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grhorizontalLineVertices1.length * 4,
				grfloatBufferHorizontalLineVerts1, GLES32.GL_DYNAMIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 2,
				GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		GLES32.glVertexAttrib3f(GLESMacros.GR_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
		GLES32.glDrawArrays(GLES32.GL_LINES, 0, 40);
		GLES32.glBindVertexArray(0);

		// ********* vertical Line vertices - 2 ***********************88
		Matrix.setIdentityM(grmodelViewMatrix, 0);
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grmodelViewProjectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, 0.0f, 0.0f, -2.5f);
		Matrix.multiplyMM(grmodelViewMatrix, 0, grmodelViewMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grmodelViewProjectionMatrix, 0,
				grperspectiveProjectionMatrix, 0, grmodelViewMatrix, 0);

		GLES32.glUniformMatrix4fv(grmvpUniform, 1, false, grmodelViewProjectionMatrix, 0);

		// bind vao
		GLES32.glBindVertexArray(grvao[0]);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvbo[0]);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grhorizontalLineVertices2.length * 4,
				grfloatBufferHorizontalLineVerts2, GLES32.GL_DYNAMIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 2,
				GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		GLES32.glVertexAttrib3f(GLESMacros.GR_ATTRIBUTE_COLOR, 0.0f, 1.0f, 0.0f);
		GLES32.glDrawArrays(GLES32.GL_LINES, 0, 40);
		GLES32.glBindVertexArray(0);

		// ********* horizontal Line - center ***********************88
		Matrix.setIdentityM(grmodelViewMatrix, 0);
		Matrix.setIdentityM(grtranslateMatrix, 0);
		Matrix.setIdentityM(grmodelViewProjectionMatrix, 0);

		Matrix.translateM(grtranslateMatrix, 0, 0.0f, 0.0f, -2.5f);
		Matrix.multiplyMM(grmodelViewMatrix, 0, grmodelViewMatrix, 0, grtranslateMatrix, 0);
		Matrix.multiplyMM(grmodelViewProjectionMatrix, 0,
				grperspectiveProjectionMatrix, 0, grmodelViewMatrix, 0);

		GLES32.glUniformMatrix4fv(grmvpUniform, 1, false, grmodelViewProjectionMatrix, 0);

		// bind vao
		GLES32.glBindVertexArray(grvao[0]);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, grvbo[0]);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, grhorizontalLineVertices.length * 4,
				grfloatBufferHorizontalLineVerts, GLES32.GL_DYNAMIC_DRAW);
		GLES32.glVertexAttribPointer(GLESMacros.GR_ATTRIBUTE_POSITION, 2,
				GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(GLESMacros.GR_ATTRIBUTE_POSITION);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		GLES32.glVertexAttrib3f(GLESMacros.GR_ATTRIBUTE_COLOR, 1.0f, 0.0f, 0.0f);
		GLES32.glDrawArrays(GLES32.GL_LINES, 0, 2);
		GLES32.glBindVertexArray(0);

		GLES32.glUseProgram(0);

		requestRender();
	}

	void uninitialize()
	{
		// destroy vao
		if(grvao[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, grvao, 0);
			grvao[0] = 0;
		}

		if(grvbo[0] != 0)
		{
			GLES32.glDeleteBuffers(1, grvbo, 0);
			grvbo[0] = 0;
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






























