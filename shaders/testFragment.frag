#version 330 core

in vec2 texOut;

out vec4 color;

uniform sampler2D texSampler;

void main(){
	color = texture(texSampler, vec2(texOut.s, 1- texOut.t));
}