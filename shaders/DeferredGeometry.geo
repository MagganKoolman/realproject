#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

vec4 in position[];
vec3 in normalOut[];
vec2 in texOut[];

vec3 out normalOut2;
vec2 out texOut2;

void main()
{
	vec3 p1 = position[1].xyz - position[0].xyz;
	vec3 p2 = position[2].xyz - position[0].xyz;
	vec3 normalOut2 = normalize(cross(p1, p2));

	for (int i = 0; i < 3; i++)
	{
		gl_Position = position[i];
		texOut2 = texOut[i];
		EmitVertex();
	}
	EndPrimitive();
}