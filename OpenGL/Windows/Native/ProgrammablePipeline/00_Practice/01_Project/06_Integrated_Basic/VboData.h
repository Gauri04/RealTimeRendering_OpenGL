#pragma once
#include<iostream>
#include<gl\glew.h> // glew.h must be included BEFORE gl.h
#include<gl\GL.h>
#include"vmath.h"
#include"CommonCode.h"
using namespace vmath;

#pragma warning(disable : 4996)




struct VboData
{
	vec3 Position;
	vec2 TexCoord;
	vec3 Normal;
	vec3 Color;
	vec3 Tangent;
	vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public :
	std::vector<struct VboData> vboData;
	std::vector<unsigned int> indices;
	std::vector<struct Texture> textures;
	std::vector<float> colors;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int vao;


	Mesh(std::vector<VboData> vboData, std::vector<unsigned int> indices, std::vector<Texture> textures, std::vector<float> colors)
	{
		this->vboData = vboData;
		this->indices = indices;
		this->textures = textures;
		this->colors = colors;
		setupMesh();
	}

	void setupMesh()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(VboData), &vboData[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// position vbo
		glEnableVertexAttribArray(GR_ATTRIBUTE_POSITION);
		glVertexAttribPointer(GR_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(VboData), (void *)0);

		// texcoord
		glEnableVertexAttribArray(GR_ATTRIBUTE_TEXCOORD);
		glVertexAttribPointer(GR_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(VboData), (void*)offsetof(VboData, TexCoord));

		// color
		glEnableVertexAttribArray(GR_ATTRIBUTE_COLOR);
		glVertexAttribPointer(GR_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(VboData), (void*)offsetof(VboData, Color));

		// normal
		glEnableVertexAttribArray(GR_ATTRIBUTE_NORMAL);
		glVertexAttribPointer(GR_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(VboData), (void*)offsetof(VboData, Normal));

		// tangent
		glEnableVertexAttribArray(GR_ATTRIBUTE_TANGENT);
		glVertexAttribPointer(GR_ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(VboData), (void*)offsetof(VboData, Tangent));

		// bitangent
		glEnableVertexAttribArray(GR_ATTRIBUTE_BITANGENT);
		glVertexAttribPointer(GR_ATTRIBUTE_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(VboData), (void*)offsetof(VboData, Bitangent));

		glBindVertexArray(0);
	}
};

