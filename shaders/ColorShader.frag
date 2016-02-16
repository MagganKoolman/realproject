#version 330 core

in vec2 texCoor;

out vec4 color;

uniform sampler2D texSampler;

void main(){
	color = texture(texSampler, vec2(texCoor.s, texCoor.t));
}