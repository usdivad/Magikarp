/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MagikarpAudioProcessor::MagikarpAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     // : AudioProcessor (BusesProperties()
        : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::mono(), true)
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

MagikarpAudioProcessor::~MagikarpAudioProcessor()
{
}

//==============================================================================
const String MagikarpAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MagikarpAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MagikarpAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MagikarpAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MagikarpAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MagikarpAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MagikarpAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MagikarpAudioProcessor::setCurrentProgram (int index)
{
}

const String MagikarpAudioProcessor::getProgramName (int index)
{
    return {};
}

void MagikarpAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MagikarpAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    _sampleRate = static_cast<float>(sampleRate);
    
    _currMidiNoteNum = -1;
    _prevMidiNoteNum = -1;
}

void MagikarpAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MagikarpAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void MagikarpAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // ================================================================
    // MIDI
    
    if (!midiMessages.isEmpty())
    {
        // Get incoming messages
        MidiBuffer::Iterator midiMessagesIterator = MidiBuffer::Iterator(midiMessages);
        bool hasNewMidiMessages = true;
        while (hasNewMidiMessages)
        {
            MidiMessage midiMessage;
            int midiSamplePosition;
            hasNewMidiMessages = midiMessagesIterator.getNextEvent(midiMessage, midiSamplePosition);
            
            // Update active MIDI notes
            if (midiMessage.isNoteOnOrOff())
            {
                // TODO: Handle velocity
                int midiNote = midiMessage.getNoteNumber();
                handleNewMidiNote(midiNote, midiMessage.isNoteOn(), midiMessage.isNoteOff());
            }
        }
    }
    // DBG(_activeMidiNotes.size() << " active MIDI notes");
    
    // ================================================================
    // Playhead
    
    AudioPlayHead* playhead = getPlayHead();
    AudioPlayHead::CurrentPositionInfo positionInfo;
    playhead->getCurrentPosition(positionInfo);
    
    int prevBeatNum = _currBeatNum;
    
    // Update members
    _currBeatNum = static_cast<int>(positionInfo.ppqPosition);
    _isPlayheadPlaying = positionInfo.isPlaying;
    _currBpm = static_cast<float>(positionInfo.bpm);
    
    
    // ================================================================
    // MIDI playback
    
    // Clear buffer
    midiMessages.clear();
    
    // Update current beat num and MIDI note index for arp
    
    // Using playhead
    // TODO:
    // - Have this depend on arp rate
//     if (_currBeatNum != prevBeatNum)
//     {
//         // Turn off previous note
//         _prevMidiNoteNum = _currMidiNoteNum;
//         midiMessages.addEvent(MidiMessage::noteOff(1, _prevMidiNoteNum), 0);
//
//         // Turn on new note
//         if (_activeMidiNotes.size() > 0)
//         {
//             _currMidiNoteIdx = (_currMidiNoteIdx + 1) % _activeMidiNotes.size();
//             _currMidiNoteNum = _activeMidiNotes[_currMidiNoteIdx];
//
//             midiMessages.addEvent(MidiMessage::noteOn(1, _currMidiNoteNum, static_cast<uint8>(60)), 0);
//         }
//         else
//         {
//             _currMidiNoteIdx = 0;
//         }
//     }
//     DBG("_currMidiNoteIdx=" << _currMidiNoteIdx << ", " << "_currBeatNum=" << _currBeatNum);
    
    // Without playhead
    int noteDuration = calculateNoteDuration();
    int numSamples = buffer.getNumSamples();
    int timeElapsed = _currMidiNoteTimeElapsed + numSamples;
    
    int noteLength = (int) (noteDuration * 0.9f); // To account for legato/staccato eventually
    
    if (timeElapsed >= noteLength)
    {
        // Turn off current note
        if (_currMidiNoteNum > 0)
        {
            midiMessages.addEvent(MidiMessage::noteOff(1, _currMidiNoteNum), 0);
        }
    }
    
    if (timeElapsed >= noteDuration)
    {
        // Calculate sample offset
        int offset = jmax(0, jmin(noteDuration - _currMidiNoteTimeElapsed, numSamples - 1));
        
        // Set previous note
        _prevMidiNoteNum = _currMidiNoteNum;
        // if (_prevMidiNoteNum > 0)
        // {
        //     midiMessages.addEvent(MidiMessage::noteOff(1, _prevMidiNoteNum), 0);
        // }
        
        // Turn on new note
        if (_activeMidiNotes.size() > 0)
        {
            _currMidiNoteIdx = (_currMidiNoteIdx + 1) % _activeMidiNotes.size();
            _currMidiNoteNum = _activeMidiNotes[_currMidiNoteIdx];

            midiMessages.addEvent(MidiMessage::noteOn(1, _currMidiNoteNum, static_cast<uint8>(60)), 0);
        }
        else
        {
            _currMidiNoteIdx = 0;
        }
    }
    _currMidiNoteTimeElapsed = (_currMidiNoteTimeElapsed + numSamples) % noteDuration;
    // if (_activeMidiNotes.size() < 1)
    // {
    //     _currMidiNoteTimeElapsed = 0;
    // }
    
    DBG("noteIdx=" << _currMidiNoteIdx << ", noteDuration=" << noteDuration << ", timeElapsed=" << _currMidiNoteTimeElapsed << ", numSamples=" << numSamples);


    // ================================================================
    // Audio
    
    ScopedNoDenormals noDenormals;
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
bool MagikarpAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MagikarpAudioProcessor::createEditor()
{
    return new MagikarpAudioProcessorEditor (*this);
}

//==============================================================================
void MagikarpAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MagikarpAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================

// Returns reference to active MIDI notes
const std::vector<int>& MagikarpAudioProcessor::getActiveMidiNotes() const
{
    return _activeMidiNotes;
}

// Add/remove new MIDI notes to active MIDI notes depending on on/off statuses
void MagikarpAudioProcessor::handleNewMidiNote(int midiNote, bool isNoteOn, bool isNoteOff)
{
    auto midiNotesIterator = std::find(_activeMidiNotes.begin(), _activeMidiNotes.end(), midiNote);
    if (midiNotesIterator != _activeMidiNotes.end())
    {
        // auto midiNoteIdx = std::distance(_activeMidiNotes.begin(), midiNotesIterator);
        
        if (isNoteOff)
        {
            _activeMidiNotes.erase(midiNotesIterator);
        }
    }
    else
    {
        if (isNoteOn)
        {
            _activeMidiNotes.push_back(midiNote);
        }
    }
}

// Calculate note duration based on numerator and denominator
int MagikarpAudioProcessor::calculateNoteDuration() const
{
    return static_cast<int>((static_cast<float>(_arpSubdivisionNumerator) * 4.0f / static_cast<float>(_arpSubdivisionDenominator)) * (60.0f / _currBpm) * _sampleRate);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MagikarpAudioProcessor();
}
