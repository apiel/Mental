#pragma once

// VsCode doesn't seems to recognize JUCE_PLUGINHOST_VST3, so I just define it myself.
#define JUCE_PLUGINHOST_VST3 1
// #define JUCE_PLUGINHOST_VST 1
// #define JUCE_PLUGINHOST_LV2 1

#include "Step.h"
#include "TrackListener.h"
#include "constants.h"

#include <JuceHeader.h>

class AudioTrack : public juce::AudioSource, public TrackListener {
private:
    double sampleRate = 0.0;
    int samplesPerBlockExpected = 0;

    juce::AudioPluginFormatManager pluginmanager;

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
        if (plugin) {
            for (auto& step : steps) {
                if (step.counter) {
                    step.counter--;
                    if (step.counter == 0) {
                        // note off
                        noteOff(step.pitch, sampleNum);
                        // printf("noteoff Step %i: %i\n", step.startStep, step.pitch);
                    }
                }
                // here might want to check for state == Status::ON
                if (stepCounter == step.startStep && conditionMet(step) && step.velocity > 0.0f) {
                    step.counter = step.length;
                    // note on
                    noteOn(step.pitch, step.velocity, sampleNum);
                    printf("[%i/%i] noteon Step: %s (%i), %i%%\n", step.startStep + 1, MAX_STEPS,
                        juce::MidiMessage::getMidiNoteName(step.pitch, true, true, 4).toStdString().c_str(),
                        step.pitch, (int)(step.velocity * 100.0f));
                }
            }
        }
    }

public:
    std::unique_ptr<juce::AudioPluginInstance> plugin;
    juce::Array<Step> steps;

    AudioTrack()
    {
        TrackEmitter::get().subscribe(this);
    }

    ~AudioTrack()
    {
        if (plugin) {
            // plugin->suspendProcessing(true);
        }
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
        midiBuffer.addEvent(juce::MidiMessage::midiClock(), sampleNum);
        if (isQuarterNote) {
            onStep(sampleNum);
        }
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        this->samplesPerBlockExpected = samplesPerBlockExpected;
        this->sampleRate = sampleRate;
        loadPlugin();
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (plugin) {
            plugin->processBlock(*bufferToFill.buffer, midiBuffer);
            midiBuffer.clear();
        } else {
            bufferToFill.clearActiveBufferRegion();
        }
    }

    void releaseResources() override
    {
        if (plugin) {
            plugin->releaseResources();
        }
    }

    void loadPlugin()
    {
        pluginmanager.addDefaultFormats();

        juce::OwnedArray<juce::PluginDescription> descs;

        juce::VST3PluginFormat format;
        // juce::VSTPluginFormat format;
        format.findAllTypesForFile(descs, "/usr/lib/vst3/Vital.vst3");

        if (descs.size() > 0) {
            juce::String error;
            plugin = pluginmanager.createPluginInstance(*descs[0], sampleRate, samplesPerBlockExpected, error);
            if (!plugin) {
                std::cout << error << "\n";
            } else {
                std::cout << "created " << descs[0]->descriptiveName << "\n";
                plugin->prepareToPlay(sampleRate, samplesPerBlockExpected);
            }
        } else {
            std::cout << "no plugins found\n";
        }
    }

    juce::String getPresetData()
    {
        if (plugin) {
            juce::MemoryBlock state;
            plugin->getStateInformation(state);
            return state.toBase64Encoding();
        }
        return "";
    }

    void loadPresetData(juce::String& data)
    {
        if (plugin) {
            // plugin->setStateInformation(data.toRawUTF8(), data.length());
            juce::MemoryBlock state;
            state.fromBase64Encoding(data);
            plugin->setStateInformation(state.getData(), (int)state.getSize());
        }
    }

    void listPresetParameters()
    {
        if (plugin) {
            auto& params = plugin->getParameters();

            std::cout << "Vital Plugin Parameters:\n";
            for (int i = 0; i < params.size(); ++i) {
                juce::AudioProcessorParameter* param = params[i];
                std::cout << i << "[" << param->getLabel().toStdString() << "]"
                          << param->getName(100).toStdString()
                          << " = " << param->getValue() << "\n";
            }
        } else {
            std::cout << "Plugin not loaded.\n";
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioTrack)
};
