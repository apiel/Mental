#pragma once

#include "AudioTempo.h"
#include "AudioTrack.h"
#include <JuceHeader.h>

#include <vector>

class Audio : public juce::AudioSource {
private:
    juce::AudioDeviceManager deviceManager;
    juce::AudioSourcePlayer audioSourcePlayer;
    juce::MixerAudioSource mixer;

    AudioTempo& audioTempo = AudioTempo::get();
    std::vector<std::unique_ptr<AudioTrack>> tracks;

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

    AudioTrack& addTrack()
    {
        tracks.push_back(std::make_unique<AudioTrack>());
        auto& track = tracks.back(); // Return reference to new track

        mixer.addInputSource(track.get(), false);

        if (auto* device = deviceManager.getCurrentAudioDevice()) {
            double currentSampleRate = device->getCurrentSampleRate();
            int currentBlockSize = device->getCurrentBufferSizeSamples();
            if (currentSampleRate > 0.0 && currentBlockSize > 0) {
                track->prepareToPlay(currentBlockSize, currentSampleRate);
            }
        }
        return *track;
    }

    AudioTrack& getTrack(int index)
    {
        jassert(index >= 0 && index < static_cast<int>(tracks.size()));
        return *tracks[index];
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        audioTempo.prepareToPlay(samplesPerBlockExpected, sampleRate);
        // for (auto& track : tracks) {
        //     track->prepareToPlay(samplesPerBlockExpected, sampleRate);
        // }
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        audioTempo.getNextAudioBlock(bufferToFill);
        mixer.getNextAudioBlock(bufferToFill);
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
