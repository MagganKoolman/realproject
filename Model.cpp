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