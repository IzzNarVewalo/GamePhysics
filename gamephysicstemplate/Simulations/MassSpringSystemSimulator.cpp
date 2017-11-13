#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_iTestCase = 0;
	m_idemoFinish = FALSE;
	m_externalForce = Vec3();
	m_inumPoints = m_inumSprings = 0;

	//demo1 appears as default
	setupDemo1();
}

//setup the scene 
void MassSpringSystemSimulator::setupDemo1()
{
	//clear attributes and scene
	setIntegrator(EULER);
	m_fMass = 10.f;
	m_fStiffness = 40.f;

	while (!m_points.empty()) {
		m_points.pop_back();
	}

	while (!m_springs.empty()) {
		m_springs.pop_back();
	}

	m_inumPoints = m_inumSprings = 0;
	m_bgravityOn = FALSE;

	//add two points and a spring
	addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), FALSE);
	addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), FALSE);
	//the first and second point in m_points
	addSpring(0, 1, 1);
}

void MassSpringSystemSimulator::setupDemo4()
{
	//clear previous setting
	while (!m_points.empty()) {
		m_points.pop_back();
	}

	while (!m_springs.empty()) {
		m_springs.pop_back();
	}

	m_inumPoints = m_inumSprings = 0;
	m_bgravityOn = TRUE;

	//first option: one mass and one spring
	addMassPoint(Vec3(-0.25f, 0.25f, 0), Vec3(-0.7f, 0, 0), FALSE);
	addMassPoint(Vec3(-0.25f, 0.5f, 0), Vec3(0.7f, 0, 0), TRUE);
	//the first and second point in m_points
	addSpring(0, 1, 0.25f);

	//second option: two masses and two springs
	addMassPoint(Vec3(0.25f, 0.498f, 0), Vec3(-0.2, 0, 0), FALSE);
	addMassPoint(Vec3(0.25f, 0.499f, 0), Vec3(0.1, 0, 0), FALSE);
	addSpring(2, 3, 0.001f);
	//"ceil"
	addMassPoint(Vec3(0.5f, 0.5f, 0), Vec3(0, 0, 0), TRUE);
	addSpring(3, 4, 0.001f);

	//third option: a pyramid
	addMassPoint(Vec3(0.f, -0.3f, 0), Vec3(-0.5f, 0, 0), FALSE);
	addMassPoint(Vec3(-0.1f, -0.4f, 0), Vec3(0.1f, 0, 0), FALSE);
	addMassPoint(Vec3(0.f, -0.4f, 0), Vec3(-0.1f, 0, 0), FALSE);
	addMassPoint(Vec3(0.1f, -0.4f, 0), Vec3(0.5f, 0, 0), FALSE);
	addMassPoint(Vec3(0.f, -0.5f, 0), Vec3(-0.1f, 0, 0), FALSE);
	addSpring(5, 6, 0.01f);
	addSpring(5, 7, 0.01f);
	addSpring(5, 8, 0.01f);
	addSpring(5, 9, 0.02f);
	addSpring(6, 7, 0.01f);
	addSpring(6, 8, 0.02f);
	addSpring(6, 9, 0.01f);
	addSpring(7, 8, 0.01f);
	addSpring(7, 9, 0.01f);
	addSpring(8, 9, 0.01f);
	
	//fourth option: one mass and one spring (vertical)
	addMassPoint(Vec3(0, 0.25f, 0), Vec3(0, 0, 0), FALSE);
	addMassPoint(Vec3(0, 0.5f, 0), Vec3(0.7, 0, 0), TRUE);
	addSpring(10, 11, 0.25f);

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
		TwAddVarRW(DUC->g_pTweakBar, "Integration", TW_TYPE_INTEGCASE, &m_iIntegrator, "");
		TwAddVarRW(DUC->g_pTweakBar, "Stiffness", TW_TYPE_FLOAT, &m_fStiffness, "min=10");
		TwAddVarRW(DUC->g_pTweakBar, "Mass", TW_TYPE_FLOAT, &m_fMass, "min=5");
		break;
	case 4:
		break;
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
	case 1:
	case 2:
	case 4:
		//MassPoints
		for (int i = 0; i < m_inumPoints; i++) {

			DUC->setUpLighting(Vec3(), Vec3(1, 1, 0), 5.0f, Vec3(1, 0.5f, 0.65f));
			DUC->drawSphere(m_points[i].position, Vec3(0.05f, 0.05f, 0.05f));
		}

		//Springs
		for (int i = 0; i < m_inumSprings; i++) {
			DUC->beginLine();
			DUC->drawLine(m_points[m_springs[i].point1].position, Vec3(0, 1, 0), m_points[m_springs[i].point2].position, Vec3(0, 0, 1));
			DUC->endLine();
		}
		break;
	case 3:
		//different colours and size of spheres
		//MassPoints
		for (int i = 0; i < m_inumPoints; i++) {

			DUC->setUpLighting(Vec3(), Vec3(1, 0, 0), 5.0f, Vec3(1, 0.5f, 0.65f));
			DUC->drawSphere(m_points[i].position, Vec3(0.02f, 0.02f, 0.02f));
		}

		//Springs
		for (int i = 0; i < m_inumSprings; i++) {
			DUC->beginLine();
			DUC->drawLine(m_points[m_springs[i].point1].position, Vec3(1, 1, 0), m_points[m_springs[i].point2].position, Vec3(1, 0, 1));
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
		setupDemo1();
		break;
	case 1:
		cout << "Demo 2 !\n";
		setupDemo1();
		break;
	case 2:
		cout << "Demo 3 !\n";
		setupDemo1();
		break;
	case 3:
		cout << "Demo 4 !\n";
		setupDemo4();
		break;
	case 4:
		cout << "Demo 5 !\n";
		setupDemo1();
		break;
	default:
		cout << "Empty Test!\n";
		break;
	}
	//cout << "position of mass0 at time 0: " << m_points[0].position << "\n";
	//cout << "velocity of mass0 at time 0: " << m_points[0].velocity << "\n";
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void MassSpringSystemSimulator::eulerStep(float timeStep)
{
	for (int i = m_inumPoints - 1; i >= 0; i--) {
		if (!m_points[i].isFixed)
		{
			//iterate velocity and position; acceleration depends on force

			m_points[i].position = m_points[i].position + timeStep * m_points[i].velocity;

			//bounce when hit the floor & velocity damping
			if (m_points[i].position.y <= -0.5f && m_bgravityOn)
				m_points[i].velocity.y *= (-0.99f);

			//bounce when hit the walls & velocity damping
			if ((m_points[i].position.x <= -0.5f || m_points[i].position.x > 0.5f) && m_bgravityOn)
				m_points[i].velocity.x *= (-0.90f);

			//divide force by mass of point to get the acceleration to compute the velocity
			m_points[i].velocity = m_points[i].velocity + timeStep * m_points[i].force / m_fMass;
		}
	}
}

void MassSpringSystemSimulator::midpointStep(float timeStep)
{
	std::vector<MassPoint> tmpPoints = m_points;

	for (int i = 0; i < m_inumPoints; i++) {
		if (!m_points[i].isFixed)
		{
			//xTilde
			tmpPoints[i].position = m_points[i].position + (timeStep / 2) * m_points[i].velocity;

			//vel at xTilde
			tmpPoints[i].velocity = m_points[i].velocity + (timeStep / 2) * m_points[i].force / m_fMass;

			//new position
			m_points[i].position = m_points[i].position + timeStep * tmpPoints[i].velocity;

			//clear forces
			for (int i = 0; i < m_inumPoints; i++) {
				if (!m_bgravityOn)
				{
					m_points[i].force = Vec3(0.f);
				}
				else {
					m_points[i].force = Vec3(0.f, -9.81f, 0.f);
				}
			}
		}
	}

	//acc at xTilde, time t+h/2
	//compute internal force for all points in the system
	for (int i = 0; i < m_inumSprings; i++) {
		//current length of spring
		m_springs[i].currentLength = norm(tmpPoints[m_springs[i].point1].position - tmpPoints[m_springs[i].point2].position);

		Vec3 tmp = m_fStiffness * (m_springs[i].currentLength - m_springs[i].initialLength);

		Vec3 force = tmp * (tmpPoints[m_springs[i].point1].position - tmpPoints[m_springs[i].point2].position) / (m_springs[i].currentLength);
		tmpPoints[m_springs[i].point1].force -= force;

		force = tmp * (tmpPoints[m_springs[i].point2].position - tmpPoints[m_springs[i].point1].position) / (m_springs[i].currentLength);
		tmpPoints[m_springs[i].point2].force -= force;
	}

	//new velocity
	for (int i = 0; i < m_inumPoints; i++)
	{
		if (!m_points[i].isFixed)
		{
			//bounding with floor
			if (m_points[i].position.y <= -0.5f && m_bgravityOn)
				m_points[i].velocity.y *= (-0.99f);
			//bounding with walls
			if ((m_points[i].position.x <= -0.5f || m_points[i].position.x > 0.5f) && m_bgravityOn)
				m_points[i].velocity.x *= (-0.90f);
			//divide the force by mass of the point to get the acceleration 
			m_points[i].velocity = m_points[i].velocity + timeStep * tmpPoints[i].force / m_fMass;
		}
	}

}

void MassSpringSystemSimulator::leapfrogStep(float timeStep)
{
	for (int i = 0; i < m_inumPoints; i++) {
		if (!m_points[i].isFixed)
		{
			m_points[i].velocity = m_points[i].velocity + timeStep * m_points[i].force / m_fMass;

			m_points[i].position = m_points[i].position + timeStep * m_points[i].velocity;
		}
	}
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	//if gravity on, accelerate in -y-direction
	for (int i = 0; i < m_inumPoints; i++) {
		if (!m_bgravityOn)
		{
			m_points[i].force = Vec3(0.f);
		}
		else {
			m_points[i].force = Vec3(0.f, -9.81f, 0.f);
		}
	}

	//compute internal force for every point; acceleration depends only on elastic forces
	for (int i = 0; i < m_inumSprings; i++) {
		//current length of spring
		m_springs[i].currentLength = norm(m_points[m_springs[i].point1].position - m_points[m_springs[i].point2].position);

		Vec3 tmp = m_fStiffness * (m_springs[i].currentLength - m_springs[i].initialLength);

		Vec3 force = tmp * (m_points[m_springs[i].point1].position - m_points[m_springs[i].point2].position) / (m_springs[i].currentLength);
		m_points[m_springs[i].point1].force -= force;

		force = tmp * (m_points[m_springs[i].point2].position - m_points[m_springs[i].point1].position) / (m_springs[i].currentLength);
		m_points[m_springs[i].point2].force -= force;
	}

	switch (m_iTestCase) {
	case 0:

		timeStep = 0.1f;
		//we use B for timestep
		if (DXUTIsKeyDown('B') && !m_idemoFinish)
		{
			!m_iIntegrator ? eulerStep(timeStep) : midpointStep(timeStep);

			for (int i = 0; i < m_inumPoints; i++) {
				cout << "demo " << m_iTestCase + 1 << " position of mass " << i << " : " << m_points[i].position << "\n";
				cout << "demo " << m_iTestCase + 1 << " velocity of mass " << i << " : " << m_points[i].velocity << "\n";
			}
			cout << "\n";

			m_idemoFinish = true;
		}
		else {
			//for backport
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
		!m_iIntegrator ? eulerStep(timeStep) : midpointStep(timeStep);
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
