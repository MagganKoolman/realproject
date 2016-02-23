#version 330

uniform mat4 VPmatrix;

in vec3 vertexPosition;

void main(void)
{
	gl_Position = VPmatrix * vec4(vertexPosition, 1.0);
}