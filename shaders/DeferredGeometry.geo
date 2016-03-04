#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 position[];
in vec3 normalOut[];
in vec2 texOut[];

out vec3 normalOut2;
out vec2 texOut2;
out vec3 positionOut;
out vec3 V;

uniform mat4 Perspective;
uniform mat4 Camera;
uniform vec3 cameraPosition;

void main()
{
	vec3 p1 = position[1].xyz - position[0].xyz;
	vec3 p2 = position[2].xyz - position[0].xyz;
	normalOut2 = normalize(cross(p1, p2));
	vec3 aVector = normalize(cameraPosition - position[1].xyz);
	if(dot(normalOut2, aVector) > 0.0f)
	{
		V = cameraPosition;
		float tu = texOut[1].t - texOut[0].t;
		float tv = texOut[2].t - texOut[0].t;
		for (int i = 0; i < 3; i++)
		{
			positionOut = ( position[i]).xyz;
			gl_Position = Perspective * Camera * position[i];
			texOut2 = texOut[i];
			if(normalOut[i].x != 0 || normalOut[i].y != 0 || normalOut[i].z != 0){
				normalOut2 = normalOut[i];
			}
			EmitVertex();
		}
		EndPrimitive();
	}
}