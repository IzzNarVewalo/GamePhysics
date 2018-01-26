#pragma once
#include "Simulator.h"

//Holzkloetzchen
struct TorqueChar {
	//page 22
	Vec3 xi, fi;
};

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
		return m_boxWall.size();
	}
	int getNumBalls() {
		return m_balls.size();
	}

	int addBox(Vec3 position, Vec3 size, int mass);
	void reset();
	void buildBoxWall(int wallSize,	float widthBox,	float heightBox);

	Mat4 getTranslatMatOf(int i);
	Mat4 getRotMatOf(int i);
	Mat4 getScaleMatOf(int i);
	Mat4 calcTransformMatrixOf(int i);

private:
	std::vector<Box> m_boxWall;
	std::vector<Ball> m_balls;
	int m_iTotalMass = 0;
};