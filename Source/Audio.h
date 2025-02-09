#pragma once

#include "AudioTempo.h"
#include "AudioTrack.h"
#include <JuceHeader.h>

class Audio : public juce::AudioSource {
private:
    juce::AudioDeviceManager deviceManager;
    juce::AudioSourcePlayer audioSourcePlayer;

    AudioTempo& audioTempo = AudioTempo::get();
    AudioTrack track1;

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

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        audioTempo.prepareToPlay(samplesPerBlockExpected, sampleRate);
        track1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        // printf("yoyoyoyo block\n");
        bufferToFill.clearActiveBufferRegion();
        audioTempo.getNextAudioBlock(bufferToFill);
        track1.getNextAudioBlock(bufferToFill);
    }

    void releaseResources() override
    {
        audioTempo.releaseResources();
        track1.releaseResources();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Audio)
};
