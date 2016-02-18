#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normalTex;

in vec3 normalOut;
in vec2 texOut;

uniform sampler2D texSampler;

void main(){
	color = texture(texSampler, vec2(texOut.s, 1- texOut.t));
	normalTex = vec4(normalOut, 1.0);
}