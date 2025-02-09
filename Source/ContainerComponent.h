#pragma once

#include <JuceHeader.h>
#include <memory> // Required for std::unique_ptr
#include <vector>

#include "Audio.h"
#include "MasterComponent.h"
#include "TrackComponent.h"
#include "constants.h"

class ContainerComponent : public juce::TabbedComponent {
protected:
    MasterComponent masterComponent;
    std::vector<std::unique_ptr<TrackComponent>> trackComponents; // Store track components as unique_ptrs

public:
    ContainerComponent()
        : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtLeft)
    {
        setOutline(0);

        addTab("Master", juce::Colours::grey, &masterComponent, false);
        masterComponent.tabId = 0;

        setCurrentTabIndex(0);
    }

    // Method to add a new track dynamically
    TrackComponent& addTrack(AudioTrack& audioTrack, juce::String name, juce::Colour color)
    {
        int trackIndex = trackComponents.size(); // Get next track index
        trackComponents.push_back(std::make_unique<TrackComponent>(color, audioTrack));

        TrackComponent& newTrack = *trackComponents.back();
        newTrack.tabId = trackIndex + 1;

        addTab(name, color, &newTrack, false);

        return newTrack;
    }

    TrackComponent& getTrack(int index)
    {
        jassert(index >= 0 && index < trackComponents.size());
        return *trackComponents[index];
    }

    void resized() override
    {
        masterComponent.setBounds(getLocalBounds());
        for (auto& trackComponent : trackComponents) {
            trackComponent->setBounds(getLocalBounds());
        }
    }

    void currentTabChanged(int newTabIndex, const juce::String& newTabName) override
    {
        masterComponent.parentTabChanged(newTabIndex, newTabName);
        for (auto& trackComponent : trackComponents) {
            trackComponent->parentTabChanged(newTabIndex, newTabName);
        }
    }
};
