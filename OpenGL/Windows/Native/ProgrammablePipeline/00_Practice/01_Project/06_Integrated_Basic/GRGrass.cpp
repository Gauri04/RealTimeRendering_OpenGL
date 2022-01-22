#include"GRGrass.h"
using namespace vmath;
#include<string>

#define FOR(q,n) for(int q=0;q<n;q++)

#pragma region grass vars
FILE* grgpFileGrass = NULL;
GLuint grshaderProgramObjectGrass;
std::vector<std::vector< glm::vec3> > grvertexDataGrass;
//
GLuint grtextureGrass;
int grwidthGrass, grheightGrass, grBPPGrass;
std::string grspathGrass;
bool grbMipMapGeneratedGrass;
GLuint grsamplerGrass;
int grtfMinificationGrass, grtfMagnificationGrass;
GLuint grtimePassedUniformGrass;
float grtim1 = 0;


glm::vec3 grvRenderScaleGrass;
int griNumGrassTrianglesGrass;
bool grbLoadedGrass;
int griRowsGrass;
int griColsGrass;
int griGrass;

GLuint grvboGrass;
GLuint grvaoGrass;


GLuint grviewMatrixUniformGrass;
GLuint greyeUniformGrass;
GLuint grmodelUniformGrass;
GLuint grcoloruniformGrass;
GLuint graplhaTestGrass;
GLuint gralphaMultiplierGrass;
GLuint grnormalMatrixUniformGrass;
GLuint grprojectionMatrixUniformGrass;
GLuint grtextureSamplerUniformGrass;

std::vector<BYTE> grdataGrass;
int grcurrentSizeGrass;


enum ETextureFilteringGrass
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

mat4 grperspectiveProjectionMatrixGrass;
mat4 grgCameraMatrixGrass = mat4::identity();
GLfloat grmodelX_Grass = 0.0f, grmodelY_Grass = 0.0f, grmodelZ_Grass = 0.0f;
#pragma endregion grass vars

// function decl
bool LoadGLTextureGrass(GLuint* texture, TCHAR resourceID[]);
bool LoadHeightMapFromImageFree(std::string sImagePath);
void AddData(void* ptrData, UINT uiDataSize);
void LoadAllTextures();
bool LoadTexture2D(std::string a_sPath, bool bGenerateMipMaps);
void SetFiltering(int a_tfMagnification, int a_tfMinification);
void SetWrap();
void SetAnisotropyLevel(float fAnisotropyLevel);
void CreateFromData(BYTE* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps);
float GetHeightFromRealVector(glm::vec3 vRealPosition);
void LoadGrassTexture();

extern "C" void GROpenLogFileGrass()
{
	if (fopen_s(&grgpFileGrass, "GRLogGrass.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot open desired file"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf_s(grgpFileGrass, "Log file created successfully. \n Program started successfully\n **** Logs ***** \n");
	}
}


extern "C" void GRInitializeGrass()
{
	GROpenLogFileGrass();
	GLuint gVertexShaderObject;
	GLuint gFragmentShaderObject;
	GLuint gGeometryShaderObject;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	LoadAllTextures();
	//////////////////////////////// V E R T E X - S H A D E R //////////////////////////


	//Define Vertex Shader Object
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER); //This command will create the Shader Object
	//Now Write vertex shader code
	const GLchar** p;
	const GLchar* vertexShaderSourceCode =

		"#version 430 core" \
		"\n"
		"layout(location = 0) in vec3 inPosition;" \
		"void main(void)" \
		"{" \
		"gl_Position = vec4(inPosition,1.0);" \

		"}";
	// GPU will run the above code. And GPU WILL RUN FOR PER VERTEX. If there are 1000 vertex. Then GPU will run this shader for
	//1000 times. We are Multiplying each vertex with the Model View Matrix.
	//And how does the GPU gets to know about at what offset the array has to be taken . Go to glVertexAttribPointer() in Display.
	// in = Input. 

	//p = &vertexShaderSourceCode;
		//Specify above source code to the vertex shader object
	glShaderSource(gVertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	//Compile the vertex shader 
	glCompileShader(gVertexShaderObject);

	//////////////// Error Checking//////////////////
	//Code for catching the errors 
	GLint iShaderCompileStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar* szInfoLog = NULL;


	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
	if (iShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(grgpFileGrass, "%s\n", szInfoLog);
				free(szInfoLog);
				GRUninitializeGrass();
				exit(0);


			}
		}
	}


	////////// GEOMETRY SHADER ////////////////
	gGeometryShaderObject = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* geometryShaderSourceCode =         //Source code of Vertex shader
		"#version 430 core" \
		"\n" \
		"layout(points) in;" \
		"layout(triangle_strip) out;" \
		"layout(max_vertices = 12) out;" \

		"uniform struct Matrices" \
		"{" \
		"mat4 projMatrix;" \
		"mat4 modelMatrix;" \
		"mat4 viewMatrix;" \
		"mat4 normalMatrix;" \
		"} matrices;" \

		"smooth out vec2 vTexCoord;" \
		"smooth out vec3 vWorldPos;" \
		"smooth out vec4 vEyeSpacePos;" \

		"uniform float fTimePassed;" \

		"mat4 rotationMatrix(vec3 axis, float angle)" \
		"{" \
		"axis = normalize(axis);" \
		"float s = sin(angle);" \
		"float c = cos(angle);" \
		"float oc = 1.0 - c;" \

		"return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0," \
		"oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0," \
		"oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0," \
		"0.0, 0.0, 0.0, 1.0);" \
		"}" \

		"vec3 vLocalSeed;" \


		"float randZeroOne()" \
		"{" \
		"uint n = floatBitsToUint(vLocalSeed.y * 214013.0 + vLocalSeed.x * 2531011.0 + vLocalSeed.z * 141251.0);" \
		"n = n * (n * n * 15731u + 789221u);" \
		"n = (n >> 9u) | 0x3F800000u;" \

		"	float fRes = 2.0 - uintBitsToFloat(n);" \
		"vLocalSeed = vec3(vLocalSeed.x + 147158.0 * fRes, vLocalSeed.y*fRes + 415161.0 * fRes, vLocalSeed.z + 324154.0*fRes);" \
		"return fRes;" \
		"}" \

		"int randomInt(int min, int max)" \
		"{" \
		"float fRandomFloat = randZeroOne();" \
		"return int(float(min) + fRandomFloat * float(max - min));" \
		"}" \

		"void main()" \
		"{" \
		"mat4 mMV = matrices.viewMatrix*matrices.modelMatrix;" \
		"mat4 mMVP = matrices.projMatrix*matrices.viewMatrix*matrices.modelMatrix;" \

		"vec3 vGrassFieldPos = gl_in[0].gl_Position.xyz;" \

		"	float PIover180 = 3.1415 / 180.0;" \
		"vec3 vBaseDir[] =" \
		"{" \
		"vec3(1.0, 0.0, 0.0)," \
		"vec3(float(cos(45.0*PIover180)), 0.0f, float(sin(45.0*PIover180)))," \
		"vec3(float(cos(-45.0*PIover180)), 0.0f, float(sin(-45.0*PIover180)))" \
		"};" \

		"float fGrassPatchSize = 3.0;" \
		"float fWindStrength = 8.0;" \

		"vec3 vWindDirection = vec3(1.0, 0.0, 1.0);" \
		"vWindDirection = normalize(vWindDirection);" \

		"for (int i = 0; i < 3; i++)" \
		"{" \


		"vec3 vBaseDirRotated = (rotationMatrix(vec3(0, 1, 0), sin(fTimePassed*0.7f)*0.1f)*vec4(vBaseDir[i], 1.0)).xyz;" \

		"vLocalSeed = vGrassFieldPos * float(i);" \
		"int iGrassPatch = randomInt(0, 3);" \

		"float fGrassPatchHeight = 3.5 + randZeroOne()*2.0;" \

		"float fTCStartX = float(iGrassPatch)*0.25f;" \
		"float fTCEndX = fTCStartX + 0.25f;" \

		"float fWindPower = 0.5f + sin(vGrassFieldPos.x / 30 + vGrassFieldPos.z / 30 + fTimePassed * (1.2f + fWindStrength / 20.0f));" \
		"if (fWindPower < 0.0f)" \
		"fWindPower = fWindPower * 0.2f;" \
		"else fWindPower = fWindPower * 0.3f;" \

		"fWindPower *= fWindStrength;" \

		"vec3 vTL = vGrassFieldPos - vBaseDirRotated * fGrassPatchSize*0.5f + vWindDirection * fWindPower;" \
		"vTL.y += fGrassPatchHeight;" \
		"gl_Position = mMVP * vec4(vTL, 1.0);" \
		"vTexCoord = vec2(fTCStartX, 1.0);" \
		"vWorldPos = vTL;" \
		"vEyeSpacePos = mMV * vec4(vTL, 1.0);" \
		"EmitVertex();" \


		"vec3 vBL = vGrassFieldPos - vBaseDir[i] * fGrassPatchSize*0.5f;" \
		"gl_Position = mMVP * vec4(vBL, 1.0);" \
		"vTexCoord = vec2(fTCStartX, 0.0);" \
		"vWorldPos = vBL;" \
		"vEyeSpacePos = mMV * vec4(vBL, 1.0);" \
		"EmitVertex();" \


		"vec3 vTR = vGrassFieldPos + vBaseDirRotated * fGrassPatchSize*0.5f + vWindDirection * fWindPower;" \
		"vTR.y += fGrassPatchHeight;" \
		"gl_Position = mMVP * vec4(vTR, 1.0);" \
		"vTexCoord = vec2(fTCEndX, 1.0);" \
		"vWorldPos = vTR;" \
		"vEyeSpacePos = mMV * vec4(vTR, 1.0);" \
		"EmitVertex();" \


		"vec3 vBR = vGrassFieldPos + vBaseDir[i] * fGrassPatchSize*0.5f;" \
		"gl_Position = mMVP * vec4(vBR, 1.0);" \
		"vTexCoord = vec2(fTCEndX, 0.0);" \
		"vWorldPos = vBR;" \
		"vEyeSpacePos = mMV * vec4(vBR, 1.0);" \
		"EmitVertex();" \

		"EndPrimitive();" \
		"}" \

		"}";

	glShaderSource(gGeometryShaderObject, 1, (const GLchar**)&geometryShaderSourceCode, NULL); //NULL is for NULL terminated source code std::string

	//compile geometry shader
	glCompileShader(gGeometryShaderObject);
	//Shader compilation error checking goes here...
	iInfoLogLength = 0;
	iShaderCompileStatus = 0;
	szInfoLog = NULL;
	glGetShaderiv(gGeometryShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
	if (iShaderCompileStatus == GL_FALSE) {
		glGetShaderiv(gGeometryShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0) {
			szInfoLog = (char*)malloc(iInfoLogLength);
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(gGeometryShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(grgpFileGrass, "GS%s\n", szInfoLog);
				free(szInfoLog);
				GRUninitializeGrass();
				exit(0);
			}
		}
	}
	/////////////////    F R A G M E N T S H A D E R            //////////////////////////
	//Define Vertex Shader Object
	//vec4(0.2, 1.0, 0.6, 1.0); 
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER); //This command will create the Shader Object
	//Now Write vertex shader code
	const GLchar* fragmentShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"	smooth in vec2 vTexCoord;" \


		"	smooth in vec3 vWorldPos;" \
		"smooth in vec4 vEyeSpacePos;" \

		"out vec4 outputColor;" \

		"uniform sampler2D gSampler;" \
		"uniform vec4 vColor = vec4(0.3, 1.0, 0.85,1.0);" \

		"uniform vec3 vEyePosition = vec3(0.0,9.0,85.0);" \



		"uniform float fAlphaTest = 0.05;" \
		"uniform float fAlphaMultiplier = 0.1;" \


		"void main(void)" \
		"{" \

		"vec4 vTexColor = texture2D(gSampler, vTexCoord);" \


		"float fNewAlpha = vTexColor.a*fAlphaMultiplier;" \
		"if (fNewAlpha < fAlphaTest)" \
		"discard;" \

		"if (fNewAlpha > 1.0f)" \
		"fNewAlpha = 0.5f;" \

		"vec4 vMixedColor = vTexColor * vColor;" \


		"outputColor = vec4(vMixedColor.zyx, fNewAlpha);" \

		"if(outputColor.r > 0.3)" \
		"discard;" \


		"}";

	//FragColor = vec4(1,1,1,1) = White Color
	//this means here we are giving color to the Triangle.




	//Specify above source code to the vertex shader object
	glShaderSource(gFragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	//Compile the vertex shader 
	glCompileShader(gFragmentShaderObject);
	//Code for catching the errors 
		   /*iShaderCompileStatus = 0;
		   iInfoLogLength = 0;*/
	szInfoLog = NULL;


	glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
	if (iShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written1;
				glGetShaderInfoLog(gFragmentShaderObject, iInfoLogLength, &written1, szInfoLog);
				fprintf(grgpFileGrass, "%s\n", szInfoLog);
				free(szInfoLog);
				GRUninitializeGrass();
				exit(0);


			}
		}
	}
	// CREATE SHADER PROGRAM OBJECT
	grshaderProgramObjectGrass = glCreateProgram();
	//attach vertex shader to the gShaderProgramObject
	glAttachShader(grshaderProgramObjectGrass, gVertexShaderObject);
	glAttachShader(grshaderProgramObjectGrass, gGeometryShaderObject);
	//attach fragment shader to the gShaderProgramObject
	glAttachShader(grshaderProgramObjectGrass, gFragmentShaderObject);


	//Pre-Linking  binding to vertexAttributes
	//glBindAttribLocation(gShaderProgramObject, AMC_ATTRIBUTE_POSITION, "vPosition");
	//Here the above line means that we are linking the GPU's variable vPosition with the CPU's  enum member  i.e AMC_ATTRIBUTE_POSITION .
	//So whatever changes will be done in AMC_ATTRIBUTE_POSITION , those will also reflect in vPosition

	//RULE : ALWAYS BIND THE ATTRIBUTES BEFORE LINKING AND BIND THE UNIFORM AFTER LINKING.

	//Link the shader program 
	glLinkProgram(grshaderProgramObjectGrass);

	//Code for catching the errors 
	GLint iProgramLinkStatus = 0;



	glGetProgramiv(grshaderProgramObjectGrass, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(grshaderProgramObjectGrass, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written3;
				glGetProgramInfoLog(grshaderProgramObjectGrass, iInfoLogLength, &written3, szInfoLog);
				fprintf(grgpFileGrass, "%s\n", szInfoLog);
				free(szInfoLog);
				GRUninitializeGrass();
				exit(0);


			}
		}
	}


	//POST Linking
	//Retrieving uniform locations 
	grmodelUniformGrass = glGetUniformLocation(grshaderProgramObjectGrass, "matrices.modelMatrix");
	grviewMatrixUniformGrass = glGetUniformLocation(grshaderProgramObjectGrass, "matrices.viewMatrix");
	greyeUniformGrass = glGetUniformLocation(grshaderProgramObjectGrass, "vEyePosition");
	grprojectionMatrixUniformGrass = glGetUniformLocation(grshaderProgramObjectGrass, "matrices.projMatrix");
	grnormalMatrixUniformGrass = glGetUniformLocation(grshaderProgramObjectGrass, "matrices.normalMatrix");
	grcoloruniformGrass = glGetUniformLocation(grshaderProgramObjectGrass, "vColor");
	grtimePassedUniformGrass = glGetUniformLocation(grshaderProgramObjectGrass, "fTimePassed");
	graplhaTestGrass = glGetUniformLocation(grshaderProgramObjectGrass, "fAlphaTest");
	gralphaMultiplierGrass = glGetUniformLocation(grshaderProgramObjectGrass, "fAlphaMultiplier");
	grtextureSamplerUniformGrass = glGetUniformLocation(grshaderProgramObjectGrass, "gSampler");

	//Here we have done all the preparations of data transfer from CPU to GPU

	const GLfloat triangleVertices[] =
	{
		0.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f
	};
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);

	glActiveTexture(GL_TEXTURE5);
	LoadHeightMapFromImageFree("heightmap.bmp");

	GRSetVarsGrass();
}

extern "C" void GRPrintGrass(const char* msg)
{
	fprintf(grgpFileGrass, msg);
}

extern "C" void GRResizeGrass(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	grperspectiveProjectionMatrixGrass = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
}


extern "C" void GRDisplayGrass()
{
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();;
	mat4 modelViewProjectionMatrix = mat4::identity();;
	mat4 modelViewMatrix = mat4::identity();;

	mat4 RotationMatrix = mat4::identity();;
	mat4 TranslateMatrix = mat4::identity();;
	mat4 scaleMatrix = mat4::identity();

	glUseProgram(grshaderProgramObjectGrass);
	modelMatrix = mat4::identity();
	//viewMatrix = mat4::identity();
	modelViewProjectionMatrix = mat4::identity();
	modelViewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(grmodelX_Grass, grmodelY_Grass, grmodelZ_Grass);
	modelMatrix = modelMatrix * scale(1.0f, 0.35f, 1.0f);
	viewMatrix = grgCameraMatrixGrass;
	glUniform1i(grtextureSamplerUniformGrass, 6);
	glUniformMatrix4fv(grprojectionMatrixUniformGrass, 1, GL_FALSE, (GLfloat*)&grperspectiveProjectionMatrixGrass);
	glUniformMatrix4fv(grviewMatrixUniformGrass, 1, GL_FALSE, (GLfloat*)&viewMatrix);
	glUniformMatrix4fv(grmodelUniformGrass, 1, GL_FALSE, modelMatrix);
	glUniform1f(grtimePassedUniformGrass, grtim1);
	//tim1 += 0.002f;
	glBindVertexArray(grvaoGrass);
	glDrawArrays(GL_POINTS, 0, griNumGrassTrianglesGrass);
	//glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

	glUseProgram(0);
}


extern "C" void GRSetViewMatrixGrass(vec3 cameraPlace, vec3 cameraView, vec3 up)
{
	grgCameraMatrixGrass = vmath::lookat(cameraPlace, cameraView, up);
}

extern "C" void GRUninitializeGrass()
{
	if (grvboGrass)
	{
		glDeleteBuffers(1, &grvboGrass);
		grvboGrass = 0;
	}
	if (grvaoGrass)
	{
		glDeleteVertexArrays(1, &grvaoGrass);
		grvaoGrass = 0;
	}

	if (grshaderProgramObjectGrass)
	{
		glUseProgram(grshaderProgramObjectGrass);
		GLsizei grshaderCount;
		int i;
		glGetProgramiv(grshaderProgramObjectGrass, GL_ATTACHED_SHADERS, &grshaderCount);

		GLuint* grpShaders = NULL;
		grpShaders = (GLuint*)malloc(sizeof(GLuint) * grshaderCount);
		if (grpShaders == NULL)
		{
			exit(0);
		}
		glGetAttachedShaders(grshaderProgramObjectGrass, grshaderCount, &grshaderCount, grpShaders);
		for (i = 0; i < grshaderCount; i++)
		{
			glDetachShader(grshaderProgramObjectGrass, grpShaders[i]);
			glDeleteShader(grpShaders[i]);
			grpShaders[i] = 0;
		}
		glUseProgram(0);
	}

	if (grgpFileGrass)
	{
		fprintf(grgpFileGrass, "\n **** End ****\nLog File closed successfully. \n Program terminated successfully");
		fclose(grgpFileGrass);
		grgpFileGrass = NULL;
	}
}

extern "C" void GRPrintVarsGrass()
{
	fprintf(grgpFileGrass, "\n model x : %f, model y : %f, model z : %f", grmodelX_Grass, grmodelY_Grass, grmodelZ_Grass);
}

extern "C" void GRSetVarsGrass()
{
	// translate varibles
	grmodelX_Grass = -15.379665f;
	grmodelY_Grass = -2.08000f;
	grmodelZ_Grass = 0.0f;


}

extern "C" void GRIncModelXGrass()
{
	grmodelX_Grass = grmodelX_Grass + 0.02f;
}

extern "C" void GRIncModelYGrass()
{
	grmodelY_Grass = grmodelY_Grass + 0.02f;
}

extern "C" void GRIncModelZGrass()
{
	grmodelZ_Grass = grmodelZ_Grass + 1.0f;
}

extern "C" void GRDecModelXGrass()
{
	grmodelX_Grass = grmodelX_Grass - 0.02f;
}

extern "C" void GRDecModelYGrass()
{
	grmodelY_Grass = grmodelY_Grass - 0.02f;
}

extern "C" void GRDecModelZGrass()
{
	grmodelZ_Grass = grmodelZ_Grass - 1.0f;
}


bool LoadHeightMapFromImageFree(std::string sImagePath)
{
	grvRenderScaleGrass = glm::vec3(100.0f, 25.0f, 100.0f);

	if (grbLoadedGrass)
	{
		grbLoadedGrass = false;
		//	ReleaseHeightmap();
	}
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(sImagePath.c_str(), 0); // Check the file signature and deduce its format

	if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(sImagePath.c_str());

	if (fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, sImagePath.c_str());
	if (!dib)
		return false;

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data
	griRowsGrass = FreeImage_GetHeight(dib);
	griColsGrass = FreeImage_GetWidth(dib);

	fprintf(grgpFileGrass, "\niRows : %d\n", griRowsGrass);
	fprintf(grgpFileGrass, "\niCols : %d\n", griColsGrass);
	// We also require our image to be either 24-bit (classic RGB) or 8-bit (luminance)
	if (bDataPointer == NULL || griRowsGrass == 0 || griColsGrass == 0 || (FreeImage_GetBPP(dib) != 24 && FreeImage_GetBPP(dib) != 8))
		return false;

	// How much to increase data pointer to get to next pixel data
	unsigned int ptr_inc = FreeImage_GetBPP(dib) == 24 ? 3 : 1;
	// Length of one row in data
	unsigned int row_step = ptr_inc * griColsGrass;

	//vboHeightmapData.CreateVBO();
	// All vertex data are here (there are iRows*iCols vertices in this heightmap), we will get to normals later
	grvertexDataGrass = std::vector< std::vector< glm::vec3> >(griRowsGrass, std::vector<glm::vec3>(griColsGrass));
	std::vector< std::vector< glm::vec2> > vCoordsData(griRowsGrass, std::vector<glm::vec2>(griColsGrass));

	float fTextureU = float(griColsGrass) * 0.1f;
	float fTextureV = float(griRowsGrass) * 0.1f;
	int m = 0;
	FOR(griGrass, griRowsGrass)
	{
		FOR(j, griColsGrass)
		{
			float fScaleC = float(j) / float(griColsGrass - 1);
			float fScaleR = float(griGrass) / float(griRowsGrass - 1);
			float fVertexHeight = float(*(bDataPointer + row_step * griGrass + j * ptr_inc)) / 255.0f;
			grvertexDataGrass[griGrass][j] = glm::vec3(-0.5f + fScaleC, fVertexHeight, -0.5f + fScaleR);
			vCoordsData[griGrass][j] = glm::vec2(fTextureU * fScaleC, fTextureV * fScaleR);
			//fprintf_s(grgpFile, "vVertexData : %f \t %f\n", vVertexData[i][j]);
		//	fprintf(grgpFile, "I am in loop %d", m);
			//m++;
		}
	}

	// Normals are here - the heightmap contains ( (iRows-1)*(iCols-1) quads, each one containing 2 triangles, therefore array of we have 3D array)
	std::vector< std::vector<glm::vec3> > vNormals[2];
	FOR(griGrass, 2)vNormals[griGrass] = std::vector< std::vector<glm::vec3> >(griRowsGrass - 1, std::vector<glm::vec3>(griColsGrass - 1));

	FOR(i, griRowsGrass - 1)
	{
		FOR(j, griColsGrass - 1)
		{
			glm::vec3 vTriangle0[] =
			{
				grvertexDataGrass[i][j],
				grvertexDataGrass[i + 1][j],
				grvertexDataGrass[i + 1][j + 1]
			};
			glm::vec3 vTriangle1[] =
			{
				grvertexDataGrass[i + 1][j + 1],
				grvertexDataGrass[i][j + 1],
				grvertexDataGrass[i][j]
			};

			glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0] - vTriangle0[1], vTriangle0[1] - vTriangle0[2]);
			glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0] - vTriangle1[1], vTriangle1[1] - vTriangle1[2]);

			vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
			vNormals[1][i][j] = glm::normalize(vTriangleNorm1);

		}
	}

	std::vector< std::vector<glm::vec3> > vFinalNormals = std::vector< std::vector<glm::vec3> >(griRowsGrass, std::vector<glm::vec3>(griColsGrass));

	FOR(griGrass, griRowsGrass)
		FOR(j, griColsGrass)
	{
		// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average std::vector
		// of all adjacent triangles' normals

		glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

		// Look for upper-left triangles
		if (j != 0 && griGrass != 0)
			FOR(k, 2)vFinalNormal += vNormals[k][griGrass - 1][j - 1];
		// Look for upper-right triangles
		if (griGrass != 0 && j != griColsGrass - 1)vFinalNormal += vNormals[0][griGrass - 1][j];
		// Look for bottom-right triangles
		if (griGrass != griRowsGrass - 1 && j != griColsGrass - 1)
			FOR(k, 2)vFinalNormal += vNormals[k][griGrass][j];
		// Look for bottom-left triangles
		if (griGrass != griRowsGrass - 1 && j != 0)
			vFinalNormal += vNormals[1][griGrass][j - 1];
		vFinalNormal = glm::normalize(vFinalNormal);

		vFinalNormals[griGrass][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row

	}




	float fGrassPatchOffsetMin = 1.5f;
	float fGrassPatchOffsetMax = 2.5f;
	float fGrassPatchHeight = 5.0f;

	glm::vec3 vCurPatchPos(-grvRenderScaleGrass.x * 0.5f + fGrassPatchOffsetMin, 0.0f, grvRenderScaleGrass.z * 0.5f - fGrassPatchOffsetMin);
	griNumGrassTrianglesGrass = 0;

	while (vCurPatchPos.x < grvRenderScaleGrass.x * 0.5f)
	{
		vCurPatchPos.z = grvRenderScaleGrass.z * 0.5f - fGrassPatchOffsetMin;

		while (vCurPatchPos.z > -grvRenderScaleGrass.z * 0.5f)
		{
			vCurPatchPos.y = GetHeightFromRealVector(vCurPatchPos) - 0.3f;
			AddData(&vCurPatchPos, sizeof(glm::vec3));

			griNumGrassTrianglesGrass += 1;

			vCurPatchPos.z -= fGrassPatchOffsetMin + (fGrassPatchOffsetMax - fGrassPatchOffsetMin) * float(rand() % 1000) * 0.001f;
		}

		vCurPatchPos.x += fGrassPatchOffsetMin + (fGrassPatchOffsetMax - fGrassPatchOffsetMin) * float(rand() % 1000) * 0.001f;

	}
	glGenVertexArrays(1, &grvaoGrass);
	glBindVertexArray(grvaoGrass);
	glGenBuffers(1, &grvboGrass);
	glBindBuffer(GL_ARRAY_BUFFER, grvboGrass);
	glBufferData(GL_ARRAY_BUFFER, grdataGrass.size(), &grdataGrass[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	grbLoadedGrass = true; // If get here, we succeeded with generating heightmap
	return true;
}
void AddData(void* ptrData, UINT uiDataSize)
{
	grdataGrass.insert(grdataGrass.end(), (BYTE*)ptrData, (BYTE*)ptrData + uiDataSize);
	grcurrentSizeGrass += uiDataSize;
}
void LoadAllTextures()
{
	std::string sTextureNames[] = { "grass.dds" };
	glActiveTexture(GL_TEXTURE6);
	LoadTexture2D("grassTexture\\grassPack6.dds", true);
	SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_TRILINEAR);
	SetWrap();
	SetAnisotropyLevel(4.0f);
}

void SetFiltering(int a_tfMagnification, int a_tfMinification)
{
	glBindSampler(0, grsamplerGrass);

	// Set magnification filter
	if (a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(grsamplerGrass, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if (a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(grsamplerGrass, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if (a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(grsamplerGrass, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(grsamplerGrass, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(grsamplerGrass, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(grsamplerGrass, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(grsamplerGrass, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	grtfMinificationGrass = a_tfMinification;
	grtfMagnificationGrass = a_tfMagnification;
}
void SetWrap()
{
	glBindSampler(0, grsamplerGrass);
	glSamplerParameteri(grsamplerGrass, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(grsamplerGrass, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void SetAnisotropyLevel(float fAnisotropyLevel)
{
	glSamplerParameterf(grsamplerGrass, GL_TEXTURE_MAX_ANISOTROPY_EXT, fAnisotropyLevel);
}
void CreateFromData(BYTE* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps)
{
	glGenTextures(1, &grtextureGrass);
	glBindTexture(GL_TEXTURE_2D, grtextureGrass);
	if (format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	// We must handle this because of internal format parameter
	else if (format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	if (bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	glGenSamplers(1, &grsamplerGrass);

	grspathGrass = "";
	grbMipMapGeneratedGrass = bGenerateMipMaps;
	grwidthGrass = a_iWidth;
	grheightGrass = a_iHeight;
	grBPPGrass = a_iBPP;
}

float GetHeightFromRealVector(glm::vec3 vRealPosition)
{
	int iColumn = int((vRealPosition.x + grvRenderScaleGrass.x * 0.5f) * float(griColsGrass) / (grvRenderScaleGrass.x));
	int iRow = int((vRealPosition.z + grvRenderScaleGrass.z * 0.5f) * float(griRowsGrass) / (grvRenderScaleGrass.z));

	iColumn = glm::min(iColumn, griColsGrass - 1);
	iRow = glm::min(iRow, griRowsGrass - 1);

	iColumn = glm::max(iColumn, 0);
	iRow = glm::max(iRow, 0);

	return grvertexDataGrass[iRow][iColumn].y;
}


bool LoadTexture2D(std::string a_sPath, bool bGenerateMipMaps)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);
	fif = FreeImage_GetFileType(a_sPath.c_str(), 0); // Check the file signature and deduce its format
	if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(a_sPath.c_str());

	if (fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, a_sPath.c_str());
	if (!dib)
		return false;
	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data
	// If somehow one of these failed (they shouldn't), return failure
	if (bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
		return false;

	GLenum format = 0;
	int bada = FreeImage_GetBPP(dib);
	if (FreeImage_GetBPP(dib) == 32)format = GL_RGBA;
	if (FreeImage_GetBPP(dib) == 24)format = GL_BGR;
	if (FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;
	if (format != 0)

		CreateFromData(bDataPointer, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, bGenerateMipMaps);

	FreeImage_Unload(dib);

	if (format == 0)
		return false;

	grspathGrass = a_sPath;

	return true; // Success

}
