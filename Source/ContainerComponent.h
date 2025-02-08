#pragma once

#include <JuceHeader.h>

#include "MasterComponent.h"
#include "TrackComponent.h"
// #include "constants.h"

class ContainerComponent : public juce::TabbedComponent {
protected:
    MasterComponent masterComponent;
    TrackComponent trackComponent1;
    // TrackComponent trackComponent2;

public:
    ContainerComponent()
        : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtLeft)
        , trackComponent1(juce::Colours::orange)
        // , trackComponent2(juce::Colours::green)
    {
        // setColour(TabbedComponent::backgroundColourId, sidebarColour);
        // setColour(TabbedComponent::outlineColourId, sidebarColour);
        setOutline(0);

        addTab("Master", juce::Colours::grey, &masterComponent, false);
        masterComponent.tabId = 0;
        addTab("Track 1", trackComponent1.color, &trackComponent1, false);
        trackComponent1.tabId = 1;
        // addTab("Track 2", trackComponent2.color, &trackComponent2, false);
        // trackComponent2.tabId = 2;

        setCurrentTabIndex(0); // Start with Track 1
    }

    void resized() override
    {
        masterComponent.setBounds(getLocalBounds());
        trackComponent1.setBounds(getLocalBounds());
        // trackComponent2.setBounds(getLocalBounds());
    }

    void currentTabChanged(int newTabIndex, const juce::String& newTabName) override
    {
        // printf("Tab changed to (%i) %s\n", newTabIndex, newTabName.toRawUTF8());
        masterComponent.parentTabChanged(newTabIndex, newTabName);
        trackComponent1.parentTabChanged(newTabIndex, newTabName);
        // trackComponent2.parentTabChanged(newTabIndex, newTabName);
    }
};
