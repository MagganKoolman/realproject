#pragma once

#include <gl\glew.h>
#include "Datastructs.h"

class Model {
private:

	GLuint VBOid;
	GLuint BUFFid;
	Material* mat;
public:
	Model();
	~Model();
	
};