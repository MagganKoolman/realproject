#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normalTex;
layout(location = 2) out vec4 specularTex;
layout(location = 3) out vec4 diffuseTex;

in vec3 normalOut;
in vec2 texOut;

uniform sampler2D texSampler;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;

void main(){
	color = texture(texSampler, vec2(texOut.s, 1- texOut.t)) + vec4(ambient, 0.0);
	normalTex = vec4(normalOut, 1.0);
	specularTex = vec4(specular, 1.0);
	diffuseTex = vec4(diffuse, 1.0);
}