#pragma once

#include "AudioTempo.h"
#include "AudioTrack.h"
#include <JuceHeader.h>

#include <vector>

class Audio : public juce::AudioSource {
private:
    juce::AudioDeviceManager deviceManager;
    juce::AudioSourcePlayer audioSourcePlayer;

    AudioTempo& audioTempo = AudioTempo::get();
    std::vector<std::unique_ptr<AudioTrack>> tracks;

        double sampleRate = 0.0;
    int samplesPerBlockExpected = 0;

    Audio()
    {
        juce::String audioError = deviceManager.initialise(0, 2, nullptr, true, "Mental");
        jassert(audioError.isEmpty());

        deviceManager.addAudioCallback(&audioSourcePlayer);
        audioSourcePlayer.setSource(this);

        // TO be removed once everything is setup
        addTrack();
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

    AudioTrack& addTrack()
    {
        tracks.push_back(std::make_unique<AudioTrack>());
        AudioTrack& track = *tracks.back(); // Return reference to new track
        track.prepareToPlay(samplesPerBlockExpected, sampleRate);
        return track;
    }

    AudioTrack& getTrack(int index)
    {
        jassert(index >= 0 && index < static_cast<int>(tracks.size()));
        return *tracks[index];
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        this->samplesPerBlockExpected = samplesPerBlockExpected;
        this->sampleRate = sampleRate;
        audioTempo.prepareToPlay(samplesPerBlockExpected, sampleRate);
        for (auto& track : tracks) {
            track->prepareToPlay(samplesPerBlockExpected, sampleRate);
        }
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        audioTempo.getNextAudioBlock(bufferToFill);
        // TODO need to use a mixer
        for (auto& track : tracks) {
            track->getNextAudioBlock(bufferToFill);
        }
    }

    void releaseResources() override
    {
        audioTempo.releaseResources();
        for (auto& track : tracks) {
            track->releaseResources();
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Audio)
};
