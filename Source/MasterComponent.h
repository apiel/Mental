#pragma once

#include "BpmComponent.h"
#include <JuceHeader.h>

class MasterComponent : public juce::Component {
private:
    BpmComponent bpmComponent;

public:
    MasterComponent()
    {
        addAndMakeVisible(bpmComponent);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(10); // Add padding around the UI
        auto bpmWidth = 150; // Set fixed width for the bpm component
        auto bpmHeight = 150; // Set fixed height for the bpm component
        bpmComponent.setBounds(area.removeFromTop(bpmHeight).removeFromLeft(bpmWidth));
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey);
    }

    int tabId = -1; // This must stay because parent component need it!
    void parentTabChanged(int newTabIndex, const juce::String& newTabName)
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterComponent)
};
