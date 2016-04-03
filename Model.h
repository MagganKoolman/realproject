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

	vec3* bBox;
public:
	Model();
	Model(Model &m);
	~Model();

	void initNormalTexture(const std::string &filePath);
	void setMaterial(Material* material);
	void setVBOid(GLuint vbo);
	void setBUFFid(GLuint bid);
	void setSize(int s);
	void draw(GLuint spID);
	void addTranslation(glm::vec3 translation);
	void Scale(glm::vec3 scale);
	void createBBox(const std::string &filePath);
	Material* getMat();
	GLuint getBuffID() const;
	int getSize();
	vec3* getBBox() const;
};