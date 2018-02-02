#include "DreiB.h"

int m_ifactor = 1; //massenfaktor

DreiB::DreiB()
{
	m_iTestCase = 0; //wegen main startet immer bei 0
	m_pDreiBSystem = new DreiBSystem(m_iTestCase);

	//m_particleColSys = new ParticleCollisionSystem();
	
	m_fextraForce = -9.81f;
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


	/*if (m_iTestCase == 0)
		DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.8f, 0.2f, 0.5f));
	else
		DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 0, 1), 1000.0f, Vec3(0.5f, 0.0f, 0.5f));*/

	for (int i = 0; i < numTemp; i++) {
		

		if (i % 3 == 0) {
			DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.8f, 0.2f, 0.5f));
		}
		else if (i % 2 == 0){
			DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.2f, 0.8f, 0.5f));
		}
		else {
			DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.5f, 0.2f, 0.8f));
		}

		
		DUC->drawRigidBody(m_pDreiBSystem->calcTransformMatrixOf(i));
	}

	//balls
	for (int i = 0; i < m_pDreiBSystem->getNumBalls(); i++) {
		DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 0, 0), 1000.0f, Vec3(1.0f, 0.0f, 0.0f));
		DUC->drawSphere(m_pDreiBSystem->getBalls()[i].ballCenter, m_pDreiBSystem->getBalls()[i].size);
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
		tempTorqueNum = temp[i].m_pDreiBSystem->getBallsTorque.size();

		for (int j = 0; j < tempTorqueNum; j++) {
			Vec3 xi = (temp[i].m_pDreiBSystem->getBallsTorque[j].xi - temp[i].m_boxCenter);
			tempTotalTorque += cross(xi, temp[i].m_pDreiBSystem->getBallsTorque[j].fi);
			tempTotalForce += temp[i].m_pDreiBSystem->getBallsTorque[j].fi;
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
	midpointStep(timeStep);
}

void DreiB::midpointStep(float timeStep)
{
	std::vector<Ball> tmpBalls = m_pDreiBSystem->getBalls();

	int numBalls = m_pDreiBSystem->getNumBalls();

	for (int i = 0; i < numBalls; i++) {
		
		//xTilde
		tmpBalls[i].ballCenter = m_pDreiBSystem->getBalls()[i].ballCenter + (timeStep / 2) * m_pDreiBSystem->getBalls()[i].velocity;

		//vel at xTilde
		tmpBalls[i].velocity = m_pDreiBSystem->getBalls()[i].velocity + (timeStep / 2) * m_pDreiBSystem->getBalls()[i].force / m_pDreiBSystem->getBalls()[i].mass;

		//new ballCenter
		m_pDreiBSystem->setCentralOfMassPosition(true, i, m_pDreiBSystem->getBalls()[i].ballCenter + timeStep * tmpBalls[i].velocity);

		//clear forces
		for (int i = 0; i < numBalls; i++) {
				
			m_pDreiBSystem->setForce(i, Vec3(0, 1, 0) * m_fextraForce);
				
		}
		
	}

	////acc at xTilde, time t+h/2
	////compute internal force for all points in the system
	//for (int i = 0; i < m_inumSprings; i++) {
	//	//current length of spring
	//	m_springs[i].currentLength = norm(tmpBalls[m_springs[i].point1].ballCenter - tmpBalls[m_springs[i].point2].ballCenter);

	//	Vec3 tmp = m_fStiffness * (m_springs[i].currentLength - m_springs[i].initialLength);

	//	Vec3 force = tmp * (tmpBalls[m_springs[i].point1].ballCenter - tmpBalls[m_springs[i].point2].ballCenter) / (m_springs[i].currentLength);
	//	tmpBalls[m_springs[i].point1].force -= force;

	//	force = tmp * (tmpBalls[m_springs[i].point2].ballCenter - tmpBalls[m_springs[i].point1].ballCenter) / (m_springs[i].currentLength);
	//	tmpBalls[m_springs[i].point2].force -= force;
	//}

	//new velocity
	for (int i = 0; i < numBalls; i++)
	{
		
		//bounding with floor
		if (m_pDreiBSystem->getBalls()[i].ballCenter.y <= -0.5f)
			m_pDreiBSystem->getBalls()[i].velocity.y *= (-0.99f);
		//bounding with walls
		if ((m_pDreiBSystem->getBalls()[i].ballCenter.x <= -0.5f || m_pDreiBSystem->getBalls()[i].ballCenter.x > 0.5f))
			m_pDreiBSystem->getBalls()[i].velocity.x *= (-0.90f);
		//divide the force by mass of the point to get the acceleration 
		m_pDreiBSystem->setLinearVelocity(true, i, m_pDreiBSystem->getBalls()[i].velocity + timeStep * tmpBalls[i].force / m_pDreiBSystem->getBalls()[i].mass);
		
	}

}

void DreiB::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;

	switch (m_iTestCase) {
	case 0:
		std::cout << "Holzkloetzchenspiel !\n";
		m_pDreiBSystem->reset();
		m_pDreiBSystem = new DreiBSystem(m_iTestCase);
		m_pDreiBSystem->setupCase0();
		break;
	case 1:
		std::cout << "2.0Holzkloetzchenspiel !\n";
		m_pDreiBSystem->reset();
		m_pDreiBSystem = new DreiBSystem(m_iTestCase);
		m_pDreiBSystem->setupCase1();
		break;
	case 2:
		std::cout << "Teilchenkoll !\n";
		m_pDreiBSystem = new DreiBSystem(m_iTestCase);
		m_pDreiBSystem->setupCase2();
		break;
	default:break;
	}
}

void DreiB::addBox(Vec3 ballCenter, Vec3 size, int mass)
{
	m_pDreiBSystem->addBox(ballCenter, size, mass);
}

