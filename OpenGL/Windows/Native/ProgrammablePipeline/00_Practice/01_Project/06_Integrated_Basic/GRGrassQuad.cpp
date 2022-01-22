#include"GRGrassQuad.h"
using namespace vmath;

FILE* grgpFileGrassQuad = NULL;

GLuint grvertexShaderObjectGrassQuad;
GLuint grfragmentShaderObjectGrassQuad;
GLuint grshaderProgramObjectGrassQuad;
GLuint grmodelMatrixUniformGrassQuad;
GLuint grviewMatrixUniformGrassQuad;
GLuint grprojectionMatrixUniformGrassQuad;
mat4 grprojectionMatrixGrassQuad;
GLuint grvaoGrassQuad;
GLuint grvboPositionGrassQuad;
GLuint grvboTextureGrassQuad;
GLfloat grangleX_GrassQuad = 0.0f;
GLfloat grangleY_GrassQuad = 0.0f;
GLfloat grangleZ_GrassQuad = 270.0f;
// POS
GLfloat grmodelX_GrassQuad = -107.200005f;
GLfloat grmodelY_GrassQuad = -1.459999f;
GLfloat grmodelZ_GrassQuad = 0.0f;
mat4 grcameraMatrixGrassQuad = mat4::identity();

GLuint grtextureGrassQuad;
GLuint grtextureSamplerUniformGrassQuad;


bool LoadGLTextureGrassQuad(GLuint*, TCHAR[]);

extern "C" void GROpenLogFileGrassQuad()
{
	if (fopen_s(&grgpFileGrassQuad, "GRLogGrassQuad.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot open GRLogGrassQuad.txt file"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf_s(grgpFileGrassQuad, "Log file created successfully. \n Program started successfully\n **** Logs ***** \n");
	}
}

extern "C" void GRPrintLogGrassQuad(const char* msg)
{
	fprintf(grgpFileGrassQuad, "\n %s", msg);
}

extern "C" void GRInitializeGrassQuad()
{
	GROpenLogFileGrassQuad();
	grvertexShaderObjectGrassQuad = glCreateShader(GL_VERTEX_SHADER);

	///// Vertex Shader
	const GLchar* grvertexShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec2 vTexCoord;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"out vec2 out_texcoord;" \
		"void main(void)" \
		"{" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"out_texcoord = vTexCoord;" \
		"}";

	glShaderSource(grvertexShaderObjectGrassQuad, 1, (const GLchar**)&grvertexShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grvertexShaderObjectGrassQuad);
	// error check for compilation
	GLint griInfoLength = 0;
	GLint griShaderCompileStatus = 0;
	char* grszInfoLog = NULL;

	glGetShaderiv(grvertexShaderObjectGrassQuad, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grvertexShaderObjectGrassQuad, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grvertexShaderObjectGrassQuad, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileGrassQuad, "\n Vertex Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				GRUninitializeGrassQuad();
			}
		}
	}

	////// Fragment Shader
	grfragmentShaderObjectGrassQuad = glCreateShader(GL_FRAGMENT_SHADER);

	// source code of shader
	const GLchar* grfragmentShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec2 out_texcoord;"
		"uniform sampler2D u_texture_sampler;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = texture(u_texture_sampler, out_texcoord);" \
		"}";

	glShaderSource(grfragmentShaderObjectGrassQuad, 1, (const GLchar**)&grfragmentShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grfragmentShaderObjectGrassQuad);
	// error check for compiation
	glGetShaderiv(grfragmentShaderObjectGrassQuad, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grfragmentShaderObjectGrassQuad, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grfragmentShaderObjectGrassQuad, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileGrassQuad, "\n Fragment Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				GRUninitializeGrassQuad();
			}
		}
	}

	//****** Shader Program *****//
	// create
	grshaderProgramObjectGrassQuad = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(grshaderProgramObjectGrassQuad, grvertexShaderObjectGrassQuad);

	// attach fragment shader to shader program
	glAttachShader(grshaderProgramObjectGrassQuad, grfragmentShaderObjectGrassQuad);

	// pre-link our attribute enum with shader's attributes
	glBindAttribLocation(grshaderProgramObjectGrassQuad, GR_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(grshaderProgramObjectGrassQuad, GR_ATTRIBUTE_TEXCOORD, "vTexCoord");

	// link shader
	glLinkProgram(grshaderProgramObjectGrassQuad);
	// error check for linking
	GLint griShaderProgramLinkStatus = 0;
	glGetProgramiv(grshaderProgramObjectGrassQuad, GL_LINK_STATUS, &griShaderProgramLinkStatus);
	if (griShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(grshaderProgramObjectGrassQuad, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(griInfoLength) * sizeof(char));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetProgramInfoLog(grshaderProgramObjectGrassQuad, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileGrassQuad, "\n Shader Program Link Log : %s", grszInfoLog);
				GRUninitializeGrassQuad();
			}
		}
	}

	// set unifrom attributes in shaders
	grmodelMatrixUniformGrassQuad = glGetUniformLocation(grshaderProgramObjectGrassQuad, "u_model_matrix");
	grviewMatrixUniformGrassQuad = glGetUniformLocation(grshaderProgramObjectGrassQuad, "u_view_matrix");
	grprojectionMatrixUniformGrassQuad = glGetUniformLocation(grshaderProgramObjectGrassQuad, "u_projection_matrix");
	grtextureSamplerUniformGrassQuad = glGetUniformLocation(grshaderProgramObjectGrassQuad, "u_texture_sampler");


	const GLfloat grcubeVertices[] =
	{

		40.0f, 63.0f, 0.0f,
		-40.0f, 63.0f, 0.0f,
		-40.0f, -63.0f, 0.0f,
		40.0f, -63.0f, 0.0f
	};
	const GLfloat grcubeTexCoords[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	//** initialize vao
	glGenVertexArrays(1, &grvaoGrassQuad);
	glBindVertexArray(grvaoGrassQuad);

	glGenBuffers(1, &grvboPositionGrassQuad);
	glBindBuffer(GL_ARRAY_BUFFER, grvboPositionGrassQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grcubeVertices), grcubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// color for square
	glGenBuffers(1, &grvboTextureGrassQuad);
	glBindBuffer(GL_ARRAY_BUFFER, grvboTextureGrassQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grcubeTexCoords), grcubeTexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_TEXCOORD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	LoadGLTextureGrassQuad(&grtextureGrassQuad, MAKEINTRESOURCE(GRTEXTURE_GRASSQUAD));


	grprojectionMatrixGrassQuad = mat4::identity();

}

extern "C" void GRResizeGrassQuad(int width, int height)
{
	grprojectionMatrixGrassQuad = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
}

extern "C" void GRDisplayGrassQuad()
{
	glUseProgram(grshaderProgramObjectGrassQuad);

	// OpenGL drawing code will start here
	mat4 grmodelMatrix = mat4::identity();
	mat4 grviewMatrix = mat4::identity();
	mat4 grprojectionMatrix = mat4::identity();
	mat4 grrotateMatrix = mat4::identity();
	mat4 grtranslateMatrix = mat4::identity();
	mat4 grscaleMatrix = mat4::identity();
	

#pragma region grass quad
	grtranslateMatrix = mat4::identity();
	grrotateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();
	grprojectionMatrix = mat4::identity();

	grtranslateMatrix = vmath::translate(grmodelX_GrassQuad, grmodelY_GrassQuad, grmodelZ_GrassQuad);
	grrotateMatrix = vmath::rotate(grangleX_GrassQuad, 0.0f, 1.0f, 0.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grangleZ_GrassQuad, 1.0f, 0.0f, 0.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grangleY_GrassQuad, 0.0f, 0.0f, 1.0f);
	

	grmodelMatrix = grtranslateMatrix * grrotateMatrix;
	grviewMatrix = grcameraMatrixGrassQuad;
	grprojectionMatrix = grprojectionMatrixGrassQuad;

	glUniformMatrix4fv(grmodelMatrixUniformGrassQuad, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grviewMatrixUniformGrassQuad, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grprojectionMatrixUniformGrassQuad, 1, GL_FALSE, grprojectionMatrix);

	// apply texture to cube
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grtextureGrassQuad);
	glUniform1i(grtextureSamplerUniformGrassQuad, 0);

	// bind vao of square
	glBindVertexArray(grvaoGrassQuad);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	// unbind vao
	glBindVertexArray(0);
#pragma endregion grass quad 

	// stop use of shader program
	glUseProgram(0);
}

extern "C" void GRUninitializeGrassQuad()
{
	if (grvaoGrassQuad)
	{
		glDeleteVertexArrays(1, &grvaoGrassQuad);
		grvaoGrassQuad = 0;
	}
	if (grvboPositionGrassQuad)
	{
		glDeleteBuffers(1, &grvboPositionGrassQuad);
		grvboPositionGrassQuad = 0;
	}
	if (grvboTextureGrassQuad)
	{
		glDeleteBuffers(1, &grvboTextureGrassQuad);
		grvboTextureGrassQuad = 0;
	}


	if (grtextureGrassQuad)
	{
		glDeleteTextures(1, &grtextureGrassQuad);
		grtextureGrassQuad = 0;
	}

	// free shader objects
	// detach vertex shader
	glDetachShader(grshaderProgramObjectGrassQuad, grvertexShaderObjectGrassQuad);
	// detach fragment shader
	glDetachShader(grshaderProgramObjectGrassQuad, grfragmentShaderObjectGrassQuad);

	glDeleteShader(grvertexShaderObjectGrassQuad);
	grvertexShaderObjectGrassQuad = 0;

	glDeleteShader(grfragmentShaderObjectGrassQuad);
	grfragmentShaderObjectGrassQuad = 0;

	glUseProgram(0);

	if (grgpFileGrassQuad)
	{
		fprintf(grgpFileGrassQuad, "\n **** End ****\nLog File closed successfully. \n Program terminated successfully");
		fclose(grgpFileGrassQuad);
		grgpFileGrassQuad = NULL;
	}


}

extern "C" void GRSetViewMatrixGrassQuad(vmath::vec3 cameraPos, vmath::vec3 cameraView, vmath::vec3 up)
{
	grcameraMatrixGrassQuad = vmath::lookat(cameraPos, cameraView, up);
}

extern "C" void GRIncAngleXGrassQuad()
{
	grangleX_GrassQuad = grangleX_GrassQuad + 0.1f;
}

extern "C" void GRIncAngleYGrassQuad()
{
	grangleY_GrassQuad = grangleY_GrassQuad + 0.1f;
}

extern "C" void GRIncAngleZGrassQuad()
{
	grangleZ_GrassQuad = grangleZ_GrassQuad + 0.1f;
}

extern "C" void GRIncModelXGrassQuad()
{
	grmodelX_GrassQuad = grmodelX_GrassQuad + 0.1f;
}

extern "C" void GRIncModelYGrassQuad()
{
	grmodelY_GrassQuad = grmodelY_GrassQuad + 0.1f;
}

extern "C" void GRIncModelZGrassQuad()
{
	grmodelZ_GrassQuad = grmodelZ_GrassQuad + 1.0f;
}

extern "C" void GRDecModelXGrassQuad()
{
	grmodelX_GrassQuad = grmodelX_GrassQuad - 0.1f;
}

extern "C" void GRDecModelYGrassQuad()
{
	grmodelY_GrassQuad = grmodelY_GrassQuad - 0.1f;
}

extern "C" void GRDecModelZGrassQuad()
{
	grmodelZ_GrassQuad = grmodelZ_GrassQuad - 1.0f;
}

extern "C" void GRPrintVarsGrassQuad()
{
	fprintf(grgpFileGrassQuad, "\n angle x : %f, angle y : %f, angle z : %f", grangleX_GrassQuad, grangleY_GrassQuad, grangleZ_GrassQuad);
	fprintf(grgpFileGrassQuad, "\n model x : %f, model y : %f, model z : %f", grmodelX_GrassQuad, grmodelY_GrassQuad, grmodelZ_GrassQuad);
}

extern "C" void GRSetVarsGrassQuad()
{
	
}

bool LoadGLTextureGrassQuad(GLuint* texture, TCHAR resourceID[])
{
	// variable declarations
	bool bResult = false;
	HBITMAP hBitmap = NULL;
	BITMAP bmp;

	//code
	// OS dependent code starts from here
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), resourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);		// cx and cy  is 0,0 for bitmap img, for icon, give width and height
	if (hBitmap)
	{
		bResult = true;
		GetObject(hBitmap, sizeof(bmp), &bmp);

		// from here starts OpenGL actual code
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		// setting of texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// MAG - Magnification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);				// MIN - Minification

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
		glGenerateMipmap(GL_TEXTURE_2D);

		DeleteObject(hBitmap);

	}

	return(bResult);

}