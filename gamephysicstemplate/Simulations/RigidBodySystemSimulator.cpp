#include "RigidBodySystemSimulator.h"

//TODO: free rigidbodysystem!!!
RigidBodySystemSimulator::RigidBodySystemSimulator()
{
	m_pRigidBodySystem = new RigidBodySystem();
	m_externalForce = Vec3 (1.0f, 1.0f, .0f);
}

const char * RigidBodySystemSimulator::getTestCasesStr()
{
	return nullptr;
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
}

void RigidBodySystemSimulator::reset()
{
}

void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
}

void RigidBodySystemSimulator::notifyCaseChanged(int testCase)
{
}

void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{
}

void RigidBodySystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void RigidBodySystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

int RigidBodySystemSimulator::getNumberOfRigidBodies()
{
	return m_pRigidBodySystem->getNumRigidBodies();
}

Vec3 RigidBodySystemSimulator::getPositionOfRigidBody(int i)
{
	return Vec3();
}

Vec3 RigidBodySystemSimulator::getLinearVelocityOfRigidBody(int i)
{
	return Vec3();
}

Vec3 RigidBodySystemSimulator::getAngularVelocityOfRigidBody(int i)
{
	return Vec3();
}

void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force)
{
}

void RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, int mass)
{
	Rigidbody rigid;
	rigid.m_boxCenter = position;
	rigid.m_boxCenter = size;
	rigid.m_imass = mass;
	m_pRigidBodySystem->getRigidBodySystem().push_back(rigid);
	m_pRigidBodySystem->incNumRigidBodies();
}

void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation)
{
	m_pRigidBodySystem->getRigidBodySystem()[i].m_orientation = orientation;
}

void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity)
{
	m_pRigidBodySystem->getRigidBodySystem()[i].m_velocity = velocity;
}
