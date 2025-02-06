#pragma once

#include <JuceHeader.h>
#include "constants.h"

class TrackSeqComponent : public juce::Component {
protected:
    struct MidiNote {
        int startStep;
        int pitch;
        int length;
    };

    juce::Array<MidiNote> midiNotes;

public:
    TrackSeqComponent()
    {
        // Example MIDI Notes (Step, Pitch, Length)
        midiNotes.add({ 0, 60, 4 }); // C4 spanning 4 steps
        midiNotes.add({ 4, 62, 2 }); // D4 spanning 2 steps
        midiNotes.add({ 8, 64, 8 }); // E4 spanning 8 steps
        midiNotes.add({ 16, 67, 4 }); // G4 spanning 4 steps
        midiNotes.add({ 20, 69, 6 }); // A4 spanning 6 steps
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(bgColour);

        int numSteps = 64;
        int numNotes = 12; // Representing an octave range
        int stepWidth = getWidth() / numSteps;
        int noteHeight = (getHeight() - 20) / numNotes; // Leave 20px for header

        // Draw Step Headers
        g.setColour(juce::Colours::white);
        g.fillRect(0, 0, getWidth(), 20);
        g.setColour(juce::Colours::black);
        for (int i = 0; i < numSteps; ++i)
        {
            int x = i * stepWidth;
            g.drawLine(x, 0, x, getHeight(), (i % 4 == 0) ? 2.0f : 1.0f);
            g.drawText(juce::String(i + 1), x + 2, 0, stepWidth, 20, juce::Justification::centred);
        }

        // Draw Grid with Piano Roll Styling
        for (int i = 0; i < numNotes; ++i)
        {
            int y = 20 + i * noteHeight;
            bool isBlackKey = juce::MidiMessage::isMidiNoteBlack(60 + i);
            g.setColour(isBlackKey ? juce::Colours::darkgrey : juce::Colours::lightgrey);
            g.fillRect(0, y, getWidth(), noteHeight);
            g.setColour(juce::Colours::grey);
            g.drawLine(0, y, getWidth(), y);
        }

        // Draw Beat & Bar Separations
        for (int i = 0; i <= numSteps; ++i)
        {
            int x = i * stepWidth;
            if (i % 16 == 0) g.setColour(juce::Colours::yellow); // Bar line
            else if (i % 4 == 0) g.setColour(juce::Colours::darkgrey); // Beat line
            else g.setColour(juce::Colours::grey);

            g.drawLine(x, 20, x, getHeight());
        }

        // Draw MIDI Notes
        for (const auto& note : midiNotes) {
            int x = note.startStep * stepWidth;
            int y = 20 + ((127 - note.pitch) % numNotes) * noteHeight; // Mapping pitch to vertical grid
            int width = note.length * stepWidth;
            int height = noteHeight;

            g.setColour(juce::Colours::blue);
            g.fillRect(x, y, width, height);
            g.setColour(juce::Colours::white);
            g.drawRect(x, y, width, height, 1);
        }
    }

    void resized() override
    {
    }

    int tabId = -1;
    void parentTabChanged(int newTabIndex, const juce::String& newTabName)
    {
    }
};
