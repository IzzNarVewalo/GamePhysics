#include "ParticleCollisionSystem.h"

ParticleCollisionSystem::ParticleCollisionSystem()
{
	setupScene();
}

void ParticleCollisionSystem::setupScene()
{
	Particle part;
	part.pos = Vec3(-0.5f, 0, 0);
	part.vel = Vec3(1, 0, 0);
	part.force = Vec3();
	part.size = 0.15f;
	part.colliding = true;
	particles.push_back(part);

	Particle part2;
	part2.pos = Vec3(0.5f, 0, 0);
	part2.vel = Vec3(-1, 0, 0);
	part2.force = Vec3();
	part2.size = 0.15f;
	part2.colliding = true;
	particles.push_back(part2);
}
