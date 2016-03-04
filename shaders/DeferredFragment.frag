#version 400

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normalTex;
layout(location = 2) out vec3 specularTex;
layout(location = 3) out vec3 diffuseTex;

in vec3 normalOut2;
in vec2 texOut2;
in vec3 positionOut;
in vec3 V;

uniform sampler2D texSampler;
uniform sampler2D normalMap;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform bool normalMapBool;


void main(){
	color = texture(texSampler, vec2(texOut2.s, 1- texOut2.t)) + vec4(ambient, 0.0);
	vec3 nmap = texture(normalMap, texOut2).xyz* 2 - 1;
	vec3 normal = normalOut2;
	if(normalMapBool){
		vec3 ve = normalize(V - positionOut);
		vec3 deltapos1 = dFdx( ve );
		vec3 deltapos2 = dFdy( ve );
		vec2 deltauv1 = dFdx( texOut2 );
		vec2 deltauv2 = dFdy( texOut2 );

		vec3 temp1 = cross(deltapos2, normal);
		vec3 temp2 = cross(normal, deltapos1);

		vec3 tangent = temp1*deltauv1.x + temp2 * deltauv2.x;
		vec3 bitangent = temp1*deltauv1.y + temp2 * deltauv2.y;

		float maxInverse = inversesqrt( max(dot(tangent,tangent), dot(bitangent,bitangent))); 

		mat3 TBNmatrix = mat3(tangent*maxInverse, bitangent*maxInverse, normalOut2);
		normal =  normalize(TBNmatrix*nmap);
	}
	normalTex = vec4(normal*0.5+0.5,1.0);
	specularTex = vec4(specular,1.0).rgb;
	diffuseTex = vec4(diffuse,1.0).rgb;
}
