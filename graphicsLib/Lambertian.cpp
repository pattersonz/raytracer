#include "Lambertian.h"

png::rgb_pixel Lambertian::getColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light*> lights, const std::vector<int> hitLights, png::rgb_pixel reflect) const
{


	Vector3D color(diffuse[0] * 0.1f * 255, diffuse[1] * 0.1f * 255, diffuse[2] * 0.1f * 255);

	for (int k = 0; k < hitLights.size(); k++)
	{
		int i = hitLights[k];
		Vector3D toLight = lights[i]->getPos() - intersect;
		float cosTheta = normal.dot(toLight) / (normal.length() * toLight.length());
		Vector3D lightMod = lights[i]->getIntensity() * MAX(0, cosTheta);

		for (int j = 0; j < 3; j++)
		{
			color[j] += 255 * diffuse[j] * lightMod[j];
		}
	}
	return png::rgb_pixel(CLAMP(0,color[0],255), CLAMP(0,color[1],255), CLAMP(0,color[2],255));

}

png::rgb_pixel Lambertian::getFragmentColor(glm::vec4 normal, std::vector<glm::vec3> lightDirs, std::vector<glm::vec3> halves, std::vector<Light*> lights) const
{
	glm::vec3 c = glm::vec3(diffuse[0], diffuse[1], diffuse[2]);
	glm::vec3 n = glm::normalize( glm::vec3(normal));


	glm::vec3 cI = c * 0.1f;
	for (int i = 0; i < lightDirs.size(); i++)
	{
		Vector3D lI = lights[i]->getIntensity();
		glm::vec3 l = glm::normalize(lightDirs[i]);
		glm::vec3 intensity = glm::vec3(lI[0], lI[1], lI[2]) * MAX(glm::dot(n,l), 0);
		cI += glm::vec3(c.x * intensity.x, c.y * intensity.y, c.z * intensity.z);
	}

	cI.x = CLAMP(0,cI.x,1);
	cI.y = CLAMP(0,cI.y,1);
	cI.z = CLAMP(0,cI.z,1);

	cI *= 250;
	return png::rgb_pixel(cI.x, cI.y, cI.z);
}