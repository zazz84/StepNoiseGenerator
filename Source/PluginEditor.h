/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class StepNoiseGeneratorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    StepNoiseGeneratorAudioProcessorEditor (StepNoiseGeneratorAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~StepNoiseGeneratorAudioProcessorEditor() override;

	// GUI setup
	static const int N_SLIDERS_COUNT = 3;
	static const int SCALE = 70;
	static const int LABEL_OFFSET = 25;
	static const int SLIDER_WIDTH = 200;
	static const int BOTTOM_MENU_HEIGHT = 50;
	static const int HUE = 95;

    //==============================================================================
	void paint (juce::Graphics&) override;
    void resized() override;

	typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

private:
    StepNoiseGeneratorAudioProcessor& audioProcessor;

	juce::AudioProcessorValueTreeState& valueTreeState;

	juce::Label m_labels[N_SLIDERS_COUNT] = {};
	juce::Slider m_sliders[N_SLIDERS_COUNT] = {};
	std::unique_ptr<SliderAttachment> m_sliderAttachment[N_SLIDERS_COUNT] = {};

	juce::TextButton typeAButton{ "x10" };
	juce::TextButton typeBButton{ "x10" };

	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonBAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepNoiseGeneratorAudioProcessorEditor)
};
