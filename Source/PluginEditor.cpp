/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SandysWahWahAudioProcessorEditor::SandysWahWahAudioProcessorEditor (SandysWahWahAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(gSlider);
    addAndMakeVisible(thetaSlider);

    gSlider.setValue(audioProcessor.getG());
    thetaSlider.setValue(audioProcessor.getTheta());
    gSlider.setRange(0.9, 1);
    thetaSlider.setRange(0.0, double_Pi / 5.0);

    gSlider.addListener(this);
    thetaSlider.addListener(this);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
}

SandysWahWahAudioProcessorEditor::~SandysWahWahAudioProcessorEditor()
{
}

//==============================================================================
void SandysWahWahAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
}

void SandysWahWahAudioProcessorEditor::resized()
{

    gSlider.setBounds(getLocalBounds().withHeight(getHeight() * 0.5));
    thetaSlider.setBounds(getLocalBounds().withHeight(getHeight() * 0.5).withY(getHeight() * 0.5));
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void SandysWahWahAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &gSlider)
    {
        audioProcessor.setG(gSlider.getValue());
    }
    else if (slider == &thetaSlider)
    {
        audioProcessor.setTheta(thetaSlider.getValue());
    }
}

