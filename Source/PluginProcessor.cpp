/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

const std::string StepNoiseGeneratorAudioProcessor::paramsNames[] = { "P1", "P2", "Volume" };

//==============================================================================
StepNoiseGeneratorAudioProcessor::StepNoiseGeneratorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	firstParameter  = apvts.getRawParameterValue(paramsNames[0]);
	secondParameter = apvts.getRawParameterValue(paramsNames[1]);
	volumeParameter = apvts.getRawParameterValue(paramsNames[2]);

	buttonAParameter = static_cast<juce::AudioParameterBool*>(apvts.getParameter("ButtonA"));
	buttonBParameter = static_cast<juce::AudioParameterBool*>(apvts.getParameter("ButtonB"));
}

StepNoiseGeneratorAudioProcessor::~StepNoiseGeneratorAudioProcessor()
{
}

//==============================================================================
const juce::String StepNoiseGeneratorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StepNoiseGeneratorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StepNoiseGeneratorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StepNoiseGeneratorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StepNoiseGeneratorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StepNoiseGeneratorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StepNoiseGeneratorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StepNoiseGeneratorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StepNoiseGeneratorAudioProcessor::getProgramName (int index)
{
    return {};
}

void StepNoiseGeneratorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StepNoiseGeneratorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{	
	m_stepStepNoiseGenerator[0].setCurrentValue(0.2123f);
	m_stepStepNoiseGenerator[1].setCurrentValue(0.8184f);
}

void StepNoiseGeneratorAudioProcessor::releaseResources()
{
	
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StepNoiseGeneratorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void StepNoiseGeneratorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	// Get params
	const auto first  = firstParameter->load();
	const auto second  = secondParameter->load();
	const auto volume = juce::Decibels::decibelsToGain(volumeParameter->load()) * juce::Decibels::decibelsToGain(-48.0f);

	// Buttons
	const auto buttonA = buttonAParameter->get();
	const auto buttonB = buttonBParameter->get();

	// Mics constants
	const int channels = getTotalNumOutputChannels();
	const int samples = buffer.getNumSamples();		
	const int sampleRate = (int)getSampleRate();

	for (int channel = 0; channel < channels; ++channel)
	{
		auto* channelBuffer = buffer.getWritePointer(channel);
		
		auto& stepNoiseGenerator = m_stepStepNoiseGenerator[channel];
		stepNoiseGenerator.setStepFactor(0.1f + 0.9f * first);
		stepNoiseGenerator.setStep(0.01f + 0.98f * second, sampleRate);
		
		
		if (buttonA)
			stepNoiseGenerator.setStepFactor(0.1f + 0.9f * first);
		else
			stepNoiseGenerator.setStepFactor(0.01f + 0.09f * first);
		
		if (buttonB)
			stepNoiseGenerator.setStep(0.1f + 0.9f * second, sampleRate);
		else
			stepNoiseGenerator.setStep(0.01f + 0.09f * second, sampleRate);
		
		for (int sample = 0; sample < samples; ++sample)
		{
			channelBuffer[sample] = volume * stepNoiseGenerator.process();
		}
	}
}

//==============================================================================
bool StepNoiseGeneratorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StepNoiseGeneratorAudioProcessor::createEditor()
{
    return new StepNoiseGeneratorAudioProcessorEditor (*this, apvts);
}

//==============================================================================
void StepNoiseGeneratorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{	
	auto state = apvts.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void StepNoiseGeneratorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(apvts.state.getType()))
			apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout StepNoiseGeneratorAudioProcessor::createParameterLayout()
{
	APVTS::ParameterLayout layout;

	using namespace juce;

	layout.add(std::make_unique<juce::AudioParameterFloat>(paramsNames[0], paramsNames[0], NormalisableRange<float>(  0.0f,  1.0f, 0.001f, 1.0f), 0.5f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(paramsNames[1], paramsNames[1], NormalisableRange<float>(  0.0f,  1.0f, 0.001f, 1.0f), 1.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(paramsNames[2], paramsNames[2], NormalisableRange<float>(-24.0f, 24.0f,   0.1f, 1.0f), 0.0f));

	layout.add(std::make_unique<juce::AudioParameterBool>("ButtonA", "ButtonA", false));
	layout.add(std::make_unique<juce::AudioParameterBool>("ButtonB", "ButtonB", false));

	return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StepNoiseGeneratorAudioProcessor();
}
