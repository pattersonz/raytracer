#version 330 core

layout(location=0) out vec4 fragmentColor;

in vec4 vNormal;
in vec3 vColor;
in vec3 lightDir;
in vec3 vHalf;



void main(void)
{
	vec3 c = vColor;
	vec3 n = normalize(vNormal.xyz);
	vec3 l = normalize(lightDir);
	vec3 h = normalize(vHalf);

	vec3 intensity = c * 0.2 + 0.8 * c * max(0.0,dot(n,l)) + pow(max(0.0,dot(n,h)), 32);
	fragmentColor = vec4(vColor.x * intensity.x, vColor.y * intensity.y, vColor.z * intensity.z, 1.0);
}
