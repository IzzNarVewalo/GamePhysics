#pragma once
#include "Simulator.h"
//kollision zweier kugeln resultiert in explosion von kugeln
//TODO: kollision fuehrt zu verschiedenen objekten
//TODO: kollision verschiedener objekte

struct Particle {
	Vec3 pos, vel, force;
	float size, remsize;
	boolean colliding;
};

class ParticleCollisionSystem {

public:

	ParticleCollisionSystem();

	std::vector<Particle> getParticles() {
		return particles;
	}

	void setupScene();

	int createParticles(Vec3 pos) {
		deleteScene();
		int num = 5;		
		for (int i = 0; i < num; i++) {
			Particle part;
			part.pos = pos;
			part.vel = Vec3(-1 + i, -2 + i, 0);
			part.force = Vec3();
			part.colliding = false;
			part.size = 0.1f;
			particles.push_back(part);
		}
		return num;
	}

	void deleteScene() {
		while (!particles.empty()) {
			particles.pop_back();
		}
	}

private:
	std::vector<Particle> particles;

};
