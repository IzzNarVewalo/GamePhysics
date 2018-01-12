#pragma once
#include "DreiBSystem.h"

//baukloetzchen und ball
//simulator
class DreiB:public Simulator{

public:
	DreiB();


private:

	DreiBSystem* m_pDreiBSystem;
};