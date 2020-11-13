/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SandysWahWahAudioProcessor::SandysWahWahAudioProcessor()
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
}

SandysWahWahAudioProcessor::~SandysWahWahAudioProcessor()
{
}

//==============================================================================
const juce::String SandysWahWahAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SandysWahWahAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SandysWahWahAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SandysWahWahAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SandysWahWahAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SandysWahWahAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SandysWahWahAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SandysWahWahAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SandysWahWahAudioProcessor::getProgramName (int index)
{
    return {};
}

void SandysWahWahAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SandysWahWahAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    fs = sampleRate;
}

void SandysWahWahAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SandysWahWahAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void SandysWahWahAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto* outputL = buffer.getWritePointer(0);
    auto* outputR = buffer.getWritePointer(1);

    auto* inputL = buffer.getWritePointer(0);
    auto* inputR = buffer.getWritePointer(1);

    //Difference equation for wah wah filter (1-G) x[n]  -(1-G)   x(n-2] + 2Gcos(thetha) y[n-1]  -(2G-1) y[n-2]
    //Represented in variables:               b0 *  x  +  b2     *  x2   +     a1       * y1   +   a2   *  y2

    b0 = (1 - g);
    b2 = -(1 - g);
    a1 = 2 * g * cos(theta);
    a2 = -(2 * g - 1);

    for (int n = 0; n < buffer.getNumSamples(); ++n)
    {
        // sin(2.0 * double_Pi * 440.0 / fs)
        //x = 2.0 * r.nextFloat() - 1.0;
        x = 0.5 * (inputL[n] + inputR[n]);

        y = b0 * x + b2 * x2 + a1 * y1 + a2 * y2;
        outputL[n] = limit(y, -1, 1);
        outputR[n] = outputL[n];

        x2 = x1;
        x1 = x;

        y2 = y1;
        y1 = y;
    }
}

//==============================================================================
bool SandysWahWahAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SandysWahWahAudioProcessor::createEditor()
{
    return new SandysWahWahAudioProcessorEditor (*this);
}

//==============================================================================
void SandysWahWahAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SandysWahWahAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


float SandysWahWahAudioProcessor::limit(float input, float min, float max)
{
    if (input > max)
        return max;

    else if (input < min)
        return min;

    return input;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SandysWahWahAudioProcessor();
}
