#version 330 core

uniform sampler2D shadowTex;

in vec2 texOut;

out vec4 color;

void main(){
	color = texture(shadowTex, texOut);
}