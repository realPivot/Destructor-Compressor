/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StandardCompressorAudioProcessorEditor::StandardCompressorAudioProcessorEditor (StandardCompressorAudioProcessor& p, AudioProcessorValueTreeState& state)
    : AudioProcessorEditor (&p), audioProcessor (p), params(state)
{
    addSlider("thresh", "Threshold", threshSlider, threshLabel, threshAttach);
    addSlider("trim", "Trim", trimSlider, trimLabel, trimAttach);

    threshSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    threshSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    threshLabel.attachToComponent(&threshSlider, false);

    trimLabel.attachToComponent(&trimSlider, true);

    setSize (400, 300);
}

StandardCompressorAudioProcessorEditor::~StandardCompressorAudioProcessorEditor()
{
}

//==============================================================================
void StandardCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void StandardCompressorAudioProcessorEditor::resized()
{
    threshSlider.setBounds(100, 0, 200, 250);
    trimSlider.setBounds(100, 250, 200, 50);
}

void StandardCompressorAudioProcessorEditor::addSlider(String name, String labelText, Slider& slider, Label& label, std::unique_ptr<SliderAttachment>& attachment)
{
    addAndMakeVisible(slider);
    attachment.reset(new SliderAttachment(params, name, slider));
    label.setText(labelText, dontSendNotification);
    addAndMakeVisible(label);
}
