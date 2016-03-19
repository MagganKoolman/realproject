#pragma once

#include <gl\glew.h>
#include "Datastructs.h"
#include "glm\gtc\matrix_transform.hpp"
#include <SOIL\SOIL.h>

class Model {
private:
	GLuint _VBOid;
	GLuint _BUFFid;
	Material* _mat;
	GLuint _normalTexture;
	glm::mat4x4 worldMat;
	int size;
	bool normalMaping;

	struct boundingBox {
		vec3 center = vec3(0,0,0);
		float xLeast, xMost, yLeast, yMost, zLeast, zMost = 0;
	} bBox;
public:
	Model();
	~Model();

	void initNormalTexture(const std::string &filePath);
	void setMaterial(Material* material);
	void setVBOid(GLuint vbo);
	void setBUFFid(GLuint bid);
	void setSize(int s);
	void draw(GLuint spID);
	void addTranslation(glm::vec3 translation);
	void createBBox(const std::string &filePath);
	Material* getMat();
	GLuint getBuffID();
	int getSize();
};