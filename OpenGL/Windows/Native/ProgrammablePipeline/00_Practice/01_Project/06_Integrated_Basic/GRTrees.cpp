#include"GRTree.h"
#include"assimp\mesh.h"
#include"assimp\scene.h"
#include"assimp\postprocess.h"
#include"filesystem.h"
#include"GRModel.h"

#pragma comment(lib, "assimp-vc141-mtd.lib")

using namespace vmath;

#pragma region tri vars
FILE* grgpFileTree = NULL;
GLuint grgVertexShaderObjectTri;
GLuint grgFragmentShadeerObjectTri;
GLuint grgShaderProgramObjectTri;
GLuint grgGeometryShaderObjectTri;
GLuint grgModelMatrixUniformTri;
GLuint grgViewMatrixUniformTri;
GLuint grgProjectionMatrixUniformTri;
Model* grgModelTree;
Model* grgModelHouse;
GLuint grgKaUniformTree;
GLuint grgKdUniformTree;
GLuint grgKsUniformTree;
GLuint grgLaUniformTree;
GLuint grgLdUniformTree;
GLuint grgLsUniformTree;
GLuint grgLightPositionUniformTree;
GLuint grgMaterialShinynessUniformTree;
vec4 grgLaTree = { 0.0f, 0.0f, 0.0f, 1.0f };
vec4 grgLdTree = { 1.0f, 1.0f, 1.0f, 1.0f };
vec4 grgLsTree = { 1.0f, 1.0f, 1.0f, 1.0f };

GLuint grgTextureSamplerUniformTree;
GLuint grgLightUniform;
GLuint grbLightTree = 0;
GLfloat grrotXHouse = 0.0f, grrotYHouse = 0.0f, grrotZHouse = 0.0f;
GLfloat grrotXTree = 0.0f, grrotYTree = 0.0f, grrotZTree = 0.0f;
GLfloat grlightPosX = 0.0f, grlightPosY = 0.0f, grlightPosZ = 0.0f;
float grmodelX_Tree = 0.0f, grmodelY_Tree = 0.0f, grmodelZ_Tree = 0.0f;
float grmodelX_House = -49.039932f, grmodelY_House = 0.200f, grmodelZ_House = -2.920021f;
vec4 grgLightPositionTree = { grlightPosX, grlightPosY, grlightPosZ, 0.0f };
vec3 grgRandPos[20];
GLuint grgRandPositionUniform[20];
mat4 grgCameraMatrixTree = mat4::identity();
mat4 grgPerspectiveProjectionMatrixTree;
#pragma endregion tri vars


bool LoadGLTextureGrass(GLuint* texture, TCHAR resourceID[]);
void DrawMeshTree(int);
void DrawMeshHouse(int);

extern "C" void GROpenLogFileTree()
{
	if (fopen_s(&grgpFileTree, "GRLogTree.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot open desired file"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf_s(grgpFileTree, "Log file created successfully. \n Program started successfully\n **** Logs ***** \n");
	}
}

extern "C" void GRPrintLogTree(const char* msg)
{
	fprintf(grgpFileTree, msg);
}

extern "C" void GRInitializeTree()
{
	GROpenLogFileTree();
	grgVertexShaderObjectTri = glCreateShader(GL_VERTEX_SHADER);

	///// Vertex Shader
	const GLchar* grvertexShaderSourceCodeTri =					// also called as "pass-through shader" as it does not have any code (main is empty, no code is there in main)
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec2 vTexCoord;" \
		"in vec3 vNormal;" \
		"in vec2 vPos;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec3 u_ka;" \
		"uniform vec3 u_kd;" \
		"uniform vec3 u_ks;" \
		"uniform vec3 u_la;" \
		"uniform vec3 u_ld;" \
		"uniform vec3 u_ls;" \
		"uniform float u_material_shinyness;" \
		"uniform vec4 u_light_position;" \
		"uniform int u_light;" \
		"uniform vec3 u_rand_pos[20];" \
		"out VS_OUT" \
		"{" \
		"vec3 out_phong_light;" \
		"vec2 out_texcoord;" \
		"} vs_out;" \
		"void main(void)" \
		"{" \
		// phong light
		"if(u_light == 1)" \
		"{" \
		"vec4 eye_coordinates =  u_view_matrix * u_model_matrix * vPosition;" \
		"vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
		"vec3 light_direction = normalize(vec3(u_light_position - eye_coordinates));" \
		"vec3 reflection_vector = reflect(-light_direction, transformed_normal);" \
		"vec3 view_vector = normalize(-eye_coordinates.xyz);" \
		"vec3 ambient = u_la * u_ka;" \
		"vec3 diffuse = u_ld * u_kd * max(dot(light_direction, transformed_normal), 0.0);" \
		"vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector, view_vector), 0.0), u_material_shinyness);" \
		"vs_out.out_phong_light = ambient + diffuse + specular;" \
		"}" \
		"else" \
		"{" \
		"vs_out.out_phong_light = vec3(1.0, 1.0f, 1.0f);" \
		"}" \
		// position
		"vec4 rand_pos = vec4(u_rand_pos[gl_InstanceID], 1.0);" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * (vPosition + rand_pos);" \
		"vs_out.out_texcoord = vTexCoord;" \
		"}";

	glShaderSource(grgVertexShaderObjectTri, 1, (const GLchar**)&grvertexShaderSourceCodeTri, NULL);

	// compile shader
	glCompileShader(grgVertexShaderObjectTri);
	// error check for compilation
	int griInfoLength = 0;
	int griShaderCompileStatus = 0;
	char *grszInfoLog = NULL;

	glGetShaderiv(grgVertexShaderObjectTri, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgVertexShaderObjectTri, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgVertexShaderObjectTri, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileTree, "\n Vertex Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				GRUninitializeTree();
			}
		}
	}


	////// Fragment Shader
	grgFragmentShadeerObjectTri = glCreateShader(GL_FRAGMENT_SHADER);

	// source code of shader
	const GLchar* grfragmentShaderSourceCodeTri =
		"#version 450 core" \
		"\n" \
		"out vec4 FragColor;" \
		"uniform sampler2D u_texture_sampler;" \
		"uniform int u_light;" \
		"in VS_OUT" \
		"{" \
		"vec3 out_phong_light;" \
		"vec2 out_texcoord;" \
		"} fs_in;" \
		"vec4 textureColor;" \
		"void main(void)" \
		"{" \
		"if(u_light == 1)" \
		"{" \
		"textureColor = texture(u_texture_sampler, fs_in.out_texcoord) * vec4(fs_in.out_phong_light, 1.0);" \
		"}" \
		"else" \
		"{" \
		"textureColor = texture(u_texture_sampler, fs_in.out_texcoord);" \
		"}" \
		"FragColor = textureColor;" \
		"}";

	glShaderSource(grgFragmentShadeerObjectTri, 1, (const GLchar**)&grfragmentShaderSourceCodeTri, NULL);

	// compile shader
	glCompileShader(grgFragmentShadeerObjectTri);
	// error check for compiation
	glGetShaderiv(grgFragmentShadeerObjectTri, GL_COMPILE_STATUS, &griShaderCompileStatus);
	if (griShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(grgFragmentShadeerObjectTri, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(char) * sizeof(griInfoLength));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetShaderInfoLog(grgFragmentShadeerObjectTri, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileTree, "\n Fragment Shader Compilation Log : %s", grszInfoLog);
				free(grszInfoLog);
				GRUninitializeTree();
			}
		}
	}

	//****** Shader Program *****//
	// create
	grgShaderProgramObjectTri = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(grgShaderProgramObjectTri, grgVertexShaderObjectTri);
	glAttachShader(grgShaderProgramObjectTri, grgGeometryShaderObjectTri);
	// attach fragment shader to shader program
	glAttachShader(grgShaderProgramObjectTri, grgFragmentShadeerObjectTri);

	// pre-link our attribute enum with shader's attributes
	glBindAttribLocation(grgShaderProgramObjectTri, GR_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(grgShaderProgramObjectTri, GR_ATTRIBUTE_TEXCOORD, "vTexCoord");
	glBindAttribLocation(grgShaderProgramObjectTri, GR_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(grgShaderProgramObjectTri);
	// error check for linking
	int griShaderProgramLinkStatus = 0;
	glGetProgramiv(grgShaderProgramObjectTri, GL_LINK_STATUS, &griShaderProgramLinkStatus);
	if (griShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(grgShaderProgramObjectTri, GL_INFO_LOG_LENGTH, &griInfoLength);
		if (griInfoLength > 0)
		{
			grszInfoLog = (char*)malloc(sizeof(griInfoLength) * sizeof(char));
			if (grszInfoLog != NULL)
			{
				GLsizei grwritten;
				glGetProgramInfoLog(grgShaderProgramObjectTri, griInfoLength, &grwritten, grszInfoLog);
				fprintf(grgpFileTree, "\n Shader Program Link Log : %s", grszInfoLog);
				GRUninitializeTree();
			}
		}
	}

	grgModelMatrixUniformTri = glGetUniformLocation(grgShaderProgramObjectTri, "u_model_matrix");
	grgViewMatrixUniformTri = glGetUniformLocation(grgShaderProgramObjectTri, "u_view_matrix");
	grgProjectionMatrixUniformTri = glGetUniformLocation(grgShaderProgramObjectTri, "u_projection_matrix");
	grgRandPositionUniform[0] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[0]");
	grgRandPositionUniform[1] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[1]");
	grgRandPositionUniform[2] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[2]");
	grgRandPositionUniform[3] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[3]");
	grgRandPositionUniform[4] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[4]");
	grgRandPositionUniform[5] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[5]");
	grgRandPositionUniform[6] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[6]");
	grgRandPositionUniform[7] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[7]");
	grgRandPositionUniform[8] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[8]");
	grgRandPositionUniform[9] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[9]");
	grgRandPositionUniform[10] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[10]");
	grgRandPositionUniform[11] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[11]");
	grgRandPositionUniform[12] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[12]");
	grgRandPositionUniform[13] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[13]");
	grgRandPositionUniform[14] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[14]");
	grgRandPositionUniform[15] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[15]");
	grgRandPositionUniform[16] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[16]");
	grgRandPositionUniform[17] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[17]");
	grgRandPositionUniform[18] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[18]");
	grgRandPositionUniform[19] = glGetUniformLocation(grgShaderProgramObjectTri, "u_rand_pos[19]");
	grgKaUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_ka");
	grgKdUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_kd");
	grgKsUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_ks");
	grgLaUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_la");
	grgLdUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_ld");
	grgLsUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_ls");
	grgLightPositionUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_light_position");
	grgLightUniform = glGetUniformLocation(grgShaderProgramObjectTri, "u_light");
	//u_texture_sampler
	grgTextureSamplerUniformTree = glGetUniformLocation(grgShaderProgramObjectTri, "u_texture_sampler");


	grgModelTree = new Model(FileSystem::getPath("model/Tree/Tree.obj"), grgpFileTree);
	grgModelHouse = new Model(FileSystem::getPath("model/house/house2/HOUSE.obj"), grgpFileTree);

	grgRandPos[0] = vec3(7.0, 1.0, -10.0);
	grgRandPos[1] = vec3(20.0, 1.0, -15.0);
	grgRandPos[2] = vec3(15.0, 1.0, -5.0);
	grgRandPos[3] = vec3(20.0, 1.0, -18.0);
	grgRandPos[4] = vec3(20.0, 1.0, -13.0);
	grgRandPos[5] = vec3(12.8, 1.0, -8.0);
	grgRandPos[6] = vec3(12.4, 1.0, -10.0);
	grgRandPos[7] = vec3(3.1, 1.0, -10.0);
	grgRandPos[8] = vec3(18.1, 1.0, -10.0);
	grgRandPos[9] = vec3(21.0, 1.0, -13.0);

	grgRandPos[10] = vec3(-7.0, 1.0, -10.0);
	grgRandPos[11] = vec3(-10.0, -2.0, -15.0);
	grgRandPos[12] = vec3(-15.0, -10.0, -5.0);
	grgRandPos[13] = vec3(-20.0, 0.0, -18.0);
	grgRandPos[14] = vec3(-20.0, -1.0, -13.0);
	grgRandPos[15] = vec3(-12.8, 1.0, -8.0);
	grgRandPos[16] = vec3(-12.4, 0.0, -10.0);
	grgRandPos[17] = vec3(-3.1, 0.0, -10.0);
	grgRandPos[18] = vec3(-18.1, -1.0, -10.0);
	grgRandPos[19] = vec3(-21.0, -1.0, -13.0);

	grgPerspectiveProjectionMatrixTree = mat4::identity();
	// set vars to const data
	GRSetVarsTree();

}

extern "C" void GRResizeTree(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	grgPerspectiveProjectionMatrixTree = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
}


extern "C" void GRDisplayTree()
{
	mat4 grmodelMatrix = mat4::identity();
	mat4 grviewMatrix = mat4::identity();
	mat4 grprojectionMatrix = mat4::identity();
	mat4 grmodelViewProjectionMatrix = mat4::identity();
	mat4 grtranslateMatrix = mat4::identity();
	mat4 grrotateMatrix = mat4::identity();

	glUseProgram(grgShaderProgramObjectTri);

	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();
	grtranslateMatrix = vmath::translate(grmodelX_Tree, grmodelY_Tree, grmodelZ_Tree);
	grrotateMatrix = vmath::rotate(grrotXTree, 0.0f, 1.0f, 0.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grrotZTree, 0.0f, 0.0f, 1.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grrotYTree, 1.0f, 0.0f, 0.0f);

	grmodelMatrix = grtranslateMatrix * grrotateMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrixTree;
	grviewMatrix = grgCameraMatrixTree;
	glUniformMatrix4fv(grgModelMatrixUniformTri, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grgViewMatrixUniformTri, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grgProjectionMatrixUniformTri, 1, GL_FALSE, grprojectionMatrix);

	//for (int j = 0; j < 10; j++)
	//{
		//glUniform3fv(grgRandPositionUniform[j], 1, grgRandPos[j]);
	//}
	glUniform3fv(grgRandPositionUniform[0], 1, grgRandPos[0]);
	glUniform3fv(grgRandPositionUniform[1], 1, grgRandPos[1]);
	glUniform3fv(grgRandPositionUniform[2], 1, grgRandPos[2]);
	glUniform3fv(grgRandPositionUniform[3], 1, grgRandPos[3]);
	glUniform3fv(grgRandPositionUniform[4], 1, grgRandPos[4]);
	glUniform3fv(grgRandPositionUniform[5], 1, grgRandPos[5]);
	glUniform3fv(grgRandPositionUniform[6], 1, grgRandPos[6]);
	glUniform3fv(grgRandPositionUniform[7], 1, grgRandPos[7]);
	glUniform3fv(grgRandPositionUniform[8], 1, grgRandPos[8]);
	glUniform3fv(grgRandPositionUniform[9], 1, grgRandPos[9]);
	glUniform3fv(grgRandPositionUniform[10], 1, grgRandPos[10]);
	glUniform3fv(grgRandPositionUniform[11], 1, grgRandPos[11]);
	glUniform3fv(grgRandPositionUniform[12], 1, grgRandPos[12]);
	glUniform3fv(grgRandPositionUniform[13], 1, grgRandPos[13]);
	glUniform3fv(grgRandPositionUniform[14], 1, grgRandPos[14]);
	glUniform3fv(grgRandPositionUniform[15], 1, grgRandPos[15]);
	glUniform3fv(grgRandPositionUniform[16], 1, grgRandPos[16]);
	glUniform3fv(grgRandPositionUniform[17], 1, grgRandPos[17]);
	glUniform3fv(grgRandPositionUniform[18], 1, grgRandPos[18]);
	glUniform3fv(grgRandPositionUniform[19], 1, grgRandPos[19]);

	for (unsigned int i = 0; i < grgModelTree->meshes.size(); i++)
	{
		DrawMeshTree(i);
	}

	// house
	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();

	grtranslateMatrix = vmath::translate(grmodelX_House, grmodelY_House, grmodelZ_House);
	grrotateMatrix = vmath::rotate(grrotXHouse, 0.0f, 1.0f, 0.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grrotYHouse, 1.0f, 0.0f, 0.0f);
	grrotateMatrix = grrotateMatrix * vmath::rotate(grrotZHouse, 0.0f, 0.0f, 1.0f);
	grmodelMatrix = grtranslateMatrix * grrotateMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrixTree;
	grviewMatrix = grgCameraMatrixTree;
	glUniformMatrix4fv(grgModelMatrixUniformTri, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grgViewMatrixUniformTri, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grgProjectionMatrixUniformTri, 1, GL_FALSE, grprojectionMatrix);
	glUniform3fv(grgRandPositionUniform[0], 1, grgRandPos[0]);
	for (unsigned int i = 0; i < grgModelHouse->meshes.size(); i++)
	{
		DrawMeshHouse(i);
	}


	/*
	// 2nd tres display
	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();
	grtranslateMatrix = vmath::translate(x2 - 20, y2, -10.0f + z2);
	grmodelMatrix = grtranslateMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrix;

	glUniformMatrix4fv(grgModelMatrixUniformTri, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grgViewMatrixUniformTri, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grgProjectionMatrixUniformTri, 1, GL_FALSE, grprojectionMatrix);

	//for (int j = 0; j < 10; j++)
	//{
		//glUniform3fv(grgRandPositionUniform[j], 1, grgRandPos[j]);
	//}
	glUniform3fv(grgRandPositionUniform[0], 1, grgRandPos[0]);
	glUniform3fv(grgRandPositionUniform[1], 1, grgRandPos[1]);
	glUniform3fv(grgRandPositionUniform[2], 1, grgRandPos[2]);
	glUniform3fv(grgRandPositionUniform[3], 1, grgRandPos[3]);
	glUniform3fv(grgRandPositionUniform[4], 1, grgRandPos[4]);
	glUniform3fv(grgRandPositionUniform[5], 1, grgRandPos[5]);
	glUniform3fv(grgRandPositionUniform[6], 1, grgRandPos[6]);
	glUniform3fv(grgRandPositionUniform[7], 1, grgRandPos[7]);
	glUniform3fv(grgRandPositionUniform[8], 1, grgRandPos[8]);
	glUniform3fv(grgRandPositionUniform[9], 1, grgRandPos[9]);
	glUniform3fv(grgRandPositionUniform[10], 1, grgRandPos[10]);
	glUniform3fv(grgRandPositionUniform[11], 1, grgRandPos[11]);
	glUniform3fv(grgRandPositionUniform[12], 1, grgRandPos[12]);
	glUniform3fv(grgRandPositionUniform[13], 1, grgRandPos[13]);
	glUniform3fv(grgRandPositionUniform[14], 1, grgRandPos[14]);
	glUniform3fv(grgRandPositionUniform[15], 1, grgRandPos[15]);
	glUniform3fv(grgRandPositionUniform[16], 1, grgRandPos[16]);
	glUniform3fv(grgRandPositionUniform[17], 1, grgRandPos[17]);
	glUniform3fv(grgRandPositionUniform[18], 1, grgRandPos[18]);
	glUniform3fv(grgRandPositionUniform[19], 1, grgRandPos[19]);

	for (unsigned int i = 0; i < grgModelTree->meshes.size(); i++)
	{
		DrawMeshTree(i);
	}

	// 3rd
	// 2nd tres display
	grtranslateMatrix = mat4::identity();
	grmodelMatrix = mat4::identity();
	grviewMatrix = mat4::identity();
	grtranslateMatrix = vmath::translate(x2 + 20, y2, 30.0f + z2);
	grmodelMatrix = grtranslateMatrix;
	grprojectionMatrix = grgPerspectiveProjectionMatrix;

	glUniformMatrix4fv(grgModelMatrixUniformTri, 1, GL_FALSE, grmodelMatrix);
	glUniformMatrix4fv(grgViewMatrixUniformTri, 1, GL_FALSE, grviewMatrix);
	glUniformMatrix4fv(grgProjectionMatrixUniformTri, 1, GL_FALSE, grprojectionMatrix);

	//for (int j = 0; j < 10; j++)
	//{
		//glUniform3fv(grgRandPositionUniform[j], 1, grgRandPos[j]);
	//}
	glUniform3fv(grgRandPositionUniform[0], 1, grgRandPos[0]);
	glUniform3fv(grgRandPositionUniform[1], 1, grgRandPos[1]);
	glUniform3fv(grgRandPositionUniform[2], 1, grgRandPos[2]);
	glUniform3fv(grgRandPositionUniform[3], 1, grgRandPos[3]);
	glUniform3fv(grgRandPositionUniform[4], 1, grgRandPos[4]);
	glUniform3fv(grgRandPositionUniform[5], 1, grgRandPos[5]);
	glUniform3fv(grgRandPositionUniform[6], 1, grgRandPos[6]);
	glUniform3fv(grgRandPositionUniform[7], 1, grgRandPos[7]);
	glUniform3fv(grgRandPositionUniform[8], 1, grgRandPos[8]);
	glUniform3fv(grgRandPositionUniform[9], 1, grgRandPos[9]);
	glUniform3fv(grgRandPositionUniform[10], 1, grgRandPos[10]);
	glUniform3fv(grgRandPositionUniform[11], 1, grgRandPos[11]);
	glUniform3fv(grgRandPositionUniform[12], 1, grgRandPos[12]);
	glUniform3fv(grgRandPositionUniform[13], 1, grgRandPos[13]);
	glUniform3fv(grgRandPositionUniform[14], 1, grgRandPos[14]);
	glUniform3fv(grgRandPositionUniform[15], 1, grgRandPos[15]);
	glUniform3fv(grgRandPositionUniform[16], 1, grgRandPos[16]);
	glUniform3fv(grgRandPositionUniform[17], 1, grgRandPos[17]);
	glUniform3fv(grgRandPositionUniform[18], 1, grgRandPos[18]);
	glUniform3fv(grgRandPositionUniform[19], 1, grgRandPos[19]);

	for (unsigned int i = 0; i < grgModelTree->meshes.size(); i++)
	{
		DrawMeshTree(i);
	}
	*/

	glUseProgram(0);
}

void DrawMeshTree(int i)
{
	grgLightPositionTree[0] = grlightPosX;
	grgLightPositionTree[1] = grlightPosY;
	grgLightPositionTree[2] = grlightPosZ;

	glBindTexture(GL_TEXTURE_2D, NULL);
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	vec3 matColor = vec3(0.0f, 0.0f, 0.0f);
	for (unsigned int j = 0; j < grgModelTree->meshes[i].textures.size(); j++)
	{
		glActiveTexture(GL_TEXTURE0 + j);

		/*std::string number;
		std::string name = grgModel->meshes[i].textures[j].type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);

		}
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);
		*/
		glBindTexture(GL_TEXTURE_2D, grgModelTree->meshes[i].textures[j].id);
		// get ambient color
		matColor[0] = grgModelTree->meshes[i].colors[0];
		matColor[1] = grgModelTree->meshes[i].colors[1];
		matColor[2] = grgModelTree->meshes[i].colors[2];
		glUniform3fv(grgKaUniformTree, 1, matColor);
		// get diffuse color
		matColor[0] = grgModelTree->meshes[i].colors[3];
		matColor[1] = grgModelTree->meshes[i].colors[4];
		matColor[2] = grgModelTree->meshes[i].colors[5];
		glUniform3fv(grgKdUniformTree, 1, matColor);
		// get specular color
		matColor[0] = grgModelTree->meshes[i].colors[6];
		matColor[1] = grgModelTree->meshes[i].colors[7];
		matColor[2] = grgModelTree->meshes[i].colors[8];
		glUniform3fv(grgKsUniformTree, 1, matColor);

		glUniform3fv(grgLaUniformTree, 1, grgLaTree);
		glUniform3fv(grgLdUniformTree, 1, grgLdTree);
		glUniform3fv(grgLsUniformTree, 1, grgLsTree);
		glUniform4fv(grgLightPositionUniformTree, 1, grgLightPositionTree);
		glUniform1f(grgMaterialShinynessUniformTree, 16);
		glUniform1i(grgLightUniform, grbLightTree);
		glUniform1i(grgTextureSamplerUniformTree, 0);
	}


	glBindVertexArray(grgModelTree->meshes[i].vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, grgModelTree->meshes[i].indices.size(), 3);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void DrawMeshHouse(int i)
{
	grgLightPositionTree[0] = grlightPosX;
	grgLightPositionTree[1] = grlightPosY;
	grgLightPositionTree[2] = grlightPosZ;
	glBindTexture(GL_TEXTURE_2D, NULL);
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int j = 0; j < grgModelHouse->meshes[i].textures.size(); j++)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		glUniform3fv(grgLaUniformTree, 1, grgLaTree);
		glUniform3fv(grgLdUniformTree, 1, grgLdTree);
		glUniform3fv(grgLsUniformTree, 1, grgLsTree);
		glUniform4fv(grgLightPositionUniformTree, 1, grgLightPositionTree);
		glUniform1f(grgMaterialShinynessUniformTree, 16);
		glUniform1i(grgTextureSamplerUniformTree, j);
		glUniform1i(grgLightUniform, grbLightTree);
		glBindTexture(GL_TEXTURE_2D, grgModelHouse->meshes[i].textures[j].id);
	}
	glBindVertexArray(grgModelHouse->meshes[i].vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, grgModelHouse->meshes[i].indices.size(), 1);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

extern "C" void GRSetViewMatrixTree(vec3 cameraPlace, vec3 cameraView, vec3 up)
{
	grgCameraMatrixTree = vmath::lookat(cameraPlace, cameraView, up);
}

extern "C" void GRUninitializeTree()
{
	if (grgModelTree)
	{
		for ( int j = 0; j < grgModelHouse->meshes.size(); j++)
		if (grgModelTree->meshes[0].vbo)
		{
			glDeleteBuffers(1, &grgModelTree->meshes[0].vbo);
			grgModelTree->meshes[0].vbo = 0;
		}
		if (grgModelTree->meshes[0].ebo)
		{
			glDeleteBuffers(1, &grgModelTree->meshes[0].ebo);
			grgModelTree->meshes[0].ebo = 0;
		}
		if (grgModelTree->meshes[0].vao)
		{
			glDeleteVertexArrays(1, &grgModelTree->meshes[0].vao);
			grgModelTree->meshes[0].vao = 0;
		}
	}
	
	// free shader objects - safe elease
	if (grgShaderProgramObjectTri)
	{
		glUseProgram(grgShaderProgramObjectTri);
		GLsizei grshaderCount;
		int i;
		glGetProgramiv(grgShaderProgramObjectTri, GL_ATTACHED_SHADERS, &grshaderCount);

		GLuint* grpShaders = NULL;
		grpShaders = (GLuint*)malloc(sizeof(GLuint) * grshaderCount);
		if (grpShaders == NULL)
		{
			exit(0);
		}
		glGetAttachedShaders(grgShaderProgramObjectTri, grshaderCount, &grshaderCount, grpShaders);
		for (i = 0; i < grshaderCount; i++)
		{
			glDetachShader(grgShaderProgramObjectTri, grpShaders[i]);
			glDeleteShader(grpShaders[i]);
			grpShaders[i] = 0;
		}
		glUseProgram(0);
	}

	if (grgpFileTree)
	{
		fprintf(grgpFileTree, "\n **** End ****\nLog File closed successfully. \n Program terminated successfully");
		fclose(grgpFileTree);
		grgpFileTree = NULL;
	}
}


extern "C" void GRIncModelXTree()
{
	grmodelX_Tree = grmodelX_Tree + 0.02f;
}

extern "C" void GRIncModelYTree()
{
	grmodelY_Tree = grmodelY_Tree + 0.02f;
}

extern "C" void GRIncModelZTree()
{
	grmodelZ_Tree = grmodelZ_Tree + 1.0f;
}

extern "C" void GRDecModelXTree()
{
	grmodelX_Tree = grmodelX_Tree - 1.0f;
}

extern "C" void GRDecModelYTree()
{
	grmodelY_Tree = grmodelY_Tree - 0.02f;
}

extern "C" void GRDecModelZTree()
{
	grmodelZ_Tree = grmodelZ_Tree - 0.02f;
}

extern "C" void GRIncModelXHouse()
{
	grmodelX_House = grmodelX_House + 0.02f;
}

extern "C" void GRIncModelYHouse()
{
	grmodelY_House = grmodelY_House + 0.02f;
}

extern "C" void GRIncModelZHouse()
{
	grmodelZ_House = grmodelZ_House + 1.0f;
}

extern "C" void GRDecModelXHouse()
{
	grmodelX_House = grmodelX_House - 1.0f;
}

extern "C" void GRDecModelYHouse()
{
	grmodelY_House = grmodelY_House - 0.02f;
}

extern "C" void GRDecModelZHouse()
{
	grmodelZ_House = grmodelZ_House - 0.02f;
}

extern "C" void GRIncAngleXTree()
{
	grrotXHouse = grrotXHouse + 0.1f;
}

extern "C" void GRIncAngleYTree()
{
	grrotYHouse = grrotYHouse + 0.1f;
}


extern "C" void GRIncAngleZTree()
{
	grrotZHouse = grrotZHouse + 0.1f;
}

extern "C" void GRPrintVarsTree()
{
	fprintf(grgpFileTree, "\n model x : %f, model y : %f, model z : %f", grmodelX_Tree, grmodelY_Tree, grmodelZ_Tree);
	fprintf(grgpFileTree, "\n house model x : %f, model y : %f, model z : %f", grmodelX_House, grmodelY_House, grmodelZ_House);
	fprintf(grgpFileTree, "\n angle x house : %f, angle y house : %f, angle z house : %f", grrotXHouse, grrotYHouse, grrotZHouse);
}

extern "C" void GRSetVarsTree()
{
	grlightPosX = 0.0f;
	grlightPosY = 0.0f;
	grlightPosZ = 0.0f;

	grrotYHouse = 0.0f;
	grrotXHouse = 241.405533f;
	grrotZHouse = 0.0f;
	// translate varibles
	grmodelX_Tree = 0.0f;
	grmodelY_Tree = 0.0f;
	grmodelZ_Tree = -20.0f;

	grmodelX_House = -49.039932f;
	grmodelY_House = 0.200f;
	grmodelZ_House = -2.920021f;

	grgLightPositionTree = vec4(-66.0f, 100.0f, -100.0f, 0.0f);
}



