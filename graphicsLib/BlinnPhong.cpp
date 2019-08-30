#include "BlinnPhong.h"

png::rgb_pixel BlinnPhong::getColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light*> lights, const std::vector<int> hitLights, png::rgb_pixel reflect) const
{


	Vector3D color(diffuse[0] * 0.1f * 255, diffuse[1] * 0.1f * 255, diffuse[2] * 0.1f * 255);

	for (int k = 0; k < hitLights.size(); k++)
	{
		int i = hitLights[k];

		Vector3D l = lights[i]->getPos() - intersect;
		l.normalize();
		Vector3D v = e - intersect;
		v.normalize();
		Vector3D vPl = v + l;
		Vector3D h( vPl * (1.0f / vPl.length()));
		
		float cosThetaNL = normal.dot(l) / (normal.length() * l.length());
		float cosThetaNH = normal.dot(h) / (normal.length() * h.length());

		Vector3D lightMod = lights[i]->getIntensity() * MAX(0, cosThetaNL);
		Vector3D specMod = lights[i]->getIntensity() * pow (MAX(0, cosThetaNH), phongEXP ); 

		for (int j = 0; j < 3; j++)
		{
			color[j] += 255 * diffuse[j] * lightMod[j] + 255 * specular[j] * specMod[j];
		}
	}
	return png::rgb_pixel(CLAMP(0,color[0],255), CLAMP(0,color[1],255), CLAMP(0,color[2],255));

}

png::rgb_pixel BlinnPhong::getFragmentColor(glm::vec4 normal, std::vector<glm::vec3> lightDirs, std::vector<glm::vec3> halves, std::vector<Light*> lights) const
{
	glm::vec3 c = glm::vec3(diffuse[0], diffuse[1], diffuse[2]);
	glm::vec3 s = glm::vec3(specular[0], specular[1], specular[2]);
	glm::vec3 n = glm::normalize( glm::vec3(normal));


	glm::vec3 cI = c * 0.1f;
	for (int i = 0; i < lightDirs.size(); i++)
	{
		Vector3D temp = lights[i]->getIntensity();
		glm::vec3 lI = glm::vec3(temp[0], temp[1], temp[2]);

		glm::vec3 l = glm::normalize(lightDirs[i]);
		glm::vec3 h = glm::normalize(halves[i]);
		glm::vec3 intensityD = (float)(MAX(0.0,dot(n,l))) * (lI);
		glm::vec3 intensityS = (float)(pow(MAX(0.0,dot(n,h)), phongEXP)) * (lI) ;
		cI += glm::vec3(c.x * intensityD.x, c.y * intensityD.y, c.z * intensityD.z) + glm::vec3(s.x * intensityS.x, s.y * intensityS.y, s.z * intensityS.z);
	}

	cI.x = CLAMP(0,cI.x,1);
	cI.y = CLAMP(0,cI.y,1);
	cI.z = CLAMP(0,cI.z,1);

	cI *= 250;
	return png::rgb_pixel(cI.x, cI.y, cI.z);
}