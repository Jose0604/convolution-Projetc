/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PrototipeRealTimeConvolutionAudioProcessor::PrototipeRealTimeConvolutionAudioProcessor()
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
    file.registerBasicFormats();

    data.reset(new juce::MemoryInputStream(BinaryData::s1r2_1c_48khz_wav,
        BinaryData::s1r2_1c_48khz_wavSize, false));

    reader.reset(file.createReaderFor(std::unique_ptr<juce::InputStream>
        (data.get())));
    contador = 0;

    L = reader->lengthInSamples;
    buffer1->setSize(1, L);    

    reader->read(buffer1.get(), 1, 0, L, true,false);
    float c = 0;
}

PrototipeRealTimeConvolutionAudioProcessor::~PrototipeRealTimeConvolutionAudioProcessor()
{
}

//==============================================================================
const juce::String PrototipeRealTimeConvolutionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PrototipeRealTimeConvolutionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PrototipeRealTimeConvolutionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PrototipeRealTimeConvolutionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PrototipeRealTimeConvolutionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PrototipeRealTimeConvolutionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PrototipeRealTimeConvolutionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PrototipeRealTimeConvolutionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PrototipeRealTimeConvolutionAudioProcessor::getProgramName (int index)
{
    return {};
}

void PrototipeRealTimeConvolutionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PrototipeRealTimeConvolutionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PrototipeRealTimeConvolutionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PrototipeRealTimeConvolutionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PrototipeRealTimeConvolutionAudioProcessor::processBlock (
    juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float* in = buffer1.getReadPointer(0);
            float* out = buffer.getWritePointer(channel);

            out[sample] = in[contador];
        }
        contador++;
        if (contador >= L) { contador = 0; }
    }
    
}

//==============================================================================
bool PrototipeRealTimeConvolutionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PrototipeRealTimeConvolutionAudioProcessor::createEditor()
{
    return new PrototipeRealTimeConvolutionAudioProcessorEditor (*this);
}

//==============================================================================
void PrototipeRealTimeConvolutionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PrototipeRealTimeConvolutionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PrototipeRealTimeConvolutionAudioProcessor();
}
