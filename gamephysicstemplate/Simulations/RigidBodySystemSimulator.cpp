#include "RigidBodySystemSimulator.h"

//TODO: free rigidbodysystem!!!
RigidBodySystemSimulator::RigidBodySystemSimulator()
{
	m_pRigidBodySystem = new RigidBodySystem();
	m_externalForce = Vec3 (1.0f, 1.0f, .0f);
	m_iTestCase = 0;
}

const char * RigidBodySystemSimulator::getTestCasesStr()
{
	return "Demo 1, Demo 2, Demo 3, Demo 4";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	DUC->update(0.1f);
	
}

void RigidBodySystemSimulator::reset()
{
}

void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	std::vector<Rigidbody> temp = m_pRigidBodySystem->getRigidBodySystem();
	int numTemp = m_pRigidBodySystem->getNumRigidBodies();
	
	DUC->setUpLighting(Vec3(), 0.4f*Vec3(1, 1, 1), 2000.0f, Vec3(0.5f, 0.5f, 0.5f));

	for (int i = 0; i < numTemp; i++) {

	}
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
