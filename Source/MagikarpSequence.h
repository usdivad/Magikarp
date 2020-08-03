//
//  MagikarpSequence.h
//  Magikarp - All
//
//  Created by David Su on 6/27/20.
//  Copyright © 2020 David Su. All rights reserved.
//

#ifndef MagikarpSequence_h
#define MagikarpSequence_h

#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"

#include "MagikarpNoteModes.h"

class MagikarpSequence
{
    
public:
    
    MagikarpSequence();
    ~MagikarpSequence();
    
    //==============================================================================
    // Euclidean rhythms :)
    
    static std::vector<bool> generateEuclideanRhythm(int pulses, int steps);
    
    //==============================================================================
    // Getters and setters
    
    void setRhythm(const std::vector<bool>& rhythm);
    const std::vector<bool>& getRhythm() const;
    
    void setNotePolyphony(const MagikarpNotePolyphony polyphony);
    const MagikarpNotePolyphony getNotePolyphony() const;
    
    void setNotePattern(const MagikarpNotePattern pattern);
    const MagikarpNotePattern getNotePattern() const;
    
    // bool isRhythmOnsetAtIndex(const int i);
    
private:
    
    //==============================================================================
    // Bjorklund's algorithm helpers
    static void buildRhythm(int level, std::vector<bool>& rhythm, std::vector<int>& count, std::vector<int>& remainder);
    
    
    //==============================================================================
    
    std::vector<bool> _rhythm;
    MagikarpNotePolyphony _notePolyphony;
    MagikarpNotePattern _notePattern;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MagikarpSequence)
};

#endif /* MagikarpSequence_h */
