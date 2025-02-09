#pragma once

#include "AudioTempo.h"
#include "AudioTrack.h"
#include <JuceHeader.h>

class Audio : public juce::AudioSource {
private:
    juce::AudioDeviceManager deviceManager;
    juce::AudioSourcePlayer audioSourcePlayer;

    AudioTempo& audioTempo = AudioTempo::get();
    AudioTrack tracks[TRACK_COUNT];

    Audio()
    {
        juce::String audioError = deviceManager.initialise(0, 2, nullptr, true, "Mental");
        jassert(audioError.isEmpty());

        deviceManager.addAudioCallback(&audioSourcePlayer);
        audioSourcePlayer.setSource(this);
    }

public:
    static Audio& get()
    {
        static Audio instance;
        return instance;
    }

    ~Audio()
    {
        audioSourcePlayer.setSource(nullptr);
        deviceManager.removeAudioCallback(&audioSourcePlayer);
        deviceManager.closeAudioDevice();
    }

    AudioTrack& getTrack(int index)
    {
        jassert(index < TRACK_COUNT);
        jassert(index >= 0);
        return tracks[index];
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        audioTempo.prepareToPlay(samplesPerBlockExpected, sampleRate);
        for (int i = 0; i < TRACK_COUNT; i++) {
            tracks[i].prepareToPlay(samplesPerBlockExpected, sampleRate);
        }
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        audioTempo.getNextAudioBlock(bufferToFill);
        // TODO need to use a mixer
        for (int i = 0; i < TRACK_COUNT; i++) {
            tracks[i].getNextAudioBlock(bufferToFill);
        }
    }

    void releaseResources() override
    {
        audioTempo.releaseResources();
        for (int i = 0; i < TRACK_COUNT; i++) {
            tracks[i].releaseResources();
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Audio)
};
