#pragma once

#include "constants.h"
#include <JuceHeader.h>

// Rename to plugin component or SynthComponent
class TrackAudioComponent : public juce::Component {
private:

public:
    TrackAudioComponent()
    {
        setWantsKeyboardFocus(true);
    }

    ~TrackAudioComponent() override
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
            // noteOn(60, 1.0, 0);
        } else if (!spaceIsDown && wasSpacePressed) // Space released
        {
            wasSpacePressed = false;
            // noteOff(60, 1);
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
        if (plugin_instance && plugin_instance->hasEditor() && !pluginEditor) {
            pluginEditor.reset(plugin_instance->createEditor());
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

    std::unique_ptr<juce::AudioPluginInstance> plugin_instance;
    std::unique_ptr<juce::AudioProcessorEditor> pluginEditor;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackAudioComponent)
};
