/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TictacteoEQAudioProcessor::TictacteoEQAudioProcessor()
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
}

TictacteoEQAudioProcessor::~TictacteoEQAudioProcessor()
{
}

//==============================================================================
const juce::String TictacteoEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TictacteoEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TictacteoEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TictacteoEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TictacteoEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TictacteoEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TictacteoEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TictacteoEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TictacteoEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void TictacteoEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TictacteoEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void TictacteoEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TictacteoEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TictacteoEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool TictacteoEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TictacteoEQAudioProcessor::createEditor()
{
 //return new TictacteoEQAudioProcessorEditor (*this);
   return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void TictacteoEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TictacteoEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout TictacteoEQAudioProcessor::createParameterLayout()
    {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               juce::ParameterID("LowCut Freq", 1),
                                                               "LowCut Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f), 20.f));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               juce::ParameterID("HighCut Freq", 1),
                                                               "HighCut Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f), 20000.f));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               juce::ParameterID("Peak Freq", 1),
                                                               "Peak Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f), 750.f));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>
                                                               (juce::ParameterID("Peak Gain", 1),
                                                               "Peak Gain", juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f), 0.0f));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Peak Quality", 1),
                                                               "Peak Quality", juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f), 1.f));
        
        juce::StringArray stringArray;
        for( int i = 0; i < 4; i++ )
        {
            juce::String str;
            str << (12 + 12 * i);
            str << "db/Oct";
            stringArray.add(str);
            
        }
        
        
        layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("LowCut Slope", 1), "LowCut Slope", stringArray, 0));
    
        layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("HighCut Slope", 1), "HighCut Slope", stringArray, 0));
        
    return layout;
        
    }

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TictacteoEQAudioProcessor();
}
