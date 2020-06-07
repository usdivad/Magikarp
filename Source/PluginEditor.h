/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MagikarpAudioProcessorEditor  : public AudioProcessorEditor,
                                      public Timer
{
public:
    MagikarpAudioProcessorEditor (MagikarpAudioProcessor&);
    ~MagikarpAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void timerCallback() override;

private:
    //==============================================================================

    Slider _numeratorSlider;
    Label _numeratorLabel;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _numeratorAttachment;
    
    Slider _denominatorSlider;
    Label _denominatorLabel;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _denominatorAttachment;
    
    //==============================================================================
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MagikarpAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MagikarpAudioProcessorEditor)
};
