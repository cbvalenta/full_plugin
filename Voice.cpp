/*
 ==============================================================================
 
 Voice.cpp
 Created: 3 Sep 2024 9:13:22am
 Author:  claudia valenta
 
 ==============================================================================
 */

#include "Voice.h"

Voice::Voice()
{
    auto& masterGain = processorChain.get<masterGainIndex>();
    masterGain.setGainLinear (0.7f);
    
    auto& filter = processorChain.get<filterIndex>();
    filter.setCutoffFrequencyHz (900.0f);
    filter.setResonance (0.8f);
    
    lfo.initialise ([] (float x) { return std::sin(x); }, 128);
    lfo.setFrequency (5.0f);
    
    adsr.setParameters(juce::ADSR::Parameters(0.1f, 3.0f, 0.5f, 0.2f));
}

void Voice::prepare (const juce::dsp::ProcessSpec& spec)
{
    tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
    processorChain.prepare(spec);
    
    lfo.prepare ({ spec.sampleRate / lfoUpdateRate, spec.maximumBlockSize, spec.numChannels });
    
    adsr.setSampleRate(spec.sampleRate);
//    adsr.setParameters(juce::ADSR::Parameters(0.1f, 3.0f, 0.5f, 0.2f));
}

void Voice::noteStarted()
{
    auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
    auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();
    
    processorChain.get<osc1Index>().setFrequency (freqHz, true);
    processorChain.get<osc1Index>().setLevel (velocity);
    
    processorChain.get<osc2Index>().setFrequency (freqHz * 1.01f, true);
    //processorChain.get<osc2Index>().setFrequency (freqHz, true);
    processorChain.get<osc2Index>().setLevel (velocity);
    
    adsr.noteOn();
}

void Voice::notePitchbendChanged()
{
    auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();
    processorChain.get<osc1Index>().setFrequency (freqHz);
    processorChain.get<osc2Index>().setFrequency (freqHz * 1.01f);
    //processorChain.get<osc2Index>().setFrequency (freqHz);
}

void Voice::noteStopped (bool)
{
    //clearCurrentNote();
    adsr.noteOff();
}

void Voice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if(isActive() && adsr.isActive() == false)
    {
        clearCurrentNote();
    }
    
    auto output = tempBlock.getSubBlock (0, (size_t) numSamples);
    output.clear();
    
    juce::dsp::ProcessContextReplacing<float> context (output);
    processorChain.process(context);
    
    juce::dsp::AudioBlock<float> (outputBuffer)
    .getSubBlock ((size_t) startSample, (size_t) numSamples)
    .add (tempBlock);
    
    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}

void Voice::setLfoFreq(float lfoFreq)
{
    lfo.setFrequency (lfoFreq);
}

void Voice::setLfoControl(float lfoControl)
{
    auto lfoOut = lfo.processSample (0.0f);
    
    if (lfoControl == 1) // LFO controlled gain
    {
        auto gain = juce::jmap (lfoOut, -1.0f, 1.0f, 0.6f, 0.9f);
        processorChain.get<masterGainIndex>().setGainLinear (gain);
    }
    else if (lfoControl == 2) // LFO controlled ADSR
    {
        auto adsr_value = juce::jmap (lfoOut, -1.0f, 1.0f, 0.25f, 0.75f);
        adsr.setParameters(juce::ADSR::Parameters(0.1f, 0.7f, adsr_value, 0.2f));
    }
    else // LFO controlled cutoff frequency
    {
        auto cutoffFreqHz = juce::jmap (lfoOut, -1.0f, 1.0f, 100.0f, 2000.0f);
        processorChain.get<filterIndex>().setCutoffFrequencyHz (cutoffFreqHz);
    }
}

void Voice::setLoHiFilter(float filter, float sampleRate)
{
    if (filter == 0)
    {
        *processorChain.get<loHiFilterIndex>().state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 800.f);
    }
    else if (filter == 1)
    {
        *processorChain.get<loHiFilterIndex>().state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 8000.f);
    }
}

