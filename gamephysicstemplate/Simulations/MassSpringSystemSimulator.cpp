#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator(int integrator) {
	m_fMass = 10.0f;
	m_fStiffness = 40.0f;
	m_fDamping = 0.0f;
	m_iIntegrator = integrator;


}

FFmpeg::FFmpeg(int fps, int crf, MODE mode)
	: m_pFILE(nullptr),
	m_pStaging(nullptr),
	m_width(-1),
	m_height(-1),
	m_mode(mode),
	m_fps(fps),
	m_crf(crf),
	m_frame(-1)
