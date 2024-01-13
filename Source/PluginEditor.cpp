/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StepNoiseGeneratorAudioProcessorEditor::StepNoiseGeneratorAudioProcessorEditor (StepNoiseGeneratorAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
	juce::Colour light = juce::Colour::fromHSV(HUE * 0.01f, 0.5f, 0.6f, 1.0f);
	juce::Colour medium = juce::Colour::fromHSV(HUE * 0.01f, 0.5f, 0.5f, 1.0f);
	juce::Colour dark = juce::Colour::fromHSV(HUE * 0.01f, 0.5f, 0.4f, 1.0f);

	getLookAndFeel().setColour(juce::Slider::thumbColourId, dark);
	getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, medium);
	getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, light);

	for (int i = 0; i < N_SLIDERS_COUNT; i++)
	{
		auto& label = m_labels[i];
		auto& slider = m_sliders[i];

		//Lable
		label.setText(StepNoiseGeneratorAudioProcessor::paramsNames[i], juce::dontSendNotification);
		label.setFont(juce::Font(24.0f * 0.01f * SCALE, juce::Font::bold));
		label.setJustificationType(juce::Justification::centred);
		addAndMakeVisible(label);

		//Slider
		slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
		slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
		addAndMakeVisible(slider);
		m_sliderAttachment[i].reset(new SliderAttachment(valueTreeState, StepNoiseGeneratorAudioProcessor::paramsNames[i], slider));
	}

	// Buttons
	addAndMakeVisible(typeAButton);
	addAndMakeVisible(typeBButton);

	typeAButton.setClickingTogglesState(true);
	typeBButton.setClickingTogglesState(true);

	buttonAAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "ButtonA", typeAButton));
	buttonBAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "ButtonB", typeBButton));

	typeAButton.setColour(juce::TextButton::buttonColourId, light);
	typeBButton.setColour(juce::TextButton::buttonColourId, light);

	typeAButton.setColour(juce::TextButton::buttonOnColourId, dark);
	typeBButton.setColour(juce::TextButton::buttonOnColourId, dark);

	setSize((int)(SLIDER_WIDTH * 0.01f * SCALE * N_SLIDERS_COUNT), (int)((SLIDER_WIDTH + BOTTOM_MENU_HEIGHT) * 0.01f * SCALE));
}

StepNoiseGeneratorAudioProcessorEditor::~StepNoiseGeneratorAudioProcessorEditor()
{
}

void StepNoiseGeneratorAudioProcessorEditor::paint (juce::Graphics& g)
{
	g.fillAll(juce::Colour::fromHSV(HUE * 0.01f, 0.5f, 0.7f, 1.0f));
}

void StepNoiseGeneratorAudioProcessorEditor::resized()
{
	// Sliders + Menus
	int width = getWidth() / N_SLIDERS_COUNT;
	int height = SLIDER_WIDTH * 0.01f * SCALE;
	juce::Rectangle<int> rectangles[N_SLIDERS_COUNT];

	for (int i = 0; i < N_SLIDERS_COUNT; ++i)
	{
		rectangles[i].setSize(width, height);
		rectangles[i].setPosition(i * width, 0);
		m_sliders[i].setBounds(rectangles[i]);

		rectangles[i].removeFromBottom((int)(LABEL_OFFSET * 0.01f * SCALE));
		m_labels[i].setBounds(rectangles[i]);
	}

	// Buttons
	const int posY = height + (int)(BOTTOM_MENU_HEIGHT * 0.01f * SCALE * 0.25f);
	const int buttonHeight = (int)(BOTTOM_MENU_HEIGHT * 0.01f * SCALE * 0.5f);
	const int buttonWidth = 2 * buttonHeight;
	const int buttonOffset = width / 2 - buttonHeight;

	typeAButton.setBounds(buttonOffset, posY, buttonWidth, buttonHeight);
	typeBButton.setBounds(width + buttonOffset, posY, buttonWidth, buttonHeight);
}