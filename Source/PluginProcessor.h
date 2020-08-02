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
    bool isNoteCurrentlyPlaying();
    
    //==============================================================================
    // Getters (+ some setters) for various members
    AudioProcessorValueTreeState& getValueTreeState() { return _valueTreeState; }
    const std::vector<int>& getActiveMidiNotes() const { return _activeMidiNotes; }
    const int getCurrMidiNoteIdx() const { return _currMidiNoteIdx; }
    const std::vector<MagikarpSequence>& getSequences() const { return _sequences; }
    const std::vector<int>& getCurrSequenceIndices() const { return _currSequenceIndices; }
    const MagikarpNotePolyphony getSequencePolyphony() const { return _sequencePolyphony; }
    void setSequencePolyphony(MagikarpNotePolyphony polyphony) { _sequencePolyphony = polyphony; }

private:
    //==============================================================================
    
    // Add/remove new MIDI notes to active MIDI notes depending on on/off statuses
    void handleNewMidiNote(int midiNote, bool isNoteOn, bool isNoteOff);
    
    // Calculate note duration based on numerator and denominator
    int calculateNoteDuration() const;
    
    // Create parameter layout for value tree state
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
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
    float _arpNoteLengthScalar = 0.9f;
    
    //==============================================================================
    // Sequencing
    std::vector<MagikarpSequence> _sequences;
    std::vector<int> _currSequenceIndices;
    int _numSequences = 5;
    MagikarpNotePolyphony _sequencePolyphony = kNotePolyphonyPoly;
    
    //==============================================================================
    // Audio
    float _sampleRate;
    
    //==============================================================================
    // Value tree state
    AudioProcessorValueTreeState _valueTreeState;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MagikarpAudioProcessor)
};
