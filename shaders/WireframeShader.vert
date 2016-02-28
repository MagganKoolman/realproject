#version 400 core

in vec3 vertexPos;

uniform mat4 World;
uniform mat4 Camera;
uniform mat4 Perspective;

void main(){
	gl_Position =  Perspective * Camera * World * vec4(vertexPos, 1.0);
}