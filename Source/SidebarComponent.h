#pragma once

#include <JuceHeader.h>

class FlatButtonLookAndFeel : public juce::LookAndFeel_V4 {
public:
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
        const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override
    {
        auto bounds = button.getLocalBounds();
        // g.setColour(juce::Colours::darkgrey);

        if (isMouseOverButton)
            g.setColour(juce::Colours::black.withAlpha(0.2f));
        else
            g.setColour(juce::Colours::black.withAlpha(0.5f));

        g.fillRect(bounds);
    }
};

class SidebarComponent : public juce::Component {
protected:
    juce::TextButton masterButton, track1Button, track2Button;
    ContainerComponent& container;
    FlatButtonLookAndFeel flatButtonLookAndFeel;

public:
    SidebarComponent(ContainerComponent& containerRef)
        : container(containerRef)
    {
        addTabButton(masterButton, 0);
        addTabButton(track1Button, 1);
        addTabButton(track2Button, 2);
    }

    ~SidebarComponent() override
    {
        masterButton.setLookAndFeel(nullptr);
        track1Button.setLookAndFeel(nullptr);
        track2Button.setLookAndFeel(nullptr);
    }

    void addTabButton(juce::TextButton& button, int index)
    {
        addAndMakeVisible(button);
        button.setButtonText(container.getTabNames()[index]);
        button.onClick = [this, index] { container.setCurrentTabIndex(index); };
        button.setLookAndFeel(&flatButtonLookAndFeel);
        button.setColour(juce::TextButton::textColourOffId, container.getTabBackgroundColour(index));
    }

    void resized() override
    {
        int h = 60;
        masterButton.setBounds(0, 0, getWidth(), h - 1);
        track1Button.setBounds(0, h, getWidth(), h - 1);
        track2Button.setBounds(0, h * 2, getWidth(), h - 1);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey);
    }
};
