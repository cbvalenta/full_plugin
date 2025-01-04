/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FullPluginAudioProcessor::FullPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    midiKeyboardComponent.setMidiChannel (2);
    midiKeyboardState.addListener (&getMidiMessageCollector());
}

FullPluginAudioProcessor::~FullPluginAudioProcessor()
{
    midiKeyboardState.removeListener (&getMidiMessageCollector());
}

//==============================================================================
const juce::String FullPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FullPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FullPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FullPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FullPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FullPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FullPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FullPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FullPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void FullPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FullPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    audioEngine.prepare ({ sampleRate, (juce::uint32) samplesPerBlock, 2 });
    sampleRateF = sampleRate;
    //audioEngine.changeFilter(getChainSettings().filterSelect, sampleRate);
    midiMessageCollector.reset (sampleRate);
}

void FullPluginAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FullPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FullPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    
    auto chainSettings = getChainSettings();
    audioEngine.changeWave(chainSettings.waveformSelect);
    audioEngine.changeLfoFreq(chainSettings.lfoFreq);
    audioEngine.changeLfoControl(chainSettings.lfoControl);
    audioEngine.changeFilter(chainSettings.filterSelect, sampleRateF);
    
    midiMessageCollector.removeNextBlockOfMessages (midiMessages, buffer.getNumSamples());
    audioEngine.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
}

ChainSettings FullPluginAudioProcessor::getChainSettings()
{
    ChainSettings settings;
    settings.waveformSelect = apvts.getRawParameterValue("Waveform Selection")->load();
    settings.lfoFreq = apvts.getRawParameterValue("LFO Frequency Selection")->load();
    settings.lfoControl = apvts.getRawParameterValue("LFO Control Selection")->load();
    settings.filterSelect = apvts.getRawParameterValue("Filter Selection")->load();
    
    return settings;
}

//==============================================================================
bool FullPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FullPluginAudioProcessor::createEditor()
{
    return new FullPluginAudioProcessorEditor (*this);
}

//==============================================================================
void FullPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FullPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FullPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout FullPluginAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("Waveform Selection", 1), "Waveform Selection", juce::StringArray{"Sine", "Square", "Sawtooth"}, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("LFO Frequency Selection", 1), "LFO Frequency Selection", juce::NormalisableRange<float>(0.1f, 20.f, 0.05f, 1.f), 5.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("LFO Control Selection", 1), "LFO Control Selection", juce::StringArray{"Cutoff Frequency", "Gain", "ADSR"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("Filter Selection", 1), "Filter Selection", juce::StringArray{"High Pass", "Low Pass"}, 0));
    
    return layout;
}
