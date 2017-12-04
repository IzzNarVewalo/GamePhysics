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

void RigidBodySystem::setAngularMomentum(int i, Vec3 L)
{
	m_rigidbodySystem[i].m_angularMomentum = L;
}

int RigidBodySystem::addRigidBody(Vec3 position, Vec3 size, int mass, int test)
{
	m_fTotalMass += mass;

	Rigidbody rigid;
	rigid.m_boxCenter = position;
	rigid.m_boxSize = size;
	rigid.m_imass = mass;

	//calculate inertia tensor in 3D
	float Ixx = mass *(size.z * size.z + size.x * size.x);
	float Iyy = mass *(size.y * size.y + size.x * size.x);
	float Izz = mass *(size.y * size.y + size.z * size.z);

	float Ixy = mass *(-size.x*size.y);
	float Ixz = mass *(-size.x*size.z); //z 2, 1 y, 3 x
	float Izy = mass *(-size.y*size.z);
	rigid.inertiaTensor = XMMatrixSet(Ixx, Izy, Ixy, .0f, Izy, Iyy, Ixz, .0f, Ixy, Ixz, Izz, .0f, .0f, .0f, .0f, 1.0f);

	rigid.inert = rigid.inertiaTensor.inverse();

	//angular momantum L
	rigid.m_angularMomentum = Vec3(.0f);

	//angular velocity w
	rigid.m_angularVelocity = Vec3(.0f);

	TorqueChar c;
	if (test != 2) {
		c.xi = Vec3(0.3f, 0.5f, 0.25f);
		c.fi = Vec3(1, 1, 0);
		rigid.m_pointsTorque.push_back(c);
	}
	else {
		int z = position.x < 0 ? 1 : 0;
		c.xi = z * Vec3(0.5f, 0.5f, 0.0f);
		c.fi = z * Vec3(1.0f, 1.0f, .0f);
		rigid.m_pointsTorque.push_back(c);
		z == 1 ? rigid.fixed = false : rigid.fixed = true;		
	}

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

void RigidBodySystem::reset()
{
	for (int i = 0; i < m_iNumRigidBodies; i++) {
		m_rigidbodySystem[i].m_boxCenter = Vec3(.0f, .0f, .0f);
	}
}

//setup for demo2. reset everything to 0
void RigidBodySystem::reset2(int test)
{
	addRigidBody(Vec3(.0f, .0f, .0f), Vec3(1.0f, 0.6f, 0.5f), 2, test);

	for (int i = 0; i < m_iNumRigidBodies; i++) {
		m_rigidbodySystem[i].m_boxCenter = Vec3(.0f, .0f, .0f);
		m_rigidbodySystem[i].m_velocity = Vec3(.0f, .0f, .0f);
		m_rigidbodySystem[i].m_angularMomentum = Vec3(.0f, .0f, .0f);
		m_rigidbodySystem[i].m_angularVelocity = Vec3(.0f, .0f, .0f);
		m_rigidbodySystem[i].m_torque = Vec3(.0f, .0f, .0f);
		m_rigidbodySystem[i].m_force = Vec3(.0f, .0f, .0f);
		m_rigidbodySystem[i].m_orientation = Quat(0, 0, M_PI_2);
	}
}

void RigidBodySystem::reset3()
{
	while (!m_rigidbodySystem.empty()) {
		m_rigidbodySystem.pop_back();
	}

	m_iNumRigidBodies = 0;
	m_fTotalMass = 0;
}



