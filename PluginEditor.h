/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawCustomRotarySlider(juce::Graphics g, int x, int y, int height, int width, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider);
};

struct CustomRotarySlider : juce::Slider
{
public:
    CustomRotarySlider()
    {
        setLookAndFeel(&lnf);
    }
    
    ~CustomRotarySlider()
    {
        setLookAndFeel(nullptr);
    }
    
    void paint(juce::Graphics& g) override;
    juce::Rectangle<int> getSliderBounds() const;
    
private:
    LookAndFeel lnf;
};

class FullPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FullPluginAudioProcessorEditor (FullPluginAudioProcessor&);
    ~FullPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FullPluginAudioProcessor& audioProcessor;
    
    juce::Label waveformLabel, lfoFreqLabel, lfoControlLabel, filterSelectionLabel;
    
    juce::ComboBox waveformSelection, lfoControl, filterSelection;
    CustomRotarySlider lfoFreq;
    
    juce::AudioProcessorValueTreeState::ComboBoxAttachment waveformSelectionAttachment, lfoControlAttachment, filterSelectionAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lfoFreqAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FullPluginAudioProcessorEditor)
};
