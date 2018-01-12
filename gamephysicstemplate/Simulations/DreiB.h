#pragma once
#ifndef DREIB_h
#define DREIB_h

#include "DreiBSystem.h"

//baukloetzchen und ball	
//simulator
class DreiB:public Simulator{

public:
	DreiB();


private:

	DreiBSystem* m_pDreiBSystem;
};
#endif