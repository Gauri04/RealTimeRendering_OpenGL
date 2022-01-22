#include"GRTerrain.h"
#include"ktx_loading_header.h"
using namespace vmath;

FILE* grgpFileTerrain = NULL;

#pragma region terrain
GLuint grgVertexShaderObject;
GLuint grgFragmentShadeerObject;
GLuint grgShaderProgramObject;
GLuint grgTesControlShaderObject;
GLuint grgTesEvaluationShaderObject;
GLuint grgGeometryShaderObject;

GLuint grgVao;
GLuint grgVbo_position;
GLuint grmodelMatrixUniformTerrain;
GLuint grviewMatrixUniformTerrain;
GLuint grprojectionMatrixUniformTerrain;
GLuint grgTextureTerragen1;
GLuint grgTextureTerragenColor;
GLuint tex_src;
GLuint tex_src2;
GLuint grgTextureSamplerUniformTexColor;
GLuint grgTextureSamplerUniformTexDisplacement;
GLuint grgDMapDepthUniform;
GLfloat grgDepth = 0.0f;
mat4 grgPerspectiveProjectionMatrix;
GLfloat x1 = 0.0f;
GLfloat gry1 = 0.0f;
GLfloat z1 = 0.0f;
GLfloat x2 = 0.0f;
GLfloat y2 = -1.0f;
GLfloat z2 = -6.0f;
bool grswitch;

GLuint grgTextureBlendMap;
GLuint grgTextureGrass;
GLuint grgTextureDirt;
GLuint grgTextureGrassFlowers;
GLuint grgTexturePath;

GLuint grgTextureSamplerUniformBlendMap;
GLuint grgTextureSamplerUniformGrass;
GLuint grgTextureSamplerUniformDirt;
GLuint grgTextureSamplerUniformGrassFlowers;
GLuint grgTextureSamplerUniformPath;
GLfloat grviewAngleX = 0.0f;
GLfloat grviewAngleY = 0.0f;
GLfloat grviewAngleZ = 0.0f;
GLfloat grviewEyeX = 0.0f, grviewEyeY = 0.0f, grviewEyeZ = 0.0f;
#pragma endregion terrain

bool LoadGLTexture(GLuint*, TCHAR[]);

extern "C" void GROpenLogFileTerrain()
{
	if (fopen_s(&grgpFileTerrain, "GRLogTerrain.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot open GRLogTerrain.txt file"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf_s(grgpFileTerrain, "Log file created successfully. \n Program started successfully\n **** Logs ***** \n");
	}
}

extern "C" void GRPrintLogTerrain(const char* msg)
{
	fprintf(grgpFileTerrain, "\n %s", msg);
}

extern "C" void GRInitializeTerrain()
{
	GROpenLogFileTerrain();

	grgVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	///// Vertex Shader
	const GLchar* grvertexShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"out VS_OUT" \
		"{" \
		"vec2 tc;" \
		"} vs_out;" \
		"void main(void)" \
		"{" \
		"const vec4 vertices[] = vec4[] (vec4(-1.5, -1.5, -0.5, 1.0)," \
		"vec4(1.5, -1.5, -0.5, 1.0)," \
		"vec4(-1.5, -1.5, 0.5, 1.0)," \
		"vec4(1.5, -1.5, 0.5, 1.0));" \
		"int x = gl_InstanceID & 127;" \
		"int y = gl_InstanceID >> 7;" \
		"vec2 offs = vec2(x, y);" \
		"vs_out.tc = (vertices[gl_VertexID].xz + offs + vec2(0.5)) / 128.0;" \
		"gl_Position = vertices[gl_VertexID] + vec4(float(x - 64), 0.0, float(y - 64), 0.0);" \
		"}";

	glShaderSource(grgVertexShaderObject, 1, (const GLchar**)&grvertexShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgVertexShaderObject);
	// error check for compilation
	GLint griInfoLength = 0;
	GLint griShaderCompileStatus = 0;
	char* grszInfoLog = NULL;

	glGetShaderiv(grgVertexShaderObject, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgVertexShaderObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgVertexShaderObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileTerrain, "\n Vertex Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);

			}
		}
		GRUninitializeTerrain();
	}

	// Tescellation control shader
	// create shader
	grgTesControlShaderObject = glCreateShader(GL_TESS_CONTROL_SHADER);

	///// Tes control Shader
	const GLchar* grtesControlShaderSourceCode =					// also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
		"#version 430 core" \
		"\n" \
		"layout(vertices = 4) out;" \
		"in VS_OUT" \
		"{" \
		"vec2 tc;"
		"} tcs_in[];" \
		"out TCS_OUT" \
		"{" \
		"vec2 tc;" \
		"} tcs_out[];" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"int i = 0;" \
		"void main(void)" \
		"{" \
		"if(gl_InvocationID == 0)" \
		"{" \
		"mat4 vp_matrix = u_projection_matrix * u_view_matrix;" \
		"vec4 p0 = vp_matrix * gl_in[0].gl_Position;" \
		"vec4 p1 = vp_matrix * gl_in[1].gl_Position;" \
		"vec4 p2 = vp_matrix * gl_in[2].gl_Position;" \
		"vec4 p3 = vp_matrix * gl_in[3].gl_Position;" \
		"p0 /= p0.w;" \
		"p1 /= p1.w;" \
		"p2 /= p2.w;" \
		"p3 /= p3.w;" \
		"if(p0.z <= 0.0 || p1.z <= 0.0 || p2.z <= 0.0 || p3.z <= 0.0)" \
		"{" \
		"gl_TessLevelOuter[0] = 0.0;" \
		"gl_TessLevelOuter[1] = 0.0;" \
		"gl_TessLevelOuter[2] = 0.0;" \
		"gl_TessLevelOuter[3] = 0.0;" \
		"}" \
		"else" \
		"{" \
		"float l0 = length(p2.xy - p0.xy) * 16.0 + 1.0;" \
		"float l1 = length(p3.xy - p2.xy) * 16.0 + 1.0;" \
		"float l2 = length(p3.xy - p1.xy) * 16.0 + 1.0;" \
		"float l3 = length(p1.xy - p0.xy) * 16.0 + 1.0;" \
		"gl_TessLevelOuter[0] = l0;" \
		"gl_TessLevelOuter[1] = l1;" \
		"gl_TessLevelOuter[2] = l2;" \
		"gl_TessLevelOuter[3] = l3;" \
		"gl_TessLevelInner[0] = min(l1, l3);" \
		"gl_TessLevelInner[1] = min(l0, l2);" \
		"}" \
		"}" \
		"gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;" \
		"tcs_out[gl_InvocationID].tc = tcs_in[gl_InvocationID].tc;" \
		"}";


	glShaderSource(grgTesControlShaderObject, 1, (const GLchar**)&grtesControlShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgTesControlShaderObject);
	// error check for compilation
	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	glGetShaderiv(grgTesControlShaderObject, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgTesControlShaderObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgTesControlShaderObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileTerrain, "\n TessilationControl Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				GRUninitializeTerrain();
			}
		}
	}

	////// Tessilation evaluation shader
	// create shader
	grgTesEvaluationShaderObject = glCreateShader(GL_TESS_EVALUATION_SHADER);

	///// Tes control Shader
	const GLchar* grtesEvaluationShaderSourceCode =					// also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
		"#version 430 core" \
		"\n" \
		"layout(quads, fractional_odd_spacing) in;" \
		"uniform sampler2D tex_displacement;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform float dmap_depth;" \
		"in TCS_OUT" \
		"{" \
		"vec2 tc;" \
		"} tes_in[];" \
		"out TES_OUT" \
		"{" \
		"vec2 tc;" \
		"} tes_out1;" \
		"int i = 0;" \
		"void main(void)" \
		"{" \
		"vec2 tc1 = mix(tes_in[0].tc, tes_in[1].tc, gl_TessCoord.x);" \
		"vec2 tc2 = mix(tes_in[2].tc, tes_in[3].tc, gl_TessCoord.x);" \
		"vec2 tc = mix(tc2, tc1, gl_TessCoord.y);" \
		"vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);" \
		"vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);" \
		"vec4 p = mix(p2, p1, gl_TessCoord.y);" \
		"p.y += texture(tex_displacement, tc).r * dmap_depth;" \
		"gl_Position = u_projection_matrix * u_view_matrix * p;" \
		"tes_out1.tc = tc;" \
		"}";

	glShaderSource(grgTesEvaluationShaderObject, 1, (const GLchar**)&grtesEvaluationShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgTesEvaluationShaderObject);
	// error check for compilation
	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	glGetShaderiv(grgTesEvaluationShaderObject, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgTesEvaluationShaderObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgTesEvaluationShaderObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileTerrain, "\n TessilationEval Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				GRUninitializeTerrain();
			}
		}
	}

	/*
	/////// Geometry shader
	grgGeometryShaderObject = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* grgeometryShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"layout(triangles, invocations = 2)in;" \
		"layout(triangle_strip,max_vertices=3)out;" \
		"uniform mat4 u_mvpMatrix;" \
		"in TESS_OUT" \
		"{" \
		"vec2 tc;" \
		"vec3 out_rand_pos[2];" \
		"} gs_in[];" \
		"out GS_OUT" \
		"{" \
		"vec2 tc;" \
		"}gs_out;" \
		"void main(void)" \
		"{" \
		"gs_out.tc = gs_in[gl_InvocationID].tc;" \
		"vec3 xyz_pos = gs_in[gl_InvocationID].out_rand_pos[gl_InvocationID];"
		"gl_Position =  (gl_in[0].gl_Position + vec4(xyz_pos, 0.0));" \
		"EmitVertex();" \
		"gl_Position =  (gl_in[1].gl_Position + vec4(xyz_pos, 0.0));" \
		"EmitVertex();" \
		"gl_Position =  (gl_in[2].gl_Position + vec4(xyz_pos, 0.0));" \
		"EmitVertex();" \
		"EndPrimitive();" \
		"}";

	glShaderSource(grgGeometryShaderObject, 1, (const GLchar**)&grgeometryShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgGeometryShaderObject);
	// error check for compilation
	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	glGetShaderiv(grgGeometryShaderObject, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgGeometryShaderObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgGeometryShaderObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileTerrain, "\n Geometry Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				Uninitialize();
			}
		}
	}
	*/

	////// Fragment Shader
	grgFragmentShadeerObject = glCreateShader(GL_FRAGMENT_SHADER);

	// source code of shader
	const GLchar* grfragmentShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"out vec4 FragColor;" \
		"uniform sampler2D tex_color;" \
		"uniform sampler2D rtexture;" \
		"uniform sampler2D gtexture;" \
		"uniform sampler2D btexture;" \
		"uniform sampler2D blendMap;" \
		"in TES_OUT" \
		"{" \
		"vec2 tc;" \
		"} fs_in;" \
		"void main(void)" \
		"{" \
		"vec4 blendMapColor = texture(blendMap, fs_in.tc);" \
		"float backTextureAmount = 1 - (blendMapColor.r, blendMapColor.g, blendMapColor.b);" \
		"vec2 tileCoords = fs_in.tc * 5.0;" \
		"vec4 backgroundTextureColor = texture(tex_color, tileCoords) * backTextureAmount;" \
		"vec4 rTextureColor = texture(rtexture, tileCoords) * blendMapColor.r;" \
		"vec4 gTextureColor = texture(gtexture, tileCoords) * blendMapColor.g;" \
		"vec4 bTextureColor = texture(btexture, tileCoords) * blendMapColor.b;" \
		"vec4 totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;" \
		"FragColor = vec4(totalColor);" \
		/*"FragColor = texture(tex_color, fs_in.tc);" \*/
		"}";

	glShaderSource(grgFragmentShadeerObject, 1, (const GLchar**)&grfragmentShaderSourceCode, NULL);

	// compile shader
	glCompileShader(grgFragmentShadeerObject);

	griInfoLength = 0;
	griShaderCompileStatus = 0;
	grszInfoLog = NULL;

	// error check for compiation
	glGetShaderiv(grgFragmentShadeerObject, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgFragmentShadeerObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgFragmentShadeerObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileTerrain, "\n Fragment Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				GRUninitializeTerrain();
			}
		}
	}

	//****** Shader Program *****//
	// create
	grgShaderProgramObject = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(grgShaderProgramObject, grgVertexShaderObject);
	glAttachShader(grgShaderProgramObject, grgTesControlShaderObject);
	glAttachShader(grgShaderProgramObject, grgTesEvaluationShaderObject);
	//glAttachShader(grgShaderProgramObject, grgGeometryShaderObject);
	// attach fragment shader to shader program
	glAttachShader(grgShaderProgramObject, grgFragmentShadeerObject);

	// pre-link our attribute enum with shader's attributes
	glBindAttribLocation(grgShaderProgramObject, GR_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(grgShaderProgramObject, GR_ATTRIBUTE_TEXCOORD, "vTexCoord");
	glBindAttribLocation(grgShaderProgramObject, GR_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(grgShaderProgramObject);
	// error check for linking
	GLint griShaderProgramLinkStatus = 0;
	glGetProgramiv(grgShaderProgramObject, GL_LINK_STATUS, &griShaderProgramLinkStatus);
	if (griShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(grgShaderProgramObject, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(griInfoLength) * sizeof(char));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetProgramInfoLog(grgShaderProgramObject, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileTerrain, "\n Shader Program Link Log : %s", grszInfoLog);
				GRUninitializeTerrain();
			}
		}
	}

	grmodelMatrixUniformTerrain = glGetUniformLocation(grgShaderProgramObject, "u_model_matrix");
	grviewMatrixUniformTerrain = glGetUniformLocation(grgShaderProgramObject, "u_view_matrix");
	grprojectionMatrixUniformTerrain = glGetUniformLocation(grgShaderProgramObject, "u_projection_matrix");
	grgTextureSamplerUniformTexColor = glGetUniformLocation(grgShaderProgramObject, "tex_color");
	grgTextureSamplerUniformTexDisplacement = glGetUniformLocation(grgShaderProgramObject, "tex_displacement");
	grgDMapDepthUniform = glGetUniformLocation(grgShaderProgramObject, "dmap_depth");
	grgTextureSamplerUniformGrass = glGetUniformLocation(grgShaderProgramObject, "gtexture");
	grgTextureSamplerUniformGrassFlowers = glGetUniformLocation(grgShaderProgramObject, "rtexture");
	grgTextureSamplerUniformPath = glGetUniformLocation(grgShaderProgramObject, "btexture");
	grgTextureSamplerUniformBlendMap = glGetUniformLocation(grgShaderProgramObject, "blendMap");

	grgTextureTerragen1 = load("terragen1.ktx");
	//grgTextureTerragenColor = load("terragen_color.ktx");
	//LoadGLTexture(&grgTextureTerragen1, MAKEINTRESOURCE(GRTERRAIN_GEN));
	LoadGLTexture(&grgTextureTerragenColor, MAKEINTRESOURCE(GRTERRAIN_COLOR));
	LoadGLTexture(&grgTextureBlendMap, MAKEINTRESOURCE(GRTERRAIN_BLENDMAP));
	LoadGLTexture(&grgTextureGrass, MAKEINTRESOURCE(GRTERRAIN_GRASS));
	LoadGLTexture(&grgTextureDirt, MAKEINTRESOURCE(GRTERRAIN_DIRT));
	LoadGLTexture(&grgTextureGrassFlowers, MAKEINTRESOURCE(GRTERRAIN_GRSSSFLOWERS));
	LoadGLTexture(&grgTexturePath, MAKEINTRESOURCE(GRTERRAIN_PATH));

	grgPerspectiveProjectionMatrix = mat4::identity();
	GRSetVarsTerrain();
}

extern "C" void GRResizeTerrain(int width, int height)
{
	grgPerspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
}


extern "C" void GRDisplayTerrain()
{
	mat4 grmodelMatrix = mat4::identity();
	mat4 grviewMatrix = mat4::identity();
	mat4 grprojectionMatrix = mat4::identity();
	mat4 grmodelViewProjectionMatrix = mat4::identity();
	mat4 grtranslateMatrix = mat4::identity();
	mat4 grrotateMatrix = mat4::identity();

	glUseProgram(grgShaderProgramObject);
	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();

	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -20.0f);
	//grrotateMatrix = vmath::rotate(180.0f, 0.0f, 0.0f, 1.0f);
	grmodelMatrix = grtranslateMatrix * grrotateMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrix;

	grviewEyeX = -61.292156f;
	//grviewEyeX = x2 * sin(grviewAngleX);
	grviewEyeY = y2 * sin(grviewAngleY);
	grviewEyeZ = -6.0f + z2;

	grviewMatrix = vmath::lookat(vec3(x2, y2, z2), vec3(grviewEyeX, grviewEyeY, grviewEyeZ), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(grmodelMatrixUniformTerrain, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grviewMatrixUniformTerrain, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grprojectionMatrixUniformTerrain, 1, GL_FALSE, grprojectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grgTextureTerragen1);
	glUniform1i(grgTextureSamplerUniformTexDisplacement, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grgTextureTerragenColor);
	glUniform1i(grgTextureSamplerUniformTexColor, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, grgTextureGrass);
	glUniform1i(grgTextureSamplerUniformGrass, 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, grgTextureGrassFlowers);
	glUniform1i(grgTextureSamplerUniformGrassFlowers, 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, grgTextureBlendMap);
	glUniform1i(grgTextureSamplerUniformBlendMap, 4);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, grgTexturePath);
	glUniform1i(grgTextureSamplerUniformPath, 5);



	glUniform1f(grgDMapDepthUniform, grgDepth);

	// bind vao
	//glBindVertexArray(grgVao);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 128 * 128);
	glBindTexture(GL_TEXTURE_2D, NULL);
	// unbind vao
	//glBindVertexArray(0);

	glUseProgram(0);

}

extern "C" void GRUninitializeTerrain()
{
	if (grgVao)
	{
		glDeleteVertexArrays(1, &grgVao);
		grgVao = 0;
	}

	if (grgVbo_position)
	{
		glDeleteBuffers(1, &grgVbo_position);
		grgVbo_position = 0;
	}

	// free shader objects
	// detach vertex shader
	glDetachShader(grgShaderProgramObject, grgVertexShaderObject);
	glDetachShader(grgShaderProgramObject, grgTesControlShaderObject);
	glDetachShader(grgShaderProgramObject, grgTesEvaluationShaderObject);
	// detach fragment shader
	glDetachShader(grgShaderProgramObject, grgFragmentShadeerObject);


	// delete vertex object
	glDeleteShader(grgVertexShaderObject);
	grgVertexShaderObject = 0;

	glDeleteShader(grgTesControlShaderObject);
	grgTesControlShaderObject = 0;

	glDeleteShader(grgTesEvaluationShaderObject);
	grgTesEvaluationShaderObject = 0;

	// delete fragment shader object
	glDeleteShader(grgFragmentShadeerObject);
	grgFragmentShadeerObject = 0;

	// unlink shader program
	glUseProgram(0);

	if (grgpFileTerrain)
	{
		fprintf(grgpFileTerrain, "\n **** End ****\nLog File closed successfully. \n Program terminated successfully");
		fclose(grgpFileTerrain);
		grgpFileTerrain = NULL;
	}
}


extern "C" void GRIncViewZTerrain()
{
	z2 = z2 - 1.1f;
}

extern "C" void GRDecViewZTerrain()
{
	z2 = z2 + 1.1f;
}

extern "C" void GRIncViewXTerrain()
{
	x2 = x2 + 1.1f;
}

extern "C" void GRDecViewXTerrain()
{
	x2 = x2 - 1.1f;
}

extern "C" void GRIncViewYTerrain()
{
	y2 = y2 + 1.1f;
}

extern "C" void GRDecViewYTerrain()
{
	y2 = y2 - 1.1f;
}

extern "C" void GRPrintVarsTerrain()
{
	fprintf(grgpFileTerrain, "\n terrain X : %f, terrainY : %f, terrainZ : %f", x2, y2, z2);
	fprintf(grgpFileTerrain, "\n view eye terrain X : %f, terrainY : %f, terrainZ : %f", grviewEyeX, grviewEyeY, grviewEyeZ);
}

extern "C" void GRIncDepthTerrain()
{
	grgDepth = grgDepth + 1.2f;
}

extern "C" void GRDecDepthTerrain()
{
	grgDepth = grgDepth - 1.02f;
}

extern "C" void GRGetViewXYZTerrain(float& x, float& y, float& z)
{
	x = x2;
	y = y2;
	z = z2;
}

extern "C" void GRGetViewEyeXYZTerrain(float& x, float& y, float& z)
{
	x = grviewEyeX;
	y = grviewEyeY;
	z = grviewEyeZ;
}
extern "C" void GRSetVarsTerrain()
{
	x2 = -68.199959f;
	y2 = -1.000000f;
	z2 = 18.200003f;
	grviewEyeX = -61.292156f;
	
}

extern "C" void GRIncAngleX()
{
	grviewAngleX = grviewAngleX + 0.05f;
}

extern "C" void GRIncAngleY()
{
	grviewAngleY = grviewAngleY + 1.0f;
}
extern "C" void GRIncAngleZ()
{
	grviewAngleZ = grviewAngleZ + 1.0f;
}

bool LoadGLTexture(GLuint* texture, TCHAR resourceID[])
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
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		// setting of texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// MAG - Magnification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);				// MIN - Minification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
		glGenerateMipmap(GL_TEXTURE_2D);

		DeleteObject(hBitmap);

	}

	return(bResult);

}


















