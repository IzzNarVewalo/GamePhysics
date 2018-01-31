#include "SphereSystem.h"

SphereSystem::SphereSystem(int i)
{
	numberSpheresAtLine = 9;
	ylevel = 0.5f - ((1.0f / (float)numberSpheresAtLine) / 2) + 0.045f;

	if (i == 1) {
		Sphere s1;
		s1.position = Vec3(-0.5f, -0.5f, 0.0f);
		s1.velocity = Vec3(1.0f, 1.0f, 0.0f);
		s1.force = Vec3(0.0f);
		spheres.push_back(s1);
		Sphere s2;
		s2.position = Vec3(0.0f, 0.0f, 0.0f);
		s2.velocity = Vec3(0.0f, 0.0f, 0.0f);
		s2.force = Vec3(0.0f);
		spheres.push_back(s2);
	}
	else {
		addSphereAreaToSystem(0.045f);
		addSphereAreaToSystem(0.045f);
	}
}

std::vector<Sphere> SphereSystem::getSpheres()
{
	return spheres;
}


void SphereSystem::addSphereAreaToSystem(float radius)
{
	//jede kugel bekommt (1/numberSpheresAtLine) platz
	float space = 1.0f / (float)numberSpheresAtLine;

	float xDist = -0.5f + space / 2 - radius;
	float zDist = -0.5f + space / 2 - radius;

	Sphere newSphere;
	for (int x = 0; x < numberSpheresAtLine; x++) {
		for (int z = 0; z < numberSpheresAtLine; z++) {
			newSphere.position = Vec3(xDist + radius, ylevel - radius, zDist + radius);
			newSphere.velocity = Vec3(.0f);
			newSphere.force = Vec3(.0f);
			spheres.push_back(newSphere);

			zDist += space;
		}
		xDist += space;
		zDist = -0.5f;
	}
	zDist = xDist = -0.5f;
	ylevel -= space;
}