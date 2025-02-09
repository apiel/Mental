#pragma once

#include <JuceHeader.h>
#include <vector>

#include "constants.h"
#include "TrackComponent.h"

class FlatButtonLookAndFeel : public juce::LookAndFeel_V4 {
public:
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
        const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override
    {
        auto bounds = button.getLocalBounds();
        // g.setColour(juce::Colours::darkgrey);

        if (isMouseOverButton)
            g.setColour(sidebarColour);
        else
            g.setColour(sidebarColour.brighter(0.03f));

        g.fillRect(bounds);
    }

    juce::Font getTextButtonFont(juce::TextButton& button, int buttonHeight) override
    {
        juce::FontOptions options(buttonHeight * 0.3f, juce::Font::bold);
        return juce::Font(options);
    }
};

class SidebarComponent : public juce::Component {
protected:
    std::vector<std::unique_ptr<juce::TextButton>> buttons;
    ContainerComponent& container;
    FlatButtonLookAndFeel flatButtonLookAndFeel;

public:
    SidebarComponent(ContainerComponent& containerRef)
        : container(containerRef)
    {
        addButton(0);
    }

    ~SidebarComponent() override
    {
        for (auto& button : buttons) {
            button->setLookAndFeel(nullptr);
        }
    }

    void addButton(int index)
    {
        auto button = std::make_unique<juce::TextButton>();
        addAndMakeVisible(*button);
        button->setButtonText(container.getTabNames()[index]);
        button->onClick = [this, index] { container.setCurrentTabIndex(index); };
        button->setLookAndFeel(&flatButtonLookAndFeel);
        button->setColour(juce::TextButton::textColourOffId, container.getTabBackgroundColour(index));
        buttons.push_back(std::move(button));
    }

    void resized() override
    {
        int h = 60;
        for (size_t i = 0; i < buttons.size(); i++) {
            buttons[i]->setBounds(0, i * h, getWidth(), h - 2);
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(sidebarColour);
    }
};