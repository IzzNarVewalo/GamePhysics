#pragma once
#include "Simulator.h"

//system of spheres
//every sphere is definied by.. 

//sphere
struct Sphere {
	Vec3 position, velocity, force;
};

class SphereSystem {

public:
	//Constructors
	SphereSystem();
	//Functions
	std::vector<Sphere> getSpheres();

private:
	//Attributes
	std::vector<Sphere> spheres; // all spheres in a system
};