#version 330 core

// comment
layout(location=0) in vec3 in_Position;
layout(location=2) in vec3 in_Color;

out vec3 vColor;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void)
{
	vColor = in_Color;
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
}








