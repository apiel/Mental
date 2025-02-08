#pragma once

#include "Step.h"
#include <JuceHeader.h>

class ScrollableComboBox : public juce::ComboBox {
public:
    ScrollableComboBox()
    {
        setLookAndFeel(&customLookAndFeel);
    }

    ~ScrollableComboBox() override
    {
        setLookAndFeel(nullptr); // Clean up
    }

    void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails& wheel) override
    {
        int currentId = getSelectedId();
        int newId = juce::jlimit(1, getNumItems(), currentId + (wheel.deltaY > 0 ? 1 : -1));
        setSelectedId(newId, juce::sendNotification);
    }

private:
    class CustomLookAndFeel : public juce::LookAndFeel_V4 {
    public:
        juce::PopupMenu::Options getOptionsForComboBoxPopupMenu(juce::ComboBox& box, juce::Label&) override
        {
            return juce::PopupMenu::Options().withTargetComponent(&box).withMinimumWidth(box.getWidth()).withPreferredPopupDirection(juce::PopupMenu::Options::PopupDirection::upwards);
        }
    };

    CustomLookAndFeel customLookAndFeel;
};

class CustomSliderLookAndFeel : public juce::LookAndFeel_V4 {
public:
    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        g.setColour(label.findColour(juce::Label::textColourId));
        g.setFont(label.getFont());
        g.drawFittedText(label.getText(), label.getLocalBounds(), juce::Justification::centred, 1);
    }
};

class NoteToolboxComponent : public juce::Component {
private:
    juce::Label velocityLabel, lengthLabel, conditionLabel, motionLabel;
    juce::Slider velocitySlider;
    ScrollableComboBox lengthSelector, conditionSelector, motionSelector;
    juce::DrawableButton deleteButton { "Delete", juce::DrawableButton::ImageFitted };

    CustomSliderLookAndFeel customLookAndFeel;

public:
    std::function<void(float)> onVelocityChange;
    std::function<void(int)> onLengthChange;
    std::function<void(int)> onConditionChange;
    std::function<void(int)> onMotionChange;
    std::function<void()> onDelete;

    ~NoteToolboxComponent()
    {
        velocitySlider.setLookAndFeel(nullptr);
    }

    NoteToolboxComponent()
    {
        addAndMakeVisible(velocityLabel);
        addAndMakeVisible(velocitySlider);
        addAndMakeVisible(lengthLabel);
        addAndMakeVisible(lengthSelector);
        addAndMakeVisible(conditionLabel);
        addAndMakeVisible(conditionSelector);
        addAndMakeVisible(motionLabel);
        addAndMakeVisible(motionSelector);
        addAndMakeVisible(deleteButton);

        velocityLabel.setText("Velocity:", juce::dontSendNotification);
        lengthLabel.setText("Length:", juce::dontSendNotification);
        conditionLabel.setText("Condition:", juce::dontSendNotification);
        motionLabel.setText("Motion:", juce::dontSendNotification);

        velocitySlider.setRange(0.0, 1.0, 0.05);
        velocitySlider.onValueChange = [this] {
            if (onVelocityChange)
                onVelocityChange(velocitySlider.getValue());
        };
        // velocitySlider.setLookAndFeel(&customLookAndFeel);
        // velocitySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        velocitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 14);
        velocitySlider.setLookAndFeel(&customLookAndFeel);

        for (int i = 1; i <= 64; ++i) {
            lengthSelector.addItem(juce::String(i), i);
        }
        lengthSelector.onChange = [this] {
            if (onLengthChange)
                onLengthChange(lengthSelector.getSelectedId());
        };
        lengthSelector.setSelectedId(4, juce::dontSendNotification);

        for (int i = 0; i < STEP_CONDITIONS_COUNT; ++i) {
            conditionSelector.addItem(stepConditions[i].name, i + 1);
        }
        conditionSelector.onChange = [this] {
            if (onConditionChange)
                onConditionChange(conditionSelector.getSelectedId() - 1);
        };
        conditionSelector.setSelectedId(0, juce::dontSendNotification);

        for (int i = 0; i < STEP_MOTIONS_COUNT; ++i) {
            motionSelector.addItem(stepMotions[i].name, i + 1);
        }
        motionSelector.onChange = [this] {
            if (onMotionChange)
                onMotionChange(motionSelector.getSelectedId() - 1);
        };
        motionSelector.setSelectedId(0, juce::dontSendNotification);

        deleteButton.setButtonText("");
        auto trashIcon = std::make_unique<juce::DrawablePath>();
        setTrashIcon(trashIcon.get());
        trashIcon->setFill(juce::Colours::grey);
        trashIcon->setStrokeFill(juce::Colours::white);
        trashIcon->setStrokeType(juce::PathStrokeType(0.5f));
        deleteButton.setImages(trashIcon.get());
        deleteButton.setClickingTogglesState(false);
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

    void setNoteDetails(float velocity, int length, int condition, int motion)
    {
        velocitySlider.setValue(velocity, juce::dontSendNotification);
        lengthSelector.setSelectedId(length, juce::dontSendNotification);
        conditionSelector.setSelectedId(condition + 1, juce::dontSendNotification);
        motionSelector.setSelectedId(motion + 1, juce::dontSendNotification);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey.withAlpha(0.7f));
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(5);
        auto labelWidth = 70;

        velocityLabel.setBounds(area.removeFromLeft(labelWidth).reduced(2));
        // velocitySlider.setBounds(area.removeFromLeft(120).reduced(2));
        auto sliderArea = area.removeFromLeft(120).reduced(2);
        velocitySlider.setBounds(sliderArea.removeFromBottom(30));

        lengthLabel.setBounds(area.removeFromLeft(labelWidth).reduced(2));
        lengthSelector.setBounds(area.removeFromLeft(100).reduced(2));
        conditionLabel.setBounds(area.removeFromLeft(labelWidth).reduced(2));
        conditionSelector.setBounds(area.removeFromLeft(100).reduced(2));
        motionLabel.setBounds(area.removeFromLeft(labelWidth).reduced(2));
        motionSelector.setBounds(area.removeFromLeft(100).reduced(2));
        deleteButton.setBounds(area.removeFromLeft(60).reduced(2));
    }
};
