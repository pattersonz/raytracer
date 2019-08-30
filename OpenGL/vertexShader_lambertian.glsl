#version 330 core

// comment
layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec3 in_Color;

out vec3 vNormal;
out vec3 vColor;
out vec3 lightDir;


uniform mat4 projMatrix;

void main(void)
{
	vColor = in_Color;
	vNormal = in_Normal;
	lightDir = normalize( vec3(25.0,0,10.0) - in_Position);
	gl_Position = projMatrix * vec4(in_Position, 1.0);
}








