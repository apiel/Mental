#pragma once

#include <JuceHeader.h>

class KnobLookAndFeel : public juce::LookAndFeel_V4 {
private:
    juce::Colour colour = juce::Colours::white;
    juce::String unit = "";

public:
    KnobLookAndFeel()
    {
    }

    struct Options {
        juce::Colour colour = juce::Colours::white;
        juce::String unit = "";
    };

    KnobLookAndFeel(const Options& options)
        : colour(options.colour)
        , unit(options.unit)
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
        // g.setColour(juce::Colours::grey.withBrightness(0.3f));
        // g.fillEllipse(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f);
        // g.setColour(juce::Colours::grey);
        g.setColour(colour.withAlpha(0.3f));
        juce::Path arcBg;
        arcBg.addArc(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f,
            rotaryStartAngle, rotaryEndAngle, true);
        g.strokePath(arcBg, juce::PathStrokeType(2.0f));

        g.setColour(colour);
        juce::Path arc;
        arc.addArc(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f,
            rotaryStartAngle, angle, true);
        g.strokePath(arc, juce::PathStrokeType(8.0f));
    }

    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        g.setColour(label.findColour(juce::Label::textColourId));
        g.setFont(label.getFont());
        g.drawFittedText(label.getText() + unit, label.getLocalBounds(), juce::Justification::centred, 1);
    }
};