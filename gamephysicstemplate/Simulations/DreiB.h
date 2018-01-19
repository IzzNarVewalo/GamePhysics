#pragma once
#ifndef DREIB_h
#define DREIB_h

#include "DreiBSystem.h"

//baukloetzchen und ball
//simulator
class DreiB :public Simulator {

public:
	DreiB();

	 //Functions
	const char * getTestCasesStr();
	void reset();
	void initUI(DrawingUtilitiesClass * DUC);
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void notifyCaseChanged(int testCase);

	void addBox(Vec3 pos, Vec3 size, int mass);

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

private:

	DreiBSystem * m_pDreiBSystem;
	Vec3 m_externalForce;

	DreiBSystem* m_pDreiBSystem;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
};
#endif