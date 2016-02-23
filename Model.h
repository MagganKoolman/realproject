#pragma once

#include <gl\glew.h>
#include "Datastructs.h"
#include "glm\gtc\matrix_transform.hpp"

class Model {
private:

	GLuint _VBOid;
	GLuint _BUFFid;
	Material* _mat;
	glm::mat4x4 worldMat;
	int size;
public:
	Model();
	~Model();
	void setMaterial(Material* material);
	void setVBOid(GLuint vbo);
	void setBUFFid(GLuint bid);
	void setSize(int s);
	void draw(GLuint spID);
	void addTranslation(glm::vec3 translation);
};