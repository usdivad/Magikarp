/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MagikarpAudioProcessorEditor::MagikarpAudioProcessorEditor (MagikarpAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // ================================================================
    // Set timer
    startTimer(20);
}

MagikarpAudioProcessorEditor::~MagikarpAudioProcessorEditor()
{
}

//==============================================================================
void MagikarpAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);

    // Display active MIDI notes
    const std::vector<int>& activeMidiNotes = processor.getActiveMidiNotes();
    std::stringstream activeMidiNotesStrStream;
    activeMidiNotesStrStream << "Active MIDI notes: {";
    for (auto it = activeMidiNotes.begin(); it != activeMidiNotes.end(); it++)
    {
        if (it != activeMidiNotes.begin())
            activeMidiNotesStrStream << ", ";
        activeMidiNotesStrStream << *it;
    }
    activeMidiNotesStrStream << "}";
    std::string activeMidiNotesStr = activeMidiNotesStrStream.str();
    g.drawFittedText(activeMidiNotesStr, getLocalBounds(), Justification::centred, 1);
}

void MagikarpAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void MagikarpAudioProcessorEditor::timerCallback()
{
    repaint();
}
