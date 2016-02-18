#version 330 core

in vec2 texCoor;

out vec4 color;

uniform sampler2D colorTex;
uniform sampler2D normalTex;

void main(){
	color = texture(colorTex , texCoor) + texture(normalTex, texCoor);
}