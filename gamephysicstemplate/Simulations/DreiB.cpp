#include "DreiB.h"


int m_ifactor = 1; //massenfaktor

DreiB::DreiB()
{
}

void DreiB::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;

	DUC->update(0.1f);
	switch (m_iTestCase)
	{
	case 0:
		TwAddButton(DUC->g_pTweakBar, "double mass", [](void * s) { m_ifactor *= 2; }, nullptr, "");
		TwAddVarRW(DUC->g_pTweakBar, "extra force factor", TW_TYPE_FLOAT, &m_fextraForce, "min=1 step=0.1 max=5");
		break;
	case 1:
		TwAddButton(DUC->g_pTweakBar, "number balls", [](void * s) { incNumBalls(); }, nullptr, "");
		TwAddVarRW(DUC->g_pTweakBar, "number balls", TW_TYPE_INT32, set(), "");
		//evtl kleidungsstuek bewerfen-> dann noch zwischen verschiedenen objekten und anzahl von baellen unterscheiden, a
		//nstatt bei demos es z uunterscheiden
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:break;
	}
}
