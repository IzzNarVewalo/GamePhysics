#ifndef SPHSYSTEMSIMULATOR_h
#define SPHSYSTEMSIMULATOR_h
#include "Simulator.h"
#include "SphereSystem.h" // add your sphere system header file

#define NAIVEACC 0
#define GRIDACC 1

//for integration
#define MIDPOINT 0
#define LEAPFROG 1

class SphereSystemSimulator:public Simulator{
public:
	// Construtors
	SphereSystemSimulator();
	// Functions
	const char * getTestCasesStr();
	const char * getIntegCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void externalForcesCalculations(float timeElapsed); 
	void leapfrogStep(float timeStep); 
	void leapfrogStepDemo2(float timeStep);
	void simulateTimestep(float timeStep);
	void setupdemo2();
	void setupdemo1();
	void resetForce();
	void onClick(int x, int y);
	void onMouse(int x, int y);

	void setMass(float mass);
	void setDampingFactor(float damping);
	void setRadius(float radius);
	void setVelocity(int i, Vec3 vel) {
		m_pSphereSystem->setVelocity(i, vel);
	}

	void setPosition(int i, Vec3 pos) {
		m_pSphereSystem->setPosition(i, pos);
	}

	int getNumberOfSpheres();
	
protected:
	// Attributes
	Vec3 externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	//mass, radius is identical
	float m_fMass;
	float m_fRadius;
	float m_fForceScaling;
	float m_fDamping;
	int   m_iNumSpheres;
	
	int   m_iKernel; // index of the m_Kernels[5], more detials in SphereSystemSimulator.cpp
	static std::function<float(float)> m_Kernels[5];
	
	int   m_iAccelerator; // switch between NAIVEACC and GRIDACC, (optionally, KDTREEACC, 2)
	int   m_iIntegrator; //I added this, so you can switch between leap frog and midpoint (like in massspringsystem) <- isn't used
	
	SphereSystem * m_pSphereSystem; // add your own sphere system member!
	boolean gridDrawn = true;

	// for Demo 3 only:
	// you will need multiple SphereSystem objects to do comparisons in Demo 3
	// m_iAccelerator should be ignored.
	// SphereSystem * m_pSphereSystemGrid; 

};

#endif