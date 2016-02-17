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
	this->size = s;
}
void Model::draw(GLuint spID) {
	glBindBuffer(GL_ARRAY_BUFFER, _BUFFid);
	glBindTexture(GL_TEXTURE_2D, _mat->texid);

	GLuint ambient = glGetUniformLocation(spID, "ambient");
	glUniform3f(ambient, this->_mat->Ka.x, this->_mat->Ka.y, this->_mat->Ka.z);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)offsetof(TriangleVertex, x));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)offsetof(TriangleVertex, nx));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)offsetof(TriangleVertex, u));
	glDrawArrays(GL_TRIANGLES, 0, size);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}