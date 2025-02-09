#pragma once

// VsCode doesn't seems to recognize JUCE_PLUGINHOST_VST3, so I just define it myself.
#define JUCE_PLUGINHOST_VST3 1
#define JUCE_PLUGINHOST_LV2 1

#include <JuceHeader.h>

#include "Audio.h"
#include "ContainerComponent.h"
#include "SidebarComponent.h"
#include "TrackSerializer.h"
#include "constants.h"

class MainComponent : public juce::Component {
protected:
    ContainerComponent container;
    SidebarComponent sidebar;
    TrackSerializer serializer;

public:
    MainComponent()
        : sidebar(container)
    {
        setSize(1200, 800);
        addAndMakeVisible(sidebar);
        addAndMakeVisible(container);

        // Initialize tracks (if needed)
        for (int i = 0; i < TRACK_COUNT; i++) {
            AudioTrack& track = Audio::get().addTrack();
            TrackComponent& trackComponent = container.addTrack(track, "Track " + juce::String(i + 1), juce::Colours::orange);
            sidebar.addButton(trackComponent.tabId);
        }
        sidebar.resized();
    }

    ~MainComponent()
    {
        for (int i = 0; i < TRACK_COUNT; i++) {
            serializer.save("track" + juce::String(i + 1) + ".json", Audio::get().getTrack(i), container.getTrack(i));
        }
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
