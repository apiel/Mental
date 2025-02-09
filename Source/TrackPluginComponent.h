#pragma once

#include "constants.h"
#include "AudioTrack.h"
#include <JuceHeader.h>

class TrackPluginComponent : public juce::Component {
private:
    AudioTrack& audioTrack;
    std::unique_ptr<juce::AudioProcessorEditor> pluginEditor;

public:
    TrackPluginComponent(AudioTrack& audioTrack)
        : audioTrack(audioTrack)
    {
        setWantsKeyboardFocus(true);
    }

    ~TrackPluginComponent() override
    {
        deleteAllChildren();
    }

    bool wasSpacePressed = false;
    bool keyStateChanged(bool isKeyDown) override
    {
        bool spaceIsDown = juce::KeyPress::isKeyCurrentlyDown(juce::KeyPress::spaceKey);

        if (spaceIsDown && !wasSpacePressed) // Space pressed for the first time
        {
            wasSpacePressed = true;
            audioTrack.noteOn(60, 1.0, 0);
        } else if (!spaceIsDown && wasSpacePressed) // Space released
        {
            wasSpacePressed = false;
            audioTrack.noteOff(60, 1);
        }
        return true;
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey);
    }

    void resized() override
    {
        if (pluginEditor && pluginEditor->isResizable()) {
            pluginEditor->setBounds(getLocalBounds());
        }
    }

    int tabId = -1;
    void parentTabChanged(int newTabIndex, const juce::String& newTabName)
    {
        if (tabId == newTabIndex) {
            showPluginEditor();
        } else {
            hidePluginEditor();
        }
    }

    void showPluginEditor()
    {
        if (audioTrack.plugin && audioTrack.plugin->hasEditor() && !pluginEditor) {
            pluginEditor.reset(audioTrack.plugin->createEditor());
            addAndMakeVisible(pluginEditor.get());
            pluginEditor->setBounds(getLocalBounds());
        }
    }

    void hidePluginEditor()
    {
        if (pluginEditor) {
            removeChildComponent(pluginEditor.get());
            pluginEditor.reset();
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackPluginComponent)
};
