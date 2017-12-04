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

int RigidBodySystem::getTotalMass()
{
	return m_fTotalMass;
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

void RigidBodySystem::setCentralOfMassPosition(int i, Vec3 pos)
{
	m_rigidbodySystem[i].m_boxCenter = pos;
}

void RigidBodySystem::setCentralOfMassVelocity(int i, Vec3 vel)
{
	m_rigidbodySystem[i].m_velocity = vel;
}

void RigidBodySystem::setRotation(int i, Quat rot)
{
	m_rigidbodySystem[i].m_orientation = rot;
}

void RigidBodySystem::setAngularVelocity(int i, Vec3 w)
{
	m_rigidbodySystem[i].m_angularVelocity = w;
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

Mat4 RigidBodySystem::getTranslatMatOf(int i)
{
	Mat4 temp;
	temp.initTranslation(m_rigidbodySystem[i].m_boxCenter.x,m_rigidbodySystem[i].m_boxCenter.y,m_rigidbodySystem[i].m_boxCenter.z);
	return temp;
}

Mat4 RigidBodySystem::getRotMatOf(int i)
{
	return m_rigidbodySystem[i].m_orientation.getRotMat();
}

Mat4 RigidBodySystem::getScaleMatOf(int i)
{
	Mat4 temp;
	temp.initScaling(m_rigidbodySystem[i].m_boxSize.x,m_rigidbodySystem[i].m_boxSize.y,m_rigidbodySystem[i].m_boxSize.z);
	return temp;
}

Mat4 RigidBodySystem::calcTransformMatrixOf(int i)
{
	return getScaleMatOf(i) * getRotMatOf(i) * getTranslatMatOf(i);
}



