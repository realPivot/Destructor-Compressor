/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StandardCompressorAudioProcessor::StandardCompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
    ,
    state(*this, nullptr, Identifier("params"), {
        std::make_unique<AudioParameterFloat>(
            "thresh",
            "Threshold",
            NormalisableRange<float>(-200.0f, 0.0f, 0.01f),
            10.0f),
        std::make_unique<AudioParameterFloat>(
            "trim",
            "Trim",
            NormalisableRange<float>(0.0f, 0.125f, 0.001f),
            0.125f)
         }
        )
#endif
{
    slopeParam = 1000.0f;
    kneeParam = 0.0f;
    attackParam = 0.0f;
    releaseParam = 0.0f;
}

StandardCompressorAudioProcessor::~StandardCompressorAudioProcessor()
{
}

//==============================================================================
const juce::String StandardCompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StandardCompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StandardCompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StandardCompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StandardCompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StandardCompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StandardCompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StandardCompressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StandardCompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void StandardCompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StandardCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int channel = 0; channel < getNumOutputChannels(); channel++) allCompressors.add(Compressor());

    threshParam = state.getRawParameterValue("thresh");
    trimParam = state.getRawParameterValue("trim");
}

void StandardCompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StandardCompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void StandardCompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{   
    float attack = 1 - std::pow(MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (attackParam / 1000.0f));
    float release = 1 - std::pow(MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (releaseParam / 1000.0f));
    
    auto thresholdInGain = Decibels::decibelsToGain<float>(*threshParam, -300);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        for (int channel = 0; channel < getTotalNumOutputChannels(); channel++)
        {
            auto* data = buffer.getWritePointer(channel);
            Compressor* comp = &allCompressors.getReference(channel); // Get reference Compressor for the current channel
            data[i] = comp->compressSample(data[i], *threshParam, slopeParam, attack, release, kneeParam); // Calculate the compressed samples with some initial values passed into the compressSample function
            data[i] /= thresholdInGain;
            data[i] *= *trimParam;
            if (data[i] > 1) data[i] = 1;
        }
    }
}

//==============================================================================
bool StandardCompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StandardCompressorAudioProcessor::createEditor()
{
    return new StandardCompressorAudioProcessorEditor (*this, state);
}

//==============================================================================
void StandardCompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto stateTree = state.copyState();
    std::unique_ptr<XmlElement> xml(stateTree.createXml());
    copyXmlToBinary(*xml, destData);
}

void StandardCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr && xmlState->hasTagName(state.state.getType())) {
        state.replaceState(ValueTree::fromXml(*xmlState));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StandardCompressorAudioProcessor();
}
