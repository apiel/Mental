#pragma once

#include <JuceHeader.h>

#include "Step.h"
#include "TrackAudioComponent.h"
#include "TrackSeqComponent.h"
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

        juce::FontOptions options(button.getHeight() * 0.4f, juce::Font::bold);
        g.setFont(juce::Font(options));

        g.setColour(button.isFrontTab() ? button.getTabBackgroundColour() : button.getTabBackgroundColour().darker(0.5f));
        g.drawFittedText(button.getButtonText(), bounds, juce::Justification::centred, 1);
    }
};

class TrackComponent : public juce::TabbedComponent {
protected:
    TrackTabLookAndFeel tabLookAndFeel;

    TrackAudioComponent audioComponent;
    TrackSeqComponent seqComponent;

    juce::Array<Step> steps;

public:
    juce::Colour color;
    TrackComponent(juce::Colour color)
        : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
        , color(color)
        , seqComponent(color, steps)
    {
        // Example MIDI Notes (Step, Pitch, Length)
        steps.add({ 0, 60, 4 }); // C4 spanning 4 steps
        steps.add({ 4, 62, 2 }); // D4 spanning 2 steps
        steps.add({ 8, 64, 8 }); // E4 spanning 8 steps
        steps.add({ 16, 67, 4 }); // G4 spanning 4 steps
        steps.add({ 20, 69, 6 }); // A4 spanning 6 steps
        steps.add({ 24, 72, 6 }); // C5 spanning 6 steps
        steps.add({ 32, 75, 6 }); // D#5 spanning 6 steps
        steps.add({ 40, 77, 6 }); // F5 spanning 6 steps
        steps.add({ 48, 80, 6 }); // G#5 spanning 6 steps
        steps.add({ 56, 83, 6 }); // B5 spanning 6 steps
        seqComponent.initScrollPosition();

        setColour(TabbedComponent::backgroundColourId, sidebarColour);
        // setColour(TabbedComponent::outlineColourId, sidebarColour);
        setOutline(0);
        setLookAndFeel(&tabLookAndFeel);
        setTabBarDepth(40);

        addTab("Sequencer", color, &seqComponent, false);
        seqComponent.tabId = 0;
        addTab("Synth engine", color, &audioComponent, false);
        audioComponent.tabId = 1;

        setCurrentTabIndex(0);
    }

    ~TrackComponent() override
    {
        setLookAndFeel(nullptr);
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
        seqComponent.parentTabChanged(newTabIndex, newTabName);
        audioComponent.parentTabChanged(newTabIndex, newTabName);
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