#include "RigidBodySystem.h"

RigidBodySystem::RigidBodySystem()
{	
	m_iNumRigidBodies = 0;
}

std::vector<Rigidbody> RigidBodySystem::getRigidBodySystem()
{
	return std::vector<Rigidbody>();
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


