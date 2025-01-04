/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FullPluginAudioProcessorEditor::FullPluginAudioProcessorEditor (FullPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
waveformSelectionAttachment(p.apvts, "Waveform Selection", waveformSelection),
lfoControlAttachment(p.apvts, "LFO Control Selection", lfoControl),
lfoFreqAttachment(p.apvts, "LFO Frequency Selection", lfoFreq),
filterSelectionAttachment(p.apvts, "Filter Selection", filterSelection)
{
    waveformLabel.setText("Select waveform", juce::dontSendNotification);
    lfoFreqLabel.setText("Select LFO Frequency", juce::dontSendNotification);
    lfoControlLabel.setText("Select component for LFO to control", juce::dontSendNotification);
    filterSelectionLabel.setText("Add a filter?", juce::dontSendNotification);
    
    waveformSelection.addItemList(p.apvts.getParameter("Waveform Selection")->getAllValueStrings(), 1);
    lfoControl.addItemList(p.apvts.getParameter("LFO Control Selection")->getAllValueStrings(), 1);
    filterSelection.addItemList(p.apvts.getParameter("Filter Selection")->getAllValueStrings(), 1);
    
    waveformSelection.setTextWhenNothingSelected("Sine");
    lfoControl.setTextWhenNothingSelected("Cutoff Frequency");
    filterSelection.setTextWhenNothingSelected("None");

    addAndMakeVisible(p.getMidiKeyboardComponent());
    addAndMakeVisible(waveformSelection);
    addAndMakeVisible(waveformLabel);
    addAndMakeVisible(lfoFreq);
    addAndMakeVisible(filterSelection);
    
    addAndMakeVisible(lfoFreqLabel);
    addAndMakeVisible(lfoControl);
    addAndMakeVisible(lfoControlLabel);
    addAndMakeVisible(filterSelectionLabel);
    
    setSize (500, 500);
}

FullPluginAudioProcessorEditor::~FullPluginAudioProcessorEditor()
{
}

//==============================================================================
void FullPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void FullPluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    audioProcessor.getMidiKeyboardComponent().setBounds (area.removeFromTop (100).reduced (10));
    waveformSelection.setBounds(10, 100, 200, 100);
    waveformLabel.setBounds(10, 170, 200, 100);
    lfoFreq.setBounds(275, 240, 200, 100);
    lfoFreqLabel.setBounds(275, 310, 200, 100);
    lfoControl.setBounds(10, 240, 200, 100);
    lfoControlLabel.setBounds(10, 310, 200, 100);
    filterSelection.setBounds(275, 100, 200, 100);
    filterSelectionLabel.setBounds(275, 170, 200, 100);
}
