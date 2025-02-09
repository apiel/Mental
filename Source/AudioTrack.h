#pragma once

// VsCode doesn't seems to recognize JUCE_PLUGINHOST_VST3, so I just define it myself.
#define JUCE_PLUGINHOST_VST3 1
// #define JUCE_PLUGINHOST_VST 1
#define JUCE_PLUGINHOST_LV2 1

#include "Step.h"
#include "TrackListener.h"
#include "constants.h"

#include <JuceHeader.h>

class AudioTrack : public juce::AudioSource, public TrackListener {
private:
    double sampleRate = 0.0;
    int samplesPerBlockExpected = 0;

    juce::AudioPluginFormatManager pluginmanager;
    std::unique_ptr<juce::AudioPluginInstance> plugin_instance;

    juce::Array<Step> steps;

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
                        printf("noteoff Step %i: %i\n", step.startStep, step.pitch);
                    }
                }
                // here might want to check for state == Status::ON
                if (stepCounter == step.startStep && conditionMet(step) && step.velocity > 0.0f) {
                    step.counter = step.length;
                    // note on
                    noteOn(step.pitch, step.velocity, sampleNum);
                    printf("noteon Step %i: %i\n", step.startStep, step.pitch);
                }
            }
        }
    }

public:
    AudioTrack()
    {
        // Instead of void onMidiClockTick(int clockCounter, bool isQuarterNote, int sampleNum) override
        // should use callback function
        TrackEmitter::get().subscribe(this);

        // Example MIDI Notes (Step, Pitch, Length)
        // steps.add({ 0, 60, 4 }); // C4 spanning 4 steps
        // steps.add({ 4, 62, 2 }); // D4 spanning 2 steps
        // steps.add({ 8, 64, 8 }); // E4 spanning 8 steps
        // steps.add({ 16, 67, 4 }); // G4 spanning 4 steps
        // steps.add({ 20, 69, 6 }); // A4 spanning 6 steps
        // steps.add({ 24, 72, 6 }); // C5 spanning 6 steps
        // steps.add({ 32, 75, 6 }); // D#5 spanning 6 steps
        // steps.add({ 40, 77, 6 }); // F5 spanning 6 steps
        // steps.add({ 48, 80, 6 }); // G#5 spanning 6 steps
        // steps.add({ 56, 83, 6 }); // B5 spanning 6 steps

        // Another simple pattern
        steps.add({ 0, 48, 1 });
        steps.add({ 8, 48, 1 });
        steps.add({ 16, 48, 1 });
        steps.add({ 24, 48, 1 });
        steps.add({ 32, 48, 1 });
        steps.add({ 40, 48, 1 });
        steps.add({ 48, 48, 1 });
        steps.add({ 56, 48, 1 });
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
        if (plugin_instance) {
            plugin_instance->processBlock(*bufferToFill.buffer, midiBuffer);
            midiBuffer.clear();
        } else {
            bufferToFill.clearActiveBufferRegion();
        }
    }

    void releaseResources() override
    {
        if (plugin_instance) {
            plugin_instance->releaseResources();
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioTrack)
};
