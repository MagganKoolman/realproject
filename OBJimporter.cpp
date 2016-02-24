#pragma once
#include "OBJimporter.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <SOIL\SOIL.h>
#include <iostream>

using namespace std;

std::string replace(std::string str, char a, char b) {
	std::string result = "";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == a) {
			result.push_back(b);
		}
		else {
			result.push_back(str[i]);
		}
	}
	return result;
}

OBJimporter::OBJimporter() {
	this->standardMat= { vec3(1,0,0),vec3(0,1,0),vec3(0,0,1), "standardMaterial", 0 };
	objectOffset = 0;
}
OBJimporter::~OBJimporter() {
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
}

GLuint OBJimporter::CreateTexture(string fileName)
{
	// Load textures
	GLuint texIndex;
	glGenTextures(1, &texIndex);

	int width, height;
	unsigned char* image;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texIndex);
	image = SOIL_load_image(("realproject/models/" + fileName).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texIndex;
}

void OBJimporter::loadMaterials(string materials)
{
	ifstream matFile("realproject/models/" + materials);
	istringstream inputString;
	string line, special, input, name;
	Material* mat = new Material();
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
				if (name == allMaterials[i]->materialName) {
					newMaterial = false;
				}
			}
			if (newMaterial) {
				if (!first) {
					allMaterials.push_back(mat);
				}
				first = false;

				mat->materialName = "";

				mat->materialName = name;
				mat->texid = 0;
			}
		}
		if (newMaterial) {
			if (line == "Ka")
			{
				inputString >> special >> mat->Ka.x >> mat->Ka.y >> mat->Ka.z;
			}
			else if (line == "Kd")
			{
				inputString >> special >> mat->Kd.x >> mat->Kd.y >> mat->Kd.z;
			}
			else if (line == "Ks")
			{
				inputString >> special >> mat->Ks.x >> mat->Ks.y >> mat->Ks.z;
			}
			else if (line == "ma")
			{
				inputString >> special >> name;
				if (!texmapid[name]) {
					texmapid[name] = CreateTexture(name);
				}
				mat->texid = texmapid[name];
			}
		}
	}
	if (newMaterial) {
		allMaterials.push_back(mat);
	}
	else {
		delete mat;
	}
}

void OBJimporter::loadObj(string fileName) {
	string input;
	ifstream objFile(fileName);
	istringstream inputString;
	string line, special, mat;
	char* specialChar = (char*)malloc(5);
	vtx v;
	nvtx n;
	tex t;
	face f;
	object* o = new object();
	o->mtl = &standardMat;

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
			inputString.str(replace(input, '/', ' '));
			inputString >> special >> f.v[0] >> f.t[0] >> f.n[0] >> f.v[1] >> f.t[1] >> f.n[1] >>f.v[2] >> f.t[2] >> f.n[2];
			o->faces.push_back(f);
		}
		else if (line == "us") {
			inputString >> special >> mat;
			if (o->faces.size() != 0) {
				objects.push_back(o);
				o = new object();
				o->mtl = &standardMat;
			}
			for (int i = 0; i < allMaterials.size(); i++) {
				if (mat == allMaterials[i]->materialName) {
					o->mtl = allMaterials[i];
					break;
				}
			}
		}
		else if (line == "mt") {
			inputString >> special >> mat;
			loadMaterials(mat);
		}
	}
	if (o->faces.size() != 0) {
		objects.push_back(o);
	}
	free(specialChar);
	objFile.close();
}
std::vector<Model*> OBJimporter::CreateTriangleData()
{

	TriangleVertex *triangleVertices;

	std::vector<Model*> result;

	Model* model = nullptr;

	int vIndex;
	int tIndex;
	int nIndex;

	GLuint vaoid;
	GLuint buffid;

	for (int o = objectOffset; o < objects.size(); o++) {
		model = new Model();
		triangleVertices = new TriangleVertex[objects[o]->faces.size() * 3];
		for (int i = 0; i < objects[o]->faces.size(); i++) {
			for (int j = 0; j < 3; j++) {
				vIndex = objects[o]->faces[i].v[j] - 1;
				tIndex = objects[o]->faces[i].t[j] - 1;
				nIndex = objects[o]->faces[i].n[j] - 1;
				triangleVertices[(3 * i) + j] = { vertices[vIndex].x, vertices[vIndex].y, vertices[vIndex].z, 
					normalVertices[nIndex].x, normalVertices[nIndex].y, normalVertices[nIndex].z,
					textureCoords[tIndex].u, textureCoords[tIndex].v };
			}
		}
		
		glGenBuffers(1, &buffid);
		glBindBuffer(GL_ARRAY_BUFFER, buffid);
		glBufferData(GL_ARRAY_BUFFER, objects[o]->faces.size() * 3 * sizeof(TriangleVertex), triangleVertices, GL_STATIC_DRAW);

		model->setBUFFid(buffid);
		model->setMaterial(objects[o]->mtl);
		model->setSize(objects[o]->faces.size() * 3);

		result.push_back(model);

		delete[] triangleVertices;
	}
	objectOffset = objects.size();
	vertices.clear();
	normalVertices.clear();
	textureCoords.clear();

	return result;
}

Model* OBJimporter::getGround(std::string heightMapFile) {
	GLuint GText = CreateTexture("head_512.png");

	int width, height;
	unsigned char* image;
	image = SOIL_load_image(("realproject/models/" + heightMapFile).c_str(), &width, &height, 0, SOIL_LOAD_L);

	TriangleVertex* vertices = new TriangleVertex[(int)(width*height*1.5)];
	int index = 0;
	for (int i = 0; i < height/2-1; i++) { 
		for (int j = 0; j < width/2-1; j++) { //float x, y, z, nx, ny, nz, u, v;                  (float)((int)image[index++]) / 200 - 2
			vertices[index++] = { ((float)2*j - width/2)/5.0f, (float)((int)image[2*i*width+2*j]) / 25 -5, ((float)2*i - height / 2) / 5.0f, 0,1,0, (float)2*j / width, (float)2*i / height };
			vertices[index++] = { ((float)2*j +2 - width / 2) / 5.0f, (float)((int)image[2*i*width+2*(j+1)]) / 25-5, ((float)2*i - height / 2) / 5.0f, 0,1,0, (float)(2*(j+1)) / width, (float)2*i / height };
			vertices[index++] = { ((float)2*j - width / 2) / 5.0f, (float)((int)image[(2*(i+1))*width+2*j]) / 25-5, ((float)2*i + 2 - height / 2) / 5.0f, 0,1,0, (float)2*j / width, (float)(2*(i+1))/ height };
			vertices[index++] = { ((float)2*j+2 - width / 2) / 5.0f, (float)((int)image[2*i*width+2*(j+1)]) / 25-5, ((float)2*i - height / 2) / 5.0f, 0,1,0, (float)(2*(j+1)) / width, (float)2*i / height };
			vertices[index++] = { ((float)2*j - width / 2) / 5.0f, (float)((int)image[(2*(i+1))*width+2*j]) / 25-5, ((float)2*i+2 - height / 2) / 5.0f, 0,1,0, (float)2*j / width, (float)(2*(i+1)) / height };
			vertices[index++] = { ((float)2*j+2 - width / 2) / 5.0f, (float)((int)image[(2*(i+1))*width+2*(j+1)]) / 25-5, ((float)2*i+2 - height / 2) / 5.0f, 0,1,0, (float)(2*(j+1)) / width, (float)(2*(i+1)) / height };
		}
	}

	Model* result = new Model();
	GLuint buffid;
	glGenBuffers(1, &buffid);
	glBindBuffer(GL_ARRAY_BUFFER, buffid);
	glBufferData(GL_ARRAY_BUFFER, index * sizeof(TriangleVertex), vertices, GL_STATIC_DRAW);
	
	result->setBUFFid(buffid);
	result->setSize(index);
	Material* mat  = new Material;
	mat->Ka = vec3(0.03,0.08,0.05);
	mat->Kd = vec3(0, 0, 0);
	mat->Ks = vec3(0, 0, 0);
	mat->materialName = "";
	mat->texid = GText;
	result->setMaterial(mat);

	delete[] vertices;
	return result;
}