/*
  ==============================================================================

    Oscillator.h
    Created: 12 Nov 2024 9:39:52am
    Author:  claudia valenta

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

template <typename Type>
class Oscillator
{
public:
    Oscillator(float select = 0)
    {
        auto& osc = processorChain.template get<oscIndex>();
        osc.initialise([](Type x)
        // Sine Wave (0)
                                {
                                    return std::sin(x);
                                }, 128);
    }
    
    void chooseWave (float select)
    {
        auto& osc = processorChain.template get<oscIndex>();
        if (select == 0) // Sine Wave
        {
            osc.initialise([](Type x)
            {
                return std::sin(x);
            }, 128);
        }
        else if (select == 1) // Square Wave
        {
            osc.initialise([](Type x)
            {
                return (x < 0) ? -1 : 1;
            }, 128);
        } else if (select == 2) // Sawtooth Wave
        {
            osc.initialise([](Type x)
           {
               return juce::jmap (x,
                                  Type (-juce::MathConstants<double>::pi),
                                  Type (juce::MathConstants<double>::pi),
                                  Type (-1),
                                  Type (1));
           }, 2);
        }
    }
    
    void setFrequency (Type newValue, bool force = false)
    {
        auto& osc = processorChain.template get<oscIndex>();
        osc.setFrequency (newValue, force);
    }
    
    void setLevel (Type newValue)
    {
        auto& gain = processorChain.template get<gainIndex>();
        gain.setGainLinear (newValue);
    }
    
    void reset() noexcept {
        processorChain.reset();
    }
    
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        processorChain.process (context);
    }
    
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        processorChain.prepare (spec);
    }
    
private:
    enum
    {
        oscIndex,
        gainIndex
    };
    
    juce::dsp::ProcessorChain<juce::dsp::Oscillator<Type>, juce::dsp::Gain<Type>> processorChain;
};
