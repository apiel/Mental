#pragma once

#include <JuceHeader.h>

class MasterComponent : public juce::Component {
protected:
public:
    MasterComponent()
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
    void currentTabChanged(int newTabIndex, const juce::String& newTabName)
    {
    }
};
