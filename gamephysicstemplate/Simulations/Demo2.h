#pragma once
#include "Simulator.h"

struct MassPoint {
	Vec3 position, velocity, force;
	boolean isFixed;
};

struct Spring {
	float initialLength, currentLength;
	int point1, point2;
};

class Demo2 {
public:
	Demo2();
	void resetScene();
	void setupScene();
	
	void setMass(float mass);
	void setStiffness(float stiffness);
	void setDampingFactor(float damping);
	void addSpring(int masspoint1, int masspoint2, float initialLength);
	int getNumberOfMassPoints();
	int getNumberOfSprings();
	Vec3 getPositionOfMassPoint(int index);
	Vec3 getVelocityOfMassPoint(int index);
	void applyExternalForce(Vec3 force);

private:
	// Data Attributes
	float m_fMass;
	float m_fStiffness;
	float m_fDamping;

	std::vector<MassPoint> m_points;
	std::vector<Spring> m_springs;
	int m_inumPoints, m_inumSprings;

	// UI Attributes
	Vec3 m_externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
};
