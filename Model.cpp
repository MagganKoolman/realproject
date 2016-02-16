#include "Model.h"

Model::Model(): _VBOid(0), _mat(nullptr) {

}

Model::~Model() {

}
void Model::setMaterial(Material* material) {
	this->_mat = material;
}
void Model::setVBOid(GLuint vbo) {
	this->_VBOid = vbo;
}
void Model::setBUFFid(GLuint bid) {
	this->_BUFFid = bid;
}
void Model::setSize(int s) {
	this->size = size;
}
void Model::draw() {
	glBindBuffer(GL_ARRAY_BUFFER, _BUFFid);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)offsetof(TriangleVertex, x));
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(TriangleVertex), (void*)offsetof(TriangleVertex, u));
	glDrawArrays(GL_TRIANGLES, 0, size);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}