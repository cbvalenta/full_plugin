/*
 ==============================================================================
 
 Voice.h
 Created: 3 Sep 2024 9:13:22am
 Author:  claudia valenta
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "Oscillator.h"

class Voice  : public juce::MPESynthesiserVoice
{
public:
    Voice();
    
    void prepare (const juce::dsp::ProcessSpec& spec);
    void noteStarted() override;
    void notePitchbendChanged() override;
    void noteStopped (bool) override;
    
    void notePressureChanged() override {}
    void noteTimbreChanged()   override {}
    void noteKeyStateChanged() override {}
    
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
    void setLfoFreq(float lfoFreq);
    void setLfoControl(float lfoOut);
    void setLoHiFilter(float filter, float sampleRate);
    
    enum
    {
        osc1Index,
        osc2Index,
        filterIndex,
        loHiFilterIndex,
        masterGainIndex
    };
    
    using doubleFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    
    juce::dsp::ProcessorChain<Oscillator<float>, Oscillator<float>, juce::dsp::LadderFilter<float>,
    doubleFilter, juce::dsp::Gain<float>> processorChain;
    
private:
    //==============================================================================
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;
    juce::ADSR adsr;
    
    static constexpr size_t lfoUpdateRate = 100;
    size_t lfoUpdateCounter = lfoUpdateRate;
    juce::dsp::Oscillator<float> lfo;
};
