#pragma once

#include <JuceHeader.h>

class SidebarComponent : public juce::Component {
protected:
    juce::TextButton masterButton, track1Button, track2Button;
    ContainerComponent& container;

public:
    SidebarComponent(ContainerComponent& containerRef) : container(containerRef)
    {
        addAndMakeVisible(masterButton);
        addAndMakeVisible(track1Button);
        addAndMakeVisible(track2Button);

        masterButton.setButtonText(containerRef.getTabNames()[0]);
        track1Button.setButtonText(containerRef.getTabNames()[1]);
        track2Button.setButtonText(containerRef.getTabNames()[2]);

        masterButton.onClick = [this] { container.setCurrentTabIndex(0); };
        track1Button.onClick = [this] { container.setCurrentTabIndex(1); };
        track2Button.onClick = [this] { container.setCurrentTabIndex(2); };
    }

    void resized() override
    {
        masterButton.setBounds(10, 10, getWidth() - 20, 40);
        track1Button.setBounds(10, 60, getWidth() - 20, 40);
        track2Button.setBounds(10, 110, getWidth() - 20, 40);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey);
    }
};
