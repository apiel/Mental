#pragma once

#include <JuceHeader.h>

#include "NoteToolboxComponent.h"
#include "Step.h"
#include "constants.h"

class TrackSeqComponent : public juce::Component, public juce::ScrollBar::Listener {
protected:
    int headerHeight = 20;
    int stepHeight = 0;
    int stepWidth = 0;

    juce::Array<Step> steps;
    juce::ScrollBar verticalScrollbar { true };
    int viewOffsetY = 0;
    int numSteps = 64;
    int numNotes = 36; // Now starting from C0 upwards

    int getMidiRangeStart()
    {
        return (120 - 24) - verticalScrollbar.getCurrentRangeStart();
    }

    std::unique_ptr<NoteToolboxComponent> toolbox = nullptr;
    void showToolbox()
    {
        if (!toolbox) {
            toolbox = std::make_unique<NoteToolboxComponent>();
            addAndMakeVisible(toolbox.get());
        }

        toolbox->setNoteDetails(selectedNote->velocity, selectedNote->length);
        toolbox->onVelocityChange = [this](float newVelocity) {
            if (selectedNote) {
                selectedNote->velocity = newVelocity;
                repaint();
            }
        };
        toolbox->onLengthChange = [this](int newLength) {
            if (selectedNote) {
                selectedNote->length = newLength;
                repaint();
            }
        };
        toolbox->onDelete = [this]() {
            if (selectedNote) {
                steps.remove(selectedNote);
                selectedNote = nullptr;
                toolbox.reset();
                repaint();
            }
        };

        updateToolboxBounds();
    }

    int toolboxWidth = 600;
    int toolboxHeight = 60;
    void updateToolboxBounds()
    {
        if (toolbox) {
            toolbox->setBounds(getWidth() - toolboxWidth - 20, getHeight() - toolboxHeight - 20, toolboxWidth, toolboxHeight);
        }
    }

public:
    juce::Colour color;

    TrackSeqComponent(juce::Colour color)
        : color(color)
    {
        // Example MIDI Notes (Step, Pitch, Length)
        steps.add({ 0, 60, 4 }); // C4 spanning 4 steps
        steps.add({ 4, 62, 2 }); // D4 spanning 2 steps
        steps.add({ 8, 64, 8 }); // E4 spanning 8 steps
        steps.add({ 16, 67, 4 }); // G4 spanning 4 steps
        steps.add({ 20, 69, 6 }); // A4 spanning 6 steps
        steps.add({ 24, 72, 6 }); // C5 spanning 6 steps
        steps.add({ 32, 75, 6 }); // D#5 spanning 6 steps
        steps.add({ 40, 77, 6 }); // F5 spanning 6 steps
        steps.add({ 48, 80, 6 }); // G#5 spanning 6 steps
        steps.add({ 56, 83, 6 }); // B5 spanning 6 steps

        addAndMakeVisible(verticalScrollbar);
        verticalScrollbar.setRangeLimits(12, 120); // C0 to C9 range
        verticalScrollbar.setCurrentRange(12, 12 + numNotes);
        verticalScrollbar.setCurrentRangeStart(((120 - 12) / 2) + 12 - numNotes / 2);
        verticalScrollbar.setColour(juce::ScrollBar::thumbColourId, color);

        if (steps.size() > 0) {
            // Find min and max pitch from the MIDI steps
            int minPitch = 127; // Max possible MIDI pitch
            int maxPitch = 0; // Min possible MIDI pitch

            for (const auto& step : steps) {
                if (step.pitch < minPitch)
                    minPitch = step.pitch;
                if (step.pitch > maxPitch)
                    maxPitch = step.pitch;
            }

            // Calculate the center of the active step range
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
            int midiPitch = midiRangeStart + (numNotes - i - 1); // Flip order so high steps are on top
            int y = headerHeight + i * stepHeight;
            bool isBlackKey = juce::MidiMessage::isMidiNoteBlack(midiPitch);
            g.setColour(isBlackKey ? seqRowBlackKeyColour : seqRowWhiteKeyColour);
            g.fillRect(0, y, getWidth(), stepHeight);
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
            g.drawText(juce::MidiMessage::getMidiNoteName(midiPitch, true, true, 4), 2, y, 40, stepHeight, juce::Justification::centredLeft);
            g.drawText(juce::MidiMessage::getMidiNoteName(midiPitch, true, true, 4), 2 + stepWidth * 16, y, 40, stepHeight, juce::Justification::centredLeft);
            g.drawText(juce::MidiMessage::getMidiNoteName(midiPitch, true, true, 4), 2 + stepWidth * 32, y, 40, stepHeight, juce::Justification::centredLeft);
            g.drawText(juce::MidiMessage::getMidiNoteName(midiPitch, true, true, 4), 2 + stepWidth * 48, y, 40, stepHeight, juce::Justification::centredLeft);
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
        for (const auto& step : steps) {
            if (step.pitch >= midiRangeStart && step.pitch < midiRangeStart + numNotes) {
                int x = step.startStep * stepWidth;
                int y = headerHeight + (numNotes - (step.pitch - midiRangeStart) - 1) * stepHeight;
                int width = step.length * stepWidth;
                int height = stepHeight;

                g.setColour(color);
                g.fillRect(x, y, width, height);

                g.setColour(juce::Colours::white);
                g.drawText(juce::MidiMessage::getMidiNoteName(step.pitch, true, true, 4), x + (step.length == 1 ? 0 : 2), y, width, stepHeight, juce::Justification::centredLeft);
            }
        }
    }

    void resized() override
    {
        stepHeight = (getHeight() - headerHeight) / numNotes; // Leave 20px for header
        stepWidth = getWidth() / numSteps;

        verticalScrollbar.setBounds(getWidth() - 10, headerHeight, 10, getHeight() - headerHeight);
        updateToolboxBounds();
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

    double dragStartPositionY = 0.0;
    double dragStartPositionX = 0.0;
    Step* selectedNote = nullptr;
    void mouseDown(const juce::MouseEvent& event) override
    {
        dragStartPositionY = event.position.y;
        dragStartPositionX = event.position.x;
        selectedNote = getMidiNoteAtPosition(dragStartPositionX, dragStartPositionY);
        if (selectedNote != nullptr) {
            if (event.mods.isRightButtonDown()) {
                steps.remove(selectedNote);
                repaint();
            } else {
                showToolbox();
            }
        } else {
            toolbox.reset();
        }
    }

    void mouseDoubleClick(const juce::MouseEvent& event) override
    {
        if (getMidiNoteAtPosition(event.position.x, event.position.y) == nullptr) {
            int clickedStep = event.position.x / stepWidth;
            int clickedPitch = getMidiRangeStart() + (numNotes - (event.position.y - headerHeight) / stepHeight);
            steps.add({ clickedStep, clickedPitch, 4 });
            selectedNote = &steps.getReference(steps.size() - 1);
            showToolbox();
            repaint();
        }
    }

    void mouseDrag(const juce::MouseEvent& event) override
    {
        if (event.mods.isMiddleButtonDown()) {
            double deltaY = (event.position.y - dragStartPositionY) / 24;
            verticalScrollbar.setCurrentRangeStart(verticalScrollbar.getCurrentRangeStart() - deltaY);
            dragStartPositionY = event.position.y;
        } else if (selectedNote != nullptr && event.mods.isLeftButtonDown()) {
            double deltaY = (event.position.y - dragStartPositionY);
            if (deltaY >= stepHeight) {
                selectedNote->pitch -= (int)(deltaY / stepHeight);
                repaint();
                dragStartPositionY = event.position.y;
            } else if (-deltaY >= stepHeight) {
                selectedNote->pitch += (int)((-deltaY) / stepHeight);
                repaint();
                dragStartPositionY = event.position.y;
            }

            double deltaX = (event.position.x - dragStartPositionX);
            if (deltaX >= stepWidth) {
                selectedNote->startStep += (int)(deltaX / stepWidth);
                repaint();
                dragStartPositionX = event.position.x;
            } else if (-deltaX >= stepWidth) {
                selectedNote->startStep -= (int)((-deltaX) / stepWidth);
                repaint();
                dragStartPositionX = event.position.x;
            }
        }
    }

    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override
    {
        int midiRangeStart = getMidiRangeStart();
        int stepWidth = getWidth() / numSteps;
        int stepHeight = (getHeight() - headerHeight) / numNotes;

        Step* step = getMidiNoteAtPosition(event.x, event.y);
        if (step != nullptr) {
            int delta = (wheel.deltaY > 0) ? 1 : -1;
            step->length = juce::jmax(1, step->length + delta);
            repaint();
            return;
        }

        // If not on a step, scroll normally
        verticalScrollbar.setCurrentRangeStart(verticalScrollbar.getCurrentRangeStart() - wheel.deltaY * 6);
        repaint();
    }

    // TODO when key A is pressed, add a new step after the last edited/move step
    // if no step has been edited, then add a new step after the last step
    // if there is no step at all then add a new step at the beginning in middle screeen..

    Step* getMidiNoteAtPosition(double eventX, double eventY)
    {
        int midiRangeStart = getMidiRangeStart();
        for (auto& step : steps) {
            int y = headerHeight + (numNotes - (step.pitch - midiRangeStart) - 1) * stepHeight;
            // If mouse is over a step, change length instead of scrolling
            if (eventY >= y && eventY < y + stepHeight && eventX >= step.startStep * stepWidth && eventX < (step.startStep + step.length) * stepWidth) {
                return &step;
            }
        }
        return nullptr;
    }

    int tabId = -1;
    void parentTabChanged(int newTabIndex, const juce::String& newTabName)
    {
    }
};
