#include "SphereSystemSimulator.h"

std::function<float(float)> SphereSystemSimulator::m_Kernels[5] = {
	[](float x) {return 1.0f; },              // Constant, m_iKernel = 0
	[](float x) {return 1.0f - x; },          // Linear, m_iKernel = 1, as given in the exercise Sheet, x = d/2r
	[](float x) {return (1.0f - x)*(1.0f - x); }, // Quadratic, m_iKernel = 2
	[](float x) {return 1.0f / (x)-1.0f; },     // Weak Electric Charge, m_iKernel = 3
	[](float x) {return 1.0f / (x*x) - 1.0f; },   // Electric Charge, m_iKernel = 4
};

// SphereSystemSimulator member functions

float x = -0.5f;
float y = -0.5f;
float z = -0.5f;

SphereSystemSimulator::SphereSystemSimulator()
{
	externalForce = Vec3(0, -9.81f, 0);
	m_iTestCase = 0; //fangen mit demo1 an
	m_iAccelerator = NAIVEACC;
	m_fMass = 10.0f;
	m_fRadius = 0.05f;
	m_fForceScaling = 0.0f;
	m_fDamping = 5.0f;
	m_iKernel = 1;
	m_iNumSpheres = 0;
	m_pSphereSystem = nullptr;
	m_iIntegrator = MIDPOINT; //0 midpoint, 1 leap frog

	//if gravity on, accelerate in -y-direction
	for (int i = 0; i < m_iNumSpheres; i++) {
		m_pSphereSystem->setForce(i, externalForce);
	}
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

	TwAddVarRW(DUC->g_pTweakBar, "Mass", TW_TYPE_FLOAT, &m_fMass, "min=0.1 step=0.1");
	TwAddVarRW(DUC->g_pTweakBar, "Radius", TW_TYPE_FLOAT, &m_fRadius, "min=0.04, step=0.001");
	TwAddVarRW(DUC->g_pTweakBar, "Number", TW_TYPE_INT32, &m_iNumSpheres, "min=1");

	switch (m_iTestCase)
	{
	case 0:
		break;
	case 1:
		TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, "min=0.1 step=0.01");
		TwAddVarRW(DUC->g_pTweakBar, "Kernel", TW_TYPE_INT32, &m_iKernel, "min=0");
		TwAddVarRW(DUC->g_pTweakBar, "Draw Grid", TW_TYPE_BOOL32, &gridDrawn, "");
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
		if (!gridDrawn) { break; }
		else {
			//einmal waagrecht und einmal senkrecht			
				//2D struktur
			for (int j = 0; j < 11; j++) {
				for (int a = 0; a < 11; a++) {
					//Abstand zwischen zellen: 0.1
					//insgesamt 10 zellen pro reihe

					Vec3 posbegin = Vec3(x, y, z * (-1));
					DUC->beginLine();
					DUC->drawLine(posbegin, Vec3(0, 0, 1), Vec3((-1) * x, y, z * (-1)), Vec3(1, 0, 0));
					DUC->endLine();
					y += 0.1f;
				}
				z += 0.1f;
				y = -0.5f;
			}
			x = y = z = -0.5f;

			for (int j = 0; j < 11; j++) {
				for (int a = 0; a < 11; a++) {
					//Abstand zwischen zellen: 0.1
					//insgesamt 10 zellen pro reihe

					Vec3 posbegin = Vec3(x, y, z);
					DUC->beginLine();
					DUC->drawLine(posbegin, Vec3(0, 0, 1), Vec3(x, y, (-1) * z), Vec3(1, 0, 0));
					DUC->endLine();
					y += 0.1f;
				}
				x += 0.1f;
				y = -0.5f;
			}
			x = y = z = -0.5f;
		}


		//Spheres
		for (int i = 0; i < m_iNumSpheres; i++) {

			DUC->setUpLighting(Vec3(), Vec3(1, 1, 0), 2000.0f, Vec3(1, 0.5f, 0.65f));
			DUC->drawSphere(tmp[i].position, Vec3(m_fRadius, m_fRadius, m_fRadius));

		}
		break;
	case 0:
		//Spheres
		for (int i = 0; i < m_iNumSpheres; i++) {

			DUC->setUpLighting(Vec3(), Vec3(0, 1, 0), 2000.0f, Vec3(0, 1.0f, 0.0f));
			DUC->drawSphere(tmp[i].position, Vec3(m_fRadius, m_fRadius, m_fRadius));
		}
		break;
	case 2:
		//different colours and size of spheres
		break;
	default:
		break;
	}

	DUC->drawSphere(Vec3(-0.45f, -0.45f, -0.45f), Vec3(m_fRadius, m_fRadius, m_fRadius));
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
		setupdemo1();
		break;
	case 1:
		cout << "Demo 2 !\n";
		setupdemo2();
		break;
	case 2:
		cout << "Demo 3 !\n";

		break;
	default:
		cout << "Empty Test!\n";
		break;
	}

	m_iNumSpheres = m_pSphereSystem->getSpheres().size();
}

void SphereSystemSimulator::externalForcesCalculations(float timeElapsed)
{
	//gravity
	//externalForce = Vec3(0, -9.81f, 0);
}

void SphereSystemSimulator::leapfrogStep(float timeStep)
{
	for (int i = 0; i < m_iNumSpheres; i++) {

		setVelocity(i, m_pSphereSystem->getVelocity(i) + timeStep * m_pSphereSystem->getForceOf(i) / m_fMass);
		setPosition(i, m_pSphereSystem->getPosition(i) + timeStep * m_pSphereSystem->getVelocity(i));

		Vec3 veltmp = m_pSphereSystem->getVelocity(i);

		//bounce when hit the floor & velocity damping
		if (m_pSphereSystem->getPosition(i).y <= -0.5f || m_pSphereSystem->getPosition(i).y >= 0.5f) {
			m_pSphereSystem->setVelocityY(i, veltmp.y *= (-0.99f));
			if (m_pSphereSystem->getPosition(i).y <= -0.5f)
				m_pSphereSystem->setPositionY(i, -0.5f);
			else
				m_pSphereSystem->setPositionY(i, 0.5f);
		}

		//bounce when hit the walls & velocity damping
		if ((m_pSphereSystem->getPosition(i).x <= -0.5f || m_pSphereSystem->getPosition(i).x >= 0.5f)) {
			m_pSphereSystem->setVelocityX(i, veltmp.x *= (-0.90f));
			if (m_pSphereSystem->getPosition(i).x <= -0.5f)
				m_pSphereSystem->setPositionX(i, -0.5f);
			else
				m_pSphereSystem->setPositionX(i, 0.5f);
		}

		if ((m_pSphereSystem->getPosition(i).z <= -0.5f || m_pSphereSystem->getPosition(i).z >= 0.5f)) {
			m_pSphereSystem->setVelocityZ(i, veltmp.z *= (-0.90f));
			if (m_pSphereSystem->getPosition(i).z <= -0.5f)
				m_pSphereSystem->setPositionZ(i, -0.5f);
			else
				m_pSphereSystem->setPositionZ(i, 0.5f);
		}
	}
}


void SphereSystemSimulator::simulateTimestep(float timeStep)
{
	int lambda = 1100;
	//wenn geadded, adden
	if (m_iNumSpheres != m_pSphereSystem->getSpheres().size()) {
		m_pSphereSystem->addSphereToSystem();
	}

	leapfrogStep(timeStep);

	//reset forces
	for (int i = 0; i < m_iNumSpheres; i++) {
		m_pSphereSystem->setForce(i, externalForce);
	}

	std::vector<int> tmpColIndizes;

	switch (m_iTestCase) {
	case 0:

		for (int i = 0; i < m_iNumSpheres; i++) {
			for (int j = 0; j < m_iNumSpheres, i != j; j++) {
				float posDif = norm(m_pSphereSystem->getPosition(i) - m_pSphereSystem->getPosition(j));
				float radQuad = m_fRadius + m_fRadius;

				//collision, naiv approach
				//compute force for every sphere according to f(d)
				if (posDif <= radQuad) {

					float strength = m_Kernels[m_iKernel]((posDif / 2 * radQuad)) * lambda;
					Vec3 directionOfForce = m_pSphereSystem->getPosition(i) - m_pSphereSystem->getPosition(j);
					//einheitsvektor
					directionOfForce /= posDif;

					Vec3 resForce = directionOfForce * strength;

					m_pSphereSystem->setForce(i, resForce);
					m_pSphereSystem->setForce(j, -resForce);
				}
			}
		}

		break;
	case 1:

		//loesche liste
		m_pSphereSystem->resetUniformGrid();

		for (int i = 0; i < m_iNumSpheres; i++) {
			Vec3 tmppos = m_pSphereSystem->getPosition(i);

			////////////////////////////zelle zuweisen
			//wertebereuch zu [0,9] verschieben. koenenn aber auch ausserhalb liegen weil noch nicht gerundet
			tmppos *= 10.0f;
			tmppos += Vec3(4.50f, 4.50f, 4.50f);
			//floor bzw ceiling zum ab bzw aufrunden
			Vec3 deci = Vec3((int)(tmppos.x * 10) % 10, (int)(tmppos.y * 10) % 10, (int)(tmppos.z * 10) % 10);

			for (int a = 0; a < 3; a++) {
				if (deci[a] > 5) {
					deci[a] = ceilf(tmppos[a]);
				}
				else {
					deci[a] = floorf(tmppos[a]);
					if (deci[a] < 0) {
						deci[a] = 0;
					}
				}
			}
			/////////////////////////////
			//ins array reinspeichern
			m_pSphereSystem->saveToArray(deci.x, deci.y, deci.z, i);
		}
		tmpColIndizes = m_pSphereSystem->getColIndezes();

		//auf collision pruefen
		for (int b = 0; b < tmpColIndizes.size(); b++) {

			std::vector<Sphere> tmp = m_pSphereSystem->getColSpheres(tmpColIndizes[b]);

			//durch liste tmp durchiterieren
			for (int c = 0; c < tmp.size(); c++) {
				for (int d = 0; d < tmp.size(), c != d; d++) {
					float posDif = norm(tmp[c].position - tmp[d].position);
					float radQuad = m_fRadius + m_fRadius;

					//collision, naiv approach
					//compute force for every sphere according to f(d)
					if (posDif <= radQuad) {

						float strength = m_Kernels[m_iKernel]((posDif / 2 * radQuad)) * lambda;
						Vec3 directionOfForce = tmp[c].position - tmp[d].position;
						//einheitsvektor
						directionOfForce /= posDif;

						Vec3 resForce = directionOfForce * strength;

						m_pSphereSystem->setForceDemo2(c, resForce, tmpColIndizes[b]);
						m_pSphereSystem->setForceDemo2(d, -resForce, tmpColIndizes[b]);
					}
				}
			}
			//
		}


		break;
	case 2:

		break;
	default:
		break;
	}
}

void SphereSystemSimulator::setupdemo2()
{
	//clear scene
	if (m_pSphereSystem == nullptr) {
		m_pSphereSystem = new SphereSystem(2);
	}
	else {
		delete m_pSphereSystem;
		m_pSphereSystem = new SphereSystem(2);
	}

	m_iNumSpheres = m_pSphereSystem->getSpheres().size();

	m_fRadius = 0.05f;
}

void SphereSystemSimulator::setupdemo1()
{
	//clear scene
	if (m_pSphereSystem == nullptr) {
		m_pSphereSystem = new SphereSystem(1);
	}
	else {
		delete m_pSphereSystem;
		m_pSphereSystem = new SphereSystem(1);
	}

	m_fRadius = 0.05f;
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
