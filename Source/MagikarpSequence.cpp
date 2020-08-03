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
    _notePolyphony(MagikarpNotePolyphony::kNotePolyphonyMono),
    _notePattern(MagikarpNotePattern::kNotePatternInputOrder)
{
    
}

MagikarpSequence::~MagikarpSequence()
{
    
}

std::vector<bool> MagikarpSequence::generateEuclideanRhythm(int pulses, int steps)
{
    // We can't have more pulses than steps
    if (pulses > steps)
    {
        pulses = steps;
    }
    
    // Compute count and remainder arrays
    std::vector<int> remainder = std::vector<int>(steps, 0);
    std::vector<int> count = std::vector<int>(steps, 0);
    int divisor = steps - pulses;
    remainder[0] = pulses;
    int level = 0;
    
    while (remainder[level] > 1)
    {
        count[level] = divisor / remainder[level];
        remainder[level+1] = divisor % remainder[level];
        divisor = remainder[level];
        level++;
    }
    
    count[level] = divisor;
    
    // Build rhythm
    std::vector<bool> rhythm = std::vector<bool>();
    buildRhythm(level, rhythm, count, remainder);
    
    // Debug printing
    // String r = "";
    // for (int ri=0; ri<rhythm.size(); ri++)
    // {
    //     r.append(rhythm[ri] ? "1" : "0", 1);
    // }
    // DBG("generateEuclideanRhythm(" << pulses << "," << steps << ")=" << r);
    
    return rhythm;
}

void MagikarpSequence::buildRhythm(int level, std::vector<bool>& rhythm, std::vector<int>& count, std::vector<int>& remainder)
{
    if (level == -1)
    {
        rhythm.push_back(false);
    }
    else if (level == -2)
    {
        rhythm.push_back(true);
    }
    else
    {
        for (int i=0; i<count[level]; i++)
        {
            buildRhythm(level-1, rhythm, count, remainder);
        }
        
        if (remainder[level] != 0)
        {
            buildRhythm(level-2, rhythm, count, remainder);
        }
    }
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
