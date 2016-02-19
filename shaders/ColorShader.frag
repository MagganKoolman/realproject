#version 330 core

in vec2 texCoor;

out vec4 color;

uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform sampler2D depthTex;
uniform sampler2D diffuse;
uniform sampler2D specular;

uniform vec3 cameraPos;
uniform mat4 Perspective;

void main(){
	vec4 c = texture(colorTex , texCoor); 
	vec3 normal = texture(normalTex, texCoor).rgb; 
	float depth = texture(depthTex, texCoor).r;
	vec3 dif = texture(diffuse, texCoor).rgb;
	vec3 spec = texture(specular, texCoor).rgb;

	vec3 light = (Perspective * vec4(vec3(0, 0, 0), 1)).xyz;
	vec3 pos = (Perspective * vec4(texCoor, depth, 1)).xyz;
	
	vec3 lv = normalize(pos - light);
	vec3 r = reflect(lv, normal);
	vec3 vc = normalize( cameraPos - pos );

	vec3 specularLight = spec * pow(max(dot(vc,r),0),5);
	vec3 diffuseLight = dif * max(dot(normal, -lv),0);
	color = vec4(0.5*(c.rgb + diffuseLight ) + specularLight, c.a); 
}
/*
	vec3 light = vec3(0,0,5);
	vec3 lv = normalize(position - light);
	vec3 r = reflect(lv, normal);
	vec3 vc = normalize(cameraPos - position);

	vec3 specular = diffuseLight * pow(max(dot(vc, r),0),5);
	vec4 mySample = texture(texSampler, vec2(texOut.s, 1- texOut.t));
	vec3 diffuse = diffuseLight* max(dot(normal, -lv),0);
	fragment_color = 0.5*(mySample.rgb + ambientLight + diffuse) + 2*specular;
*/