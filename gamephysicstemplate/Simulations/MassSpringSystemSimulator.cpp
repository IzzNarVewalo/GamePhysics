#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	setupDemo1();
}

//reset position and velocity of points that move each change
//setup the scene
void MassSpringSystemSimulator::setupDemo1()
{
	m_iTestCase = 0;
	setIntegrator(EULER);
	m_idemoFinish = FALSE;
	m_externalForce = Vec3();
	m_inumPoints = m_inumSprings = 0;
	m_fMass = 10.f;
	m_fStiffness = 40.f;

	//first option
	addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), FALSE);
	addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), TRUE);
	//the first and second point in m_points
	addSpring(0, 1, 1);
	
	//second option
	addMassPoint(Vec3(1, 0, 0), Vec3(-1, 0, 0), FALSE);
	addMassPoint(Vec3(1, 2, 0), Vec3(1, 0, 0), FALSE);	
	addSpring(2, 3, 1);
	//"Decke"
	addMassPoint(Vec3(1, 3, 0), Vec3(0, 0, 0), TRUE);
	addSpring(3, 4, 1);
}

void MassSpringSystemSimulator::resetDemo1()
{
	m_points[0].position = Vec3(0, 0, 0);
	m_points[0].velocity = Vec3(-1, 0, 0);

	m_points[2].position = Vec3(1, 0, 0);
	m_points[2].velocity = Vec3(-1, 0, 0);

	m_points[3].position = Vec3(1, 2, 0);
	m_points[3].velocity = Vec3(1, 0, 0);
	
}


void MassSpringSystemSimulator::setupDemo4()
{
	while (!m_points.empty()) {
		m_points.pop_back();
	}

	while (!m_springs.empty()) {
		m_springs.pop_back();
	}

	m_inumPoints = m_inumSprings = 0;

	//first option
	addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), FALSE);
	addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), TRUE);
	//the first and second point in m_points
	addSpring(0, 1, 1);

	//second option
	addMassPoint(Vec3(1, 0, 0), Vec3(-1, 0, 0), FALSE);
	addMassPoint(Vec3(1, 2, 0), Vec3(1, 0, 0), FALSE);
	addSpring(2, 3, 1);
	//"Decke"
	addMassPoint(Vec3(1, 3, 0), Vec3(0, 0, 0), TRUE);
	addSpring(3, 4, 1);

	setIntegrator(EULER);
}

const char * MassSpringSystemSimulator::getTestCasesStr()
{
	return "Demo 1, Demo 2, Demo 3, Demo 4, Demo 5";
}

const char * MassSpringSystemSimulator::getIntegCasesStr()
{
	return "Euler, Midpoint";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;

	DUC->update(0.1f);
	TwType TW_TYPE_INTEGCASE = TwDefineEnumFromString("Integration", getIntegCasesStr());
	switch (m_iTestCase)
	{
	case 0:
		TwAddVarRW(DUC->g_pTweakBar, "Integration", TW_TYPE_INTEGCASE, &m_iIntegrator, "");
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		TwAddVarRW(DUC->g_pTweakBar, "Integration", TW_TYPE_INTEGCASE, &m_iIntegrator, "");
		TwAddVarRW(DUC->g_pTweakBar, "# Points", TW_TYPE_INT32, &m_inumPoints, "min=1");
		TwAddVarRW(DUC->g_pTweakBar, "# Springs", TW_TYPE_INT32, &m_inumSprings, "min=1");
		TwAddVarRW(DUC->g_pTweakBar, "stiffness", TW_TYPE_INT32, &m_fStiffness, "min=10");
		TwAddVarRW(DUC->g_pTweakBar, "mass", TW_TYPE_INT32, &m_fMass, "min=5");
		break;
	case 5:
		break;
	default:break;
	}
}

void MassSpringSystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

	//reset points' velocity and position

}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	switch (m_iTestCase) {
	case 0:
	case 1:
	case 2:
	case 4:
	case 3:
		//MassPoints
		for (int i = 0; i < m_inumPoints; i++) {

			DUC->setUpLighting(Vec3(), Vec3(1, 1, 0), 5.0f, Vec3(1, 0.5f, 0.65f));
			DUC->drawSphere(m_points[i].position, Vec3(0.05, 0.05, 0.05));
		}

		//Springs
		for (int i = 0; i < m_inumSprings; i++) {
			DUC->beginLine();
			DUC->drawLine(m_points[m_springs[i].point1].position, Vec3(0, 1, 0), m_points[m_springs[i].point2].position, Vec3(0, 0, 1));
			DUC->endLine();
		}
		break;
	default:
		break;
	}
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;

	switch (m_iTestCase)
	{
	case 0:
		cout << "Demo 1 !\n";
		resetDemo1();
		break;
	case 1:
		cout << "Demo 2 !\n";
		resetDemo1();
		break;
	case 2:
		cout << "Demo 3 !\n";
		resetDemo1();
		break;
	case 3:
		cout << "Demo 4 !\n";
		setupDemo4();
		break;
	case 4:
		cout << "Demo 5 !\n";
		resetDemo1();
		break;
	default:
		cout << "Empty Test!\n";
		break;
	}
	cout << "position of mass0 at time 0: " << m_points[0].position << "\n";
	cout << "velocity of mass0 at time 0: " << m_points[0].velocity << "\n";
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void MassSpringSystemSimulator::eulerStep(float timeStep)
{

	for (int i = m_inumPoints - 1; i >= 0; i--) {
		if (!m_points[i].isFixed)
		{
			//iterate velocity and position; acceleration depends on position				
			Vec3 acc = Vec3(-1.f, -1.f, -1.f) * (m_fStiffness / m_fMass) * m_points[i].position;

			m_points[i].position = m_points[i].position + timeStep * m_points[i].velocity;

			m_points[i].velocity = m_points[i].velocity + timeStep * acc;

			cout << i << " has " << acc << " " << m_points[i].position << " " << m_points[i].velocity;
		}
	}
}

void MassSpringSystemSimulator::midpointStep(float timeStep)
{
	for (int i = 0; i < m_inumPoints; i++) {
		if (!m_points[i].isFixed)
		{
			//xTilde
			Vec3 halfPos = m_points[i].position + timeStep / 2 * m_points[i].velocity;
			//acc at x, time t
			Vec3 acc = Vec3(-1.f, -1.f, -1.f) * (m_fStiffness / m_fMass) * m_points[i].position;
			//vel at xTilde
			Vec3 halfVel = m_points[i].velocity + timeStep / 2 * acc;

			//new position
			m_points[i].position = m_points[i].position + timeStep * halfVel;
			//acc at xTilde, time t+h/2
			Vec3 halfAcc = Vec3(-1.f, -1.f, -1.f) * (m_fStiffness / m_fMass) * halfPos;
			//new velocity
			m_points[i].velocity = m_points[i].velocity + timeStep * halfAcc;
		}
	}
}

void MassSpringSystemSimulator::leapfrogStep(float timeStep)
{
	for (int i = 0; i < m_inumPoints; i++) {
		if (!m_points[i].isFixed)
		{
			Vec3 acc = Vec3(-1.f, -1.f, -1.f) * (m_fStiffness / m_fMass) * m_points[i].position;

			m_points[i].velocity = m_points[i].velocity + timeStep * acc;

			m_points[i].position = m_points[i].position + timeStep * m_points[i].velocity;

		}
	}
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	switch (m_iTestCase) {
	case 0:

		timeStep = 0.1f;
		if (DXUTIsKeyDown('B') && !m_idemoFinish)
		{
			!m_iIntegrator ? eulerStep(timeStep) : midpointStep(timeStep);

			for (int i = 0; i < m_inumPoints; i++) {
				cout << "demo " << m_iTestCase + 1 << " position of mass " << i << " at first step: " << m_points[i].position << "\n";
				cout << "demo " << m_iTestCase + 1 << " velocity of mass " << i << " at first step: " << m_points[i].velocity << "\n";
			}

			m_idemoFinish = true;
		}
		else {
			//fuer backport
			if (!DXUTIsKeyDown('B'))
				m_idemoFinish = false;
		}
		break;
	case 1:
		//timeStep = 0.005f;
		eulerStep(timeStep);
		break;
	case 2:
		//timeStep = 0.005f;
		midpointStep(timeStep);
		break;
	case 3:
		//wenn Anzahl der points nicht mit der wkl länge übereinstrimmt, dann fuege random points hinzu


		break;
	case 4:
		//timeStep = 0.005f;
		leapfrogStep(timeStep);
		break;
	default:
		break;
	}

}

void MassSpringSystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void MassSpringSystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void MassSpringSystemSimulator::setMass(float mass)
{
	m_fMass = mass;
}

void MassSpringSystemSimulator::setStiffness(float stiffness)
{
	m_fStiffness = stiffness;
}

void MassSpringSystemSimulator::setDampingFactor(float damping)
{
	m_fDamping = damping;
}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed)
{
	MassPoint point;
	point.position = position;
	point.velocity = Velocity;
	point.isFixed = isFixed;
	m_points.push_back(point);
	m_inumPoints++;
	return m_inumPoints - 1;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
	Spring spring;
	spring.point1 = masspoint1;
	spring.point2 = masspoint2;
	spring.initialLength = initialLength;
	m_springs.push_back(spring);
	m_inumSprings++;
}

int MassSpringSystemSimulator::getNumberOfMassPoints()
{
	return m_inumPoints;
}

int MassSpringSystemSimulator::getNumberOfSprings()
{
	return m_inumSprings;
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index)
{
	return m_points[index].position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index)
{
	return m_points[index].velocity;
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{
}
