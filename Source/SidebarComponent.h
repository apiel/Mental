#pragma once

#include <JuceHeader.h>

class SidebarComponent : public juce::Component {
protected:
    juce::TextButton track1Button, track2Button;
    ContainerComponent& container;

public:
    SidebarComponent(ContainerComponent& containerRef) : container(containerRef)
    {
        addAndMakeVisible(track1Button);
        addAndMakeVisible(track2Button);

        track1Button.setButtonText(containerRef.getTabNames()[0]);
        track2Button.setButtonText(containerRef.getTabNames()[1]);

        track1Button.onClick = [this] { container.setCurrentTabIndex(0); };
        track2Button.onClick = [this] { container.setCurrentTabIndex(1); };
    }

    void resized() override
    {
        track1Button.setBounds(10, 50, getWidth() - 20, 40);
        track2Button.setBounds(10, 100, getWidth() - 20, 40);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey);
    }
};
