/*
 ==============================================================================
 
 AudioEngine.cpp
 Created: 3 Sep 2024 9:09:00am
 Author:  claudia valenta
 
 ==============================================================================
 */

#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    for (auto i = 0; i < maxNumVoices; ++i)
        addVoice (new Voice);
    
    setVoiceStealingEnabled (true);
}

void AudioEngine::prepare (const juce::dsp::ProcessSpec& spec) noexcept
{
    setCurrentPlaybackSampleRate (spec.sampleRate);
    
    for (auto* v : voices)
        dynamic_cast<Voice*> (v)->prepare (spec);
    
    //fxChain.prepare (spec);
}

void AudioEngine::changeWave(float select)
{
    for (auto* v : voices)
    {
        dynamic_cast<Voice*> (v)->processorChain.get<Voice::osc1Index>().chooseWave(select);
        dynamic_cast<Voice*> (v)->processorChain.get<Voice::osc2Index>().chooseWave(select);
    }
}

void AudioEngine::changeLfoFreq(float lfoFreq)
{
    for (auto* v : voices)
    {
        dynamic_cast<Voice*> (v)->setLfoFreq(lfoFreq);
        dynamic_cast<Voice*> (v)->setLfoFreq(lfoFreq);
    }
}

void AudioEngine::changeLfoControl(float lfoControl)
{
    for (auto* v : voices)
    {
        dynamic_cast<Voice*> (v)->setLfoControl(lfoControl);
        dynamic_cast<Voice*> (v)->setLfoControl(lfoControl);
    }
}

void AudioEngine::changeFilter(float filter, float sampleRate)
{
    for (auto* v : voices)
    {
        dynamic_cast<Voice*> (v)->setLoHiFilter(filter, sampleRate);
        dynamic_cast<Voice*> (v)->setLoHiFilter(filter, sampleRate);
    }
}
