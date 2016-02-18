#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normalTex;

in vec3 normalOut;
in vec2 texOut;

uniform sampler2D texSampler;

uniform vec3 ambient;

void main(){
<<<<<<< HEAD
	color = texture(texSampler, vec2(texOut.s, 1- texOut.t));
=======
	color = texture(texSampler, vec2(texOut.s, 1- texOut.t)) + vec4(ambient, 0.0);
>>>>>>> d10d130848557a1eb71234c4ec428f8c91ea7690
	normalTex = vec4(normalOut, 1.0);
}