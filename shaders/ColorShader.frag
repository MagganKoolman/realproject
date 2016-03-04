#version 400 core

in vec2 texCoor;

out vec4 color;

uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform sampler2D depthTex;
uniform sampler2D diffuse;
uniform sampler2D specular;

uniform sampler2D shadowMap;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform mat4 invPerspective;
uniform mat4 LightWP;

void main(){
	vec4 c = texture(colorTex , texCoor); 
	vec3 normal = texture(normalTex, texCoor).rgb*2-1; 
	float depth = texture(depthTex, texCoor).r;
	vec3 dif = texture(diffuse, texCoor).rgb;
	vec3 spec = texture(specular, texCoor).rgb;

	vec4 pos = (invPerspective * (vec4(texCoor.x, texCoor.y, depth, 1)*2-1));
	pos /= pos.w;

	vec4 shadowPos = LightWP * pos;
	shadowPos /= shadowPos.w;
	float shadowDepth = texture(shadowMap, vec2( shadowPos.x * 0.5 + 0.5, shadowPos.y * 0.5 + 0.5)).r;
	float shadow = 0.0;
	if (shadowPos.z - 0.001 < shadowDepth * 2 - 1)
		shadow = 1.0;
    
	vec3 lightDir = lightPos - pos.xyz;   
    normal = normalize(normal);
    lightDir = normalize(lightDir);
    
    vec3 eyeDir = normalize(cameraPos-pos.xyz);
    vec3 vHalfVector = reflect(-lightDir.xyz, normal);
    
	vec3 specularColor = spec*pow(max(dot(eyeDir,vHalfVector),0.0), 20);
	vec3 diffuseColor = max(dif*dot(lightDir, normal), 0.0);

    color = c  + vec4(0.4*(diffuseColor + specularColor),1) * shadow;
	//color = vec4(texture(normalTex, texCoor).rgb * 2 -1, 1.0);
	//color = vec4(normal,1);
	//color = vec4(vec3(shadow),1);
}