#include "SphereSystem.h"

SphereSystem::SphereSystem(int i)
{
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
		int i = 0;
		while (i != 100) {
			addSphereToSystem(); i++;
		}

		zLevel = 5;
		ylevel--;

		i = 0;
		while (i != 100) {
			addSphereToSystem(); i++;
		}
	}
}

std::vector<Sphere> SphereSystem::getSpheres()
{
	return spheres;
}


void SphereSystem::addSphereToSystem()
{
	Sphere newSphere;
	newSphere.position = Vec3(0.1f * xLevel - 0.05f, 0.1f * ylevel - 0.05f, 0.1f * zLevel - 0.05f);
	newSphere.velocity = Vec3();
	newSphere.force = Vec3(0.0f, -9.81f, 0.0f);
	xLevel--;

	if (zLevel <= -5 && xLevel <= -5) {
		ylevel--; zLevel = xLevel = 5;
	}

	if (xLevel <= -5) {
		zLevel--; xLevel = 5;
	}
		
	spheres.push_back(newSphere);
	
}

