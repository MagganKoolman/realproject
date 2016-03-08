#version 430 core

uniform sampler2D computeTex;

in vec2 texCoor;

out vec4 color;

void main(){
	color = texture(computeTex, texCoor);
}