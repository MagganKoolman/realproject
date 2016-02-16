#version 330 core

in vec3 vertexPos;
in vec3 colors;

out vec3 fragmentColor;

uniform mat4 Camera;
uniform mat4 Perspective;

void main(){
	//gl_Position.xyz = vertexPos.xyz;	
	//gl_Position.w = 1.0;
	gl_Position = Perspective * Camera * vec4(vertexPos, 1.0);
	fragmentColor = colors - vertexPos * 0.5;
}