#include "DreiBSystem.h"


DreiBSystem::DreiBSystem() {
	//wallSize, widthBox, heightBox
	buildBoxWall(10, 0.1f, 0.05f);	
}

void DreiBSystem::buildBoxWall(int wallSize, float widthBox, float heightBox) {
	float offset = 0;
	float xPosInit = -0.45f;
	float yPosInit = -0.475f;

	for (int i = 0; i < wallSize; i++) {
		for (int j = 0; j < wallSize - i; j++) {
			addBox(Vec3(xPosInit + j*widthBox + offset, yPosInit + i*heightBox, .0f), Vec3(widthBox - 0.02f, heightBox, 0.1f), 1);
		}
		offset += widthBox / 2;
	}
}

int DreiBSystem::addBox(Vec3 position, Vec3 size, int mass)
{
	m_iTotalMass += mass;

	Box box;
	box.m_boxCenter = position;
	box.m_boxSize = size;
	box.m_imass = mass;

	//calculate inertia tensor in 3D
	float Ixx = mass *(size.z * size.z + size.x * size.x);
	float Iyy = mass *(size.y * size.y + size.x * size.x);
	float Izz = mass *(size.y * size.y + size.z * size.z);

	float Ixy = mass *(-size.x*size.y);
	float Ixz = mass *(-size.x*size.z); //z 2, 1 y, 3 x
	float Izy = mass *(-size.y*size.z);
	box.inertiaTensor = XMMatrixSet(Ixx, Izy, Ixy, .0f, Izy, Iyy, Ixz, .0f, Ixy, Ixz, Izz, .0f, .0f, .0f, .0f, 1.0f);

	box.inertiaTensor = box.inertiaTensor.inverse();

	//angular momantum L
	box.m_angularMomentum = Vec3(.0f);

	//angular velocity w
	box.m_angularVelocity = Vec3(.0f);

	TorqueChar c;
	
	c.xi = Vec3(0.3f, 0.5f, 0.25f);
	c.fi = Vec3(1, 1, 0);
	box.m_pointsTorque.push_back(c);
	
	m_boxWall.push_back(box);
	return m_boxWall.size() - 1;
}

void DreiBSystem::reset()
{
	addBox(Vec3(.0f, .0f, .0f), Vec3(0.1f, 0.1f, 0.1f), 1);
}

Mat4 DreiBSystem::getTranslatMatOf(int i)
{
	Mat4 temp;
	temp.initTranslation(m_boxWall[i].m_boxCenter.x, m_boxWall[i].m_boxCenter.y, m_boxWall[i].m_boxCenter.z);
	return temp;
}

Mat4 DreiBSystem::getRotMatOf(int i)
{
	return m_boxWall[i].m_orientation.getRotMat();
}

Mat4 DreiBSystem::getScaleMatOf(int i)
{
	Mat4 temp;
	temp.initScaling(m_boxWall[i].m_boxSize.x, m_boxWall[i].m_boxSize.y, m_boxWall[i].m_boxSize.z);
	return temp;
}

Mat4 DreiBSystem::calcTransformMatrixOf(int i)
{
	return getScaleMatOf(i) * getRotMatOf(i) * getTranslatMatOf(i);
}
