#version 330

uniform mat4 VPmatrix;
uniform mat4 World;

in vec3 vertexPosition;

void main(void)
{
	gl_Position = VPmatrix * World * vec4(vertexPosition, 1.0);
}