#pragma once

#include "constants.h"
#include <JuceHeader.h>

class TrackSeqComponent : public juce::Component, public juce::ScrollBar::Listener {
protected:
    int headerHeight = 20;
    int noteHeight = 0;
    int stepWidth = 0;

    struct MidiNote {
        int startStep;
        int pitch;
        int length;
    };

    juce::Array<MidiNote> midiNotes;
    juce::ScrollBar verticalScrollbar { true };
    int viewOffsetY = 0;
    int numSteps = 64;
    int numNotes = 36; // Now starting from C0 upwards

    int getMidiRangeStart()
    {
        return (120 - 24) - verticalScrollbar.getCurrentRangeStart();
    }

public:
    juce::Colour color;

    TrackSeqComponent(juce::Colour color)
        : color(color)
    {
        // Example MIDI Notes (Step, Pitch, Length)
        midiNotes.add({ 0, 60, 4 }); // C4 spanning 4 steps
        midiNotes.add({ 4, 62, 2 }); // D4 spanning 2 steps
        midiNotes.add({ 8, 64, 8 }); // E4 spanning 8 steps
        midiNotes.add({ 16, 67, 4 }); // G4 spanning 4 steps
        midiNotes.add({ 20, 69, 6 }); // A4 spanning 6 steps
        midiNotes.add({ 24, 72, 6 }); // C5 spanning 6 steps
        midiNotes.add({ 32, 75, 6 }); // D#5 spanning 6 steps
        midiNotes.add({ 40, 77, 6 }); // F5 spanning 6 steps
        midiNotes.add({ 48, 80, 6 }); // G#5 spanning 6 steps
        midiNotes.add({ 56, 83, 6 }); // B5 spanning 6 steps

        addAndMakeVisible(verticalScrollbar);
        verticalScrollbar.setRangeLimits(12, 120); // C0 to C9 range
        verticalScrollbar.setCurrentRange(12, 12 + numNotes);
        verticalScrollbar.setCurrentRangeStart(((120 - 12) / 2) + 12 - numNotes / 2);
        verticalScrollbar.setColour(juce::ScrollBar::thumbColourId, color);

        if (midiNotes.size() > 0) {
            // Find min and max pitch from the MIDI notes
            int minPitch = 127; // Max possible MIDI pitch
            int maxPitch = 0; // Min possible MIDI pitch

            for (const auto& note : midiNotes) {
                if (note.pitch < minPitch)
                    minPitch = note.pitch;
                if (note.pitch > maxPitch)
                    maxPitch = note.pitch;
            }

            // Calculate the center of the active note range
            int midiRange = maxPitch - minPitch;
            int midiCenter = (minPitch + maxPitch) / 2;

            // Make sure we keep it within valid scroll limits
            int scrollStart = juce::jlimit(12, 120 - numNotes, midiCenter - numNotes / 2 - midiRange / 2);
            verticalScrollbar.setCurrentRangeStart(scrollStart);
        }

        verticalScrollbar.addListener(this);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(bgColour);

        int midiRangeStart = getMidiRangeStart();

        // Draw Step Headers
        g.setColour(seqHeaderColour);
        g.fillRect(0, 0, getWidth(), headerHeight);

        g.setColour(seqHeaderTextColour);
        g.setFont(juce::Font(juce::FontOptions(11.0f)));
        for (int i = 0; i < numSteps; ++i) {
            int x = i * stepWidth;
            g.drawLine(x, 0, x, getHeight(), (i % 4 == 0) ? 2.0f : 1.0f);

            // Calculate the center of the text to rotate around
            float centerX = x + stepWidth / 2.0f;
            float centerY = 10.0f; // Adjust based on desired pivot point

            g.saveState(); // Save the current state

            // Rotate the text by -90 degrees (or any desired angle)
            g.addTransform(juce::AffineTransform::rotation(juce::MathConstants<float>::pi * -0.25f, centerX, centerY));

            // Draw the rotated text
            g.drawText(juce::String(i + 1), x, 0, stepWidth, headerHeight, juce::Justification::centred);
            g.restoreState();
        }

        // Draw Grid with Piano Roll Styling & Note Names
        for (int i = 0; i < numNotes; ++i) {
            int midiPitch = midiRangeStart + (numNotes - i - 1); // Flip order so high notes are on top
            int y = headerHeight + i * noteHeight;
            bool isBlackKey = juce::MidiMessage::isMidiNoteBlack(midiPitch);
            g.setColour(isBlackKey ? seqRowBlackKeyColour : seqRowWhiteKeyColour);
            g.fillRect(0, y, getWidth(), noteHeight);
            g.setColour(seqRowSeparatorColour);
            g.drawLine(0, y, getWidth(), y);

            // g.setColour(midiPitch % 12 == 0 ? seqNoteColour : seqNoteLighterColour);
            if (midiPitch % 12 == 0) {
                g.setColour(seqNoteColour);
                g.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
            } else {
                g.setColour(seqNoteLighterColour);
                g.setFont(juce::Font(juce::FontOptions(10.0f, juce::Font::plain)));
            }
            g.drawText(juce::MidiMessage::getMidiNoteName(midiPitch, true, true, 4), 2, y, 40, noteHeight, juce::Justification::centredLeft);
            g.drawText(juce::MidiMessage::getMidiNoteName(midiPitch, true, true, 4), 2 + stepWidth * 16, y, 40, noteHeight, juce::Justification::centredLeft);
            g.drawText(juce::MidiMessage::getMidiNoteName(midiPitch, true, true, 4), 2 + stepWidth * 32, y, 40, noteHeight, juce::Justification::centredLeft);
            g.drawText(juce::MidiMessage::getMidiNoteName(midiPitch, true, true, 4), 2 + stepWidth * 48, y, 40, noteHeight, juce::Justification::centredLeft);
        }

        // Draw Beat & Bar Separations
        for (int i = 0; i <= numSteps; ++i) {
            int x = i * stepWidth;
            if (i % 16 == 0)
                g.setColour(seqBarColour); // Bar line
            else if (i % 4 == 0)
                g.setColour(seqBeatColour); // Beat line
            else
                g.setColour(seqColSeparatorColour);

            g.drawLine(x, headerHeight, x, getHeight());
        }

        g.setFont(juce::Font(juce::FontOptions(10.0f, juce::Font::plain)));
        // Draw MIDI Notes
        for (const auto& note : midiNotes) {
            if (note.pitch >= midiRangeStart && note.pitch < midiRangeStart + numNotes) {
                int x = note.startStep * stepWidth;
                int y = headerHeight + (numNotes - (note.pitch - midiRangeStart) - 1) * noteHeight;
                int width = note.length * stepWidth;
                int height = noteHeight;

                g.setColour(color);
                g.fillRect(x, y, width, height);

                g.setColour(juce::Colours::white);
                g.drawText(juce::MidiMessage::getMidiNoteName(note.pitch, true, true, 4), x + (note.length == 1 ? 0 : 2), y, width, noteHeight, juce::Justification::centredLeft);
            }
        }
    }

    void resized() override
    {
        noteHeight = (getHeight() - headerHeight) / numNotes; // Leave 20px for header
        stepWidth = getWidth() / numSteps;

        verticalScrollbar.setBounds(getWidth() - 10, headerHeight, 10, getHeight() - headerHeight);
    }

    void scrollBarMoved(juce::ScrollBar* scrollbar, double newRangeStart) override
    {
        if (scrollbar == &verticalScrollbar) {
            viewOffsetY = static_cast<int>(newRangeStart);
            repaint();
        }
    }

    bool keyPressed(const juce::KeyPress& key) override
    {
        return false;
    }

    double dragStartPosition = 0.0;
    void mouseDown(const juce::MouseEvent& event) override
    {
        dragStartPosition = event.position.y;
    }
    void mouseDrag(const juce::MouseEvent& event) override
    {
        if (event.mods.isMiddleButtonDown()) {
            double deltaY = (event.position.y - dragStartPosition) / 24;
            verticalScrollbar.setCurrentRangeStart(verticalScrollbar.getCurrentRangeStart() - deltaY);
            dragStartPosition = event.position.y;
        } else if (event.mods.isLeftButtonDown()) {
            // double deltaY = (event.position.y - dragStartPosition);
            // if (deltaY >= noteHeight) {

            // }
        }
    }

    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override
    {
        int midiRangeStart = getMidiRangeStart();
        int stepWidth = getWidth() / numSteps;
        int noteHeight = (getHeight() - headerHeight) / numNotes;

        MidiNote* note = getMidiNoteAtPosition(event);
        if (note != nullptr) {
            int delta = (wheel.deltaY > 0) ? 1 : -1;
            note->length = juce::jmax(1, note->length + delta);
            repaint();
            return;
        }

        // If not on a note, scroll normally
        verticalScrollbar.setCurrentRangeStart(verticalScrollbar.getCurrentRangeStart() - wheel.deltaY * 6);
        repaint();
    }

    MidiNote* getMidiNoteAtPosition(const juce::MouseEvent& event)
    {
        int midiRangeStart = getMidiRangeStart();
        for (auto& note : midiNotes) {
            int y = headerHeight + (numNotes - (note.pitch - midiRangeStart) - 1) * noteHeight;
            // If mouse is over a note, change length instead of scrolling
            if (event.y >= y && event.y < y + noteHeight && event.x >= note.startStep * stepWidth && event.x < (note.startStep + note.length) * stepWidth) {
                return &note;
            }
        }
        return nullptr;
    }

    int tabId = -1;
    void parentTabChanged(int newTabIndex, const juce::String& newTabName)
    {
    }
};
