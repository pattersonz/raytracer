#version 330 core

// comment
layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec3 in_Color;

out vec4 vNormal;
out vec3 vColor;
out vec3 vHalf;
out vec3 lightDir;


uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

//normal view*Model inverse transpose

void main(void)
{
	vec4 pos = viewMatrix * modelMatrix * vec4(in_Position, 1.0);

	vec4 lightPos =  viewMatrix * vec4(vec3(0.0,0,100.0), 1.0);

	vColor = in_Color;
	vec3 v = normalize(-pos.xyz);
	vNormal = normalMatrix * vec4(in_Normal, 0.0);
	lightDir = normalize(  lightPos.xyz - pos.xyz);
	vHalf = normalize( v + lightDir);

	gl_Position = projMatrix * pos;
}








