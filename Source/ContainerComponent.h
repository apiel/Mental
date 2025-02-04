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

    // ContainerComponent()
    // {
    //   addAndMakeVisible(trackComponent1);
    //   addAndMakeVisible(trackComponent2);

    //   trackComponent1.setVisible(true);
    //   trackComponent2.setVisible(false);

    //   trackComponent1.toFront(true);
    // }

    // void showTrack(int trackIndex)
    // {
    //   if (trackIndex == 0)
    //   {
    //     printf("Showing track 1\n");
    //     trackComponent1.setVisible(true);
    //     trackComponent2.setVisible(false);
    //     trackComponent1.toFront(true);
    //   }
    //   else
    //   {
    //     printf("Showing track 2\n");
    //     trackComponent1.setVisible(false);
    //     trackComponent2.setVisible(true);
    //     trackComponent2.toFront(true);
    //   }
    //   // resized();
    //   repaint();
    // }

    void resized() override
    {
        trackComponent1.setBounds(getLocalBounds());
        trackComponent2.setBounds(getLocalBounds());
    }

    // void paint(juce::Graphics &g) override
    // {
    //   // g.fillAll(juce::Colours::darkgrey); // Background color
    //   g.setColour(juce::Colours::white);
    //   g.setFont(20.0f);
    //   g.drawText("Container", getLocalBounds(), juce::Justification::centred);
    // }
};
