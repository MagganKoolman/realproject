#version 330 core

layout(location = 0) out vec4 color;

in vec2 texOut;

uniform sampler2D texSampler;

void main(){
	//color = texture(texSampler, vec2(texOut.s, texOut.t));
	color = vec4(1.0, 0.5, 0.3, 1.0);
}