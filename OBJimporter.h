#pragma once

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>

#include "Model.h"
#include "Datastructs.h"

#include <gl\glew.h>
#include <SOIL\SOIL.h>

vector<Material> allMaterials;
vector<vtx> vertices;
vector<nvtx> normalVertices;
vector<tex> textureCoords;

map<string, GLuint> texmapid;

using namespace std;
struct vtx {
	float x, y, z;
};
struct nvtx {
	float x, y, z;
};
struct tex {
	float u, v;
};
struct face {
	int v[3], n[3], t[3];
};

GLuint CreateTexture(string fileName)
{
	// Load textures
	GLuint texIndex;
	glGenTextures(1, &texIndex);

	int width, height;
	unsigned char* image;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texIndex);
	image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texIndex;
}

void loadMaterials(string materials)
{
	ifstream matFile(materials);
	istringstream inputString;
	string line, special, input, name;
	Material mat;
	bool first = true;
	bool newMaterial = true;
	while (getline(matFile, input))
	{
		inputString.clear();
		inputString.str(input);
		line = input.substr(0, 2);

		if (line == "ne")
		{
			inputString >> special >> name;
			newMaterial = true;
			for (int i = 0; i < allMaterials.size(); i++) {
				if (name == allMaterials[i].materialName) {
					newMaterial = false;
				}
			}
			if (newMaterial) {
				if (!first) {
					allMaterials.push_back(mat);
				}
				first = false;

				mat.materialName = "";

				mat.materialName = name;
				mat.texid = 0;
			}
		}
		if (newMaterial) {
			if (line == "Ka")
			{
				inputString >> special >> mat.Ka.x >> mat.Ka.y >> mat.Ka.z;
			}
			else if (line == "Kd")
			{
				inputString >> special >> mat.Kd.x >> mat.Kd.y >> mat.Kd.z;
			}
			else if (line == "Ks")
			{
				inputString >> special >> mat.Ks.x >> mat.Ks.y >> mat.Ks.z;
			}
			else if (line == "ma")
			{
				inputString >> special >> name;
				if (!texmapid[name]) {
					texmapid[name] = CreateTexture(name);
				}
				mat.texid = texmapid[name];
			}
		}
	}
	if (newMaterial) {
		allMaterials.push_back(mat);
	}
}

void loadObj(string fileName) {
	string input;
	ifstream objFile(fileName);
	istringstream inputString;
	string line, special, mat;
	char* specialChar = (char*)malloc(5);
	vtx v;
	nvtx n;
	tex t;
	face f;
	Model* o = new Object();
	o->mat = &standardMat;

	while (getline(objFile, input))
	{
		inputString.clear();
		inputString.str(input);
		line = input.substr(0, 2);

		if (line == "v ") {
			inputString >> special >> v.x >> v.y >> v.z;
			vertices.push_back(v);
		}
		else if (line == "vn") {
			inputString >> special >> n.x >> n.y >> n.z;
			normalVertices.push_back(n);
		}
		else if (line == "vt") {
			inputString >> special >> t.u >> t.v;
			textureCoords.push_back(t);
		}
		else if (line == "f ") {
			sscanf(input.c_str(), "%s %i/%i/%i %i/%i/%i %i/%i/%i", specialChar, &f.v[0], &f.t[0], &f.n[0], &f.v[1], &f.t[1], &f.n[1], &f.v[2], &f.t[2], &f.n[2]);
			o->addFace(f);
		}
		else if (line == "us") {
			inputString >> special >> mat;
			if (!o->isEmpty()) {
				objects.push_back(o);
				o = new Object();
				o->mat = &standardMat;
			}
			for (int i = 0; i < allMaterials.size(); i++) {
				if (mat == allMaterials[i].materialName) {
					o->mat = &allMaterials[i];
					break;
				}
			}
		}
		else if (line == "mt") {
			inputString >> special >> mat;
			loadMaterials(mat);
		}
	}
	if (!o->isEmpty()) {
		objects.push_back(o);
	}
	free(specialChar);
	objFile.close();
	CreateTriangleData();
}

void CreateTriangleData()
{
	TriangleVertex *triangleVertices;

	int vIndex;
	int tIndex;
	int nIndex;
	for (uint o = objectOffset; o < objects.size(); o++) {
		triangleVertices = new TriangleVertex[objects[o]->faces.size() * 3];
		for (uint i = 0; i < objects[o]->faces.size(); i++) {
			for (int j = 0; j < 3; j++) {
				vIndex = objects[o]->faces[i].v[j] - 1;
				tIndex = objects[o]->faces[i].t[j] - 1;
				nIndex = objects[o]->faces[i].n[j] - 1;
				triangleVertices[(3 * i) + j] = { vertices[vIndex].x, vertices[vIndex].y, vertices[vIndex].z, textureCoords[tIndex].u, textureCoords[tIndex].v,
					normalVertices[nIndex].x, normalVertices[nIndex].y ,normalVertices[nIndex].z };
			}
		}
		glGenVertexArrays(1, &gVertexAttribute);
		objects[o]->VAOid = gVertexAttribute;
		glBindVertexArray(objects[o]->VAOid);
		glGenBuffers(1, &gVertexBuffer);
		objects[o]->BUFFid = gVertexBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, objects[o]->BUFFid);
		glBufferData(GL_ARRAY_BUFFER, objects[o]->faces.size() * 3 * sizeof(TriangleVertex), triangleVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		delete[] triangleVertices;
	}
	objectOffset = objects.size();
	vertices.clear();
	normalVertices.clear();
	textureCoords.clear();
}