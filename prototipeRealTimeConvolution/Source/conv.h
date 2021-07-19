/*
  ==============================================================================

    conv.h
    Created: 21 Jun 2021 4:42:26pm
    Author:  José, Pablo, Leonardo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class circularConvolution
{
public:
    
    circularConvolution()
    {
        file.registerBasicFormats();

        data.reset(new juce::MemoryInputStream(BinaryData::s1r2_1c_48khz_wav,
            BinaryData::s1r2_1c_48khz_wavSize, false));

        reader.reset(file.createReaderFor(std::unique_ptr<juce::InputStream>
            (data.get())));

        source.reset(new juce::AudioFormatReaderSource(reader.get(), true));
    }
    ~circularConvolution(){}

private:
    
    juce::AudioFormatManager file;
    std::unique_ptr<juce::AudioFormatReader> reader;
    std::unique_ptr<juce::MemoryInputStream> data;
    std::unique_ptr<juce::AudioFormatReaderSource> source;

};
