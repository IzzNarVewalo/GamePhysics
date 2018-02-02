#include "RigidBodySystem.h"

RigidBodySystem::RigidBodySystem()
{
	m_iNumRigidBodies = 0;
	m_fTotalMass = 0;

	//Demo1
	setupDemoOneRigidbody();
}

std::vector<Rigidbody> RigidBodySystem::getRigidBodySystem()
{
	return m_rigidbodySystem;
}

int RigidBodySystem::getNumRigidBodies()
{
	return m_iNumRigidBodies;
}

int RigidBodySystem::getTotalMassOf(int i)
{
	return m_rigidbodySystem[i].m_imass;
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

void RigidBodySystem::setAngularMomentum(int i, Vec3 L)
{
	m_rigidbodySystem[i].m_angularMomentum = L;
}

int RigidBodySystem::addRigidBody(Vec3 position, Vec3 size, int mass)
{
	Rigidbody rigid;
	rigid.m_boxCenter = position;
	rigid.m_boxSize = size;
	rigid.m_imass = mass;

	//calculate inertia tensor in 3D
	float Ixx = mass *(size.z * size.z + size.y * size.y);
	float Iyy = mass *(size.z * size.z + size.x * size.x);
	float Izz = mass *(size.y * size.y + size.x * size.x);

	float Ixy = mass *(-size.x*size.y);
	float Ixz = mass *(-size.x*size.z);
	float Izy = mass *(-size.y*size.z);
	rigid.inertiaTensor = XMMatrixSet(Ixx, Izy, Ixy, .0f, Izy, Iyy, Ixz, .0f, Ixy, Ixz, Izz, .0f, .0f, .0f, .0f, 1.0f);

	//angular momantum L
	rigid.m_angularMomentum = Vec3(.0f);

	//angular velocity w
	rigid.m_angularVelocity = Vec3(.0f);

	TorqueChar c;
	c.xi = Vec3(0.3f, 0.5f, 0.25f);
	c.fi = Vec3(1, 1, 0);
	rigid.m_pointsTorque.push_back(c);

	m_rigidbodySystem.push_back(rigid);
	m_iNumRigidBodies++;

	return m_iNumRigidBodies - 1;
}

Mat4 RigidBodySystem::getTranslatMatOf(int i)
{
	Mat4 temp;
	temp.initTranslation(m_rigidbodySystem[i].m_boxCenter.x, m_rigidbodySystem[i].m_boxCenter.y, m_rigidbodySystem[i].m_boxCenter.z);
	return temp;
}

Mat4 RigidBodySystem::getRotMatOf(int i)
{
	return m_rigidbodySystem[i].m_orientation.getRotMat();
}

Mat4 RigidBodySystem::getScaleMatOf(int i)
{
	Mat4 temp;
	temp.initScaling(m_rigidbodySystem[i].m_boxSize.x, m_rigidbodySystem[i].m_boxSize.y, m_rigidbodySystem[i].m_boxSize.z);
	return temp;
}

Mat4 RigidBodySystem::calcTransformMatrixOf(int i)
{
	return getScaleMatOf(i) * getRotMatOf(i) * getTranslatMatOf(i);
}

Vec3 RigidBodySystem::getXiOf(int i, int j)
{
	return m_rigidbodySystem[i].m_pointsTorque[j].xi;
}

void RigidBodySystem::setBoxCenterToZero()
{
	for (int i = 0; i < m_iNumRigidBodies; i++) {
		m_rigidbodySystem[i].m_boxCenter = Vec3(.0f, .0f, .0f);
	}
}

//setup for demo2 and demo1
void RigidBodySystem::setupDemoOneRigidbody()
{
	addRigidBody(Vec3(.0f, .0f, .0f), Vec3(1.0f, 0.6f, 0.5f), 2);
	setRotation(0, Quat(0, 0, M_PI_2));
}

void RigidBodySystem::cleanScene()
{
	while (!m_rigidbodySystem.empty()) {
		m_rigidbodySystem.pop_back();
	}

	m_iNumRigidBodies = 0;
	m_fTotalMass = 0;
}

void RigidBodySystem::addRandomTorquesTo(int i)
{
	m_rigidbodySystem[i].m_pointsTorque.clear();

	//add some random torques -> TODO: really random
	TorqueChar c;
	int z = m_rigidbodySystem[i].m_boxCenter.x < 0 ? 1 : -1;
	c.xi = z * Vec3(0.2f, 0.2f, 0.0f);
	c.fi = z * Vec3(2.0f, 2.0f, .0f);
	m_rigidbodySystem[i].m_pointsTorque.push_back(c);
	z == 1 ? m_rigidbodySystem[i].fixed = false : m_rigidbodySystem[i].fixed = true;
}



