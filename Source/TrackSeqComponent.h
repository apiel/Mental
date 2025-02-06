#pragma once

#include <JuceHeader.h>

class TrackSeqComponent : public juce::Component {
protected:
public:
    TrackSeqComponent()
    {
    }

    void resized() override
    {
    }

    void paint(juce::Graphics& g) override
    {
        // g.fillAll(juce::Colours::pink);
    }

    int tabId = -1;
    void parentTabChanged(int newTabIndex, const juce::String& newTabName)
    {
    }
};
