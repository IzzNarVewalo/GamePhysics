#include "MassSpringSystemSimulator.h"



MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_fMass = 10.0f;
	m_fStiffness = 40.0f;
	m_fDamping = 0;
	setIntegrator(EULER);

	m_externalForce = Vec3();
	m_iTestCase = 0;


}


const char * MassSpringSystemSimulator::getTestCasesStr()
{
	return "Demo 1, Demo 2, Demo 3, Demo 4";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	switch (m_iTestCase)
	{
	case 0:break;
	case 1:
		/*TwAddVarRW(DUC->g_pTweakBar, "Num Spheres", TW_TYPE_INT32, &m_iNumSpheres, "min=1");
		TwAddVarRW(DUC->g_pTweakBar, "Sphere Size", TW_TYPE_FLOAT, &m_fSphereSize, "min=0.01 step=0.01");*/
		break;
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
	Vec3 scale = Vec3(0.1f, 0.1f, 0.1f);

	//Light Colour, Specular Colour, Specular Power, Diffuse Colour
	DUC->setUpLighting(Vec3(0.5f, 0.5f, 0), Vec3(1, 0, 0), 0.5f, Vec3(0.5f, 0, 0.5f));
	
	for (int i = 0; i < pointList.size(); i++) {
		Point point = pointList[i];
		//Position: from -0.5 to 0.5 in every axis, Scale
		DUC->drawSphere(point.position, scale);
	}

	for (int i = 0; i < springList.size(); i++) {
		Spring spring = springList[i];

		DUC->beginLine();
		//Start Position, Start Colour, End Position, Start Colour
		DUC->drawLine(spring.point1, Vec3(0.3f, 0.35f, 0.35f), spring.point2, Vec3(0.9f, 0, 0.1f));
		DUC->endLine();
	}

	
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	switch (m_iIntegrator) {
	case EULER:
		integrateEuler(timeStep);
		break;
	case MIDPOINT:
		integrateMidpoint(timeStep);
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

void MassSpringSystemSimulator::integrateEuler(float timeStep)
{
	
	for (int i = 0; i < pointList.size(); i++) {
		//y1 = y0 + hf(x0, y0)
		pointList[i].position = getPositionOfMassPoint(i) + timeStep * getVelocityOfMassPoint(i);

	}


}

void MassSpringSystemSimulator::integrateMidpoint(float timeStep)
{
	for (int i = 0; i < pointList.size(); i++) {
		//y1 = y0 + h * f(x0, y0)
		Vec3 posPlusH = getPositionOfMassPoint(i) + timeStep * getVelocityOfMassPoint(i);
		Vec3 velHalfH = (posPlusH - getPositionOfMassPoint(i)) / timeStep;

		pointList[i].position = getPositionOfMassPoint(i) + timeStep * velHalfH;
	}
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
	Point newPoint;
	newPoint.position = position;
	newPoint.velocity = Velocity;
	newPoint.force = 0;
	if (isFixed) {
		newPoint.isFixed = isFixed;
	}
	pointList.push_back(newPoint);
	return pointList.size() - 1;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
	Spring newSpring;
	newSpring.point1 = masspoint1;
	newSpring.point2 = masspoint1;
	newSpring.initialLength = initialLength;
}

int MassSpringSystemSimulator::getNumberOfMassPoints()
{
	return pointList.size();
}

int MassSpringSystemSimulator::getNumberOfSprings()
{
	return springList.size();
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index)
{
	return pointList[index].position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index)
{
	return pointList[index].velocity;
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{
	//F = m*a
	for (int i = 0; i < pointList.size(); i++)
	{
		Vec3 accel = force / m_fMass;

	}
}
