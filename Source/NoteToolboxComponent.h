#pragma once

#include <JuceHeader.h>

class NoteToolboxComponent : public juce::Component {
public:
    std::function<void(int)> onPitchChange;
    std::function<void(int)> onLengthChange;
    std::function<void()> onDelete;

    NoteToolboxComponent()
    {
        addAndMakeVisible(pitchLabel);
        addAndMakeVisible(pitchSlider);
        addAndMakeVisible(lengthLabel);
        addAndMakeVisible(lengthSlider);
        addAndMakeVisible(deleteButton);

        pitchLabel.setText("Pitch:", juce::dontSendNotification);
        lengthLabel.setText("Length:", juce::dontSendNotification);

        pitchSlider.setRange(0, 127, 1);
        pitchSlider.onValueChange = [this] {
            if (onPitchChange)
                onPitchChange((int)pitchSlider.getValue());
        };

        lengthSlider.setRange(1, 16, 1);
        lengthSlider.onValueChange = [this] {
            if (onLengthChange)
                onLengthChange((int)lengthSlider.getValue());
        };

        deleteButton.setButtonText("Delete");
        deleteButton.onClick = [this] {
            if (onDelete)
                onDelete();
        };
    }

    void setNoteDetails(int pitch, int length)
    {
        pitchSlider.setValue(pitch, juce::dontSendNotification);
        lengthSlider.setValue(length, juce::dontSendNotification);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey.withAlpha(0.7f)); // Semi-transparent dark grey background
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(5);
        pitchLabel.setBounds(area.removeFromTop(20));
        pitchSlider.setBounds(area.removeFromTop(30));
        lengthLabel.setBounds(area.removeFromTop(20));
        lengthSlider.setBounds(area.removeFromTop(30));
        deleteButton.setBounds(area.removeFromTop(30));
    }

private:
    juce::Label pitchLabel, lengthLabel;
    juce::Slider pitchSlider, lengthSlider;
    juce::TextButton deleteButton;
};
