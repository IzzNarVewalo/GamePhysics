#pragma once
#include "Simulator.h"

//this class defines a system of rigidbodies
//every rigidbody is defined by its position, orientation, velocity

//defines one rigidbody
struct Rigidbody{
	Vec3 m_boxCenter, m_boxSize, m_velocity;
	Quat m_orientation;
	int m_imass;
};

class RigidBodySystem {
public:
	//Constructors
	RigidBodySystem();

	//Functions
	std::vector<Rigidbody> getRigidBodySystem();
	int getNumRigidBodies();
	void decNumRigidBodies();
	void incNumRigidBodies();
	
private:
	//Attributes
	std::vector<Rigidbody> m_rigidbodySystem;
	int m_iNumRigidBodies;

};