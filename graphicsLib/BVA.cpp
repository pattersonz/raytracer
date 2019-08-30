#include "BVA.h"

void BVA::mergeSort(std::vector<BVA*>& list, const int type)
{
	if (list.size() <= 1)
		return;

	std::vector<BVA *> l, r;
	int midPoint = list.size() / 2;
	for (int i = 0; i < midPoint; i++)
		l.push_back(list[i]);
	for (int i = midPoint; i < list.size(); i++)
		r.push_back(list[i]);

	mergeSort(l, type);
	mergeSort(r, type);

	int lSize = l.size(), rSize = r.size();
	int j = 0, k = 0;
	for (int i = 0; i < list.size(); i++)
	{
		if ( j == lSize)
			list[i] = r[k++];
		else if ( k == rSize)
			list[i] = l[j++];
		else
		{
			float lMid, rMid;
			if (type == 0)
			{
				lMid = (l[j]->xmin + l[j]->xmax) / 2.0f;
				rMid = (r[k]->xmin + r[k]->xmax) / 2.0f; 
			}
			else if (type == 1)
			{
				lMid = (l[j]->ymin + l[j]->ymax) / 2.0f;
				rMid = (r[k]->ymin + r[k]->ymax) / 2.0f; 
			}
			else
			{
				lMid = (l[j]->ymin + l[j]->ymax) / 2.0f;
				rMid = (r[k]->ymin + r[k]->ymax) / 2.0f; 
			}
			list[i] = (lMid < rMid ? l[j++] : r[k++]);
		}
	}



}

BVA::BVA(std::vector<BVA *> m, int height)
{
	isLeaf = false;
	if (m.size() == 1)
	{
		leftBox = m[0];
		rightBox = 0;

		xmin = leftBox->xmin;
		xmax = leftBox->xmax;
		ymin = leftBox->ymin;
		ymax = leftBox->ymax;
		zmin = leftBox->zmin;
		zmax = leftBox->zmax;

		model = 0;
		return;
	}
	else if (m.size() == 2)
	{
		leftBox = m[0];
		rightBox = m[1];

		xmin = GETMIN(leftBox->xmin, rightBox->xmin);
		xmax = GETMAX(leftBox->xmax, rightBox->xmax);
		ymin = GETMIN(leftBox->ymin, rightBox->ymin);
		ymax = GETMAX(leftBox->ymax, rightBox->ymax);
		zmin = GETMIN(leftBox->zmin, rightBox->zmin);
		zmax = GETMAX(leftBox->zmax, rightBox->zmax);

		model = 0;
		return;
	}

	mergeSort(m, height % 3);


	std::vector<BVA *> l, r;
	int midPoint = m.size() / 2;
	for (int i = 0; i < midPoint; i++)
		l.push_back(m[i]);
	for (int i = midPoint; i < m.size(); i++)
		r.push_back(m[i]);

	leftBox = new BVA(l, height + 1);
	rightBox = new BVA(r, height + 1);

	xmin = GETMIN(leftBox->xmin, rightBox->xmin);
	xmax = GETMAX(leftBox->xmax, rightBox->xmax);
	ymin = GETMIN(leftBox->ymin, rightBox->ymin);
	ymax = GETMAX(leftBox->ymax, rightBox->ymax);
	zmin = GETMIN(leftBox->zmin, rightBox->zmin);
	zmax = GETMAX(leftBox->zmax, rightBox->zmax);

	model = 0;
}

BVA* BVA::createBVA(const std::vector<Model *> models)
{
	std::vector<BVA *> boxes;

	for (int i = 0; i < models.size(); i++)
	{
		BVA* b = new BVA(models[i]);
		boxes.push_back(b);
	}

	BVA* root = new BVA(boxes, 0);
	return root;
}




bool BVA::checkCollisions(const Vector3D dir, const Vector3D e, const float minT,float& maxT, Model*& modelHit, Vector3D& norm) const
{
	if ( rayHitsThisVol(dir, e, minT) )
	{
		//printf("x:%lf %lf Y:%lf %lf Z:%lf %lf\n", xmin, xmax, ymin, ymax, zmin, zmax);
		if (isLeaf)
		{
			if (model->getCollisions(dir, e, minT, maxT, modelHit, norm))
			{
				modelHit = model;
				return true;
			}
			
			else
			{
				modelHit = 0;
				maxT = 0;
				return false;
			}
		}
		else
		{
			bool hitLeft = false, hitRight = false;
			modelHit = 0;
			Model* modelHitLeft = 0;
			Model* modelHitRight = 0;
			float maxTLeft = maxT, maxTRight = maxT;
			Vector3D normL(1,0,0), normR(1,0,0);

			if (leftBox != 0)
				hitLeft = leftBox->checkCollisions(dir, e, minT, maxTLeft, modelHitLeft, normL);
			if (rightBox != 0)
				hitRight = rightBox->checkCollisions(dir, e, minT, maxTRight, modelHitRight, normR);

			if (maxT != 0.0f && maxT < maxTLeft && maxT < maxTRight)
				return false;

			if (maxTLeft != 0.0f)
			{
				maxT = maxTLeft;
				modelHit = modelHitLeft;
				norm = normL;
				if (maxTRight != 0.0f && maxTRight < maxT)
				{
					maxT = maxTRight;
					modelHit = modelHitRight;
					norm = normR;
				}
			}
			else if (maxTRight != 0.0f)
			{
				maxT = maxTRight;
				modelHit = modelHitRight;
				norm = normR;
			}

			return hitLeft || hitRight;
		}
	}
	else
	{
		modelHit = 0;
		maxT = 0.0f;
		return false;
	}

}


bool BVA::rayHitsThisVol(const Vector3D dir, const Vector3D e, const float minT) const
{
	Vector3D d = dir;


	float txN, txX, tyN, tyX, tzN, tzX;
	if (d[0] != 0.0f)
	{
		txN = (xmin - e[0]) / d[0];
		txX = (xmax - e[0]) / d[0];
	}
	else
	{
		if ((xmin - e[0] < 0 && xmax - e[0] < 0) ||
			(xmin - e[0] > 0 && xmax - e[0] > 0))
			return false;
		txN = std::numeric_limits<float>::min();
		txX = std::numeric_limits<float>::max();
	}
	if (d[1] != 0.0f)
	{
		tyN = (ymin - e[1]) / d[1];
		tyX = (ymax - e[1]) / d[1];
	}
	else
	{
		if ((ymin - e[1] < 0 && ymax - e[1] < 0) ||
			(ymin - e[1] > 0 && ymax - e[1] > 0))
			return false;
		tyN = std::numeric_limits<float>::min();
		tyX = std::numeric_limits<float>::max();
	}
	if (d[2] != 0.0f)
	{
		tzN = (zmin - e[2]) / d[2];
		tzX = (zmax - e[2]) / d[2];
	}
	else
	{
		if ((zmin - e[2] < 0 && zmax - e[2] < 0) ||
			(zmin - e[2] > 0 && zmax - e[2] > 0))
			return false;
		tzN = std::numeric_limits<float>::min();
		tzX = std::numeric_limits<float>::max();
	}

	if (txN > txX)
	{
		float temp = txN;
		txN = txX;
		txX = temp;
	}
	if (tyN > tyX)
	{
		float temp = tyN;
		tyN = tyX;
		tyX = temp;
	}	
	if (tzN > tzX)
	{
		float temp = tzN;
		tzN = tzX;
		tzX = temp;
	}
	//printf("x:%lf %lf Y:%lf %lf Z:%lf %lf T:%lf\n", txN, txX, tyN, tyX, tzN, tzX, minT);
	float far, near;
	near = txN;
	far = txX;
	if (near < tyN)
		near = tyN;
	if (near < tzN)
		near = tzN;
	if (far > tyX)
		far = tyX;
	if (far > tzX)
		far = tzX;


	if (far < near)
		return false;
	else
		return true;



}

void BVA::updateLeafShaders(Shader * newS)
{
	if (isLeaf)
	{
		model->updateShader(newS);
	}
	else
	{
		if (leftBox != 0)
			leftBox->updateLeafShaders(newS);
		if (rightBox != 0)
			rightBox->updateLeafShaders(newS);
	}
}