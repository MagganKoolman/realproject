#version 400

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normalTex;
layout(location = 2) out vec3 specularTex;
layout(location = 3) out vec3 diffuseTex;

in vec3 normalOut2;
in vec2 texOut2;
in vec3 tangent;
in vec3 V;

uniform sampler2D texSampler;
uniform sampler2D normalMap;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;

mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}
 
vec3 perturb_normal( vec3 N, vec3 vv, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
	vec3 map = texture(normalMap, texOut2).xyz;
    map = map * 255./127. - 128./127.;
    mat3 TBN = cotangent_frame(N, -vv, texcoord);
    return normalize(TBN * map);
}

void main(){
	color = texture(texSampler, vec2(texOut2.s, 1- texOut2.t)) + vec4(ambient, 0.0);
	vec3 nmap = texture(normalMap, vec2(texOut2.s, 1-texOut2.t)).xyz;
	vec3 forward = vec3(0,0,1);
	float l = length(cross(forward,normalOut2));
	float d = dot(forward, normalOut2);
	mat3x3 basis = mat3x3(tangent,
							cross(normalOut2, tangent), 
							normalOut2);

	
	vec3 ve = normalize(V - tangent); 
	vec3 normal =  perturb_normal(normalOut2, ve, texOut2);

	normalTex = vec4(normal*0.5+0.5,1.0);
	specularTex = vec4(specular,1.0).rgb;
	diffuseTex = vec4(diffuse,1.0).rgb;
}
