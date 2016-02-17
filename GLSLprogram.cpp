#include "GLSLprogram.h"
#include <fstream>
#include <vector>

GLSLprogram::GLSLprogram(): _programID(0), _vertexShader(0), _fragmentShader(0), _numAttributes(0), _frameBuffer(0), _texture(0), _depthRenderBuffer(0),
_normalTexture(0){

}

GLSLprogram::~GLSLprogram() {

}

void GLSLprogram::compileShaders(const std::string& vertexPath, const std::string& fragmentPath) {
	_programID = glCreateProgram();
	_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShader == 0) {
		std::cout << "Vertex shader fucka!";
	}
	_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShader == 0) {
		std::cout << "Fragment shader fucka!";
	}

	compileShader(vertexPath, _vertexShader);
	compileShader(fragmentPath, _fragmentShader);
}

void GLSLprogram::linkShaders() {
	

	glAttachShader(_programID, _vertexShader);
	glAttachShader(_programID, _fragmentShader);

	glLinkProgram(_programID);

	GLint success = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		glDeleteProgram(_programID);
		glDeleteShader(_vertexShader);
		glDeleteShader(_fragmentShader);
		
		std::printf("%s\n", &(errorLog[0]));
	}

	glDetachShader(_programID, _vertexShader);
	glDetachShader(_programID, _fragmentShader);
	glDeleteShader(_vertexShader);
	glDeleteShader(_fragmentShader);
}

void GLSLprogram::initFrameBuffer() {
	glGenFramebuffers(1, &_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

	glEnable(GL_TEXTURE_2D); 
	glEnable(GL_DEPTH_TEST);

	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1080, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &_normalTexture);
	glBindTexture(GL_TEXTURE_2D, _normalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1080, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &_depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1080, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, _normalTexture, 0);
	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers); 
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "No framebuffer!!!!";
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void GLSLprogram::addAttribute(const std::string& attributeName) {
	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}

void GLSLprogram::use() {
	glUseProgram(_programID);	
	for (int i = 0; i < _numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLSLprogram::unUse() {
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLSLprogram::compileShader(const std::string& filePath, GLuint shaderID) {
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail()) {
		std::cout << "Fil fucka: " + filePath;
	}
	std::string fileContent = "";
	std::string line;

	while (std::getline(shaderFile, line)) {
		fileContent += line + "\n";
	}
	shaderFile.close();

	const char* contentsPtr = fileContent.c_str();
	glShaderSource(shaderID, 1, &contentsPtr, nullptr);
	glCompileShader(shaderID);

	GLint success = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(shaderID);
		std::printf("%s\n", &(errorLog[0]));
	}
}

GLuint GLSLprogram::getTexture() const
{
	return this->_texture;
}

GLuint GLSLprogram::getProgramID() const
{
	return this->_programID;
}

