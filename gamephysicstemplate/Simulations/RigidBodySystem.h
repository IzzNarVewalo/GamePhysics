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
	boolean fixed;
	//hopefully inertia tensor
	Mat4 inertiaTensor, inert;	
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
	int getTotalMass();
	void decNumRigidBodies();
	void incNumRigidBodies();
	void setTotalTorque(int i, Vec3 torque);
	void setTotalForce(int i, Vec3 force);
	void setCentralOfMassPosition(int i, Vec3 pos);
	void setCentralOfMassVelocity(int i, Vec3 vel);
	void setRotation(int i, Quat rot);
	void setAngularVelocity(int i, Vec3 w);
	void setAngularMomentum(int i, Vec3 L);
	int addRigidBody(Vec3 position, Vec3 size, int mass, int test);

	Mat4 getTranslatMatOf(int i);
	Mat4 getRotMatOf(int i);	
	Mat4 getScaleMatOf(int i);
	Mat4 calcTransformMatrixOf(int i);
	Vec3 getXiOf(int i, int j);

	//setze rigidbody auf pos (0,0,0)
	void reset();
	//setze alle werte auf (0,0,0) 
	void reset2(int test);
	//loesche die ganze szene
	void reset3();
		
private:
	//Attributes
	std::vector<Rigidbody> m_rigidbodySystem;
	int m_iNumRigidBodies;
	int m_fTotalMass;
};