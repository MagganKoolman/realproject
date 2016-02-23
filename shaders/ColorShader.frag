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
	vec3 normal = texture(normalTex, texCoor).rgb*2-1; 
	float depth = texture(depthTex, texCoor).r;
	vec3 dif = texture(diffuse, texCoor).rgb;
	vec3 spec = texture(specular, texCoor).rgb;

	vec3 light = vec3(10,0,5);
	vec4 pos = (Perspective * (vec4(texCoor.x, texCoor.y, depth, 1)*2-1));
	pos /= pos.w;

    vec3 lightDir = light - pos.xyz;
    
    normal = normalize(normal);
    lightDir = normalize(lightDir);
    
    vec3 eyeDir = normalize(cameraPos-pos.xyz);
    vec3 vHalfVector = reflect(-lightDir.xyz, normal);
    
	vec3 specularColor = spec*pow(max(dot(eyeDir,vHalfVector),0.0), 20);
	vec3 diffureColor = dif*dot(lightDir, normal);

    color = c + vec4(diffureColor + specularColor,1);
	/*float shadowAmount = texture(colorTex, vec3(texCoor, 1.0));
	color = vec4(shadowAmount*0.5, shadowAmount, 1.0, 1.0);*/
}