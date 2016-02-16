#pragma once

#include <gl\glew.h>
#include "Datastructs.h"

class Model {
private:

	GLuint _VBOid;
	GLuint _BUFFid;
	Material* _mat;
public:
	Model();
	~Model();
	void setMaterial(Material* material);
	void setVBOid(GLuint vbo);
	void setBUFFid(GLuint bid);
};