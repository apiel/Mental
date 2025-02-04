#pragma once

// VsCode doesn't seems to recognize JUCE_PLUGINHOST_VST3, so I just define it myself.
#define JUCE_PLUGINHOST_VST3 1
#define JUCE_PLUGINHOST_LV2 1

#include <JuceHeader.h>

#include "ContainerComponent.h"
#include "SidebarComponent.h"

class MainComponent : public juce::Component {
protected:
    SidebarComponent sidebar;
    ContainerComponent container;

public:
    MainComponent()
        : sidebar([this](int trackIndex) {
            /*container.showTrack(trackIndex);*/
            container.setCurrentTabIndex(trackIndex);
        })
    {
        setSize(1200, 800);
        addAndMakeVisible(sidebar);
        addAndMakeVisible(container);
    }

    // ~MainComponent() override
    // {
    // }

    void resized() override
    {
        int sidebarWidth = 200; // Adjust as needed
        sidebar.setBounds(0, 0, sidebarWidth, getHeight()); // Position sidebar on the left
        container.setBounds(sidebarWidth, 0, getWidth() - sidebarWidth, getHeight());
    }

    void paint(juce::Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
