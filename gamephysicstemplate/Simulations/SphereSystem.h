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
	SphereSystem(int i);
	//Functions
	std::vector<Sphere> getSpheres();
	//fuegt neue kugel in naechstem level hinzu
	void addSphereToSystem();
	void setPosition(int i, Vec3 pos) {
		spheres[i].position = pos;
	}

	void setPositionX(int i, float x) {
		spheres[i].position.x = x;
	}

	void setPositionY(int i, float x) {
		spheres[i].position.y = x;
	}

	void setPositionZ(int i, float x) {
		spheres[i].position.z = x;
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
	void setVelocityZ(int i, float velz) {
		spheres[i].velocity.z = velz;
	}

	void setForce(int i, Vec3 force) {
		spheres[i].force = force;
	}

	Vec3 getVelocity(int i) {
		return spheres[i].velocity;
	}

	Vec3 getPosition(int i) {
		return spheres[i].position;
	}

	Vec3 getForceOf(int i) {
		return spheres[i].force;
	}

	void popBack() {
		spheres.pop_back();
	}

	Sphere* getUniformGrid() {
		return uniformGrif;
	}

	int getNumGrid() {
		return m * 10;
	}

private:
	//Attributes
	std::vector<Sphere> spheres; // all spheres in a system
	//skalierungsfaktor zum hinzufuegen von neuen kugeln (auf y-achse)
	int ylevel = 5;
	//skalierungsfaktor zum hinzufuegen von neuen kugeln (auf x-achse)
	int xLevel = 5;
	int zLevel = 5;

	//uniform grid
	int m = 1000; //number of cells in a grid 10*10*10
	Sphere* uniformGrif = new Sphere[10 * m];

};