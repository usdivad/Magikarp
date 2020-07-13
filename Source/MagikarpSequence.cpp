//
//  MagikarpSequence.cpp
//  Magikarp - All
//
//  Created by David Su on 6/27/20.
//  Copyright © 2020 David Su. All rights reserved.
//

#include "MagikarpSequence.h"

MagikarpSequence::MagikarpSequence() :
    _rhythm({}),
    _notePolyphony(MagikarpNotePolyphony::kMono),
    _notePattern(MagikarpNotePattern::kInputOrder)
{
    
}

MagikarpSequence::~MagikarpSequence()
{
    
}

void MagikarpSequence::setRhythm(const std::vector<bool>& rhythm)
{
    _rhythm = rhythm;
}

const std::vector<bool>& MagikarpSequence::getRhythm() const
{
    return _rhythm;
}

void MagikarpSequence::setNotePolyphony(const MagikarpNotePolyphony polyphony)
{
    _notePolyphony = polyphony;
}

const MagikarpNotePolyphony MagikarpSequence::getNotePolyphony() const
{
    return _notePolyphony;
}

void MagikarpSequence::setNotePattern(const MagikarpNotePattern pattern)
{
    _notePattern = pattern;
}

const MagikarpNotePattern MagikarpSequence::getNotePattern() const
{
    return _notePattern;
}
