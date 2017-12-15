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
	//fuegt neue kugel in naechstem level hinzu
	void addSphereToSystem();

private:
	//Attributes
	std::vector<Sphere> spheres; // all spheres in a system
	//skalierungsfaktor zum hinzufuegen von neuen kugeln (auf y-achse)
	int ylevel = 5;
	//skalierungsfaktor zum hinzufuegen von neuen kugeln (auf x-achse)
	int xLevel = 5;
};