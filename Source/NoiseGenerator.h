/*
  ==============================================================================

    NoiseGenerator.h
    Created: 30 Dec 2023 9:06:31pm
    Author:  zazz

  ==============================================================================
*/

#pragma once


//==============================================================================
class StepNoiseGenerator
{
public:
	StepNoiseGenerator() {};

	static const int DEFAULT_SAMPLE_RATE = 44100;

	void setCurrentValue(float currentValue)
	{
		m_currentValue = currentValue;
	}
	// Step value has to in range (0, 1);
	void setStep(float step, int sampleRate)
	{
		m_step = step * DEFAULT_SAMPLE_RATE / sampleRate;
	}
	void setStepFactor(float stepFactor)
	{
		m_stepFactor = stepFactor;
	}
	float process();

private:
	float m_step = 0.2f;
	float m_stepFactor = 1.0f;
	float m_currentValue = 0.4239f;
};