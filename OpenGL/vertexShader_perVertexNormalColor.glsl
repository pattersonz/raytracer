#version 330 core

// comment
layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;

out vec3 vColor;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void)
{
	vColor = vec3( (in_Normal.x + 1.0f) / 2.0f, (in_Normal.y + 1.0f) / 2.0f, (in_Normal.z + 1.0f) / 2.0f );
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
}








