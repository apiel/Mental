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
        addAndMakeVisible(lengthSelector);
        addAndMakeVisible(deleteButton);

        pitchLabel.setText("Pitch:", juce::dontSendNotification);
        lengthLabel.setText("Length:", juce::dontSendNotification);

        pitchSlider.setRange(0, 127, 1);
        pitchSlider.onValueChange = [this] {
            if (onPitchChange)
                onPitchChange((int)pitchSlider.getValue());
        };

        for (int i = 1; i <= 64; ++i)
            lengthSelector.addItem(juce::String(i), i);

        lengthSelector.onChange = [this] {
            if (onLengthChange)
                onLengthChange(lengthSelector.getSelectedId());
        };
        lengthSelector.setSelectedId(4, juce::dontSendNotification);

        deleteButton.setButtonText("");
        deleteButton.setClickingTogglesState(false);

        auto trashIcon = std::make_unique<juce::DrawablePath>();
        setTrashIcon(trashIcon.get());
        trashIcon->setFill(juce::Colours::grey);
        trashIcon->setStrokeFill(juce::Colours::white);
        trashIcon->setStrokeType(juce::PathStrokeType(0.5f));

        deleteButton.setImages(trashIcon.get());
        deleteButton.onClick = [this] {
            if (onDelete)
                onDelete();
        };
    }

    void setTrashIcon(juce::DrawablePath* icon)
    {
        juce::Path path;

        // Lid
        path.startNewSubPath(4, 4);
        path.lineTo(16, 4);
        // path.lineTo(16, 6);
        // path.lineTo(4, 6);
        path.closeSubPath();

        // Handle
        path.startNewSubPath(8, 2);
        path.lineTo(12, 2);
        path.lineTo(12, 4);
        path.lineTo(8, 4);
        path.closeSubPath();

        // Trash body
        path.startNewSubPath(6, 6);
        path.lineTo(14, 6);
        path.lineTo(13, 14);
        path.lineTo(7, 14);
        path.closeSubPath();

        // Vertical ridges
        path.startNewSubPath(8, 8);
        path.lineTo(8.5, 12);
        path.startNewSubPath(10, 8);
        path.lineTo(10, 12);
        path.startNewSubPath(12, 8);
        path.lineTo(11.5, 12);

        icon->setPath(path);
    }

    void setNoteDetails(int pitch, int length)
    {
        pitchSlider.setValue(pitch, juce::dontSendNotification);
        lengthSelector.setSelectedId(length, juce::dontSendNotification);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey.withAlpha(0.7f)); // Semi-transparent dark grey background
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(5);
        auto itemWidth = area.getWidth() / 5;
        pitchLabel.setBounds(area.removeFromLeft(itemWidth).reduced(2));
        pitchSlider.setBounds(area.removeFromLeft(itemWidth).reduced(2));
        lengthLabel.setBounds(area.removeFromLeft(itemWidth).reduced(2));
        lengthSelector.setBounds(area.removeFromLeft(itemWidth).reduced(2));
        deleteButton.setBounds(area.removeFromLeft(itemWidth).reduced(2));
    }

private:
    juce::Label pitchLabel, lengthLabel;
    juce::Slider pitchSlider;
    juce::ComboBox lengthSelector;
    juce::DrawableButton deleteButton { "Delete", juce::DrawableButton::ImageFitted };
};
