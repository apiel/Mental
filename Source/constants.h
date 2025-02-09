#pragma once

#include <JuceHeader.h>

juce::Colour sidebarColour = juce::Colours::black.brighter(0.1);
juce::Colour bgColour = juce::Colours::black.brighter(0.2);

juce::Colour seqHeaderColour = juce::Colours::black.brighter(0.70);
juce::Colour seqHeaderTextColour = juce::Colours::black.brighter(0.10);
juce::Colour seqRowWhiteKeyColour = juce::Colours::black.brighter(0.3);
juce::Colour seqRowBlackKeyColour = juce::Colours::black.brighter(0.35);
juce::Colour seqRowSeparatorColour = juce::Colours::black.brighter(0.40);
juce::Colour seqColSeparatorColour = juce::Colours::black.brighter(0.45);
juce::Colour seqBarColour = juce::Colours::black.brighter(0.08);
juce::Colour seqBeatColour = juce::Colours::black.brighter(0.20);
juce::Colour seqNoteColour = juce::Colours::white.darker(0.10);
juce::Colour seqNoteLighterColour = juce::Colours::black.brighter(0.70);

const int MAX_STEPS = 64;
const int TRACK_COUNT = 4;

enum TrackStatus {
    MUTED = 0,
    ON = 1,
    NEXT = 2
};
