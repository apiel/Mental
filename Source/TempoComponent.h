#pragma once

#include <JuceHeader.h>

class CustomKnobLookAndFeel : public juce::LookAndFeel_V4 {
private:
    juce::Colour sliderColour = juce::Colours::white;

public:
    CustomKnobLookAndFeel()
    {
    }

    CustomKnobLookAndFeel(juce::Colour colour)
        : sliderColour(colour)
    {
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
        juce::Slider& slider) override
    {
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centerX = (float)x + (float)width * 0.5f;
        auto centerY = (float)y + (float)height * 0.5f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // Draw background circle
        g.setColour(juce::Colours::grey.withBrightness(0.3f));
        g.fillEllipse(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f);
        g.setColour(juce::Colours::grey);
        juce::Path arcBg;
        arcBg.addArc(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f,
            rotaryStartAngle, rotaryEndAngle, true);
        g.strokePath(arcBg, juce::PathStrokeType(2.0f));

        g.setColour(sliderColour);
        juce::Path arc;
        arc.addArc(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f,
            rotaryStartAngle, angle, true);
        g.strokePath(arc, juce::PathStrokeType(6.0f));
    }

    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        g.setColour(label.findColour(juce::Label::textColourId));
        g.setFont(label.getFont());
        g.drawFittedText(label.getText() + " bpm", label.getLocalBounds(), juce::Justification::centred, 1);
    }
};

class TempoComponent : public juce::AudioAppComponent {
private:
    juce::Slider tempoKnob;
    CustomKnobLookAndFeel customLookAndFeel;
    double sampleRate = 0.0;
    int samplesPerBlockExpected = 0;
    double bpm = 160.0;

    void updateTempo()
    {
        bpm = tempoKnob.getValue();
        // Here you can send a notification to update timing in other components
        DBG("Tempo changed to: " << bpm << " BPM");
    }

public:
    TempoComponent()
        : customLookAndFeel(juce::Colours::aqua)
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
        updateTempo();
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion(); // Silence the output
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TempoComponent)
};
