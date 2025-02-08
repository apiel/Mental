#pragma once

// VsCode doesn't seems to recognize JUCE_PLUGINHOST_VST3, so I just define it myself.
#define JUCE_PLUGINHOST_VST3 1
// #define JUCE_PLUGINHOST_VST 1
#define JUCE_PLUGINHOST_LV2 1

#include "TrackListener.h"
#include "constants.h"
#include <JuceHeader.h>

class TrackAudioComponent : public juce::AudioAppComponent, public TrackListener {
private:
    juce::Array<Step>& steps;

    juce::MidiBuffer midiBuffer;
    int stepCounter = 0;
    bool isPlaying = false;
    int loopCounter = 0;

    bool conditionMet(Step& step)
    {
        return stepConditions[step.condition].conditionMet(loopCounter);
    }

    void onStep(int sampleNum)
    {
        stepCounter++;
        // uint8_t state = status.get();
        // If we reach the end of the sequence, we reset the step counter
        if (stepCounter >= MAX_STEPS) {
            stepCounter = 0;
            loopCounter++;
            // props.audioPluginHandler->sendEvent(AudioEventType::SEQ_LOOP, track);
            // if (state == Status::NEXT) {
            //     status.set(Status::ON);
            // }
        }
        if (plugin_instance) {
            for (auto& step : steps) {
                if (step.counter) {
                    step.counter--;
                    if (step.counter == 0) {
                        // note off
                        noteOff(step.pitch, sampleNum);
                        printf("noteoff Step %i: %i\n", stepCounter, step.pitch);
                    }
                }
                // here might want to check for state == Status::ON
                if (stepCounter == step.startStep && conditionMet(step) && step.velocity > 0.0f) {
                    step.counter = step.length;
                    // note on
                    noteOn(step.pitch, step.velocity, sampleNum);
                    printf("noteon Step %i: %i\n", stepCounter, step.pitch);
                }
            }
        }
    }

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

    void noteOn(int midiNote, float velocity, int sampleOffset)
    {
        juce::MidiMessage noteOn = juce::MidiMessage::noteOn(1, midiNote, velocity);
        midiBuffer.addEvent(noteOn, sampleOffset);
    }

    void noteOff(int midiNote, int sampleOffset)
    {
        juce::MidiMessage noteOff = juce::MidiMessage::noteOff(1, midiNote);
        midiBuffer.addEvent(noteOff, sampleOffset);
    }

    void onMidiClockTick(int clockCounter, bool isQuarterNote, int sampleNum) override
    {
        if (isQuarterNote)
        {
            onStep(sampleNum);
        }
        
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
            plugin_instance->processBlock(*bufferToFill.buffer, midiBuffer);
            midiBuffer.clear();
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
