#pragma once

#include "KnobLookAndFeel.h"
#include "ClockListener.h"
#include <JuceHeader.h>

class TempoComponent : public juce::AudioAppComponent {
private:
    juce::Slider tempoKnob;
    KnobLookAndFeel customLookAndFeel;
    double sampleRate = 0.0;
    int samplesPerBlockExpected = 0;
    double bpm = 160.0;
    int sampleCountTarget = 0;
    int sampleCounter = 0;

    ClockEmitter& clockEmitter = ClockEmitter::get();

    void updateTempo()
    {
        bpm = tempoKnob.getValue();
        if (bpm > 0.0) {
            sampleCountTarget = static_cast<int>((sampleRate * 60.0f / bpm) / 12.0f);
        }
        // Here you can send a notification to update timing in other components
        DBG("Tempo changed to: " << bpm << " BPM (" << sampleCountTarget << " samples per block)");
    }

public:
    TempoComponent()
        : customLookAndFeel({ juce::Colours::palevioletred, " BPM" })
    {
        setAudioChannels(0, 2);

        tempoKnob.setLookAndFeel(&customLookAndFeel);
        tempoKnob.setSliderStyle(juce::Slider::Rotary);
        tempoKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        tempoKnob.setRange(30.0, 300.0, 1.0); // Tempo range from 30 to 300 BPM
        tempoKnob.setValue(160.0); // Default tempo
        tempoKnob.onValueChange = [this] { updateTempo(); };
        addAndMakeVisible(tempoKnob);
    }

    ~TempoComponent() override
    {
        tempoKnob.setLookAndFeel(nullptr);
        shutdownAudio();
    }

    void releaseResources() override
    {
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        tempoKnob.setBounds(area.removeFromTop(100));
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        this->samplesPerBlockExpected = samplesPerBlockExpected;
        this->sampleRate = sampleRate;
        sampleCounter = 0;
        updateTempo();
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        sampleCounter += bufferToFill.numSamples;

        while (sampleCounter >= sampleCountTarget) {
            clockEmitter.sendClockTick();
            sampleCounter -= sampleCountTarget;
        }

        bufferToFill.clearActiveBufferRegion();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TempoComponent)
};
