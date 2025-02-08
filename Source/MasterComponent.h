#pragma once

#include "TempoComponent.h"
#include <JuceHeader.h>

class MasterComponent : public juce::Component {
private:
    TempoComponent tempoComponent;

public:
    MasterComponent()
    {
        addAndMakeVisible(tempoComponent);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(10); // Add padding around the UI
        auto tempoWidth = 150; // Set fixed width for the tempo component
        auto tempoHeight = 150; // Set fixed height for the tempo component
        tempoComponent.setBounds(area.removeFromTop(tempoHeight).removeFromLeft(tempoWidth));
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
