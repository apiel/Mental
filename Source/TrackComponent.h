#pragma once

#include <JuceHeader.h>

#include "TrackAudioComponent.h"

class TrackComponent : public juce::TabbedComponent {
protected:
    TrackAudioComponent audioComponent;

public:
    TrackComponent()
        : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
    {
        addTab("Audio", juce::Colours::grey, &audioComponent, false);
        audioComponent.tabId = 0;

        setCurrentTabIndex(0);
    }
    void resized() override
    {
        audioComponent.setBounds(getLocalBounds());
    }

    int tabId = -1;
    void parentTabChanged(int newTabIndex, const juce::String& newTabName)
    {
        if (tabId == newTabIndex) {
            audioComponent.parentTabChanged(getCurrentTabIndex(), newTabName);
        } else {
            audioComponent.parentTabChanged(-1, "");
        }
    }

    void currentTabChanged(int newTabIndex, const juce::String& newTabName) override
    {
        // audioComponent.currentTabChanged(newTabIndex, newTabName);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackComponent)
};
