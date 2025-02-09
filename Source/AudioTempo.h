#pragma once

#include "TrackListener.h"
#include <JuceHeader.h>

class AudioTempo : public juce::AudioSource {
private:
    double sampleRate = 0.0;
    int samplesPerBlockExpected = 0;
    double bpm = 160.0;
    int sampleCountTarget = 0;
    int sampleCounter = 0;

    TrackEmitter& trackEmitter = TrackEmitter::get();

    AudioTempo()
    {
    }

public:
    static AudioTempo& get()
    {
        static AudioTempo instance;
        return instance;
    }

    void setBpm(double newBpm)
    {
        if (bpm > 0.0) {
            bpm = newBpm;
            sampleCountTarget = static_cast<int>((sampleRate * 60.0f / bpm) / 12.0f);
            // Here you can send a notification to update timing in other components
            DBG("Tempo changed to: " << bpm << " BPM (" << sampleCountTarget << " samples per block)");
        }
    }

    double getBpm() { return bpm; }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        this->samplesPerBlockExpected = samplesPerBlockExpected;
        this->sampleRate = sampleRate;
        sampleCounter = 0;
        setBpm(bpm);
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (sampleCountTarget) {
            // printf("block %d\n", sampleNum);

            // sampleCounter += bufferToFill.numSamples;

            // while (sampleCounter >= sampleCountTarget) {
            //     trackEmitter.sendClockTick();
            //     sampleCounter -= sampleCountTarget;
            // }

            int numSamples = bufferToFill.numSamples;

            for (int sample = 0; sample < numSamples; ++sample) {
                sampleCounter++;

                if (sampleCounter >= sampleCountTarget) // Time to send a tick?
                {
                    // printf("tick %d\n", sampleNum);
                    // trackEmitter.sendClockTick(sampleNum);
                    trackEmitter.sendClockTick(sample);
                    sampleCounter = 0; // Reset counter
                }
            }
        }
    }

    void releaseResources() override
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioTempo)
};
