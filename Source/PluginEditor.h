/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/**
*/
class StandardCompressorAudioProcessorEditor  : 
    public 
    juce::AudioProcessorEditor
{
public:
    StandardCompressorAudioProcessorEditor (StandardCompressorAudioProcessor&, AudioProcessorValueTreeState&);
    ~StandardCompressorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void addSlider(String name, String labelText, Slider& slider, Label& label, std::unique_ptr<SliderAttachment>& attachment);

private:
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StandardCompressorAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& params;
    Slider threshSlider, trimSlider;
    Label threshLabel, trimLabel;
    std::unique_ptr<SliderAttachment> threshAttach, trimAttach;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandardCompressorAudioProcessorEditor)
};
