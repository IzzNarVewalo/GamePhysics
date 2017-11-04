#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_iTestCase = EULER;
	m_externalForce = Vec3();
	m_inumPoints = m_inumSprings = 0;
	addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), FALSE);
	addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), TRUE);
}

const char * MassSpringSystemSimulator::getTestCasesStr()
{
	return "Euler, Midpoint, Leapfrog";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	switch (m_iTestCase)
	{
	case 0:
		TwAddVarRW(DUC->g_pTweakBar, "# Points", TW_TYPE_INT32, &m_inumPoints, "min=1");
		TwAddVarRW(DUC->g_pTweakBar, "# Springs", TW_TYPE_INT32, &m_inumSprings, "min=1");
		break;
	case 1:break;
	case 2:break;
	default:break;
	}
}

void MassSpringSystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	switch (m_iTestCase) {
	case 0:
		for (int i = 0; i < m_points.size(); i++) {
			//initial point
			DUC->setUpLighting(Vec3(), Vec3(1, 1, 0), 5.0f, Vec3(1, 0.5f, 0.65f));
			DUC->drawSphere(m_points[i].position, Vec3(0.05, 0.05, 0.05));
		}

		DUC->beginLine();
		DUC->drawLine(m_points[0].position, Vec3(0, 1, 0), m_points[1].position, Vec3(0, 0, 1));
		DUC->endLine();

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
	case EULER:
		cout << "Euler !\n";
		cout << "position of mass0: " << m_points[0].position;
		break;
	case LEAPFROG:
		cout << "Leapfrog\n";
		//m_iNumSpheres = 100;
		//m_fSphereSize = 0.05f;
		break;
	case MIDPOINT:
		cout << "Midpoint !\n";
		break;
	default:
		cout << "Empty Test!\n";
		break;
	}
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	switch (m_iTestCase) {
	case 0:
		//first time step
		for (int i = 0; i < m_points.size(); i++) {
			if (!m_points[i].isFixed)
				m_points[i].position = m_points[i].position + timeStep * m_points[i].velocity;
		}

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
	m_points.push_back(point);
	m_inumPoints++;
	return m_inumPoints - 1;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
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
