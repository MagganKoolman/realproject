#include "Model.h"
#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(): _VBOid(0), _mat(nullptr), _normalTexture(0) {
	normalMaping = false;
	bBox = new vec3[4];
}

Model::Model(Model &m) {
	this->_VBOid = m._VBOid;
	this->_BUFFid = m._BUFFid;
	this->setMaterial(m._mat);
	this->_normalTexture = m._normalTexture;
	this->worldMat = m.worldMat;
	this->size = m.size;
	this->normalMaping = m.normalMaping;

	this->bBox = new vec3[4];
	for (int i = 0; i < 4; i++)
	{
		bBox[i] = m.bBox[i];
	}
}

Model::~Model() {
	if (_mat->pointers <= 1)
		delete this->_mat;
	else
		_mat->pointers--;
	delete bBox;
}

void Model::initNormalTexture(const std::string &filePath) {
	this->normalMaping = true;
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
GLuint Model::getBuffID() const{
	return this->_BUFFid;
}
int Model::getSize() {
	return this->size;
}
void Model::addTranslation(glm::vec3 translation) {
	this->worldMat = glm::translate(worldMat, translation);
	for (int i = 0; i < 4; i++)
		bBox[i] = vec3(worldMat * glm::vec4(bBox[i], 1));
}
void Model::draw(GLuint spID) {
	GLuint nMapBool = glGetUniformLocation(spID, "normalMapBool");
	glUniform1i(nMapBool, normalMaping);

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

void Model::createBBox(const std::string &filePath) {
	std::ifstream file("models/boundbox/" + filePath);	
	std::string input;
	std::istringstream inputString;
	float coordinates[6];
	std::getline(file, input);
	inputString.clear();
	inputString.str(input);
	inputString >> coordinates[0] >> coordinates[1] >> coordinates[2] >> coordinates[3] >> coordinates[4] >> coordinates[5];
	int counter = 0;
	for (int i = 0; i < 2; i++) 
			for (int k = 4; k < 6; k++)
				bBox[counter++] = vec3(coordinates[i], 0, coordinates[k]);
}

vec3* Model::getBBox() const {
	return bBox;
}
void Model::Scale(glm::vec3 scale) {
	this->worldMat = glm::scale(worldMat, scale);
	for (int i = 0; i < 4; i++)
		bBox[i] = vec3(worldMat * glm::vec4(bBox[i], 1));
}
