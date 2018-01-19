#pragma once
#include "Simulator.h"

//Holzkloetzchen
struct Box {
	//position = boxCenter
	Vec3 m_boxCenter, m_boxSize, m_linearVelocity, m_angularMomentum, m_angularVelocity, m_totalTorque, m_totalForce;
	Quat m_orientation;
	int m_imass;
	//hopefully inertia tensor
	Mat4 inertiaTensor;
	//mass points producing toques -> m_torque is total torque
	std::vector<TorqueChar> m_pointsTorque;
};

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

	int addBox(Vec3 pos, Vec3 size, int mass);

	Mat4 getTranslatMatOf(int i);
	Mat4 getRotMatOf(int i);
	Mat4 getScaleMatOf(int i);
	Mat4 calcTransformMatrixOf(int i);

private:
	std::vector<Box> m_boxWall;
	std::vector<Ball> m_balls;
	int m_itotalMass;
	int m_iNumBoxes, m_iNumBalls;
};