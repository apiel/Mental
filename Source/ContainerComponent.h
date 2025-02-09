#pragma once

#include <JuceHeader.h>

#include "Audio.h"
#include "MasterComponent.h"
#include "TrackComponent.h"
#include "constants.h"

class ContainerComponent : public juce::TabbedComponent {
protected:
    MasterComponent masterComponent;
    TrackComponent trackComponents[TRACK_COUNT] = {
        TrackComponent(juce::Colours::orange, Audio::get().getTrack(0)),
    };

public:
    ContainerComponent()
        : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtLeft)
    {
        setOutline(0);

        addTab("Master", juce::Colours::grey, &masterComponent, false);
        masterComponent.tabId = 0;

        for (int i = 0; i < TRACK_COUNT; i++) {
            addTab("Track " + std::to_string(i + 1), juce::Colours::orange, &trackComponents[i], false);
            trackComponents[i].tabId = i + 1;
        }

        setCurrentTabIndex(0);
    }

    void resized() override
    {
        masterComponent.setBounds(getLocalBounds());
        for (int i = 0; i < TRACK_COUNT; i++) {
            trackComponents[i].setBounds(getLocalBounds());
        }
    }

    void currentTabChanged(int newTabIndex, const juce::String& newTabName) override
    {
        masterComponent.parentTabChanged(newTabIndex, newTabName);
        for (int i = 0; i < TRACK_COUNT; i++) {
            trackComponents[i].parentTabChanged(newTabIndex, newTabName);
        }
    }
};
