/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void LookAndFeel::drawCustomRotarySlider(juce::Graphics g, int x, int y, int height, int width, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float>(x, y, width, height);
    
    if( auto* custSlider = dynamic_cast<CustomRotarySlider*>(&slider))
    {
        auto center = bounds.getCentre();
        
        juce::Path p;
        
        juce::Rectangle<float> r;
        r.setLeft(center.getX() - 2);
        r.setRight(center.getX() + 2);
        r.setTop(bounds.getY());
        r.setBottom(center.getY() - 14 * 1.5);
        
        p.addRoundedRectangle(r, 2.f);
        
        jassert(rotaryStartAngle < rotaryEndAngle);
        
        auto sliderAngRad = juce::jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);
        
        p.applyTransform(juce::AffineTransform().rotated(sliderAngRad, center.getX(), center.getY()));
        
        g.fillPath(p);
        
        r.setSize(12, 16);
        r.setCentre(center);
        
        g.setColour(juce::Colours::black);
        g.fillRect(r);
        
        g.setColour(juce::Colours::white);
    }
}

void CustomRotarySlider::paint(juce::Graphics& g)
{
    auto startAng = juce::degreesToRadians(180.f + 45.f);
    auto endAng = juce::degreesToRadians(180.f - 45.f) + juce::MathConstants<float>::twoPi;
    
    auto range = getRange();
    
    auto sliderBounds = getSliderBounds();
    
    getLookAndFeel().drawRotarySlider(g, sliderBounds.getX(), sliderBounds.getY(), sliderBounds.getHeight(), sliderBounds.getWidth(), juce::jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0), startAng, endAng, *this);
}

juce::Rectangle<int> CustomRotarySlider::getSliderBounds() const
{
    auto bounds = getLocalBounds();
    
    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());
    
    size -= 28;
    
    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX() + 50, 0);
    r.setY(2);
    
    return r;
}

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
