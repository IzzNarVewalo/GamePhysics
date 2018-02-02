#include "DreiBSystem.h"

DreiBSystem::DreiBSystem() {

	resetScene();
}

void DreiBSystem::buildBoxWall(int wallSize, float widthBox, float heightBox) {
	float offset = 0;
	float xPosInit = -0.45f;
	float yPosInit = -0.475f;

	for (int i = 0; i < wallSize; i++) {
		for (int j = 0; j < wallSize - i; j++) {
			addBox(Vec3(xPosInit + j*widthBox + offset, yPosInit + i*heightBox + 0.02f, (j%2)*0.05f), Vec3(widthBox - 0.02f, heightBox, 0.1f), 10);
		}
		offset += widthBox / 2;
	}
}

int DreiBSystem::addBall(Vec3 pos, float size, int mass, Vec3 vel, boolean fixed = false, boolean spring = false)
{
	Box ball;
	ball.m_angularMomentum = ball.m_angularVelocity = Vec3(.0f);
	ball.m_boxCenter = pos;
	ball.m_boxSize = size;
	ball.m_imass = mass;
	ball.isBall = true;
	ball.m_bfixed = fixed;
	ball.isSpring = spring;
	//intern for better collision
	ball.m_orientation = Quat(0, M_PI_4, M_PI_4);
	ball.m_linearVelocity = vel;
	//calculate inertia tensor in 3D
	float Ixx = mass * (2 * size * size);
	float Ixy = mass * (-size * size);
	ball.m_inertiaTensor = XMMatrixSet(Ixx, Ixy, Ixy, .0f, Ixy, Ixx, Ixy, .0f, Ixy, Ixy, Ixx, .0f, .0f, .0f, .0f, 1.0f);
	
	ball.m_totalForce = Vec3(0.0f);

	m_objects.push_back(ball);
	++m_iNumBoxes;

	return m_iNumBoxes - 1;
}

int DreiBSystem::addBox(Vec3 position, Vec3 size, int mass)
{
	Box box;
	box.m_boxCenter = position;
	box.m_boxSize = size;
	box.m_imass = mass;
	box.m_bfixed = false;
	box.isBall = false;
	box.isSpring = false;

	//calculate inertia tensor in 3D
	float Ixx = mass *(size.z * size.z + size.x * size.x);
	float Iyy = mass *(size.y * size.y + size.x * size.x);
	float Izz = mass *(size.y * size.y + size.z * size.z);

	float Ixy = mass *(-size.x*size.y);
	float Ixz = mass *(-size.x*size.z); //z 2, 1 y, 3 x
	float Izy = mass *(-size.y*size.z);
	box.m_inertiaTensor = XMMatrixSet(Ixx, Izy, Ixy, .0f, Izy, Iyy, Ixz, .0f, Ixy, Ixz, Izz, .0f, .0f, .0f, .0f, 1.0f);

	//angular momantum L
	box.m_angularMomentum = Vec3(.0f);

	//angular velocity w
	box.m_angularVelocity = Vec3(.0f);

	box.m_orientation = Quat(0, 0, 0);

	//force die gegen gravity wirkt
	box.m_totalForce = Vec3(0.0f);
	box.m_linearVelocity = Vec3(.0f);

	m_objects.push_back(box);
	++m_iNumBoxes;

	return m_objects.size() - 1;
}

void DreiBSystem::reset()
{
	addBox(Vec3(.0f, .0f, .0f), Vec3(0.1f, 0.1f, 0.1f), 1);
}

Mat4 DreiBSystem::getTranslatMatOf(int i)
{
	Mat4 temp;
	temp.initTranslation(m_objects[i].m_boxCenter.x, m_objects[i].m_boxCenter.y, m_objects[i].m_boxCenter.z);
	return temp;
}

Mat4 DreiBSystem::getRotMatOf(int i)
{
	return m_objects[i].m_orientation.getRotMat();
}

Mat4 DreiBSystem::getScaleMatOf(int i)
{
	Mat4 temp;
	temp.initScaling(m_objects[i].m_boxSize.x, m_objects[i].m_boxSize.y, m_objects[i].m_boxSize.z);
	return temp;
}

Mat4 DreiBSystem::calcTransformMatrixOf(int i)
{
	return getScaleMatOf(i) * getRotMatOf(i) * getTranslatMatOf(i);
}

void DreiBSystem::resetScene()
{
	//delete everything
	m_iNumBoxes = 0;
	m_objects.clear();

	//make new scene
	buildBoxWall(10, 0.1f, 0.05f);
	addBall(Vec3(-0.2f, -0.35f, -0.5f), 0.05f, 300, Vec3(0, 0, 8));
	addBall(Vec3(0.2f, -0.35f, -0.5f), 0.05f, 200, Vec3(0, 0, 9));

	Spring spring;
	spring.initialLength = 0.7f;
	spring.point1 = addBall(Vec3(0.2f, 0.5f, 0.2f), 0.05f, 300, Vec3(0.01f, -10.0f, 0.0f), false, true);
	spring.point2 = addBall(Vec3(0, 0.5, 0), 0.05f, 10, Vec3(.0f), true, true);
	m_spring = spring;
}
