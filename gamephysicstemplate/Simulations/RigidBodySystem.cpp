#include "RigidBodySystem.h"

RigidBodySystem::RigidBodySystem()
{
	m_iNumRigidBodies = 0;
	m_fTotalMass = 0;
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

void RigidBodySystem::setTotalTorque(int i, Vec3 torque)
{
	m_rigidbodySystem[i].m_torque = torque;
}

void RigidBodySystem::setTotalForce(int i, Vec3 force)
{
	m_rigidbodySystem[i].m_force = force;
}

int RigidBodySystem::addRigidBody(Vec3 position, Vec3 size, int mass)
{
	m_fTotalMass += mass;

	Rigidbody rigid;
	rigid.m_boxCenter = position;
	rigid.m_boxSize = size;
	rigid.m_imass = mass;
	
	//calculate inertia tensor in 3D
	float Ixx = 1 / 12 * mass *(size.z * size.z + size.x * size.x);
	float Iyy = 1 / 12 * mass *(size.y * size.y + size.x * size.x);
	float Izz = 1 / 12 * mass *(size.y * size.y + size.z * size.z );
	rigid.inertiaTensor = XMMatrixSet(Ixx, .0f, .0f, .0f, .0f, Iyy, .0f, .0f, .0f, .0f, Izz, .0f, .0f, .0f, .0f, 1.0f);
	
	//angular momantum L
	rigid.m_angularMomentum = Vec3(.0f);

	//angular velocity w
	rigid.m_angularVelocity = Vec3(.0f);

	m_rigidbodySystem.push_back(rigid);	
	m_iNumRigidBodies++;

	return m_iNumRigidBodies - 1;
}



