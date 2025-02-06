#pragma once

#include <JuceHeader.h>
#include "constants.h"

class TrackSeqComponent : public juce::Component, public juce::ScrollBar::Listener
{
protected:
    struct MidiNote {
        int startStep;
        int pitch;
        int length;
    };

    juce::Array<MidiNote> midiNotes;
    juce::ScrollBar verticalScrollbar{ true };
    int viewOffsetY = 0;

public:
    TrackSeqComponent()
    {
        // Example MIDI Notes (Step, Pitch, Length)
        midiNotes.add({ 0, 60, 4 });   // C4 spanning 4 steps
        midiNotes.add({ 4, 62, 2 });   // D4 spanning 2 steps
        midiNotes.add({ 8, 64, 8 });   // E4 spanning 8 steps
        midiNotes.add({ 16, 67, 4 });  // G4 spanning 4 steps
        midiNotes.add({ 20, 69, 6 });  // A4 spanning 6 steps
        midiNotes.add({ 24, 72, 6 });  // C5 spanning 6 steps
        midiNotes.add({ 32, 75, 6 });  // D#5 spanning 6 steps
        midiNotes.add({ 40, 77, 6 });  // F5 spanning 6 steps
        midiNotes.add({ 48, 80, 6 });  // G#5 spanning 6 steps
        midiNotes.add({ 56, 83, 6 });  // B5 spanning 6 steps

        addAndMakeVisible(verticalScrollbar);
        verticalScrollbar.setRangeLimits(0, 127);
        verticalScrollbar.setCurrentRange(0, 12);
        verticalScrollbar.addListener(this);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(bgColour);

        int numSteps = 64;
        int numNotes = 36; // Visible range
        int stepWidth = getWidth() / numSteps;
        int noteHeight = (getHeight() - 20) / numNotes; // Leave 20px for header

        int midiRangeStart = verticalScrollbar.getCurrentRangeStart(); 

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

        // Draw Grid with Piano Roll Styling & Note Names
        for (int i = 0; i < numNotes; ++i)
        {
            int midiPitch = midiRangeStart + i;
            int y = 20 + i * noteHeight;
            bool isBlackKey = juce::MidiMessage::isMidiNoteBlack(midiPitch);
            g.setColour(isBlackKey ? juce::Colours::darkgrey : juce::Colours::lightgrey);
            g.fillRect(0, y, getWidth(), noteHeight);
            g.setColour(juce::Colours::grey);
            g.drawLine(0, y, getWidth(), y);

            // Draw note name at the start of every bar
            if (midiPitch <= 127 && (midiPitch % 12 == 0)) {
                g.setColour(juce::Colours::black);
                g.drawText(juce::MidiMessage::getMidiNoteName(midiPitch, true, true, 4), 
                           2, y, 40, noteHeight, juce::Justification::centredLeft);
            }
        }

        // Draw Beat & Bar Separations
        for (int i = 0; i <= numSteps; ++i)
        {
            int x = i * stepWidth;
            if (i % 16 == 0) g.setColour(juce::Colours::yellow); // Bar line
            else if (i % 4 == 0) g.setColour(juce::Colours::khaki); // Beat line
            else g.setColour(juce::Colours::grey);

            g.drawLine(x, 20, x, getHeight());
        }

        // Draw MIDI Notes
        for (const auto& note : midiNotes) {
            if (note.pitch >= midiRangeStart && note.pitch < midiRangeStart + numNotes)
            {
                int x = note.startStep * stepWidth;
                int y = 20 + (numNotes - (note.pitch - midiRangeStart) - 1) * noteHeight;
                int width = note.length * stepWidth;
                int height = noteHeight;

                g.setColour(juce::Colours::blue);
                g.fillRect(x, y, width, height);
                g.setColour(juce::Colours::white);
                g.drawRect(x, y, width, height, 1);
            }
        }
    }

    void resized() override
    {
        verticalScrollbar.setBounds(getWidth() - 10, 20, 10, getHeight() - 20);
    }

    void scrollBarMoved(juce::ScrollBar* scrollbar, double newRangeStart) override
    {
        if (scrollbar == &verticalScrollbar)
        {
            viewOffsetY = static_cast<int>(newRangeStart);
            repaint();
        }
    }

    int tabId = -1;
    void parentTabChanged(int newTabIndex, const juce::String& newTabName)
    {
    }
};
