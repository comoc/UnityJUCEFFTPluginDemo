/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#ifdef _WIN32
#include <Windows.h>
static SRWLOCK g_SRWLock;
#else
#error Some exclusion control is required to read/write the FFT data.
#endif

static float g_fftData[JucefftforUnityAudioProcessor::fftSize];

extern "C" __declspec(dllexport) void GetFFTData(float* buffer)
{
#ifdef _WIN32
	AcquireSRWLockShared(&g_SRWLock);
#endif
	memcpy(buffer, g_fftData, JucefftforUnityAudioProcessor::fftSize * sizeof(float));
#ifdef _WIN32
	ReleaseSRWLockShared(&g_SRWLock);
#endif
}

//==============================================================================
JucefftforUnityAudioProcessor::JucefftforUnityAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
	, forwardFFT(fftOrder)
{
	InitializeSRWLock(&g_SRWLock);
}

JucefftforUnityAudioProcessor::~JucefftforUnityAudioProcessor()
{
}

//==============================================================================
const String JucefftforUnityAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JucefftforUnityAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JucefftforUnityAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JucefftforUnityAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JucefftforUnityAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JucefftforUnityAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JucefftforUnityAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JucefftforUnityAudioProcessor::setCurrentProgram (int index)
{
}

const String JucefftforUnityAudioProcessor::getProgramName (int index)
{
    return {};
}

void JucefftforUnityAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void JucefftforUnityAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void JucefftforUnityAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JucefftforUnityAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void JucefftforUnityAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	if (buffer.getNumChannels() > 0)
    {
        for (auto i = 0; i < buffer.getNumSamples(); ++i)
			pushNextSampleIntoFifo(buffer.getSample(0, i));

		if (nextFFTBlockReady)
		{

			forwardFFT.performFrequencyOnlyForwardTransform (fftData);
#ifdef _WIN32
			AcquireSRWLockExclusive(&g_SRWLock);
#endif
			memcpy(g_fftData, fftData, 2048);
#ifdef _WIN32
			ReleaseSRWLockExclusive(&g_SRWLock);
#endif
			nextFFTBlockReady = false;
		}
    }

}
void JucefftforUnityAudioProcessor::pushNextSampleIntoFifo (float sample) noexcept
{
    if (fifoIndex == fftSize)    // [8]
    {
        if (! nextFFTBlockReady) // [9]
        {
            zeromem (fftData, sizeof (fftData));
            memcpy (fftData, fifo, sizeof (fifo));
            nextFFTBlockReady = true;
        }
        fifoIndex = 0;
    }
    fifo[fifoIndex++] = sample;  // [9]
}

//==============================================================================
bool JucefftforUnityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* JucefftforUnityAudioProcessor::createEditor()
{
    return new JucefftforUnityAudioProcessorEditor (*this);
}

//==============================================================================
void JucefftforUnityAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JucefftforUnityAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JucefftforUnityAudioProcessor();
}
