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
