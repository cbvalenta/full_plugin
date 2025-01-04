/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

//==============================================================================
/**
*/

struct ChainSettings
{
    float lfoFreq { 5 }, waveformSelect { 0 }, lfoControl { 0 }, filterSelect {2};
};

class FullPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FullPluginAudioProcessor();
    ~FullPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    ChainSettings getChainSettings();

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::MidiKeyboardState& getMidiKeyboardState() noexcept { return midiKeyboardState; }
    juce::MidiMessageCollector& getMidiMessageCollector() noexcept { return midiMessageCollector; }
    juce::MidiKeyboardComponent& getMidiKeyboardComponent() noexcept { return midiKeyboardComponent; }
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "Parameters", createParameterLayout()};
    
private:
    //==============================================================================
    AudioEngine audioEngine;
    
    juce::MidiKeyboardState midiKeyboardState;
    juce::MidiKeyboardComponent midiKeyboardComponent { midiKeyboardState, juce::MidiKeyboardComponent::horizontalKeyboard };
    juce::MidiMessageCollector midiMessageCollector;
    
    float sampleRateF;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FullPluginAudioProcessor)
};
