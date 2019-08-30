#version 330 core

layout(location=0) out vec4 fragmentColor;

in vec3 vNormal;
in vec3 vColor;
in vec3 lightDir;


void main(void)
{
	vec3 n = normalize(vNormal);
	vec3 l = lightDir;
	vec3 intensity = vec3(1.0,1.0,1.0) * dot(n,l);
	fragmentColor = vec4(vColor.x * intensity.x, vColor.y * intensity.y, vColor.z * intensity.z, 1.0);
}
