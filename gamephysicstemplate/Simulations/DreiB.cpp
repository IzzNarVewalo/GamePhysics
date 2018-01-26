#include "DreiB.h"

DreiB::DreiB() : Simulator()
{
	m_pDreiBSystem = new DreiBSystem();
}

DreiB::~DreiB() 
{
	delete m_pDreiBSystem;
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
}

void DreiB::addBox(Vec3 position, Vec3 size, int mass)
{
	m_pDreiBSystem->addBox(position, size, mass);
}


