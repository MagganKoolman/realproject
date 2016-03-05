#version 400 core

in vec2 position;
in vec2 texturePos;
out vec2 texCoor;

void main(){
	texCoor = texturePos;
	gl_Position.xy = position;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
}