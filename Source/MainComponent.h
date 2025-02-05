#pragma once

// VsCode doesn't seems to recognize JUCE_PLUGINHOST_VST3, so I just define it myself.
#define JUCE_PLUGINHOST_VST3 1
#define JUCE_PLUGINHOST_LV2 1

#include <JuceHeader.h>

#include "ContainerComponent.h"
#include "SidebarComponent.h"

class MainComponent : public juce::Component {
protected:
    ContainerComponent container;

public:
    MainComponent()
    {
        setSize(1200, 800);
        addAndMakeVisible(container);
    }

    void resized() override
    {
        container.setBounds(0, 0, getWidth(), getHeight());
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
