#pragma once

#include <JuceHeader.h>

#include "TrackComponent.h"

class ContainerComponent : public juce::TabbedComponent {
protected:
    TrackComponent trackComponent1;
    TrackComponent trackComponent2;

public:
    ContainerComponent()
        : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
    {
        addTab("Track 1", juce::Colours::lightblue, &trackComponent1, false);
        addTab("Track 2", juce::Colours::lightgreen, &trackComponent2, false);

        setCurrentTabIndex(0); // Start with Track 1
    }

    void resized() override
    {
        trackComponent1.setBounds(getLocalBounds());
        trackComponent2.setBounds(getLocalBounds());
    }

};
