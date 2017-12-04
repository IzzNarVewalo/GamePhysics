#include "RigidBodySystemSimulator.h"

int j = -1;
float m_fextraForce = 1;

RigidBodySystemSimulator::RigidBodySystemSimulator()
{
	m_pRigidBodySystem = new RigidBodySystem();
	m_iTestCase = 0;
	first = second = true;

	//fuer demo 1
	addRigidBody(Vec3(.0f, .0f, .0f), Vec3(1.0f, 0.6f, 0.5f), 2);	
	setOrientationOf(0, Quat(0, 0, M_PI_2));

	j = -1;
}

const char * RigidBodySystemSimulator::getTestCasesStr()
{
	return "Demo 1, Demo 2, Demo 3, Demo 4";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	switch (m_iTestCase) {
	case 1: TwAddVarRW(DUC->g_pTweakBar, "extra force factor", TW_TYPE_FLOAT, &m_fextraForce, "min=1 step=0.1 max=5");
		break;
	default:
		break;
	}
}

void RigidBodySystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

}

void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	int numTemp = m_pRigidBodySystem->getNumRigidBodies();

	DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 1), 0.2f, Vec3(0.5f, 0.5f, 0.5f));

	for (int i = 0; i < numTemp; i++) {
		DUC->drawRigidBody(m_pRigidBodySystem->calcTransformMatrixOf(i));
	}
}

void RigidBodySystemSimulator::notifyCaseChanged(int testCase)
{
	int i = testCase;
	TorqueChar c;
	switch (i) {
	case 0: cout << "demo 1!\n";
		m_fextraForce = 1.0f;
		m_iTestCase = 0;
		if (i != j)
		{
			m_pRigidBodySystem->reset3();
			m_pRigidBodySystem->reset2(i);
		}
		first = true;
		break;
	case 1: cout << "demo2!\n";
		m_iTestCase = 1;
		if (i != j) {
			m_pRigidBodySystem->reset3();
			m_pRigidBodySystem->reset2(i);
			m_fextraForce = 1.0f;
		}
		break;

	case 2: cout << "demo3!\n";
		m_iTestCase = 2;
		//loesche alles
		if (i != j) {
			m_pRigidBodySystem->reset3();
		}
		//erstelle neues setup mit zwei koerpern
		addRigidBody(Vec3(0.5f, 0.5f, 0.0f), Vec3(0.5f, 0.6f, 0.5f), 2);
		addRigidBody(Vec3(-0.5f, -0.5f, 0.0f), Vec3(0.5f, 0.6f, 0.5f), 3);
		
		//xi und fi f�r torques setzen		
		//m_pRigidBodySystem->addTorque(0, Vec3(0.5f, 0.5f, 0.0f), Vec3(-1.0f, -1.0f, .0f));
		//m_pRigidBodySystem->addTorque(1, Vec3(0.3f, 0.5f, 0.25f), Vec3(1.0f, 1.0f, .0f));
		
		setOrientationOf(0, Quat(0, 0, M_PI_2));
		setOrientationOf(1, Quat(0, 0, 0));
		break;
	default: cout << "default\n";
	}
	if (m_iTestCase != 2)
		m_pRigidBodySystem->reset();
	j = i;
}

//page 25 'external forces'
void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed)
{

	std::vector<Rigidbody> temp = m_pRigidBodySystem->getRigidBodySystem();
	Vec3 tempTotalTorque;
	Vec3 tempTotalForce;
	int tempTorqueNum = 0;

	for (int i = 0; i < m_pRigidBodySystem->getNumRigidBodies(); i++) {

		tempTotalTorque = Vec3(.0f);
		tempTotalForce = Vec3(.0f);
		tempTorqueNum = temp[i].m_pointsTorque.size();

		for (int j = 0; j < tempTorqueNum; j++) {
			Vec3 xi = (temp[i].m_pointsTorque[j].xi - temp[i].m_boxCenter);
			tempTotalTorque += cross(xi, temp[i].m_pointsTorque[j].fi);
			tempTotalForce += temp[i].m_pointsTorque[j].fi;
		}

		//set total Torque
		m_pRigidBodySystem->setTotalTorque(i, tempTotalTorque);
		//set total Force
		m_pRigidBodySystem->setTotalForce(i, tempTotalForce * m_fextraForce);
	}

}

void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{
	//euler integration at page 25
	std::vector<Rigidbody> temp = m_pRigidBodySystem->getRigidBodySystem();
	int num = m_pRigidBodySystem->getNumRigidBodies();

	for (int i = 0; i < num; i++) {
		//x
		m_pRigidBodySystem->setCentralOfMassPosition(i, (temp[i].m_boxCenter + timeStep * temp[i].m_velocity));
		//v linear velocity
		setVelocityOf(i, (temp[i].m_velocity + timeStep * temp[i].m_force / (m_pRigidBodySystem->getTotalMass())));
		//r
		Quat orient = temp[i].m_orientation + (timeStep)* Quat(temp[i].m_angularVelocity.x, temp[i].m_angularVelocity.y, temp[i].m_angularVelocity.z, 1.0f) * temp[i].m_orientation;
		setOrientationOf(i, (orient.unit()));
		//w angular velocity		
		m_pRigidBodySystem->setAngularVelocity(i, temp[i].m_angularVelocity + timeStep * temp[i].m_torque);

		//L angular momentum -> we do not need it, as we use the second equation
		/*Mat4 tr = m_pRigidBodySystem->getRotMatOf(i);
		tr.transpose();

		temp[i].inertiaTensor = m_pRigidBodySystem->getRotMatOf(i) * temp[i].inertiaTensor * tr;
		m_pRigidBodySystem->setAngularMomentum(i, temp[i].inertiaTensor.transformVector(temp[i].m_angularVelocity));
		*/

		//I do not know why this does not work, maybe the factor is just too big, so it rotates too fast
		//m_pRigidBodySystem->setAngularVelocity(i, temp[i].m_angularVelocity + timeStep * temp[i].inert.transformVector(temp[i].m_torque));


		if (first) {
			cout << "------------------------demo1 case------------------------\n";
			cout << "linear and angular velocity of the body: " << getAngularVelocityOfRigidBody(i) << ", " << getLinearVelocityOfRigidBody(i) << "\n";

			Vec3 point = temp[i].m_boxCenter + m_pRigidBodySystem->getRotMatOf(i).transformVector(Vec3(0.3f, 0.5f, 0.25f));
			Vec3 vel = temp[i].m_velocity + cross(temp[i].m_angularVelocity, Vec3(0.3f, 0.5f, 0.25f));

			cout << "world space velocity of point (0.3 0.5 0.25) in world space (" << point << "): " << vel << "\n\n";
			first = false;
		}
		else {
			if (!first && second) {
				cout << "linear and angular velocity of the body: " << getAngularVelocityOfRigidBody(i) << ", " << getLinearVelocityOfRigidBody(i) << "\n";

				Vec3 point = temp[i].m_boxCenter + m_pRigidBodySystem->getRotMatOf(i).transformVector(Vec3(0.3f, 0.5f, 0.25f));
				Vec3 vel = temp[i].m_velocity + cross(temp[i].m_angularVelocity, Vec3(0.3f, 0.5f, 0.25f));

				cout << "world space velocity of point (0.3 0.5 0.25) in world space (" << point << "): " << vel << "\n";
				second = false;
			}
		}
	}
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

//in world space
Vec3 RigidBodySystemSimulator::getPositionOfRigidBody(int i)
{
	Vec3 xi = m_pRigidBodySystem->getXiOf(i, 0) - m_pRigidBodySystem->getRigidBodySystem()[i].m_boxCenter;
	return m_pRigidBodySystem->getRigidBodySystem()[i].m_boxCenter + m_pRigidBodySystem->getRotMatOf(i).transformVector(xi);
}

Vec3 RigidBodySystemSimulator::getLinearVelocityOfRigidBody(int i)
{
	Vec3 xi = (m_pRigidBodySystem->getXiOf(i, 0) - m_pRigidBodySystem->getRigidBodySystem()[i].m_boxCenter);
	return m_pRigidBodySystem->getRigidBodySystem()[i].m_velocity + cross(m_pRigidBodySystem->getRigidBodySystem()[i].m_angularVelocity, xi);
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
	m_pRigidBodySystem->addRigidBody(position, size, mass, m_iTestCase);
}

void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation)
{
	m_pRigidBodySystem->setRotation(i, orientation);
}

void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity)
{
	m_pRigidBodySystem->setCentralOfMassVelocity(i, velocity);
}
