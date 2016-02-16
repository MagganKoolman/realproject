#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>

typedef glm::vec3 vec3;

struct Vertex {
	float x, y, z;
	GLbyte r, g, b;
};

struct Material {
	glm::vec3 Ka, Kd, Ks;
	std::string materialName;
	GLuint texid;
};

struct TriangleVertex
{
	float x, y, z, u, v, nx, ny, nz;
};