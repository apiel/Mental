#pragma once

#include <JuceHeader.h>

#include "MasterComponent.h"
#include "TrackComponent.h"

juce::Colour containerTabbarColour = juce::Colours::black.brighter(0.1);

class SideTabLookAndFeel : public juce::LookAndFeel_V4 {
protected:
public:
    void drawTabButton(juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        g.fillAll(isMouseOver ? containerTabbarColour : containerTabbarColour.brighter(0.03f));

        auto bounds = button.getLocalBounds();

        juce::FontOptions options(button.getHeight() * 0.2f, juce::Font::bold);
        g.setFont(juce::Font(options));

        g.setColour(button.getTabBackgroundColour());
        g.drawFittedText(button.getButtonText(), bounds, juce::Justification::centred, 1);
    }

    int getTabButtonBestWidth(juce::TabBarButton&, int tabDepth) override
    {
        return tabDepth;
    }
};

class ContainerComponent : public juce::TabbedComponent {
protected:
    SideTabLookAndFeel tabLookAndFeel;

    MasterComponent masterComponent;
    TrackComponent trackComponent1;
    TrackComponent trackComponent2;

public:
    ContainerComponent()
        : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtLeft)
    {
        setTabBarDepth(80);
        setColour(TabbedComponent::backgroundColourId, containerTabbarColour);
        setLookAndFeel(&tabLookAndFeel);

        addTab("Master", juce::Colours::grey, &masterComponent, false);
        masterComponent.tabId = 0;
        addTab("Track 1", juce::Colours::orange, &trackComponent1, false);
        trackComponent1.tabId = 1;
        addTab("Track 2", juce::Colours::green, &trackComponent2, false);
        trackComponent2.tabId = 2;

        setCurrentTabIndex(0); // Start with Track 1
    }

    ~ContainerComponent()
    {
        setLookAndFeel(nullptr);
    }

    void resized() override
    {
        TabbedComponent::resized();

        // masterComponent.setBounds(getLocalBounds());
        // trackComponent1.setBounds(getLocalBounds());
        // trackComponent2.setBounds(getLocalBounds());
    }

    void currentTabChanged(int newTabIndex, const juce::String& newTabName) override
    {
        // printf("Tab changed to (%i) %s\n", newTabIndex, newTabName.toRawUTF8());
        masterComponent.parentTabChanged(newTabIndex, newTabName);
        trackComponent1.parentTabChanged(newTabIndex, newTabName);
        trackComponent2.parentTabChanged(newTabIndex, newTabName);
    }
};
