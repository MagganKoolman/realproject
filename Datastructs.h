#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>

struct Vertex {
	float x, y, z;
	GLbyte r, g, b;
};

struct Material {
	glm::vec3 Ka, Kd, Ks;
	std::string materialName;
	GLuint texid;
};

