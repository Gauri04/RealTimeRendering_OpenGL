#include"GRSky.h"
using namespace vmath;

FILE* grgpFileSky = NULL;

#pragma region sky vars
// sky vars
GLuint grgVertexShaderObjectSky;
GLuint grgFragmentShadeerObjectSky;
GLuint grgShaderProgramObjectSky;
float grsphereVertices[1146];
float grsphereNormals[1146];
float grsphereTextures[764];
unsigned short grsphereElements[2280];
GLuint grgNumVertices;
GLuint grgNumElements;
GLuint grgVboSpherePosition;
GLuint grgVboSphereNormal;
GLuint grgVboSphereElement;
GLuint grgVaoSphere;
GLuint grgModelMatrixUniform;
GLuint grgViewMatrixUniform;
GLuint grgProjectionMatrixUniform;
GLuint grgLightPositionUniform;
GLuint grgScaleUniform;
GLuint grgTextureSamplerUniform;
GLuint grgTextureSamplerNoiseUniform;
GLuint grgSkyColorUniform;
GLuint grgCloudColorUniform;
GLuint grgResolutionUniform;
GLuint grgTimeUniform;

float grgScale = 2.0f;
float grgTime = 0.0f;

GLfloat grgViewX = 0.0f, grgViewY = 0.0f, grgViewZ = -20.0f;
mat4 grcameraMatrixSky = mat4::identity();
mat4 grprojectionMatrixSky = mat4::identity();
#pragma endregion sky vars

bool LoadGLTextureGrassQuad(GLuint*, TCHAR[]);

extern "C" void GROpenLogFileSky()
{
	if (fopen_s(&grgpFileSky, "GRLogSky.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot open GRLogGrassSky.txt file"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf_s(grgpFileSky, "Log file created successfully. \n Program started successfully\n **** Logs ***** \n");
	}
}

extern "C" void GRPrintLogSky(const char* msg)
{
	fprintf(grgpFileSky, "\n %s", msg);
}

extern "C" void GRInitializeSky()
{
	GROpenLogFileSky();
	// *************************************** Sky Shaders *********************************************

	getSphereVertexData(grsphereVertices, grsphereNormals, grsphereTextures, grsphereElements);
	grgNumVertices = getNumberOfSphereVertices();
	grgNumElements = getNumberOfSphereElements();

	// create shader
	grgVertexShaderObjectSky = glCreateShader(GL_VERTEX_SHADER);

	///// Vertex Shader
	const GLchar* grvertexShaderSourceCodeSky =					// also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"in vec3 vTexcoord;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec3 u_light_position;" \
		"uniform float u_scale;" \
		"out vec2 out_position;" \
		"void main(void)" \
		"{" \
		"out_position = vPosition.xy * u_scale;" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";

	glShaderSource(grgVertexShaderObjectSky, 1, (const GLchar**)&grvertexShaderSourceCodeSky, NULL);

	// compile shader
	glCompileShader(grgVertexShaderObjectSky);
	// error check for compilation
	int griInfoLength = 0;
	int griShaderCompileStatus = 0;
	char *grszInfoLog = NULL;

	glGetShaderiv(grgVertexShaderObjectSky, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgVertexShaderObjectSky, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgVertexShaderObjectSky, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileSky, "\n Vertex Shader Compilation Log for sky : %s", grszInfoLog);
				free(grszInfoLog);
				GRUninitializeSky();
			}
		}
	}

	////// Fragment Shader
	grgFragmentShadeerObjectSky = glCreateShader(GL_FRAGMENT_SHADER);

	// source code of shader
	const GLchar* grfragmentShaderSourceCodeSky =
		"#version 450 core" \
		"\n" \
		"in vec2 out_position;" \
		"uniform vec2 u_resolution;" \
		"uniform vec2 u_mouse;" \
		"uniform float u_time;" \
		"uniform float u_scale;" \
		"out vec4 FragColor;" \
		"int num_octaves = 5;" \
		"float random(vec2 st)" \
		"{" \
		"return(fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123));"
		"}" \
		"float noise(vec2 st)" \
		"{" \
		"vec2 i = floor(st);" \
		"vec2 f = fract(st);" \
		"float a = random(i);" \
		"float b = random(i + vec2(1.0, 0.0));" \
		"float c = random(i + vec2(0.0, 1.0));" \
		"float d = random(i + vec2(1.0, 1.0));" \
		"vec2 u = f * f * (3.0 - 2.0 * f);" \
		"return(mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y);" \
		"}" \
		"float fbm(vec2 st)" \
		"{" \
		"float v = 0.0;" \
		"float a = 0.5;" \
		"vec2 shift = vec2(100.0);" \
		"mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));" \
		"for(int i = 0; i < num_octaves; ++i)" \
		"{" \
		"v += a * noise(st);" \
		"st = rot * st * 2.0 + shift;" \
		"a *= 0.5;" \
		"}" \
		"return(v);" \
		"}" \
		"void main(void)" \
		"{" \
		"vec2 st = out_position;" \
		"vec3 color = vec3(0.0);"
		"vec2 q = vec2(0.0);" \
		"q.x = fbm(st + 0.0 * u_time);" \
		"q.y = fbm(st + vec2(1.0));" \
		"vec2 r = vec2(0.0);" \
		"r.x = fbm(st + 1.0 * q + vec2(1.7, 9.2) + 0.15 * u_time);" \
		"r.y = fbm(st + 1.0 * q + vec2(8.3, 2.8) + 0.126 * u_time);" \
		"float f = fbm(st + r);" \
		"color = mix(vec3(0.0, 0.0, 0.8), vec3(0.01, 0.01, 0.8), clamp((f * f) * 4.0, 0.0, 1.0));" \
		"color = mix(color, vec3(1.0, 1.0, 1.0), clamp(length(q), 0.0, 1.0));" \
		"color = mix(color, vec3(0.8, 0.8, 0.92), clamp(length(r.x), 0.0, 1.0));" \
		"FragColor = vec4(color, 1.0);" \
		"}";

	glShaderSource(grgFragmentShadeerObjectSky, 1, (const GLchar**)&grfragmentShaderSourceCodeSky, NULL);

	// compile shader
	glCompileShader(grgFragmentShadeerObjectSky);
	// error check for compiation
	glGetShaderiv(grgFragmentShadeerObjectSky, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgFragmentShadeerObjectSky, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgFragmentShadeerObjectSky, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileSky, "\n Fragment Shader Compilation Log for sky : %s", grszInfoLog);
				free(grszInfoLog);
				GRUninitializeSky();
			}
		}
	}

	//****** Shader Program *****
	// create
	grgShaderProgramObjectSky = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(grgShaderProgramObjectSky, grgVertexShaderObjectSky);

	// attach fragment shader to shader program
	glAttachShader(grgShaderProgramObjectSky, grgFragmentShadeerObjectSky);

	// pre-link our attribute enum with shader's attributes
	glBindAttribLocation(grgShaderProgramObjectSky, GR_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(grgShaderProgramObjectSky, GR_ATTRIBUTE_TEXCOORD, "vTexcoord");
	glBindAttribLocation(grgShaderProgramObjectSky, GR_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(grgShaderProgramObjectSky);
	// error check for linking
	int griShaderProgramLinkStatus = 0;
	glGetProgramiv(grgShaderProgramObjectSky, GL_LINK_STATUS, &griShaderProgramLinkStatus);
	if (griShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(grgShaderProgramObjectSky, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(griInfoLength) * sizeof(char));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetProgramInfoLog(grgShaderProgramObjectSky, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileSky, "\n Shader Program Link Log for sky : %s", grszInfoLog);
				GRUninitializeSky();
			}
		}
	}

	grgModelMatrixUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_model_matrix");
	grgViewMatrixUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_view_matrix");
	grgProjectionMatrixUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_projection_matrix");
	grgLightPositionUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_light_position");
	grgScaleUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_scale");
	grgTextureSamplerNoiseUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_noise");
	grgSkyColorUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_sky_color");
	grgCloudColorUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_cloud_color");
	grgResolutionUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_resolution");
	grgTimeUniform = glGetUniformLocation(grgShaderProgramObjectSky, "u_time");

#pragma region sky vao
	// sphere vao, vbo
	glGenVertexArrays(1, &grgVaoSphere);
	glBindVertexArray(grgVaoSphere);

	glGenBuffers(1, &grgVboSpherePosition);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboSpherePosition);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grsphereVertices), grsphereVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// normal for cube
	glGenBuffers(1, &grgVboSphereNormal);
	glBindBuffer(GL_ARRAY_BUFFER, grgVboSphereNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grsphereNormals), grsphereNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// element vbo
	glGenBuffers(1, &grgVboSphereElement);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grsphereElements), grsphereElements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
#pragma endregion sky vao


}

extern "C" void GRResizeSky(int width, int height)
{
	grprojectionMatrixSky = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
}

extern "C" void GRDisplaySky()
{

	mat4 grmodelMatrix = mat4::identity();
	mat4 grviewMatrix = mat4::identity();
	mat4 grprojectionMatrix = mat4::identity();
	mat4 grmodelViewProjectionMatrix = mat4::identity();
	mat4 grtranslateMatrix = mat4::identity();
	mat4 grrotateMatrix = mat4::identity();


	// **************************************** sky *****************************
	glUseProgram(grgShaderProgramObjectSky);
	vec3 greye = vec3(grgViewX, grgViewY, grgViewZ);
	vec3 grcenter = vec3(1.0f, 0.0f, 0.0f);
	vec3 grup = vec3(0.0f, 1.0f, 0.0f);
	vec3 grcloudColor = vec3(0.992f, 1.0f, 0.9608f);
	vec3 grskyColor = vec3(0.851f, 0.3765f, 0.2314f);
	mat4 grscaleMatrix = mat4::identity();
	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();
	grprojectionMatrix = mat4::identity();


	grtranslateMatrix = vmath::translate(0.0f, 0.0f, -35.0f);
	//grrotateMatrix = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
	grviewMatrix = vmath::lookat(greye, grcenter, grup);
	grscaleMatrix = vmath::scale(300.0f, 300.0f, 300.0f);
	grmodelMatrix = grtranslateMatrix * grscaleMatrix;
	grprojectionMatrix = grprojectionMatrixSky;

	glUniform1f(grgTimeUniform, grgTime);
	glUniform1f(grgScaleUniform, grgScale);

	glUniformMatrix4fv(grgModelMatrixUniform, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grgViewMatrixUniform, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grgProjectionMatrixUniform, 1, GL_FALSE, grprojectionMatrix);


	// bind vao
	glBindVertexArray(grgVaoSphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grgVboSphereElement);
	glDrawElements(GL_TRIANGLES, grgNumElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	// stop use of shader program
	glUseProgram(0);

	if (grgTime >= 360.0f)
		grgTime = 0.0f;
	grgTime = grgTime + 0.1f;
}

extern "C" void GRUninitializeSky()
{
	if (grgVaoSphere)
	{
		glDeleteVertexArrays(1, &grgVaoSphere);
		grgVaoSphere = 0;
	}
	
}

extern "C" void GRSetViewMatrixSky(vmath::vec3 cameraPos, vmath::vec3 cameraView, vmath::vec3 up)
{
	grcameraMatrixSky = vmath::lookat(cameraPos, cameraView, up);
}


extern "C" void GRIncViewXSky()
{
	grgViewX = grgViewX + 1.1f;
}

extern "C" void GRDecViewXSky()
{
	grgViewX = grgViewX - 1.1f;
}

extern "C" void GRIncViewYSky()
{
	grgViewY = grgViewY + 1.1f;
}

extern "C" void GRDecViewYSky()
{
	grgViewY = grgViewY - 1.1f;
}

extern "C" void GRIncViewZSky()
{
	grgViewZ = grgViewZ + 1.1f;
}

extern "C" void GRDecViewZSky()
{
	grgViewZ = grgViewZ - 1.1f;
}

extern "C" void GRIncScaleSky()
{
	grgScale = grgScale * 0.5f;
}

extern "C" void GRDecScaleSky()
{
	grgScale = grgScale / 0.5f;
}










