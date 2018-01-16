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
	const char * getTestCasesStr() = 0;
	void reset() = 0;
	void initUI(DrawingUtilitiesClass * DUC) = 0;
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext) = 0;
	void externalForcesCalculations(float timeElapsed) = 0;
	void simulateTimestep(float timeStep) = 0;
	void notifyCaseChanged(int testCase) = 0;

	void onClick(int x, int y) = 0 {
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

	DreiBSystem* m_pDreiBSystem;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
};
#endif