#pragma once

#include<vector>
#include<map>
#include<assimp/postprocess.h>
#include<assimp/mesh.h>
#include<assimp/scene.h>
#include <assimp/Importer.hpp>

#include"VboData.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#pragma warning(disable : 4996)

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	FILE* gpFile;
	string directory;
	bool gammaCorrection;

	Model(string const& path, FILE* gpFile)
	{
		this->gpFile = gpFile;
		loadModel(path);
	}
	void loadModel(string const& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals
			| aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		//?bool bcheck = scene->HasMaterials();
		// error check
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			fprintf(gpFile, "\n Assimp : Error while loading model : %s", importer.GetErrorString());
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// get pointer to meshes in the model
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh  processMesh(aiMesh* mesh, const aiScene* scene)
	{
		vector<VboData> vboData;
		vector<unsigned int> indices;
		vector<Texture> textures;
		vector<float> colors;
		aiColor3D color(0.0f, 0.0f, 0.0f);

		int matIndex = mesh->mMaterialIndex;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			VboData data;
			vmath::vec3 vect;
			vect[0] = mesh->mVertices[i].x;
			vect[1] = mesh->mVertices[i].y;
			vect[2] = mesh->mVertices[i].z;
			data.Position = vect;

			if (scene->HasMaterials())
			{
				int matIndex = mesh->mMaterialIndex;
				aiMaterial *mat = scene->mMaterials[matIndex];
				// ambient
				mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
				colors.push_back(color[0]);
				colors.push_back(color[1]);
				colors.push_back(color[2]);
				// diffuse
				mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
				colors.push_back(color[0]);
				colors.push_back(color[1]);
				colors.push_back(color[2]);
				// specular
				mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
				colors.push_back(color[0]);
				colors.push_back(color[1]);
				colors.push_back(color[2]);	
			}
			// normals
			if (mesh->HasNormals())
			{
				vect[0] = mesh->mNormals[i].x;
				vect[1] = mesh->mNormals[i].y;
				vect[2] = mesh->mNormals[i].z;
				data.Normal = vect;
			}
			// trxture coords
			if (mesh->mTextureCoords[0])
			{
				vmath::vec2 vec;
				vec[0] = mesh->mTextureCoords[0][i].x;
				vec[1] = mesh->mTextureCoords[0][i].y;
				data.TexCoord = vec;

				// tangent
				vect[0] = mesh->mTangents[i].x;
				vect[1] = mesh->mTangents[i].y;
				vect[2] = mesh->mTangents[i].z;
				data.Tangent = vect;

				// bitagnent
				vect[0] = mesh->mBitangents[i].x;
				vect[1] = mesh->mBitangents[i].y;
				vect[2] = mesh->mBitangents[i].z;
				data.Bitangent = vect;
			}
			else
				data.TexCoord = vmath::vec2(0.0f, 0.0f);

			vboData.push_back(data);
		}

		// get indices from faces
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// process material
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// diffuse maps
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// specular maps
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// normal map
		vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// height map
		vector<Texture> hieghtMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), hieghtMaps.begin(), hieghtMaps.end());

		return(Mesh(vboData, indices, textures, colors));
	}

	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Texture> vecTextures;
		for (int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			// check if texture is already loaded, if so, continue to next iteration by skipping to load texture
			for (int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					vecTextures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}	
			}
			if (!skip)
			{
				// if texture hasn't been loaded yet, then load the texture
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				vecTextures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}
		return vecTextures;
	}
};

unsigned TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string fileName = string(path);
	fileName = directory + '/' + fileName;
	unsigned int textureId;
	glGenTextures(1, &textureId);
	
	int width, height, nrComponents;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}

	return(textureId);

}
