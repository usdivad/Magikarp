/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "MagikarpSequence.h"

//==============================================================================
MagikarpAudioProcessorEditor::MagikarpAudioProcessorEditor (MagikarpAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Numerator
    _numeratorSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _numeratorSlider.setTextBoxStyle(Slider::TextBoxRight, false, 45, 25);
    addAndMakeVisible(_numeratorSlider);
    
    _numeratorLabel.setText("Numerator", NotificationType::dontSendNotification);
    _numeratorLabel.setJustificationType(Justification::centredTop);
    _numeratorLabel.attachToComponent(&_numeratorSlider, false);
    
    _numeratorAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTreeState(), "NUMERATOR", _numeratorSlider);
    
    // Denominator
    _denominatorSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _denominatorSlider.setTextBoxStyle(Slider::TextBoxRight, false, 45, 25);
    addAndMakeVisible(_denominatorSlider);
    
    _denominatorLabel.setText("Denominator", NotificationType::dontSendNotification);
    _denominatorLabel.setJustificationType(Justification::centredTop);
    _denominatorLabel.attachToComponent(&_denominatorSlider, false);
    
    _denominatorAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTreeState(), "DENOMINATOR", _denominatorSlider);
    
    // ================================================================
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
    g.drawFittedText(activeMidiNotesStr, getLocalBounds(), Justification::centredBottom, 1);
    
    // ================
    // Visualizing sequences and notes
    
    // Get sequence info
    MagikarpSequence sequence = processor.getSequence();
    int currSequenceIdx = (processor.getCurrSequenceIdx() - 1) % sequence.getRhythm().size();
    bool isCurrSequenceIdxAnOnset = sequence.getRhythm().empty() ? false : sequence.getRhythm()[currSequenceIdx];
    
    // Print rhythm
    String r = "";
    for (int i=0; i<sequence.getRhythm().size(); i++)
    {
        r.append(sequence.getRhythm()[i] ? "1" : "0", 1);
    }
    DBG("rhythm: " << r);
    
    // Calculate circle properties
    const float circleRadius = 100;
    const float circleX = (getWidth() / 5) - (circleRadius / 2);
    const float circleY = (getHeight() / 2) - (circleRadius / 2);
    const float circleThickness = isCurrSequenceIdxAnOnset && processor.isNoteCurrentlyPlaying() ? 10 : 2;
    
    // Draw
    g.setColour(Colours::white);
    g.drawEllipse(circleX, circleY, circleRadius, circleRadius, circleThickness);
}

void MagikarpAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    int w = 100;
    int h = 100;
    int centerX = (getWidth() * 0.5f) - (w * 0.5f);
    int centerY = (getHeight() * 0.5f) - (h * 0.5f);
    
    int numY = centerY - (h * 0.5f);
    int denY = centerY + (h * 0.5f);

    _numeratorSlider.setBounds(centerX, numY, w, h);
    _denominatorSlider.setBounds(centerX, denY, w, h);
}

void MagikarpAudioProcessorEditor::timerCallback()
{
    repaint();
}
