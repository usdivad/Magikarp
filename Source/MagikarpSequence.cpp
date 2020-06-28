//
//  MagikarpSequence.cpp
//  Magikarp - All
//
//  Created by David Su on 6/27/20.
//  Copyright © 2020 David Su. All rights reserved.
//

#include "MagikarpSequence.h"

MagikarpSequence::MagikarpSequence() :
    m_Rhythm({}),
    m_NotePolyphony(MagikarpNotePolyphony::kMono),
    m_NotePattern(MagikarpNotePattern::kInputOrder)
{
    
}

MagikarpSequence::~MagikarpSequence()
{
    
}

void MagikarpSequence::setRhythm(const std::vector<bool>& rhythm)
{
    m_Rhythm = rhythm;
}

const std::vector<bool>& MagikarpSequence::getRhythm()
{
    return m_Rhythm;
}

void MagikarpSequence::setNotePolyphony(const MagikarpNotePolyphony polyphony)
{
    m_NotePolyphony = polyphony;
}

const MagikarpNotePolyphony MagikarpSequence::getNotePolyphony()
{
    return m_NotePolyphony;
}

void MagikarpSequence::setNotePattern(const MagikarpNotePattern pattern)
{
    m_NotePattern = pattern;
}

const MagikarpNotePattern MagikarpSequence::getNotePattern()
{
    return m_NotePattern;
}
