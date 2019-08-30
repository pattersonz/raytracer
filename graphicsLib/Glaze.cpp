#include "Glaze.h"

png::rgb_pixel Glaze::getColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light*> lights, const std::vector<int> hitLights, png::rgb_pixel reflect) const
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

	float solid = 1.0f - amountReflect;

	color[0] = solid * color[0] + amountReflect * reflect.red;
	color[1] = solid * color[1] + amountReflect * reflect.green;
	color[2] = solid * color[2] + amountReflect * reflect.blue;
	return png::rgb_pixel(CLAMP(0,color[0],255), CLAMP(0,color[1],255), CLAMP(0,color[2],255));

}

png::rgb_pixel Glaze::getFragmentColor(glm::vec4 normal, std::vector<glm::vec3> lightDirs, std::vector<glm::vec3> halves, std::vector<Light*> lights) const
{
	return png::rgb_pixel(0,0,0);
}