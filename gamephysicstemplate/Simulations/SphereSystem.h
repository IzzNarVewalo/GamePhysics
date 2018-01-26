#pragma once
#include <array>
#include "Simulator.h"


//system of spheres
//every sphere is definied by.. 

//sphere
struct Sphere {
	Vec3 position, velocity, force;
};

const int cubeSize = 1000; //number of cells in a grid 10*10*10
const int bucketSize = 10;

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

	void setForceDemo2(int i, Vec3 force, int index) {
		uniformGrid[index][i].force = force;
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

	int getNumGrid() {
		return cubeSize * bucketSize;
	}

	void saveToArray(int x, int y, int z, int i) {
		int index = x + y * 10 + z * 100;

		uniformGrid[index].push_back(spheres[i]);

		if (uniformGrid[index].size() == 2) {
			colIndizes.push_back(index);
		}
	}

	std::vector<int> getColIndezes() {
		return colIndizes;
	}

	std::vector<Sphere> getColSpheres(int index) {
		return uniformGrid[index];
	}

	void resetUniformGrid() {
		for (int i = 0; i < uniformGrid.size(); i++) {
			uniformGrid[i].clear();
		}
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
	std::array<std::vector<Sphere>, cubeSize> uniformGrid;
	std::vector<int> colIndizes;
};