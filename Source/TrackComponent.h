#pragma once

#include <JuceHeader.h>

#include "TrackAudioComponent.h"
#include "constants.h"

class TrackTabLookAndFeel : public juce::LookAndFeel_V4 {
public:
    void drawTabButton(juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        g.fillAll(isMouseOver ? sidebarColour : sidebarColour.brighter(0.05f));

        auto bounds = button.getLocalBounds();

        // g.setColour(isMouseOver ? sidebarColour : sidebarColour.brighter(0.03f));
        // bounds.setWidth(bounds.getWidth() - 1);
        // g.fillRect(bounds);

        juce::FontOptions options(button.getHeight() * 0.5f, juce::Font::bold);
        g.setFont(juce::Font(options));

        g.setColour(button.getTabBackgroundColour());
        g.drawFittedText(button.getButtonText(), bounds, juce::Justification::centred, 1);
    }
};

class TrackComponent : public juce::TabbedComponent {
protected:
    TrackTabLookAndFeel tabLookAndFeel;

    TrackAudioComponent audioComponent;

public:
    juce::Colour color;
    TrackComponent(juce::Colour color)
        : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
        , color(color)
    {
        setColour(TabbedComponent::backgroundColourId, sidebarColour);
        // setColour(TabbedComponent::outlineColourId, sidebarColour);
        setOutline(0);
        setLookAndFeel(&tabLookAndFeel);

        // addTab("Audio", juce::Colours::grey, &audioComponent, false);
        addTab("Synth engine", color, &audioComponent, false);
        audioComponent.tabId = 0;

        setCurrentTabIndex(0);
    }
    // void resized() override
    // {
    //     audioComponent.setBounds(getLocalBounds());
    // }

    void resized() override
    {
        TabbedComponent::resized();
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

// Instead of custom sidebar...:

// juce::Colour containerTabBarColour = juce::Colours::black.brighter(0.1);

// class SideTabLookAndFeel : public juce::LookAndFeel_V4 {
// protected:
// public:
//     void drawTabButton(juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override
//     {
//         // g.fillAll(isMouseOver ? containerTabBarColour : containerTabBarColour.brighter(0.03f));

//         auto bounds = button.getLocalBounds();

//         g.setColour(isMouseOver ? containerTabBarColour : containerTabBarColour.brighter(0.03f));
//         bounds.setWidth(bounds.getWidth() - 1);
//         g.fillRect(bounds);

//         juce::FontOptions options(button.getHeight() * 0.2f, juce::Font::bold);
//         g.setFont(juce::Font(options));

//         g.setColour(button.getTabBackgroundColour());
//         g.drawFittedText(button.getButtonText(), bounds, juce::Justification::centred, 1);
//     }

//     int getTabButtonBestWidth(juce::TabBarButton&, int tabDepth) override
//     {
//         return tabDepth;
//     }
// };