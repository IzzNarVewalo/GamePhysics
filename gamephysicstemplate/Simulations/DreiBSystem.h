#pragma once
#include "Simulator.h"

struct Spring {
	float initialLength, currentLength;
	int point1, point2;
};
//for torque
struct TorqueChar {
	//page 22
	Vec3 xi, fi;
};

struct Box {
	Vec3 m_boxCenter, m_boxSize, m_linearVelocity, m_angularMomentum, m_angularVelocity, m_totalTorque, m_totalForce;
	Quat m_orientation;
	int m_imass;
	boolean m_bfixed;
	//hopefully inertia tensor
	Mat4 m_inertiaTensor;
	//mass points producing toques -> m_torque is total torque
	std::vector<TorqueChar> m_pointsTorque;
};

struct Ball {
	Vec3 ballCenter, velocity, force;
	float size;
	int mass;
	Box boundingBox;
};

class DreiBSystem
{
public:
	DreiBSystem();
	~DreiBSystem() {};

	//Getter & Setter
	std::vector<Box> getBoxWalls() {
		return m_boxWall;
	}
	std::vector<Ball> getBalls() {
		return m_balls;
	}
	int getTotalMass() {
		return m_iTotalMass;
	}
	int getNumBoxes() {
		return m_iNumBoxes;
	}
	int getNumBalls() {
		return m_iNumBalls;
	}


	void setNumBalls(int num) {
		m_iNumBalls = num;
	}

	void pushBackTorque(int i, Vec3 torque, Vec3 force) {
		TorqueChar t;
		t.fi = force;
		t.xi = torque;
		m_boxWall[i].m_pointsTorque.push_back(t);
	}

	void setTotalTorque(int i, Vec3 torque) {
		m_boxWall[i].m_totalTorque = torque;
	}
	void setTotalForce(int i, Vec3 force) {
		m_boxWall[i].m_totalForce = force;
	}
	void setCentralOfMassPosition(boolean ball, int i, Vec3 pos) {
		ball ? m_balls[i].ballCenter = pos : m_boxWall[i].m_boxCenter = pos;
	}
	void setLinearVelocity(boolean ball, int i, Vec3 vel) {
		ball ? m_balls[i].velocity = vel : m_boxWall[i].m_linearVelocity = vel;
	}
	void setRotation(int i, Quat rot) {
		m_boxWall[i].m_orientation = rot;
	}
	void setAngularVelocity(int i, Vec3 w) {
		m_boxWall[i].m_angularVelocity = w;
	}
	void setAngularMomentum(int i, Vec3 L) {
		m_boxWall[i].m_angularMomentum = L;
	}



	//fuegt kloetzchen zum turm hinzu
	int addBox(Vec3 position, Vec3 size, int mass);
	void reset();
	void buildBoxWall(int wallSize, float widthBox, float heightBox);

	int createBall(Vec3 pos, float size, int mass, Vec3 vel);

	Mat4 getTranslatMatOf(int i);
	Mat4 getRotMatOf(int i);
	Mat4 getScaleMatOf(int i);
	Mat4 calcTransformMatrixOf(int i);
	Spring m_spring;

private:

	std::vector<Box> m_boxWall;
	std::vector<Ball> m_balls;

	int m_iTotalMass = 0;
	int m_iNumBoxes, m_iNumBalls;

};