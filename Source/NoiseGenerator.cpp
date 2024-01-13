/*
  ==============================================================================

    NoiseGenerator.cpp
    Created: 30 Dec 2023 9:06:31pm
    Author:  zazz

  ==============================================================================
*/

#include "NoiseGenerator.h"

//==============================================================================
float StepNoiseGenerator::process()
{
	m_currentValue += m_step;
	if (m_currentValue > 1.0f)
	{
		m_step *= m_stepFactor;
		m_currentValue -= 1.0f;
	}

	return (2.0f * m_currentValue) - 1.0f;
};