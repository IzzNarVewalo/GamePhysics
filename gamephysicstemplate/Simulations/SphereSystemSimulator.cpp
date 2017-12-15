#include "SphereSystemSimulator.h"

std::function<float(float)> SphereSystemSimulator::m_Kernels[5] = {
	[](float x) {return 1.0f; },              // Constant, m_iKernel = 0
	[](float x) {return 1.0f - x; },          // Linear, m_iKernel = 1, as given in the exercise Sheet, x = d/2r
	[](float x) {return (1.0f - x)*(1.0f - x); }, // Quadratic, m_iKernel = 2
	[](float x) {return 1.0f / (x)-1.0f; },     // Weak Electric Charge, m_iKernel = 3
	[](float x) {return 1.0f / (x*x) - 1.0f; },   // Electric Charge, m_iKernel = 4
};

// SphereSystemSimulator member functions

SphereSystemSimulator::SphereSystemSimulator()
{
	externalForce = Vec3();
	m_iTestCase = 0; //fangen mit demo1 an
	m_iAccelerator = NAIVEACC;
	m_fMass = 10.0f;
	m_fRadius = 0.05f;
	m_fForceScaling = 0.0f;
	m_fDamping = 5.0f;
	m_pSphereSystem = new SphereSystem();
	m_iKernel = 0;
	m_iNumSpheres = m_pSphereSystem->getSpheres().size();
	m_iIntegrator = MIDPOINT; //0 midpoint, 1 leap frog


}

const char * SphereSystemSimulator::getTestCasesStr()
{
	return "Demo 1, Demo 2, Demo 3";
}

const char * SphereSystemSimulator::getIntegCasesStr()
{
	return "Midpoint, Leap Frog";
}

void SphereSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;

	DUC->update(0.1f);
	TwType TW_TYPE_INTEGCASE = TwDefineEnumFromString("Integration", getIntegCasesStr());
	switch (m_iTestCase)
	{
	case 0:
		TwAddVarRW(DUC->g_pTweakBar, "Integration", TW_TYPE_INTEGCASE, &m_iIntegrator, "");
		TwAddVarRW(DUC->g_pTweakBar, "Mass", TW_TYPE_FLOAT, &m_fMass, "min=5");
		TwAddVarRW(DUC->g_pTweakBar, "Number", TW_TYPE_INT32, &m_iNumSpheres, "min=1");
		TwAddVarRW(DUC->g_pTweakBar, "Radius", TW_TYPE_FLOAT, &m_fRadius, "min=0.04, step=0.001");
		break;
	case 1:
		break;
	case 2:
		break;
	default:break;
	}
}

void SphereSystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	std::vector<Sphere> tmp = m_pSphereSystem->getSpheres();

	switch (m_iTestCase) {
	case 1:
		break;
	case 0:
		//Spheres
		for (int i = 0; i < m_iNumSpheres; i++) {

			DUC->setUpLighting(Vec3(), Vec3(1, 1, 0), 2000.0f, Vec3(1, 0.5f, 0.65f));
			DUC->drawSphere(tmp[i].position, Vec3(m_fRadius, m_fRadius, m_fRadius));
			//TODO: draw grid 
		}
		break;
	case 2:
		//different colours and size of spheres
		break;
	default:
		break;
	}
}

void SphereSystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void SphereSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
	case 0:
		cout << "Demo 1 !\n";

		break;
	case 1:
		cout << "Demo 2 !\n";

		break;
	case 2:
		cout << "Demo 3 !\n";

		break;
	default:
		cout << "Empty Test!\n";
		break;
	}
}

void SphereSystemSimulator::externalForcesCalculations(float timeElapsed)
{
	//gravity
	externalForce = Vec3(0, -9.81f, 0);
}

//TODO: instead of spring forces use repulsion forces
void SphereSystemSimulator::midpointStep(float timeStep)
{
	std::vector<Sphere> tmp = m_pSphereSystem->getSpheres();

	for (int i = 0; i < m_iNumSpheres; i++) {

		//xTilde
		tmp[i].position = tmp[i].position + (timeStep / 2) * tmp[i].velocity;

		//vel at xTilde
		tmp[i].velocity = tmp[i].velocity + (timeStep / 2) * tmp[i].force / m_fMass;

		//new position
		tmp[i].position = tmp[i].position + timeStep * tmp[i].velocity;

		//clear forces
		for (int i = 0; i < m_iNumSpheres; i++) {
			tmp[i].force = externalForce;
		}

	}

	//acc at xTilde, time t+h/2
	//compute internal force for all points in the system
	//for (int i = 0; i < m_iNumSpheres; i++) {
	//	//current length of spring
	//	float dist = norm(tmp[i].position - tmp[m_springs[i].point2].position);

	//	Vec3 tmp = m_fStiffness * (m_springs[i].currentLength - m_springs[i].initialLength);

	//	Vec3 force = tmp * (tmp[m_springs[i].point1].position - tmp[m_springs[i].point2].position) / (m_springs[i].currentLength);
	//	tmp[m_springs[i].point1].force -= force;

	//	force = tmp * (tmp[m_springs[i].point2].position - tmp[m_springs[i].point1].position) / (m_springs[i].currentLength);
	//	tmp[m_springs[i].point2].force -= force;
	//}

	//new velocity
	for (int i = 0; i < m_iNumSpheres; i++)
	{
		//bounding with floor
		if (tmp[i].position.y <= -0.5f)
			tmp[i].velocity.y *= (-0.99f);
		//bounding with walls
		if ((tmp[i].position.x <= -0.5f || tmp[i].position.x > 0.5f))
			tmp[i].velocity.x *= (-0.90f);
		//divide the force by mass of the point to get the acceleration 
		tmp[i].velocity = tmp[i].velocity + timeStep * tmp[i].force / m_fMass;

	}

}

void SphereSystemSimulator::leapfrogStep(float timeStep)
{
	std::vector<Sphere> tmp = m_pSphereSystem->getSpheres();

	for (int i = 0; i < m_iNumSpheres; i++) {

		setVelocity(i, tmp[i].velocity + timeStep * tmp[i].force / m_fMass);
		setPosition(i, tmp[i].position + timeStep * tmp[i].velocity);

		Vec3 veltmp = m_pSphereSystem->getVelocity(i);

		//bounce when hit the floor & velocity damping
		if (m_pSphereSystem->getPosition(i).y <= -0.5f || m_pSphereSystem->getPosition(i).y >= 0.5f)
			m_pSphereSystem->setVelocityY(i, veltmp.y *= (-0.99f));

		//bounce when hit the walls & velocity damping
		if ((m_pSphereSystem->getPosition(i).x <= -0.5f || m_pSphereSystem->getPosition(i).x >= 0.5f))
			m_pSphereSystem->setVelocityX(i, veltmp.x *= (-0.90f));
	}
}


void SphereSystemSimulator::simulateTimestep(float timeStep)
{	
	//wenn geadded, adden
	if (m_iNumSpheres != m_pSphereSystem->getSpheres().size()) {
		m_pSphereSystem->addSphereToSystem();
	}

	std::vector<Sphere> tmp = m_pSphereSystem->getSpheres();
	int a = 0;

	//if gravity on, accelerate in -y-direction
	for (int i = 0; i < m_iNumSpheres; i++) {
		tmp[i].force = externalForce;
	}


	switch (m_iTestCase) {
	case 0:
		//!m_iIntegrator ? midpointStep(timeStep) : leapfrogStep(timeStep);

		leapfrogStep(timeStep);
		//TODO: check for collisions
		a = 0;
		for (int i = 0; i < m_iNumSpheres; i++) {
			for (int j = 0; j < m_iNumSpheres, i != a; j++) {
				float posDif = norm(tmp[i].position - tmp[a].position);
				float radQuad = m_fRadius + m_fRadius;

				//collision, naiv approach
				//compute force for every sphere according to f(d)
				if (posDif > radQuad) {
					tmp[j].force = m_iKernel * (1 - posDif / radQuad);
				}
			}
			a++;
		}

		break;

	case 2:

		break;
	case 3:

		break;
	default:
		break;
	}
}

void SphereSystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void SphereSystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void SphereSystemSimulator::setMass(float mass)
{
	m_fMass = mass;
}

void SphereSystemSimulator::setDampingFactor(float damping)
{
	m_fDamping = damping;
}

void SphereSystemSimulator::setRadius(float radius)
{
	m_fRadius = radius;
}

int SphereSystemSimulator::getNumberOfSpheres()
{
	return m_iNumSpheres;
}
