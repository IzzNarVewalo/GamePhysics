#include "RigidBodySystem.h"

RigidBodySystem::RigidBodySystem()
{		
	m_iNumRigidBodies = 0;
}

std::vector<Rigidbody> RigidBodySystem::getRigidBodySystem()
{
	return m_rigidbodySystem;
}

int RigidBodySystem::getNumRigidBodies()
{
	return m_iNumRigidBodies;
}

void RigidBodySystem::decNumRigidBodies()
{
	m_iNumRigidBodies--;
}

void RigidBodySystem::incNumRigidBodies()
{
	m_iNumRigidBodies++;
}

void RigidBodySystem::addRigidBody(Vec3 position, Vec3 size, int mass)
{
	Rigidbody rigid;
	rigid.m_boxCenter = position;
	rigid.m_boxSize = size;
	rigid.m_imass = mass;
	m_rigidbodySystem.push_back(rigid);
}


