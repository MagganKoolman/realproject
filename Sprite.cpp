#include "Sprite.h"
#include "datastructs.h"

#include <cstddef>

Sprite::Sprite() {
	_vboID = 0;
}

Sprite::~Sprite() {
	if (_vboID != 0)
		glDeleteBuffers(1, &_vboID);
}

void Sprite::init(float x, float y, float width, float height) {
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	Vertex vertexData[6];

	//first triangle
	vertexData[0].x = x + width;
	vertexData[0].y = y + height;
	vertexData[0].z = -2;
	
	vertexData[1].x = x;
	vertexData[1].y = y + height;
	vertexData[1].z = -2;

	vertexData[2].x = x;
	vertexData[2].y = y;
	vertexData[2].z = -2;

	//second triangle
	vertexData[3].x = x;
	vertexData[3].y = y;
	vertexData[3].z = -2;

	vertexData[4].x = x + width;
	vertexData[4].y = y;
	vertexData[4].z = -2;

	vertexData[5].x = x + width;
	vertexData[5].y = y + height;
	vertexData[5].z = -2;

	for (int i = 0; i < 6; i++)
	{
		vertexData[i].r = 200;
		vertexData[i].g = 120;
		vertexData[i].b = 80;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw() {
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, r));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}