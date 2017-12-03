#pragma once
#include "Simulator.h"

//this class defines a system of rigidbodies
//every rigidbody is defined by its position, orientation, velocity

//for torque
struct TorqueChar {
	//page 22
	Vec3 xi, fi;
};

//defines one rigidbody
struct Rigidbody{
	Vec3 m_boxCenter, m_boxSize, m_velocity, m_angularMomentum, m_angularVelocity, m_torque, m_force;
	Quat m_orientation;
	int m_imass;
	//hopefully inertia tensor
	XMMATRIX inertiaTensor;	
	//mass points producing toques -> m_torque is total torque
	std::vector<TorqueChar> m_pointsTorque;
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
	void setTotalTorque(int i, Vec3 torque);
	void setTotalForce(int i, Vec3 force);
	int addRigidBody(Vec3 position, Vec3 size, int mass);
		
private:
	//Attributes
	std::vector<Rigidbody> m_rigidbodySystem;
	int m_iNumRigidBodies;
	float m_fTotalMass;
};