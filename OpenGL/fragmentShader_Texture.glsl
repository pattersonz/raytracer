#version 330 core

layout(location=0) out vec4 fragmentColor;

in vec4 vNormal;
in vec3 lightDir;
in vec2 vTCoord;
in vec3 vHalf;

uniform sampler2D texUnit;


void main(void)
{
	vec3 n = normalize(vNormal.xyz);
	vec3 l = normalize(lightDir);
	vec3 h = normalize(vHalf);
	vec3 c = texture(texUnit, vTCoord).rgb;

	vec3 intensity = c * 0.2 + 0.8 * c * max(0.0,dot(n,l)) + c * pow(max(0.0,dot(n,h)), 32);
	fragmentColor = vec4(intensity, 1.0);
}
