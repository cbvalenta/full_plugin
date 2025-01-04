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
    juce::Slider lfoFreq;
    
    juce::AudioProcessorValueTreeState::ComboBoxAttachment waveformSelectionAttachment, lfoControlAttachment, filterSelectionAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lfoFreqAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FullPluginAudioProcessorEditor)
};
