#version 400 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

vec3 in normalOut[];
vec2 in texOut[];

vec3 out normalOut2;
vec2 out texOut2;

void main()
{
	vec3 side1 = gl_in[1].gl_Position - gl_in[0].gl_Position;
	vec3 side2 = gl_in[2].gl_Position - gl_in[0].gl_Position;
	vec3 normal = cross(side1, side2);
	gl_Position = gl_in[0].gl_Position;
    EmitVertex();
	gl_Position = gl_in[1].gl_Position;
     EmitVertex();
	gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
	normalOut2 = normal;
	texOut2 = texOut;
}