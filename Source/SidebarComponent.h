#pragma once

#include <JuceHeader.h>

class SidebarComponent : public juce::Component {
protected:
    juce::TextButton track1Button, track2Button;
    std::function<void(int)> setTab;

public:
    SidebarComponent(std::function<void(int)> setTabCallback)
        : setTab(setTabCallback)
    {
        addAndMakeVisible(track1Button);
        addAndMakeVisible(track2Button);

        track1Button.setButtonText("Show Track 1");
        track2Button.setButtonText("Show Track 2");

        track1Button.onClick = [this] { setTab(0); };
        track2Button.onClick = [this] { setTab(1); };
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
