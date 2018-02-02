#include "DreiB.h"

int m_ifactor = 1; //massenfaktor
boolean collided = false;

DreiB::DreiB()
{
	m_pDreiBSystem = new DreiBSystem();

	//m_particleColSys = new ParticleCollisionSystem();
	//m_iTestCase = 0; //wegen main startet immer bei 0
	m_fexternalForce = Vec3(0.0f);
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
	int numTemp = m_pDreiBSystem->getNumObjects();
	float red = 0;
	float green = 0;
	float blue = 0;
	float factor = 1.0f / numTemp * 4;

	for (int i = 0; i < numTemp; i++) {
		if (!m_pDreiBSystem->getObjects()[i].isBall) {
			if (i % 3 == 0) {
				DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.8f, 0.2f, 0.5f));
			}
			else if (i % 2 == 0) {
				DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.2f, 0.8f, 0.5f));
			}
			else {
				DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 1, 0), 2000.0f, Vec3(0.5f, 0.2f, 0.8f));
			}
			DUC->drawRigidBody(m_pDreiBSystem->calcTransformMatrixOf(i));
		}
		else {
			DUC->setUpLighting(Vec3(0, 0, 0), Vec3(1, 0, 0), 1000.0f, Vec3(1.0f, 0.0f, 0.0f));
			DUC->drawSphere(m_pDreiBSystem->getObjects()[i].m_boxCenter, m_pDreiBSystem->getObjects()[i].m_boxSize);
		}
	}

	//springs
	DUC->beginLine();
	DUC->drawLine(m_pDreiBSystem->getObjects()[m_pDreiBSystem->m_spring.point1].m_boxCenter, Vec3(0, 1, 0), m_pDreiBSystem->getObjects()[m_pDreiBSystem->m_spring.point2].m_boxCenter, Vec3(0, 0, 1));
	DUC->endLine();
}

void DreiB::externalForcesCalculations(float timeElapsed)
{
	//gravity
	m_fexternalForce = Vec3(0, -9.81f, 0) * 10.0f;

	//iteriere durch boxen, baelle brauchen keine torques (?)
	std::vector<Box> temp = m_pDreiBSystem->getObjects();
	Vec3 tempTotalTorque;
	Vec3 tempTotalForce;
	int tempTorqueNum = 0;

	for (int i = 0; i < m_pDreiBSystem->getNumObjects(); i++) {
		if (!temp[i].m_bfixed) {

			tempTotalTorque = Vec3(.0f);
			tempTotalForce = Vec3(.0f);

			tempTorqueNum = temp[i].m_pointsTorque.size();

			for (int j = 0; j < tempTorqueNum; j++) {
				Vec3 xi = (temp[i].m_pointsTorque[j].xi - temp[i].m_boxCenter);
				tempTotalTorque += cross(xi, temp[i].m_pointsTorque[j].fi);
				tempTotalForce += temp[i].m_pointsTorque[j].fi;
			}

			//nur wer torque hat und ball ist bekommt gravity
			if (temp[i].isBall || (tempTorqueNum > 0 || collided)) {
				tempTotalForce = m_fexternalForce;
			}
			else {
				tempTotalForce = Vec3(0.0f);
			}
			//tempTotalForce = m_fexternalForce;

			//set total Torque
			m_pDreiBSystem->setTotalTorque(i, tempTotalTorque);
			//set total Force
			m_pDreiBSystem->setTotalForce(i, tempTotalForce);
		}
	}
}

void DreiB::simulateTimestep(float timeStep)
{
	std::vector<Box> temp = m_pDreiBSystem->getObjects();
	int num = m_pDreiBSystem->getNumObjects();

	//wall and balls
	for (int i = 0; i < num; i++) {
		if (temp[i].m_bfixed) { continue; }
		//x position
		m_pDreiBSystem->setCentralOfMassPosition(i, (temp[i].m_boxCenter + timeStep * temp[i].m_linearVelocity));

		//rest on the floor
		if (temp[i].m_boxCenter.y <= -0.475f && !temp[i].isBall)
			temp[i].m_linearVelocity.y = 0.0f;

		//v linear velocity		
		m_pDreiBSystem->setLinearVelocity(i, (temp[i].m_linearVelocity + timeStep * (temp[i].m_totalForce)
			/ (m_pDreiBSystem->getTotalMassOf(i))));

		if (!temp[i].isBall) {
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
	}
	temp = m_pDreiBSystem->getObjects();

	float fixed = m_pDreiBSystem->m_spring.point2;
	float wreckingBall = m_pDreiBSystem->m_spring.point1;
	//instead of spring, a rope -> constraint
	//                       fixed                                            wrecking ball
	Vec3 vectorToFixed = temp[fixed].m_boxCenter - temp[wreckingBall].m_boxCenter;
	float currentSpringLength = norm(vectorToFixed);
	float deltaLength = (currentSpringLength - m_pDreiBSystem->m_spring.initialLength);
	Vec3 deltaPos = deltaLength*(vectorToFixed / currentSpringLength);
	m_pDreiBSystem->setCentralOfMassPosition(wreckingBall, deltaPos + temp[wreckingBall].m_boxCenter);

	//velocity -> not slowing down
	m_pDreiBSystem->setLinearVelocity(wreckingBall, temp[wreckingBall].m_linearVelocity -
		(dot(temp[wreckingBall].m_linearVelocity, vectorToFixed) * vectorToFixed));

	temp = m_pDreiBSystem->getObjects();

	//check for collisions
	for (int i = 0; i < num; i++) {
		for (int j = 55; j < 59; i != j, j++) {

			if (temp[i].m_bfixed)
				continue;

			GamePhysics::Mat4 AM = m_pDreiBSystem->calcTransformMatrixOf(j);
			GamePhysics::Mat4 BM = m_pDreiBSystem->calcTransformMatrixOf(i);

			CollisionInfo simpletest = checkCollisionSAT(AM, BM);

			if (simpletest.isValid) {
				std::printf("Collision\n");
				//auf welcher flaeche welches koerpers steht die normale?
				//wenn n positiv, dann steht es auf B, sonst auf A

				if (dot(temp[j].m_linearVelocity - temp[i].m_linearVelocity, simpletest.normalWorld) >= 0)
					continue;

				collided = true;

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

				//Set torque -> not working
				//m_pDreiBSystem->pushBackTorque(i, simpletest.collisionPointWorld, Vec3(2, 4, 0));
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
		resetScene();
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
	m_pDreiBSystem->setLinearVelocity(i, vel);
}

