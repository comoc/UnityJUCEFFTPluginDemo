/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class JucefftforUnityAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    JucefftforUnityAudioProcessor();
    ~JucefftforUnityAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
	void pushNextSampleIntoFifo(float sample) noexcept;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JucefftforUnityAudioProcessor)

public:
	enum
    {
        fftOrder = 10,           // [1]
        fftSize  = 1 << fftOrder // [2]
    };

private:
	dsp::FFT forwardFFT;            // [3]
	float fifo [fftSize];           // [4]
    float fftData [2 * fftSize];    // [5]
    int fifoIndex = 0;              // [6]
    bool nextFFTBlockReady = false; // [7]
};
