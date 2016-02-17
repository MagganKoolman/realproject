#version 330 core

in vec3 vertexPos;
in vec3 normal;
in vec2 texCoorIn;

out vec3 normalOut;
out vec2 texOut;

uniform mat4 Camera;
uniform mat4 Perspective;

void main(){
	gl_Position = Perspective * Camera * vec4(vertexPos, 1.0);
	normalOut = normal;
	texOut = texCoorIn;
}