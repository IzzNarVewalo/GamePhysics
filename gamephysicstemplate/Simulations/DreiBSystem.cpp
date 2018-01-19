#include "DreiBSystem.h"

Mat4 DreiBSystem::getTranslatMatOf(int i)
{
	Mat4 temp;
	temp.initTranslation(m_boxWall[i].m_boxCenter.x, m_boxWall[i].m_boxCenter.y, m_boxWall[i].m_boxCenter.z);
	return temp;
}

Mat4 DreiBSystem::getRotMatOf(int i)
{
	return m_boxWall[i].m_orientation.getRotMat();
}

Mat4 DreiBSystem::getScaleMatOf(int i)
{
	Mat4 temp;
	temp.initScaling(m_boxWall[i].m_boxSize.x, m_boxWall[i].m_boxSize.y, m_boxWall[i].m_boxSize.z);
	return temp;
}

Mat4 DreiBSystem::calcTransformMatrixOf(int i)
{
	return getScaleMatOf(i) * getRotMatOf(i) * getTranslatMatOf(i);
}
