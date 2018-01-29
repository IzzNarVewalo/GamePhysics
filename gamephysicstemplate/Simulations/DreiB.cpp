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
	float red = 0;
	float green = 0;
	float blue = 0;
	float factor = 1.0f / numTemp * 4;

	/*if (m_iTestCase == 0)
		DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.8f, 0.2f, 0.5f));
	else
		DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 0, 1), 1000.0f, Vec3(0.5f, 0.0f, 0.5f));*/

	for (int i = 0; i < numTemp - 3; i++) {
		/*std::cout << "factor: " << factor << endl;
		if (red <= 1 && green <= 0) {
			red += factor;

			std::cout << "erstes if" << red << endl;
		}
		else if (green <= 1){
			green += factor;
			red -= factor;
			std::cout << "zweites if !\n";
		}
		else {
			blue += factor;
			green -= factor;
		}*/

		if (i % 3 == 0) {
			DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.8f, 0.2f, 0.5f));
		}
		else if (i % 2 == 0) {
			DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.2f, 0.8f, 0.5f));
		}
		else {
			DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.5f, 0.2f, 0.8f));
		}

		//DUC->setUpLighting(Vec3(0, 0, 0), Vec3(0.5f, 0.5f, 0.5f), 2000.0f, Vec3(red, green, blue));
		DUC->drawRigidBody(m_pDreiBSystem->calcTransformMatrixOf(i));
	}

	//balls
	for (int i = 0; i < m_pDreiBSystem->getNumBalls() - 1; i++) {
		DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 0, 0), 1000.0f, Vec3(1.0f, 0.0f, 0.0f));
		DUC->drawSphere(m_pDreiBSystem->getBalls()[i].ballCenter, m_pDreiBSystem->getBalls()[i].size);
	}

	DUC->beginLine();
	DUC->drawLine(m_pDreiBSystem->getBalls()[m_pDreiBSystem->m_spring.point1].ballCenter, Vec3(0, 1, 0), m_pDreiBSystem->getBalls()[m_pDreiBSystem->m_spring.point2].ballCenter, Vec3(0, 0, 1));
	DUC->endLine();

	/*for (int i = 0; i < m_particleColSys->getParticles().size(); i++) {
		DUC->setUpLighting(Vec3(1, 0, 0), Vec3(1, 0, 0), 22.0f, Vec3(1, 1, 0));
		DUC->drawSphere(m_particleColSys->getParticles()[i].pos, m_particleColSys->getParticles()[i].size);
	}*/
}

void DreiB::externalForcesCalculations(float timeElapsed)
{
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
	}
}

void DreiB::simulateTimestep(float timeStep)
{
	std::vector<Box> temp = m_pDreiBSystem->getBoxWalls();
	int num = m_pDreiBSystem->getNumBoxes();

	//wall
	for (int i = 0; i < num; i++) {

		if (temp[i].m_linearVelocity.x != (.0f)) {
			temp[i].m_totalForce.y += -9.81f;
		}

		//x position
		m_pDreiBSystem->setCentralOfMassPosition(false, i, (temp[i].m_boxCenter + timeStep * temp[i].m_linearVelocity));
		//v linear velocity
		if (i == 55 || i == 56)
			m_pDreiBSystem->setLinearVelocity(false, i, (temp[i].m_linearVelocity + timeStep * (temp[i].m_totalForce + Vec3(0, -40.0f, 0)) / (m_pDreiBSystem->getTotalMass())));

		else
			m_pDreiBSystem->setLinearVelocity(false, i, (temp[i].m_linearVelocity + timeStep * (temp[i].m_totalForce) / (m_pDreiBSystem->getTotalMass())));
		//r see page 58
		Quat orient = temp[i].m_orientation + (timeStep / 2)* Quat(temp[i].m_angularVelocity.x, temp[i].m_angularVelocity.y, temp[i].m_angularVelocity.z, 1.0f) * temp[i].m_orientation;
		setOrientationOf(i, (orient.unit()));
		//L angular momentum page 56		
		m_pDreiBSystem->setAngularMomentum(i, temp[i].m_angularMomentum + timeStep * temp[i].m_totalTorque);

		//w angular velocity
		Mat4 tr = m_pDreiBSystem->getRotMatOf(i);
		tr.transpose();

		//calculate the inverse inertia tensor
		temp[i].m_inertiaTensor = m_pDreiBSystem->getRotMatOf(i) * temp[i].m_inertiaTensor * tr;
		m_pDreiBSystem->setAngularVelocity(i, temp[i].m_inertiaTensor.transformVector(temp[i].m_angularMomentum));
	}

	std::vector<Ball> tmp = m_pDreiBSystem->getBalls();
	int numb = m_pDreiBSystem->getNumBalls() - 2;
	//balls
	for (int i = 0; i < numb; i++) {
		//x position
		m_pDreiBSystem->setCentralOfMassPosition(true, i, (tmp[i].ballCenter + timeStep * tmp[i].velocity));
		//v linear velocity
		m_pDreiBSystem->setLinearVelocity(true, i, (tmp[i].velocity + timeStep * (tmp[i].force + Vec3(0, -40, 0)) / tmp[i].mass));
	}

	//spring
	//accelerate in -y-direction
	tmp[2].force = Vec3(0, -9.81f, 0);

	//compute internal force for every point; acceleration depends only on elastic forces
	for (int i = 2; i < 4; i++) {
		//current length of spring
		m_pDreiBSystem->m_spring.currentLength = norm(tmp[m_pDreiBSystem->m_spring.point1].ballCenter - tmp[m_pDreiBSystem->m_spring.point2].ballCenter);

		Vec3 tmps = 70 * (m_pDreiBSystem->m_spring.currentLength - m_pDreiBSystem->m_spring.initialLength);

		Vec3 force = tmps * (tmp[m_pDreiBSystem->m_spring.point1].ballCenter - tmp[m_pDreiBSystem->m_spring.point2].ballCenter) / (m_pDreiBSystem->m_spring.currentLength);
		tmp[m_pDreiBSystem->m_spring.point1].force -= force;

		force = tmps * (tmp[m_pDreiBSystem->m_spring.point2].ballCenter - tmp[m_pDreiBSystem->m_spring.point1].ballCenter) / (m_pDreiBSystem->m_spring.currentLength);
		tmp[m_pDreiBSystem->m_spring.point2].force -= force;
	}

	m_pDreiBSystem->setLinearVelocity(true, 2, tmp[2].velocity + timeStep * tmp[2].force / 3);
	m_pDreiBSystem->setLinearVelocity(false, 57, tmp[2].velocity + timeStep * tmp[2].force / 3);
	m_pDreiBSystem->setCentralOfMassPosition(true, 2, m_pDreiBSystem->getBalls()[2].ballCenter + timeStep * m_pDreiBSystem->getBalls()[2].velocity);
	m_pDreiBSystem->setCentralOfMassPosition(false, 57, m_pDreiBSystem->getBalls()[2].ballCenter + timeStep * m_pDreiBSystem->getBalls()[2].velocity);
	
	temp = m_pDreiBSystem->getBoxWalls();

	//check for collisions
	for (int i = 0; i < num - 1; i++) {
		for (int j = 55; j < 58; j++) {

			GamePhysics::Mat4 AM = m_pDreiBSystem->calcTransformMatrixOf(j);
			GamePhysics::Mat4 BM = m_pDreiBSystem->calcTransformMatrixOf(i);

			CollisionInfo simpletest = checkCollisionSAT(AM, BM);

			if (simpletest.isValid) {
				std::printf("Collision\n");
				//auf welcher flaeche welches koerpers steht die normale?
				//wenn n positiv, dann steht es auf B, sonst auf A

				if (dot(temp[j].m_linearVelocity - temp[i].m_linearVelocity, simpletest.normalWorld) >= 0)
					return;

				if (temp[i].m_pointsTorque.empty()) {
					m_pDreiBSystem->pushBackTorque(i, simpletest.collisionPointWorld, Vec3(2, 4, 0));
				}

				Vec3 deltaVel = (temp[j].m_linearVelocity - temp[i].m_linearVelocity);

				//calculate impulse J
				//they should stop flying into each other
				//bouncies: c=1 fully elasitc c=0 plastic
				float c = 1;
				float J = dot(-(1 + c) * deltaVel, simpletest.normalWorld);
				float nenner;
				float massterm = 1.0f / temp[i].m_imass + 1.0f / temp[j].m_imass;
				Vec3 b = (cross(temp[i].m_inertiaTensor.transformVector(cross(temp[i].m_boxCenter, simpletest.normalWorld)), temp[i].m_boxCenter));
				Vec3 a = (cross(temp[j].m_inertiaTensor.transformVector(cross(temp[j].m_boxCenter, simpletest.normalWorld)), temp[j].m_boxCenter));
				nenner = massterm + (dot((a + b), simpletest.normalWorld));
				J = J / nenner;

				//velocity update
				setVelocityOf(j, (temp[j].m_linearVelocity + J * (simpletest.normalWorld / temp[j].m_imass)));
				setVelocityOf(i, (temp[i].m_linearVelocity - J * (simpletest.normalWorld / temp[i].m_imass)));

				m_pDreiBSystem->setAngularMomentum(j, temp[j].m_angularMomentum + 10 * (cross(temp[j].m_boxCenter, J*simpletest.normalWorld)));
				m_pDreiBSystem->setAngularMomentum(i, temp[i].m_angularMomentum - 10 * (cross(temp[i].m_boxCenter, J*simpletest.normalWorld)));
			}
		}
	}
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

void DreiB::setOrientationOf(int i, Quat orient)
{
	m_pDreiBSystem->setRotation(i, orient);
}

void DreiB::setVelocityOf(int i, Vec3 vel)
{
	m_pDreiBSystem->setLinearVelocity(false, i, vel);
}

