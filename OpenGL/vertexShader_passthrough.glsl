#version 330 core

// comment
layout(location=0) in vec3 in_Position;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
void main(void)
{
  gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
}








