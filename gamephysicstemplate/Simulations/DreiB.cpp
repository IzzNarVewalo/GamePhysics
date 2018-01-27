#include "DreiB.h"

int m_ifactor = 1; //massenfaktor

DreiB::DreiB()
{
	m_pDreiBSystem = new DreiBSystem();

	//m_particleColSys = new ParticleCollisionSystem();
	//m_iTestCase = 0; //wegen main startet immer bei 0
	m_fextraForce = 0.0f;
}

DreiB::~DreiB()
{
	delete m_pDreiBSystem;
}

void DreiB::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;

	//int tmpnumballs = m_pDreiBSystem->getNumBalls();
	int tmpnumballs = 0;
	DUC->update(0.1f);	
	switch (m_iTestCase)
	{
	case 0:
		//masse erhoehen, kraft erhoehen
		TwAddButton(DUC->g_pTweakBar, "double mass", [](void * s) { m_ifactor *= 2; }, nullptr, "");
		TwAddVarRW(DUC->g_pTweakBar, "extra force factor", TW_TYPE_FLOAT, &m_fextraForce, "min=1 step=0.1 max=5");
		break;
	case 1:
		//baelleanzahl aendern
		
		//TwAddVarRW(DUC->g_pTweakBar, "number balls", TW_TYPE_INT32, &tmpnumballs, "min=1");
		//m_pDreiBSystem->setNumBalls(tmpnumballs);
		//evtl kleidungsstuek bewerfen-> dann noch zwischen verschiedenen objekten und anzahl von baellen unterscheiden, a
		//nstatt bei demos es z uunterscheiden
		
	case 2:
		break;
	default:break;
	}
}

void DreiB::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	int numTemp = m_pDreiBSystem->getNumBoxes();

	if (m_iTestCase == 0)
		DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.8f, 0.2f, 0.5f));
	else
		DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 0, 1), 1000.0f, Vec3(0.5f, 0.0f, 0.5f));

	for (int i = 0; i < numTemp; i++) {
		DUC->drawRigidBody(m_pDreiBSystem->calcTransformMatrixOf(i));
	}

	/*for (int i = 0; i < m_particleColSys->getParticles().size(); i++) {
		DUC->setUpLighting(Vec3(1, 0, 0), Vec3(1, 0, 0), 22.0f, Vec3(1, 1, 0));
		DUC->drawSphere(m_particleColSys->getParticles()[i].pos, m_particleColSys->getParticles()[i].size);
	}*/
}

void DreiB::externalForcesCalculations(float timeElapsed)
{
	/*
	//iteriere durch boxen, baelle brauchen keine torques (?)
	std::vector<Box> temp = m_pDreiBSystem->getBoxWalls();
	Vec3 tempTotalTorque;
	Vec3 tempTotalForce;
	int tempTorqueNum = 0;

	for (int i = 0; i < m_pDreiBSystem->getNumBoxes(); i++) {

		tempTotalTorque = Vec3(.0f);
		tempTotalForce = Vec3(.0f);
		tempTorqueNum = temp[i].m_pointsTorque.size();

		for (int j = 0; j < tempTorqueNum; j++) {
			Vec3 xi = (temp[i].m_pointsTorque[j].xi - temp[i].m_boxCenter);
			tempTotalTorque += cross(xi, temp[i].m_pointsTorque[j].fi);
			tempTotalForce += temp[i].m_pointsTorque[j].fi;
		}

		//set total Torque
		m_pDreiBSystem->setTotalTorque(i, tempTotalTorque);
		//set total Force
		m_pDreiBSystem->setTotalForce(i, tempTotalForce * m_fextraForce);
	}*/
}
//benutzen Midpoint, wei stabil
void DreiB::simulateTimestep(float timeStep)
{

}

void DreiB::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;

	switch (m_iTestCase) {
	case 0:
		std::cout << "Holzkloetzchenspiel !\n";
		break;
	case 1:
		std::cout << "2.0Holzkloetzchenspiel !\n";
		break;
	case 2:
		std::cout << "Teilchenkoll !\n";
		break;
	default:break;
	}
}

void DreiB::addBox(Vec3 position, Vec3 size, int mass)
{
	m_pDreiBSystem->addBox(position, size, mass);
}

