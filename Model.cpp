#include "Model.h"

Model::Model(): _VBOid(0), _mat(nullptr), _normalTexture(0) {
	
}

Model::~Model() {
	if (_mat->pointers <= 1)
		delete this->_mat;
	else
		_mat->pointers--;
}

void Model::initNormalTexture(const std::string &filePath) {
	glEnable(GL_TEXTURE_2D);

	unsigned char* image;
	int width, height;
	glGenTextures(1, &_normalTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _normalTexture);

	image = SOIL_load_image(("models/" + filePath).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(image);

}

void Model::setMaterial(Material* material) {
	material->pointers++;
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

Material* Model::getMat() {
	return this->_mat;
}
GLuint Model::getBuffID() {
	return this->_BUFFid;
}
int Model::getSize() {
	return this->size;
}
void Model::addTranslation(glm::vec3 translation) {
	this->worldMat = glm::translate(worldMat, translation);
}
void Model::draw(GLuint spID) {

	GLuint texLocation = glGetUniformLocation(spID, "texSampler");
	glUniform1i(texLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _mat->texid);

	texLocation = glGetUniformLocation(spID, "normalMap");
	glUniform1i(texLocation, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _normalTexture);

	GLuint ambient = glGetUniformLocation(spID, "ambient");
	GLuint diffuse = glGetUniformLocation(spID, "diffuse");
	GLuint specular = glGetUniformLocation(spID, "specular");
	GLuint world = glGetUniformLocation(spID, "World");
	glUniform3f(ambient, this->_mat->Ka.x, this->_mat->Ka.y, this->_mat->Ka.z);
	glUniform3f(diffuse, this->_mat->Kd.x, this->_mat->Kd.y, this->_mat->Kd.z);
	glUniform3f(specular, this->_mat->Ks.x, this->_mat->Ks.y, this->_mat->Ks.z);
	glUniformMatrix4fv(world, 1, GL_FALSE, &this->worldMat[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, _BUFFid);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)offsetof(TriangleVertex, x));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)offsetof(TriangleVertex, nx));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)offsetof(TriangleVertex, u));
	glDrawArrays(GL_TRIANGLES, 0, size);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}