/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SandysWahWahAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SandysWahWahAudioProcessor();
    ~SandysWahWahAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    float limit(float input, float min, float max);

    float getG() { return g; };
    float getTheta() { return theta; };

    void setG(float val) { g = val; };
    void setTheta(float val) { theta = val; };
	
private:

    int fs;
	
    Random r;

    //Plugin states
    float x = 0;
    float x1 = 0;
    float x2 = 0;

    float y = 0;
    float y1 = 0;
    float y2 = 0;

    float b0, b2, a1, a2;

    float g = 0.95;
    float theta = 0.05 * double_Pi;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SandysWahWahAudioProcessor)
};
