#version 400

in vec3 vertexPos;
in vec2 texCoorIn;

out vec4 position;
out vec2 texOut;

uniform mat4 World;

void main(){
	position = World * vec4(vertexPos, 1.0);
	texOut = texCoorIn;
}