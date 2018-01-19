#pragma once
#include "Simulator.h"

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

//for torque
struct TorqueChar {
	//page 22
	Vec3 xi, fi;
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
	~DreiBSystem();

	//Getter & Setter
	std::vector<Box> getBoxWalls() {
		return m_boxWall;
	}
	std::vector<Ball> getBalls() {
		return m_balls;
	}
	int getTotalMass() {
		return m_itotalMass;
	}
	int getNumBoxes() {
		return m_iNumBoxes;
	}
	int getNumBalls() {
		return m_iNumBalls;
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


private:

	std::vector<Box> m_boxWall;
	std::vector<Ball> m_balls;

	int m_itotalMass;
	int m_iNumBoxes, m_iNumBalls;

};