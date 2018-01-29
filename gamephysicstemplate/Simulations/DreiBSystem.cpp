#include "DreiBSystem.h"

DreiBSystem::DreiBSystem() {

	m_iNumBalls = m_iNumBoxes = 0;
	//wallSize, widthBox, heightBox
	buildBoxWall(10, 0.1f, 0.05f);
	createBall(Vec3(0, -0.35f, -0.5f), 0.05f, 3, Vec3(0, 0,8));
	createBall(Vec3(0.2f, -0.35f, -0.5f), 0.05f, 3, Vec3(0, 0, 9));

	Spring spring;
	spring.initialLength = 0.7f;
	spring.point1 = createBall(Vec3(0, -0.3f, 0.2f), 0.05f, 3, Vec3(1, 1, -2.0f));
	spring.point2 = createBall(Vec3 (0, 0.5, 0), 0.05f, 10, Vec3(.0f));
	m_spring = spring;
}

void DreiBSystem::buildBoxWall(int wallSize, float widthBox, float heightBox) {
	float offset = 0;
	float xPosInit = -0.45f;
	float yPosInit = -0.475f;

	for (int i = 0; i < wallSize; i++) {
		for (int j = 0; j < wallSize - i; j++) {
			addBox(Vec3(xPosInit + j*widthBox + offset, yPosInit + i*heightBox, (j%2)*0.05f), Vec3(widthBox - 0.02f, heightBox, 0.1f), 10);
		}
		offset += widthBox / 2;
	}
}

int DreiBSystem::createBall(Vec3 pos, float size, int mass, Vec3 vel)
{
	Ball ball;
	ball.ballCenter = pos;
	ball.mass = mass;
	ball.size = size;
	ball.velocity = vel;
	ball.force = Vec3(0.0f);

	//bounding box attached to ball
	Box boundingBox;
	boundingBox.m_angularMomentum = boundingBox.m_angularVelocity = Vec3(.0f);
	boundingBox.m_boxCenter = pos;
	boundingBox.m_boxSize = size;
	boundingBox.m_imass = mass;
	boundingBox.m_orientation = Quat(0, 0, 0);
	boundingBox.m_linearVelocity = vel;
	//calculate inertia tensor in 3D
	float Ixx = mass * (2 * size * size);
	float Ixy = mass * (-size * size);
	boundingBox.m_inertiaTensor = XMMatrixSet(Ixx, Ixy, Ixy, .0f, Ixy, Ixx, Ixy, .0f, Ixy, Ixy, Ixx, .0f, .0f, .0f, .0f, 1.0f);

	ball.boundingBox = boundingBox;

	m_boxWall.push_back(boundingBox);
	++m_iNumBoxes;
	m_balls.push_back(ball);
	++m_iNumBalls;

	return m_iNumBalls - 1;
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
	box.m_inertiaTensor = XMMatrixSet(Ixx, Izy, Ixy, .0f, Izy, Iyy, Ixz, .0f, Ixy, Ixz, Izz, .0f, .0f, .0f, .0f, 1.0f);

	//angular momantum L
	box.m_angularMomentum = Vec3(.0f);

	//angular velocity w
	box.m_angularVelocity = Vec3(.0f);

	box.m_orientation = Quat(0, 0, 0);

	m_boxWall.push_back(box);
	++m_iNumBoxes;

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