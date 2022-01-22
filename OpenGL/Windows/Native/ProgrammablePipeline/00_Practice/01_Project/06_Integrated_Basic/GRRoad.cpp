#include"GRRoad.h"
#include"GRStack.h"

using namespace vmath;

/*
float grangleX_Road, grangleY_Road, grangleZ_Road;
FILE* grgpFileRoad = NULL;
mat4 grgPerspectiveProjectionMatrixRoad;
GLuint grgVertexShaderObjectRoad;
GLuint grgFragmentShadeerObjectRoad;
GLuint grgShaderProgramObjectRoad;

GLuint grgVboSquarePositionRoad;
GLuint grgVboSquareNormalRoad;
GLuint grgVboSquareTexCoordRoad;
GLuint grgVaoSquareRoad;
GLuint grgVboSquareTangentRoad;

// uniforms
GLuint grgModelMarixUniformRoad;
GLuint grgViewMatrixUniformRoad;
GLuint grgProjectionMatrixUniformRoad;
GLuint grgLightPositionUniformRoad;
GLuint grgTextureSamplerUniformRoad;
GLuint grgNormalUniformRoad;
GLuint grgTextureRoad;
GLuint grgTextureNormalRoad;

vec4 grgLightPosition;
*/

 float grangleX_Road = 0.0f, grangleY_Road = 0.0f, grangleZ_Road = 0.0f;
 float grmodelX_Road = 0.0f, grmodelY_Road = 0.0f, grmodelZ_Road = 0.0f;
 FILE* grgpFileRoad = NULL;
 mat4 grgPerspectiveProjectionMatrixRoad;
 GLuint grgVertexShaderObjectRoad;
 GLuint grgFragmentShadeerObjectRoad;
 GLuint grgShaderProgramObjectRoad;
 GLuint grgVertexShaderObjectPavement;
 GLuint grgFragmentShadeerObjectPavement;
 GLuint grgShaderProgramObjectPavement;

 GLuint grgVboSquarePositionRoad;
 GLuint grgVboSquareNormalRoad;
 GLuint grgVboSquareTexCoordRoad;
 GLuint grgVaoSquareRoad;
 GLuint grgVboSquareTangentRoad;
 GLuint grgVaoPavement;
 GLuint grgVboPositionPavement;
 GLuint grgVboTexturePavement;

// uniforms
 GLuint grgModelMatrixUniformRoad;
 GLuint grgViewMatrixUniformRoad;
 GLuint grgProjectionMatrixUniformRoad;
 GLuint grgLightPositionUniformRoad;
 GLuint grgTextureSamplerUniformRoad;
GLuint grgNormalUniformRoad;
 GLuint grgTextureRoad;
 GLuint grgTextureNormalRoad;
 GLuint grgTextureFootpath;
 GLuint grgTextureNormalFootpath;
 GLuint grgTexturePavement;
 GLuint grgTextureSamplerUniformPavement;
 mat4 grgCameraMatrixRoad = mat4::identity();

 vec4 grgLightPosition = { 0.0f, 0.0f, 0.0f, 0.0 };			// 1.0 (W) for positional light

 bool LoadGLTextureRoad(GLuint* texture, TCHAR resourceID[]);

 extern "C" void GROpenLogFileRoad()
 {
	 if (fopen_s(&grgpFileRoad, "GRLogRoad.txt", "w") != 0)
	 {
		 MessageBox(NULL, TEXT("Cannot open desired file"), TEXT("Error"), MB_OK | MB_ICONERROR);
		 exit(0);
	 }
	 else
	 {
		 fprintf_s(grgpFileRoad, "Log file created successfully. \n Program started successfully\n **** Logs ***** \n");
	 }
 }

 extern "C" void GRPrintLog(const char* msg)
 {
	 fprintf(grgpFileRoad, msg);
 }

 extern "C" void GRInitializeRoad()
 {
	 GROpenLogFileRoad();
	 grgVertexShaderObjectRoad = glCreateShader(GL_VERTEX_SHADER);

	 const GLchar* grvertexShaderSourceCode =
		 "#version 450 core" \
		 "\n" \
		 "in vec4 vPosition;" \
		 "in vec3 vNormal;" \
		 "in vec3 vTangent;" \
		 "in vec2 vTexCoord;" \
		 "uniform mat4 u_model_matrix;" \
		 "uniform mat4 u_view_matrix;" \
		 "uniform mat4 u_projection_matrix;" \
		 "uniform vec4 u_light_position;" \
		 "out vec2 out_texcoord;" \
		 "out vec3 out_light_direction;" \
		 "out vec3 out_eye_coordinates;" \
		 "out vec3 out_rim_col;" \
		 "void main(void)" \
		 "{" \
		 "vec4 P = u_view_matrix * u_model_matrix * vPosition;" \
		 "vec3 N = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
		 "vec3 T = normalize(mat3(u_view_matrix * u_model_matrix) * vTangent);" \
		 "vec3 B = cross(N, T);" \
		 "vec3 L = vec3(u_light_position) - P.xyz;" \
		 "vec3 V = -P.xyz;" \
		 "out_light_direction = normalize(vec3(dot(L, T), dot(L, B), dot(L, N)));" \
		 "out_eye_coordinates = normalize(vec3(dot(V, T), dot(V, B), dot(V, N)));" \
		 "out_texcoord = vTexCoord;" \
		 "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		 "}";

	 /*
	 const GLchar* grvertexShaderSourceCode =
		 "#version 450 core" \
		 "\n" \
		 "in vec4 vPosition;" \
		 "in vec3 vNormal;" \
		 "in vec3 vTangent;" \
		 "in vec2 vTexCoord;" \
		 "uniform mat4 u_model_matrix;" \
		 "uniform mat4 u_view_matrix;" \
		 "uniform mat4 u_projection_matrix;" \
		 "uniform vec4 u_light_position;" \
		 "out vec2 out_texcoord;" \
		 "out vec3 out_tangent_view_pos;" \
		 "out vec3 out_tangent_light_pos;" \
		 "out vec3 out_frag_pos;" \
		 "out vec3 out_tangent_frag_pos;" \
		 "out vec3 out_view_position;" \
		 "void main(void)" \
		 "{" \
		 "mat3 normal_matrix = transpose(inverse(mat3(u_model_matrix)));" \
		 "vec3 T = normalize(normal_matrix * vTangent);" \
		 "vec3 N = normalize(normal_matrix * vNormal);" \
		 "T = normalize(T - dot(T, N) * N);" \
		 "vec3 B = cross(N, T);" \
		 "mat3 TBN = transpose(mat3(T, B, N));" \
		 "out_frag_pos = vec3(u_model_matrix * vPosition);" \
		 "out_tangent_light_pos = TBN * vec3(u_light_position);" \
		 "out_tangent_view_pos = TBN * vec3(u_view_matrix * u_model_matrix * vPosition);" \
		 "out_tangent_frag_pos = TBN * out_frag_pos;" \
		 "out_texcoord = vTexCoord;" \
		 "out_view_position = vec3(u_view_matrix * u_model_matrix * vPosition);" \
		 "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		 "}";
		 */

	 glShaderSource(grgVertexShaderObjectRoad, 1, (const GLchar**)&grvertexShaderSourceCode, NULL);

	 // compile shader
	 glCompileShader(grgVertexShaderObjectRoad);
	 // error check for compilation
	 GLint griInfoLength = 0;
	 GLint griShaderCompileStatus = 0;
	 char* grszInfoLog = NULL;

	 glGetShaderiv(grgVertexShaderObjectRoad, GL_COMPILE_STATUS, &griShaderCompileStatus);
	 if (griShaderCompileStatus == GL_FALSE)
	 {
		 glGetShaderiv(grgVertexShaderObjectRoad, GL_INFO_LOG_LENGTH, &griInfoLength);
		 if (griInfoLength > 0)
		 {
			 grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			 if (grszInfoLog != NULL)
			 {
				 GLsizei grwritten;
				 glGetShaderInfoLog(grgVertexShaderObjectRoad, griInfoLength, &grwritten, grszInfoLog);
				 fprintf(grgpFileRoad, "\n Vertex Shader Compilation Log : %s", grszInfoLog);
				 free(grszInfoLog);

			 }
		 }
		 GRUninitializeRoad();
	 }

	 ////// Fragment Shader
	 grgFragmentShadeerObjectRoad = glCreateShader(GL_FRAGMENT_SHADER);

	 const GLchar* grfragmentShaderSourceCode =
		 "#version 450 core" \
		 "\n" \
		 "uniform sampler2D u_tex_color;" \
		 "uniform sampler2D u_tex_normal;" \
		 "in vec2 out_texcoord;" \
		 "in vec3 out_light_direction;" \
		 "in vec3 out_eye_coordinates;" \
		 "in vec3 out_rim_col;" \
		 "out vec4 FragColor;" \
		 "void main(void)" \
		 "{" \
		 "vec3 V = normalize(out_eye_coordinates);" \
		 "vec3 L = normalize(out_light_direction);" \
		 "vec3 N = normalize(texture(u_tex_normal, out_texcoord).rgb * 2.0 - vec3(1.0));" \
		 "vec3 R = reflect(-L, N);" \
		 "vec3 color = texture(u_tex_color, out_texcoord).rgb;"
		 "vec3 ambient = 0.1 * color;" \
		 "vec3 diffuse_albedo = texture(u_tex_color, out_texcoord).rgb;" \
		 "vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;" \
		 "vec3 specular_albedo = vec3(0.5);" \
		 "vec3 specular = max(pow(dot(R, V), 20.0), 0.0) * specular_albedo;" \
		 "FragColor =  vec4(ambient + diffuse + specular, 1.0);" \
		 "}";
	 /*
	 const GLchar* grfragmentShaderSourceCode =
		 "#version 450 core" \
		 "\n" \
		 "uniform sampler2D u_tex_color;" \
		 "uniform sampler2D u_tex_normal;" \
		 "in vec2 out_texcoord;" \
		 "in vec3 out_tangent_view_pos;" \
		 "in vec3 out_tangent_light_pos;" \
		 "in vec3 out_frag_pos;" \
		 "in vec3 out_tangent_frag_pos;" \
		 "uniform vec4 u_light_position;" \
		 "in vec3 out_view_position;" \
		 "out vec4 FragColor;" \
		 "void main(void)" \
		 "{" \
		 "vec3 normal = texture(u_tex_normal, out_texcoord).rgb;" \
		 "normal = normalize(normal * 2.0 - 1.0);" \
		 "vec3 color = texture(u_tex_color, out_texcoord).rgb;" \
		 "vec3 ambient = 0.1 * color;" \
		 "vec3 lightDir = normalize(out_tangent_light_pos - out_tangent_frag_pos);" \
		 "float diff = max(dot(lightDir, normal), 0.0);" \
		 "vec3 diffuse = diff * color;" \
		 "vec3 viewDir = normalize(out_tangent_view_pos - out_tangent_frag_pos);" \
		 "vec3 reflectDir = reflect(-lightDir, normal);" \
		 "vec3 halfwayDir = normalize(lightDir + viewDir);" \
		 "float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);" \
		 "vec3 specular = vec3(0.2) * spec;" \
		 "FragColor = vec4(ambient + diffuse + specular, 1.0);" \
		 "}";
	 */
	 glShaderSource(grgFragmentShadeerObjectRoad, 1, (const GLchar**)&grfragmentShaderSourceCode, NULL);

	 // compile shader
	 glCompileShader(grgFragmentShadeerObjectRoad);
	 // error check for compiation
	 glGetShaderiv(grgFragmentShadeerObjectRoad, GL_COMPILE_STATUS, &griShaderCompileStatus);
	 if (griShaderCompileStatus == GL_FALSE)
	 {
		 glGetShaderiv(grgFragmentShadeerObjectRoad, GL_INFO_LOG_LENGTH, &griInfoLength);
		 if (griInfoLength > 0)
		 {
			 grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			 if (grszInfoLog != NULL)
			 {
				 GLsizei grwritten;
				 glGetShaderInfoLog(grgFragmentShadeerObjectRoad, griInfoLength, &grwritten, grszInfoLog);
				 fprintf(grgpFileRoad, "\n Fragment Shader Compilation Log : %s", grszInfoLog);
				 free(grszInfoLog);

			 }
		 }
		 GRUninitializeRoad();
	 }

	 //****** Shader Program *****//
	 // create
	 grgShaderProgramObjectRoad = glCreateProgram();

	 // attach vertex shader to shader program
	 glAttachShader(grgShaderProgramObjectRoad, grgVertexShaderObjectRoad);

	 // attach fragment shader to shader program
	 glAttachShader(grgShaderProgramObjectRoad, grgFragmentShadeerObjectRoad);

	 // pre-link our attribute enum with shader's attributes
	 glBindAttribLocation(grgShaderProgramObjectRoad, GR_ATTRIBUTE_POSITION, "vPosition");
	 glBindAttribLocation(grgShaderProgramObjectRoad, GR_ATTRIBUTE_NORMAL, "vNormal");
	 glBindAttribLocation(grgShaderProgramObjectRoad, GR_ATTRIBUTE_TEXCOORD, "vTexCoord");
	 glBindAttribLocation(grgShaderProgramObjectRoad, GR_ATTRIBUTE_TANGENT, "vTangent");

	 // link shader
	 glLinkProgram(grgShaderProgramObjectRoad);
	 // error check for linking
	 GLint griShaderProgramLinkStatus = 0;
	 glGetProgramiv(grgShaderProgramObjectRoad, GL_LINK_STATUS, &griShaderProgramLinkStatus);
	 if (griShaderProgramLinkStatus == GL_FALSE)
	 {
		 glGetProgramiv(grgShaderProgramObjectRoad, GL_INFO_LOG_LENGTH, &griInfoLength);
		 if (griInfoLength > 0)
		 {
			 grszInfoLog = (char*)malloc(sizeof(griInfoLength) * sizeof(char));
			 if (grszInfoLog != NULL)
			 {
				 GLsizei grwritten;
				 glGetProgramInfoLog(grgShaderProgramObjectRoad, griInfoLength, &grwritten, grszInfoLog);
				 fprintf(grgpFileRoad, "\n Shader Program Link Log : %s", grszInfoLog);
				 GRUninitializeRoad();
			 }
		 }
	 }

	 grgModelMatrixUniformRoad = glGetUniformLocation(grgShaderProgramObjectRoad, "u_model_matrix");
	 grgViewMatrixUniformRoad = glGetUniformLocation(grgShaderProgramObjectRoad, "u_view_matrix");
	 grgProjectionMatrixUniformRoad = glGetUniformLocation(grgShaderProgramObjectRoad, "u_projection_matrix");
	 grgLightPositionUniformRoad = glGetUniformLocation(grgShaderProgramObjectRoad, "u_light_position");
	 grgTextureSamplerUniformRoad = glGetUniformLocation(grgShaderProgramObjectRoad, "u_tex_color");
	 grgNormalUniformRoad = glGetUniformLocation(grgShaderProgramObjectRoad, "u_tex_normal");



	 vec3 tangent1;
	 vec3 bitangent1;
	 vec3 tangent2;
	 vec3 bitangent2;
	 vec3 edge1, edge2;
	 vec3 pos1, pos2, pos3, pos4;
	 vec2 uv1, uv2, uv3, uv4;

	 pos1 = vec3(-1.0f, 2.0f, 0.0f);
	 pos2 = vec3(-1.0f, -2.0f, 0.0f);
	 pos3 = vec3(1.0f, -2.0f, 0.0f);
	 pos4 = vec3(1.0f, 2.0f, 0.0f);
	 uv1 = vec2(0.0f, 1.0f);
	 uv2 = vec2(0.0f, 0.0f);
	 uv3 = vec2(1.0f, 0.0f);
	 uv4 = vec2(1.0f, 1.0f);

	 edge1 = pos2 - pos1;
	 edge2 = pos3 - pos1;
	 vec2 deltaUV1 = uv2 - uv1;
	 vec2 deltaUV2 = uv3 - uv1;
	 float f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);
	 // triangle1
	 tangent1[0] = f * (deltaUV1[1] * edge1[0] - deltaUV1[1] * edge2[0]);
	 tangent1[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
	 tangent1[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);

	 bitangent1[0] = f * (-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0]);
	 bitangent1[1] = f * (-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1]);
	 bitangent1[2] = f * (-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]);
	 // triangle2
	 edge1 = pos3 - pos1;
	 edge2 = pos4 - pos1;
	 deltaUV1 = uv3 - uv1;
	 deltaUV2 = uv4 - uv1;

	 f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

	 tangent2[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
	 tangent2[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
	 tangent2[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);

	 bitangent2[0] = f * (-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0]);
	 bitangent2[1] = f * (-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1]);
	 bitangent2[2] = f * (-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]);

	 const GLfloat grsquareVertices[] =
	 {
		 pos1[0], pos1[1], pos1[2],
		 pos2[0], pos2[1], pos2[2],
		 pos3[0], pos3[1], pos3[2],

		 pos1[0], pos1[1], pos1[2],
		 pos3[0], pos3[1], pos3[2],
		 pos4[0], pos4[1], pos4[2]

	 };
	 const GLfloat grsquareTexCoords[] =
	 {
		 uv1[0], uv1[1],
		 uv2[0], uv2[1],
		 uv3[0], uv3[1],

		 uv1[0], uv1[1],
		 uv3[0], uv3[1],
		 uv4[0], uv4[1]
	 };

	 const GLfloat grsquareNormals[] =
	 {
		 0.0f, 0.0f, 1.0f,					// one normal for single surface 
		 0.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f,

		 0.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f
	 };
	 const GLfloat grsquareTangent[] =
	 {
		 tangent1[0], tangent1[1], tangent1[2],
		 tangent1[0], tangent1[1], tangent1[2],
		 tangent1[0], tangent1[1], tangent1[2],

		 tangent2[0], tangent2[1], tangent2[2],
		 tangent2[0], tangent2[1], tangent2[2],
		 tangent2[0], tangent2[1], tangent2[2]
	 };


	 //** road vao_cube
	 glGenVertexArrays(1, &grgVaoSquareRoad);
	 glBindVertexArray(grgVaoSquareRoad);

	 glGenBuffers(1, &grgVboSquarePositionRoad);
	 glBindBuffer(GL_ARRAY_BUFFER, grgVboSquarePositionRoad);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(grsquareVertices), grsquareVertices, GL_STATIC_DRAW);
	 glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 // color for square
	 glGenBuffers(1, &grgVboSquareTexCoordRoad);
	 glBindBuffer(GL_ARRAY_BUFFER, grgVboSquareTexCoordRoad);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(grsquareTexCoords), grsquareTexCoords, GL_STATIC_DRAW);
	 glVertexAttribPointer(GR_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(GR_ATTRIBUTE_TEXCOORD);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 glGenBuffers(1, &grgVboSquareNormalRoad);
	 glBindBuffer(GL_ARRAY_BUFFER, grgVboSquareNormalRoad);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(grsquareNormals), grsquareNormals, GL_STATIC_DRAW);
	 glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 glGenBuffers(1, &grgVboSquareTangentRoad);
	 glBindBuffer(GL_ARRAY_BUFFER, grgVboSquareTangentRoad);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(grsquareTangent), grsquareTangent, GL_STATIC_DRAW);
	 glVertexAttribPointer(GR_ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(GR_ATTRIBUTE_TANGENT);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 glBindVertexArray(0);

	 // footpath vao
	 glGenVertexArrays(1, &grgVaoPavement);
	 glBindVertexArray(grgVaoPavement);

	 glGenBuffers(1, &grgVboPositionPavement);
	 glBindBuffer(GL_ARRAY_BUFFER, grgVboPositionPavement);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(grsquareVertices), grsquareVertices, GL_STATIC_DRAW);
	 glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 // color for square
	 glGenBuffers(1, &grgVboTexturePavement);
	 glBindBuffer(GL_ARRAY_BUFFER, grgVboTexturePavement);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(grsquareTexCoords), grsquareTexCoords, GL_STATIC_DRAW);
	 glVertexAttribPointer(GR_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(GR_ATTRIBUTE_TEXCOORD);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 glBindVertexArray(0);

	 LoadGLTextureRoad(&grgTextureRoad, MAKEINTRESOURCE(GRBRICK_TEXTURE));
	 LoadGLTextureRoad(&grgTextureNormalRoad, MAKEINTRESOURCE(GRBRICK_NORMAL));
	 LoadGLTextureRoad(&grgTextureFootpath, MAKEINTRESOURCE(GRTEXTURE_FOOTPATH));
	 LoadGLTextureRoad(&grgTextureNormalFootpath, MAKEINTRESOURCE(GRTEXTURE_NORMAL_FOOTPATH));

	 grgPerspectiveProjectionMatrixRoad = mat4::identity();

	 // set vars to const data
	 GRSetVarsRoad();

 }

 extern "C" void GRResizeRoad(int width, int height)
 {
	 glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	 grgPerspectiveProjectionMatrixRoad = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
 }


 extern "C" void GRDisplayRoad()
 {

	 glUseProgram(grgShaderProgramObjectRoad);

	 // OpenGL drawing code will start here
	 mat4 grmodelMatrix = mat4::identity();
	 mat4 grviewMatrix = mat4::identity();
	 mat4 grprojectionMatrix = mat4::identity();
	 mat4 grrotateMatrix = mat4::identity();
	 mat4 grtranslateMatrix = mat4::identity();
	 mat4 grscaleMatrix = mat4::identity();

	 // road 0
	 grtranslateMatrix = vmath::translate(grmodelX_Road, grmodelY_Road, grmodelZ_Road);
	 grviewMatrix = grgCameraMatrixRoad;
	 grrotateMatrix = grrotateMatrix * vmath::rotate(grangleX_Road, 0.0f, 1.0f, 0.0f);
	 grrotateMatrix = grrotateMatrix * vmath::rotate(grangleZ_Road, 0.0f, 0.0f, 1.0f);
	 grrotateMatrix = grrotateMatrix * vmath::rotate(grangleY_Road, 1.0f, 0.0f, 0.0f);
	 grmodelMatrix = grtranslateMatrix * grrotateMatrix;
	 PushToStack(grmodelMatrix);

	 // road 1
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 0.0f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 2
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 3
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 4
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 5
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 6
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 7
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 8
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 9
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 10
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 11
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 12
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 13
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 14
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 15
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 16
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;

	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 17
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 18
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);



	 // road 19
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 20
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 21
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 22
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 23
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 24
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 25
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 26
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 27
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);


	 // road 28
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 29
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 30
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 31
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 // road 32
	 grmodelMatrix = mat4::identity();
	 grviewMatrix = mat4::identity();
	 grprojectionMatrix = mat4::identity();
	 grrotateMatrix = mat4::identity();
	 grtranslateMatrix = mat4::identity();
	 grscaleMatrix = mat4::identity();

	 grtranslateMatrix = vmath::translate(0.0f, 3.999997f, 0.0f);
	 grprojectionMatrix = grgPerspectiveProjectionMatrixRoad;
	 grmodelMatrix = grtranslateMatrix;
	 grmodelMatrix = PushToStack(grmodelMatrix);
	 grviewMatrix = grgCameraMatrixRoad;
	 glUniform4fv(grgLightPositionUniformRoad, 1, grgLightPosition);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, grgTextureRoad);
	 glUniform1i(grgTextureSamplerUniformRoad, 0);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, grgTextureNormalRoad);
	 glUniform1i(grgNormalUniformRoad, 1);

	 glUniformMatrix4fv(grgProjectionMatrixUniformRoad, 1, GL_FALSE, grprojectionMatrix);
	 glUniformMatrix4fv(grgViewMatrixUniformRoad, 1, GL_FALSE, grviewMatrix);
	 glUniformMatrix4fv(grgModelMatrixUniformRoad, 1, GL_FALSE, grmodelMatrix);

	 glBindVertexArray(grgVaoSquareRoad);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);

	 PopFromStack(); //32

	 PopFromStack(); //31
	 PopFromStack(); //30
	 PopFromStack(); //29
	 PopFromStack(); //28
	 PopFromStack(); //27
	 PopFromStack(); //26
	 PopFromStack(); //25
	 PopFromStack(); //24
	 PopFromStack(); //23
	 PopFromStack(); //22
	 PopFromStack(); //21
	 PopFromStack(); //20
	 PopFromStack(); //19
	 PopFromStack(); //18	
	 PopFromStack(); //17
	 PopFromStack(); //16
	 PopFromStack(); //15	
	 PopFromStack(); //14
	 PopFromStack(); //13
	 PopFromStack(); //12
	 PopFromStack(); //11
	 PopFromStack(); //10
	 PopFromStack(); //9
	 PopFromStack(); //8
	 PopFromStack(); //7
	 PopFromStack(); //6
	 PopFromStack(); //5
	 PopFromStack(); //4
	 PopFromStack(); //3
	 PopFromStack(); //2
	 PopFromStack();//1

	 PopFromStack();


 }

 extern "C" void GRSetViewMatrixRoad(vec3 cameraPlace, vec3 cameraView, vec3 up)
 {
	 grgCameraMatrixRoad = vmath::lookat(cameraPlace, cameraView, up);
 }

 extern "C" void GRUninitializeRoad()
 {
	 if (grgVboSquarePositionRoad)
	 {
		 glDeleteBuffers(1, &grgVboSquarePositionRoad);
		 grgVboSquarePositionRoad = 0;
	 }
	 if (grgVboSquareNormalRoad)
	 {
		 glDeleteBuffers(1, &grgVboSquareNormalRoad);
		 grgVboSquareNormalRoad = 0;
	 }
	 if (grgVboSquareTexCoordRoad)
	 {
		 glDeleteBuffers(1, &grgVboSquareTexCoordRoad);
		 grgVboSquareTexCoordRoad = 0;
	 }
	 if (grgVboSquareTangentRoad)
	 {
		 glDeleteBuffers(1, &grgVboSquareTangentRoad);
		 grgVboSquareTangentRoad = 0;
	 }

	 if (grgVboPositionPavement)
	 {
		 glDeleteBuffers(1, &grgVboPositionPavement);
		 grgVboPositionPavement = 0;
	 }
	 if (grgVboTexturePavement)
	 {
		 glDeleteBuffers(1, &grgVboTexturePavement);
		 grgVboTexturePavement = 0;
	 }

	 // delete square vao and vbo
	 if (grgVaoSquareRoad)
	 {
		 glDeleteVertexArrays(1, &grgVaoSquareRoad);
		 grgVaoSquareRoad = 0;
	 }
	 if (grgVaoPavement)
	 {
		 glDeleteVertexArrays(1, &grgVaoPavement);
		 grgVaoPavement = 0;
	 }
	 if (grgTextureRoad)
	 {
		 glDeleteTextures(1, &grgTextureRoad);
		 grgTextureRoad = 0;
	 }
	 if (grgTextureNormalRoad)
	 {
		 glDeleteTextures(1, &grgTextureNormalRoad);
		 grgTextureNormalRoad = 0;
	 }
	 if (grgTextureFootpath)
	 {
		 glDeleteTextures(1, &grgTextureFootpath);
		 grgTextureFootpath = 0;
	 }
	 if (grgTextureNormalFootpath)
	 {
		 glDeleteTextures(1, &grgTextureNormalFootpath);
		 grgTextureNormalFootpath = 0;
	 }


	 // free shader objects - safe elease
	 if (grgShaderProgramObjectRoad)
	 {
		 glUseProgram(grgShaderProgramObjectRoad);
		 GLsizei grshaderCount;
		 int i;
		 glGetProgramiv(grgShaderProgramObjectRoad, GL_ATTACHED_SHADERS, &grshaderCount);

		 GLuint* grpShaders = NULL;
		 grpShaders = (GLuint*)malloc(sizeof(GLuint) * grshaderCount);
		 if (grpShaders == NULL)
		 {
			 exit(0);
		 }
		 glGetAttachedShaders(grgShaderProgramObjectRoad, grshaderCount, &grshaderCount, grpShaders);
		 for (i = 0; i < grshaderCount; i++)
		 {
			 glDetachShader(grgShaderProgramObjectRoad, grpShaders[i]);
			 glDeleteShader(grpShaders[i]);
			 grpShaders[i] = 0;
		 }
		 glUseProgram(0);
	 }

	 if (grgpFileRoad)
	 {
		 fprintf(grgpFileRoad, "\n **** End ****\nLog File closed successfully. \n Program terminated successfully");
		 fclose(grgpFileRoad);
		 grgpFileRoad = NULL;
	 }
 }

 extern "C" void GRIncAngleXRoad()
 {
	 grangleX_Road = grangleX_Road + 1.3f;
 }

 extern "C" void GRIncAngleYRoad()
 {
	 grangleY_Road = grangleY_Road + 1.3f;
 }

 extern "C" void GRIncAngleZRoad()
 {
	 grangleZ_Road = grangleZ_Road + 1.3f;
 }

 extern "C" void GRDecAngleYRoad()
 {
	 grangleY_Road = grangleY_Road - 0.010f;
 }

 extern "C" void GRIncModelXRoad()
 {
	 grmodelX_Road = grmodelX_Road + 1.0f;
 }

 extern "C" void GRIncModelYRoad()
 {
	 grmodelY_Road = grmodelY_Road + 1.0f;
 }

 extern "C" void GRIncModelZRoad()
 {
	 grmodelZ_Road = grmodelZ_Road + 1.0f;
 }

 extern "C" void GRDecModelXRoad()
 {
	 grmodelX_Road = grmodelX_Road - 1.0f;
 }

 extern "C" void GRDecModelYRoad()
 {
	 grmodelY_Road = grmodelY_Road - 1.0f;
 }

 extern "C" void GRDecModelZRoad()
 {
	 grmodelZ_Road = grmodelZ_Road - 1.0f;
 }

 extern "C" void GRPrintVarsRoad()
 {
	 fprintf(grgpFileRoad, "\n angle x : %f, angle y : %f, angle z : %f", grangleX_Road, grangleY_Road, grangleZ_Road);
	 fprintf(grgpFileRoad, "\n model x : %f, model y : %f, model z : %f", grmodelX_Road, grmodelY_Road, grmodelZ_Road);
 }

 extern "C" void GRSetVarsRoad()
 {
	 // float grangleX_Road = 0.0f, grangleY_Road = 270.0f, grangleZ_Road = 0.0f;
	 //float grmodelX_Road = -66.23930f, grmodelY_Road = -1.459999f, grmodelZ_Road = 61.759964f;
	 grangleX_Road = 0.0f;
	 grangleY_Road = 270.0f;
	 grangleZ_Road = 0.0f;

	 // translate varibles
	 grmodelX_Road = -66.23930f;
	 grmodelY_Road = -1.459999f;
	 grmodelZ_Road = 61.759964f;

	 grgLightPosition = vec4(-66.0f, 100.0f, -100.0f, 0.0f);
 }

 bool LoadGLTextureRoad(GLuint* texture, TCHAR resourceID[])
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
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			// wrap the texture around x axis (Texture's "S" = x axis)
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// MAG - Magnification
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);				// MIN - Minification

		 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
		 glGenerateMipmap(GL_TEXTURE_2D);

		 DeleteObject(hBitmap);

	 }

	 return(bResult);

 }


