#pragma once

#include "AudioTempo.h"
#include "KnobLookAndFeel.h"
#include "TrackListener.h"
#include <JuceHeader.h>

class TempoComponent : public juce::Component {
private:
    juce::Slider tempoKnob;
    KnobLookAndFeel customLookAndFeel;
    AudioTempo& audioTempo = AudioTempo::get();

public:
    TempoComponent()
        : customLookAndFeel({ juce::Colours::palevioletred, " BPM" })
    {
        tempoKnob.setLookAndFeel(&customLookAndFeel);
        tempoKnob.setSliderStyle(juce::Slider::Rotary);
        tempoKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        tempoKnob.setRange(30.0, 300.0, 1.0); // Tempo range from 30 to 300 BPM
        tempoKnob.setValue(audioTempo.getBpm()); // Default tempo
        tempoKnob.onValueChange = [this] {
            audioTempo.setBpm(tempoKnob.getValue());
        };
        addAndMakeVisible(tempoKnob);
    }

    ~TempoComponent() override
    {
        tempoKnob.setLookAndFeel(nullptr);
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

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TempoComponent)
};
