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
	void setPosition(int i, Vec3 pos) {
		spheres[i].position = pos;
	}
	void setVelocity(int i, Vec3 vel) {
		spheres[i].velocity = vel;
	}
	void setVelocityX(int i, float velx) {
		spheres[i].velocity.x = velx;
	}
	void setVelocityY(int i, float vely) {
		spheres[i].velocity.y = vely;
	}

	Vec3 getVelocity(int i) {
		return spheres[i].velocity;
	}

	Vec3 getPosition(int i) {
		return spheres[i].position;
	}

private:
	//Attributes
	std::vector<Sphere> spheres; // all spheres in a system
	//skalierungsfaktor zum hinzufuegen von neuen kugeln (auf y-achse)
	int ylevel = 5;
	//skalierungsfaktor zum hinzufuegen von neuen kugeln (auf x-achse)
	int xLevel = 5;
	int zLevel = 5;
};