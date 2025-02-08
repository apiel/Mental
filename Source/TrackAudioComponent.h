#pragma once

// VsCode doesn't seems to recognize JUCE_PLUGINHOST_VST3, so I just define it myself.
#define JUCE_PLUGINHOST_VST3 1
// #define JUCE_PLUGINHOST_VST 1
#define JUCE_PLUGINHOST_LV2 1

#include "TrackListener.h"
#include <JuceHeader.h>

class TrackAudioComponent : public juce::AudioAppComponent, public TrackListener {
private:
    juce::Array<Step>& steps;

public:
    TrackAudioComponent(juce::Array<Step>& stepsRef)
        : steps(stepsRef)
    {
        setAudioChannels(0, 2);
        TrackEmitter::get().subscribe(this);
    }

    ~TrackAudioComponent() override
    {
        // if (pluginEditor) {
        //     removeChildComponent(pluginEditor.get()); // Ensure it's removed before deletion
        //     pluginEditor.reset();
        //     pluginEditor = nullptr;
        // }

        // if (plugin_instance) {
        //     // plugin_instance->releaseResources();
        //     // plugin_instance.reset();
        //     plugin_instance->suspendProcessing(true);
        //     plugin_instance->releaseResources();
        //     plugin_instance.reset();
        //     plugin_instance = nullptr;
        // }

        // pluginEditor = nullptr;
        // plugin_instance = nullptr;

        deleteAllChildren();
        shutdownAudio();
    }

    void onMidiClockTick(int clockCounter, bool isQuarterNote)
    {
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

    int samplesPerBlockExpected = 0;
    double sampleRate = 0.0;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        this->samplesPerBlockExpected = samplesPerBlockExpected;
        this->sampleRate = sampleRate;
        loadPlugin();
    }

    void releaseResources() override
    {
        if (plugin_instance) {
            plugin_instance->releaseResources();
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

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (plugin_instance) {
            juce::MidiBuffer midi; // Empty MIDI buffer
            plugin_instance->processBlock(*bufferToFill.buffer, midi);
        } else {
            bufferToFill.clearActiveBufferRegion();
        }
    }

    juce::AudioPluginFormatManager pluginmanager;
    std::unique_ptr<juce::AudioPluginInstance> plugin_instance;
    std::unique_ptr<juce::AudioProcessorEditor> pluginEditor;
    void loadPlugin()
    {
        pluginmanager.addDefaultFormats();

        juce::OwnedArray<juce::PluginDescription> descs;

        juce::VST3PluginFormat format;
        // juce::VSTPluginFormat format;
        format.findAllTypesForFile(descs, "/usr/lib/vst3/Vital.vst3");

        if (descs.size() > 0) {
            juce::String error;
            plugin_instance = pluginmanager.createPluginInstance(*descs[0], sampleRate, samplesPerBlockExpected, error);
            if (!plugin_instance) {
                std::cout << error << "\n";
            } else {
                std::cout << "created " << descs[0]->descriptiveName << "\n";
                plugin_instance->prepareToPlay(sampleRate, samplesPerBlockExpected);
            }
        } else {
            std::cout << "no plugins found\n";
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackAudioComponent)
};
