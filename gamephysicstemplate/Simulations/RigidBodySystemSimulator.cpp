#include "RigidBodySystemSimulator.h"

//TODO: free rigidbodysystem!!!
RigidBodySystemSimulator::RigidBodySystemSimulator()
{
	m_pRigidBodySystem = new RigidBodySystem();
	m_externalForce = Vec3 (1.0f, 1.0f, .0f);
	m_iTestCase = 0;

	//fuer demo 1
	addRigidBody(Vec3(.0f, .0f, .0f), Vec3(1.0f, 0.6f, 0.5f), 2);
	setOrientationOf(0, Quat(90));
}

const char * RigidBodySystemSimulator::getTestCasesStr()
{
	return "Demo 1, Demo 2, Demo 3, Demo 4";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	DUC->update(0.1f);
	DUC->drawRigidBody(Mat4(2));
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
		DUC->drawRigidBody(Mat4(XMMATRIX()));
	}
}

void RigidBodySystemSimulator::notifyCaseChanged(int testCase)
{
}

//page 25 'external forces'
void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed)
{	
	std::vector<Rigidbody> temp = m_pRigidBodySystem->getRigidBodySystem();
	Vec3 tempTotalTorque = Vec3(.0f);
	Vec3 tempTotalForce = Vec3(.0f);
	for (int i = 0; i < m_pRigidBodySystem->getNumRigidBodies(); i++) {

		int tempTorqueNum = temp[i].m_pointsTorque.size();
		for (int j = 0; j < tempTorqueNum; j++) {
			tempTotalTorque += cross(temp[i].m_pointsTorque[j].xi, temp[i].m_pointsTorque[j].fi);
			tempTotalForce += temp[i].m_pointsTorque[j].fi;
		}

		//set total Torque
		m_pRigidBodySystem->setTotalTorque(i, tempTotalTorque);
		//set total Force
		m_pRigidBodySystem->setTotalForce(i, tempTotalForce);
	}
}

void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{
	//euler integration at page 25


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
	return m_pRigidBodySystem->getRigidBodySystem()[i].m_boxCenter;
}

Vec3 RigidBodySystemSimulator::getLinearVelocityOfRigidBody(int i)
{
	return m_pRigidBodySystem->getRigidBodySystem()[i].m_velocity;
}

Vec3 RigidBodySystemSimulator::getAngularVelocityOfRigidBody(int i)
{
	return m_pRigidBodySystem->getRigidBodySystem()[i].m_angularMomentum;
}

void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force)
{

}

void RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, int mass)
{	
	m_pRigidBodySystem->addRigidBody(position, size, mass);	
}

void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation)
{
	m_pRigidBodySystem->getRigidBodySystem()[i].m_orientation = orientation;
}

void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity)
{
	m_pRigidBodySystem->getRigidBodySystem()[i].m_velocity = velocity;
}
