/*
 ==============================================================================
 
 AudioEngine.h
 Created: 3 Sep 2024 9:09:00am
 Author:  claudia valenta
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "Voice.h"

class AudioEngine  : public juce::MPESynthesiser
{
public:
    static constexpr auto maxNumVoices = 4;
    
    //==============================================================================
    AudioEngine();
    
    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec) noexcept;
    
    void changeWave(float select);
    void changeLfoFreq(float lfoFreq);
    void changeLfoControl(float lfoControl);
    void changeFilter(float filter, float sampleRate);
    
private:
    //==============================================================================
    void renderNextSubBlock (juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples) override
    {
        MPESynthesiser::renderNextSubBlock (outputAudio, startSample, numSamples);
        
//                auto block = juce::dsp::AudioBlock<float> (outputAudio);
//                auto blockToUse = block.getSubBlock ((size_t) startSample, (size_t) numSamples);
//                auto contextToUse = juce::dsp::ProcessContextReplacing<float> (blockToUse);
//                fxChain.process (contextToUse);
    }
    
//        enum
//        {
//            reverbIndex
//        };
//    
//        juce::dsp::ProcessorChain<juce::dsp::Reverb> fxChain;
};
