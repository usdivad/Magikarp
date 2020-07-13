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

#include "MagikarpNoteModes.h"

class MagikarpSequence
{
    
public:
    
    MagikarpSequence();
    ~MagikarpSequence();
    
    // bool isRhythmOnsetAtIndex(const int i);
    
    void setRhythm(const std::vector<bool>& rhythm);
    const std::vector<bool>& getRhythm();
    
    void setNotePolyphony(const MagikarpNotePolyphony polyphony);
    const MagikarpNotePolyphony getNotePolyphony();
    
    void setNotePattern(const MagikarpNotePattern pattern);
    const MagikarpNotePattern getNotePattern();
    
private:
    
    std::vector<bool> _rhythm;
    MagikarpNotePolyphony _notePolyphony;
    MagikarpNotePattern _notePattern;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MagikarpSequence)
};

#endif /* MagikarpSequence_h */
