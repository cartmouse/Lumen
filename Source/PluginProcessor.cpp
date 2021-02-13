/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LumenAudioProcessor::LumenAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
    parameters(*this, nullptr, Identifier("LumenUp"),
        {
            std::make_unique<AudioParameterFloat>("dial",
                                                 "Dial",
                                                 NormalisableRange<float>(-1.0f, 1.0f, 0.01f),
                                                 0.0f)
        })
#endif
{
    dialParam = parameters.getRawParameterValue("dial");
}

LumenAudioProcessor::~LumenAudioProcessor()
{
}

//==============================================================================
const String LumenAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LumenAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LumenAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LumenAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LumenAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LumenAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LumenAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LumenAudioProcessor::setCurrentProgram (int index)
{
}

const String LumenAudioProcessor::getProgramName (int index)
{
    return {};
}

void LumenAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LumenAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    filter.prepare(spec);
    filter.reset();

    mSampleRate = sampleRate;
}

void LumenAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LumenAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void LumenAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto dialAbsValue = abs(*dialParam);
    auto startFreq = 3000.0f;
    float centreFreq;

    if (*dialParam < 0.0f)
        centreFreq = startFreq + 2800.0f * *dialParam;
    else 
        centreFreq = startFreq + 5000.0f * *dialParam;

    auto q = 0.5f - dialAbsValue * 0.4f;
    auto gain = 1.0f + dialAbsValue * 0.8f;

    dsp::AudioBlock <float> block(buffer);
    *filter.state = *dsp::IIR::Coefficients<float>::makePeakFilter(mSampleRate, centreFreq, q, gain);
    filter.process(dsp::ProcessContextReplacing<float>(block));

    buffer.applyGain(1.0f / gain);
}

//==============================================================================
bool LumenAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* LumenAudioProcessor::createEditor()
{
    return new LumenAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void LumenAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void LumenAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LumenAudioProcessor();
}
