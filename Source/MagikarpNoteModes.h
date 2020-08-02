//
//  MagikarpNoteModes.h
//  Magikarp - All
//
//  Created by David Su on 6/27/20.
//  Copyright © 2020 David Su. All rights reserved.
//

#ifndef MagikarpNoteModes_h
#define MagikarpNoteModes_h

enum MagikarpNotePolyphony
{
    kNotePolyphonyNone=0,
    kNotePolyphonyMono,
    kNotePolyphonyPoly
};

enum MagikarpNotePattern
{
    kNotePatternNone=0,
    kNotePatternInputOrder,
    kNotePatternInputOrderReversed,
    kNotePatternAscending,
    kNotePatternDescending,
    kNotePatternRandom
    // kTotalNumNotePatterns
};

#endif /* MagikarpNoteModes_h */
