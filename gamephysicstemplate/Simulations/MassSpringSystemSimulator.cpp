#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{

}

void integrateEuler() {

}

void integrateMidpoint() {

}

const char * MassSpringSystemSimulator::getTestCasesStr()
{
	return nullptr;
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
}

void MassSpringSystemSimulator::reset()
{
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	

	//Light Colour, Specular Colour, Specular Power, Diffuse Colour
	DUC->setUpLighting(Vec3(0.5f, 0.5f, 0), Vec3(1, 0, 0), 0.5f, Vec3(0.5f, 0, 0.5f));
	//Position: from -0.5 to 0.5 in every axis, Scale
	DUC->drawSphere(Vec3(0, 0, 0), Vec3(1, 1, 1));

	//DUC->beginLine();
	////Start Position, Start Colour, End Position, Start Colour
	//DUC->drawLine(Vec3(0, 0, 0), Vec3(0.3f, 0.35f, 0.35f), Vec3(0, 0.5f, 0), Vec3(0.9f, 0, 0.1f));
	//DUC->endLine();
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
}

void MassSpringSystemSimulator::onClick(int x, int y)
{
}

void MassSpringSystemSimulator::onMouse(int x, int y)
{
}

void MassSpringSystemSimulator::setMass(float mass)
{
}

void MassSpringSystemSimulator::setStiffness(float stiffness)
{
}

void MassSpringSystemSimulator::setDampingFactor(float damping)
{
}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed)
{
	return 0;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
}

int MassSpringSystemSimulator::getNumberOfMassPoints()
{
	return 0;
}

int MassSpringSystemSimulator::getNumberOfSprings()
{
	return 0;
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index)
{
	return Vec3();
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index)
{
	return Vec3();
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{
}
