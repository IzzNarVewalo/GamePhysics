#pragma once
#ifndef DREIB_h
#define DREIB_h

#include "DreiBSystem.h"
#include "ParticleCollisionSystem.h"

//baukloetzchen und ball
//simulator
class DreiB :public Simulator {

public:
	DreiB();
	~DreiB();

	const char * getTestCasesStr() {
		return "Holzkloetzchenspiel, 2.0Holzkloetzchenspiel, Teilchenkol";
	};

	void reset() {
		m_mouse.x = m_mouse.y = 0;
		m_trackmouse.x = m_trackmouse.y = 0;
		m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
	};
	void initUI(DrawingUtilitiesClass * DUC);
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void notifyCaseChanged(int testCase);

	void onClick(int x, int y) {
		m_trackmouse.x = x;
		m_trackmouse.y = y;
	};
	void onMouse(int x, int y) {
		m_oldtrackmouse.x = x;
		m_oldtrackmouse.y = y;
		m_trackmouse.x = x;
		m_trackmouse.y = y;
	};


	void setNumBalls(int num) {
		m_pDreiBSystem->setNumBalls(num);
	}

	void incNumBalls();

	void addBox(Vec3 pos, Vec3 size, int mass);


private:

	DreiBSystem* m_pDreiBSystem;
	float m_fextraForce;

	//
	//ParticleCollisionSystem* m_particleColSys;


	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
};
#endif