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
    SidebarComponent sidebar;

public:
    MainComponent()
        : sidebar(container)
    {
        setSize(1200, 800);
        addAndMakeVisible(sidebar);
        addAndMakeVisible(container);
    }

    void resized() override
    {
        int sidebarWidth = 80;
        sidebar.setBounds(0, 0, sidebarWidth, getHeight());
        container.setBounds(sidebarWidth, 0, getWidth() - sidebarWidth, getHeight());
    }

    // void paint(juce::Graphics& g) override
    // {
    //     g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    // }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
