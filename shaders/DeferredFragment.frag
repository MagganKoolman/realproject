#version 400 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normalTex;
layout(location = 2) out vec3 specularTex;
layout(location = 3) out vec3 diffuseTex;

in vec3 normalOut2;
in vec2 texOut2;

uniform sampler2D texSampler;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;

void main(){
	color = texture(texSampler, vec2(texOut2.s, 1- texOut2.t)) + vec4(ambient, 0.0);

	vec3 normal = normalize(normalOut2);

	normalTex = vec4(normal*0.5+0.5,1.0);
	specularTex = vec4(specular,1.0).rgb;
	diffuseTex = vec4(diffuse,1.0).rgb;
}