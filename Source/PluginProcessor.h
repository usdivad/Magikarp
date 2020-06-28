/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "MagikarpSequence.h"

//==============================================================================
/**
*/
class MagikarpAudioProcessor  : public AudioProcessor,
                                public ValueTree::Listener
{
public:
    //==============================================================================
    MagikarpAudioProcessor();
    ~MagikarpAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // //==============================================================================
    // void valueTreePropertyChanged(ValueTree& treeWhosePropertyhasChanged, const Identifier& property) override;
    
    //==============================================================================
    AudioProcessorValueTreeState& getValueTreeState() { return m_ValueTreeState; }
    
    //==============================================================================
    const std::vector<int>& getActiveMidiNotes() const;

private:
    //==============================================================================
    // MIDI
    std::vector<int> _activeMidiNotes;
    int _currMidiNoteIdx;
    int _currMidiNoteNum;
    int _prevMidiNoteNum;
    int _currMidiNoteTimeElapsed; // In samples
    
    //==============================================================================
    // Playhead
    int _currBeatNum;
    bool _isPlayheadPlaying;
    float _currBpm;
    
    //==============================================================================
    // Arp params
    int _arpSubdivisionNumerator = 1;
    int _arpSubdivisionDenominator = 4;
    
    //==============================================================================
    // Sequencing
    MagikarpSequence m_Sequence;
    int _currSequenceIdx;
    
    //==============================================================================
    // Audio
    float _sampleRate;
    
    //==============================================================================
    // Value tree state
    AudioProcessorValueTreeState m_ValueTreeState;
    
    //==============================================================================
    void handleNewMidiNote(int midiNote, bool isNoteOn, bool isNoteOff);
    int calculateNoteDuration() const;
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MagikarpAudioProcessor)
};
