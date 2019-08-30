#version 330 core

// comment
layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec3 in_Color;
layout(location=3) in vec2 in_TexCoord;

out vec4 vNormal;
out vec3 lightDir;
out vec3 vHalf;
out vec2 vTCoord;


uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

void main(void)
{


	vTCoord = in_TexCoord; 

	vec4 pos = viewMatrix * modelMatrix * vec4(in_Position, 1.0);

	vec4 lightPos =  viewMatrix * vec4(vec3(0.0,20.0,-5.0), 1.0);

	vec3 v = normalize(-pos.xyz);
	vNormal = normalMatrix * vec4(in_Normal, 0.0);
	lightDir = normalize(  lightPos.xyz - pos.xyz);
	vHalf = normalize( v + lightDir);

	gl_Position = projMatrix * pos;
}



