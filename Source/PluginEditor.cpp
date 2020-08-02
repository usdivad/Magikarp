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
    const int sliderTextWidth = 40;
    const int sliderTextHeight = 20;
    
    // Numerator
    _numeratorSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _numeratorSlider.setTextBoxStyle(Slider::TextBoxBelow, false, sliderTextWidth, sliderTextHeight);
    addAndMakeVisible(_numeratorSlider);
    
    // _numeratorLabel.setText("Numerator", NotificationType::dontSendNotification);
    // _numeratorLabel.setJustificationType(Justification::centredTop);
    // _numeratorLabel.attachToComponent(&_numeratorSlider, false);
    
    _numeratorAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTreeState(), "NUMERATOR", _numeratorSlider);
    
    // Denominator
    _denominatorSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _denominatorSlider.setTextBoxStyle(Slider::TextBoxBelow, false, sliderTextWidth, sliderTextHeight);
    addAndMakeVisible(_denominatorSlider);
    
    // _denominatorLabel.setText("Denominator", NotificationType::dontSendNotification);
    // _denominatorLabel.setJustificationType(Justification::centredTop);
    // _denominatorLabel.attachToComponent(&_denominatorSlider, false);
    
    _denominatorAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTreeState(), "DENOMINATOR", _denominatorSlider);
    
    // Sequence polyphony
    _sequencePolyphonyComboBox.addItem("mono", kNotePolyphonyMono);
    _sequencePolyphonyComboBox.addItem("poly", kNotePolyphonyPoly);
    _sequencePolyphonyComboBox.setSelectedId(kNotePolyphonyPoly);
    _sequencePolyphonyAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.getValueTreeState(), "POLYPHONY", _sequencePolyphonyComboBox);
    addAndMakeVisible(_sequencePolyphonyComboBox);

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
    
    const std::vector<MagikarpSequence>& sequences = processor.getSequences();
    const std::vector<int>& currSequenceIndices = processor.getCurrSequenceIndices();
    const MagikarpNotePolyphony sequencePolyphony = processor.getSequencePolyphony();
    const int numSequencesToDraw = sequencePolyphony == kNotePolyphonyPoly ? sequences.size() : 1;
    
    for (int si=0; si<numSequencesToDraw; si++)
    {
        // Get sequence info
        const MagikarpSequence& sequence = sequences[si];
        const std::vector<bool>& rhythm = sequence.getRhythm();
        int currSequenceIdx = currSequenceIndices[si];
        currSequenceIdx = currSequenceIdx > 0 ? currSequenceIdx - 1 : (int)rhythm.size() - 1;
        
        // Print rhythm
        String r = "";
        for (int ri=0; ri<rhythm.size(); ri++)
        {
            r.append(rhythm[ri] ? "1" : "0", 1);
        }
        DBG("" << si << ": currSeqIdx=" << currSequenceIdx << ", rhythm: " << r << ", " << "poly: " << sequencePolyphony);
        
        // Calculate circle properties
        const float circleRadius = sequencePolyphony == kNotePolyphonyPoly ? 25 * (si + 1) : 100;
        const float circleX = getWidth() / 2;
        const float circleY = getHeight() / 2;
        const float circleArcStep = MathConstants<float>::twoPi / rhythm.size();
        const float circleArcPadding = sequencePolyphony == kNotePolyphonyMono ? circleArcStep * (0.1f - (0.05f * si / rhythm.size())) : circleArcStep * 0.2f;
        
        // Draw
        for (int i=0; i<rhythm.size(); i++)
        {
            Path p;
            
            bool isOnset = rhythm[i];
            bool isCurrSequenceIdx = i == currSequenceIdx;
            
            Colour arcColour = Colours::grey;
            float arcThickness = 2;
            
            if (isOnset)
            {
                arcColour = Colours::lightgrey;
                arcThickness = 5;
            }
            
            if (isCurrSequenceIdx && processor.isNoteCurrentlyPlaying())
            {
                if (isOnset)
                {
                    arcColour = Colours::mediumseagreen;
                }
                arcThickness *= 2;
            }
        

            float fromRadians = (circleArcStep * i) + circleArcPadding;
            float toRadians = (circleArcStep * (i + 1)) - circleArcPadding;
            p.addCentredArc(circleX, circleY, circleRadius, circleRadius, 0.0f, fromRadians, toRadians, true);
            
            g.setColour(arcColour);
            g.strokePath(p, PathStrokeType(arcThickness));
        }
    }
}

void MagikarpAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    const int w = 75;
    const int h = w;
    const int centerX = (getWidth() * 0.5f) - (w * 0.5f);
    const int centerY = (getHeight() * 0.5f) - (h * 0.5f);
    
    const int numY = centerY - (h * 0.5f);
    const int denY = centerY + (h * 0.5f);
    
    const int polyX = getWidth() * 0.75f;
    const int polyY = centerY;

    _numeratorSlider.setBounds(centerX, numY, w, h);
    _denominatorSlider.setBounds(centerX, denY, w, h);
    _sequencePolyphonyComboBox.setBounds(polyX, polyY, w, h*0.5f);
}

void MagikarpAudioProcessorEditor::timerCallback()
{
    repaint();
}
