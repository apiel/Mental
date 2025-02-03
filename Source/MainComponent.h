#pragma once

// VsCode doesn't seems to recognize JUCE_PLUGINHOST_VST3, so I just define it myself.
#define JUCE_PLUGINHOST_VST3 1

#include <JuceHeader.h>

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent()
    {
        setSize(1200, 800);
        setAudioChannels(0, 2);
        loadPlugin();
    }

    ~MainComponent() override
    {
        pluginEditor.reset();    // Destroy the editor first
        plugin_instance.reset(); // Then destroy the plugin instance
        shutdownAudio();
    }

    void paint(juce::Graphics &g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

        g.setFont(juce::FontOptions(16.0f));
        g.setColour(juce::Colours::white);
        g.drawText("Hello World!", getLocalBounds(), juce::Justification::centred, true);
    }

    // void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        if (plugin_instance)
        {
            plugin_instance->setRateAndBufferSizeDetails(sampleRate, samplesPerBlockExpected);
            plugin_instance->prepareToPlay(sampleRate, samplesPerBlockExpected);
        }
    }

    void releaseResources() override
    {
        if (plugin_instance)
            plugin_instance->releaseResources();
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override
    {
        if (plugin_instance)
        {
            juce::MidiBuffer midi; // Empty MIDI buffer (for now)
            plugin_instance->processBlock(*bufferToFill.buffer, midi);
        }
    }

    juce::AudioPluginFormatManager pluginmanager;
    std::unique_ptr<juce::AudioPluginInstance> plugin_instance;
    std::unique_ptr<juce::AudioProcessorEditor> pluginEditor;
    void loadPlugin()
    {
        pluginmanager.addDefaultFormats();

        juce::OwnedArray<juce::PluginDescription> descs;
        juce::VST3PluginFormat v3format;
        v3format.findAllTypesForFile(descs, "/usr/lib/vst3/Vital.vst3");

        if (descs.size() > 0)
        {
            juce::String error;
            plugin_instance = pluginmanager.createPluginInstance(*descs[0], 44100, 512, error);
            if (!plugin_instance)
            {
                std::cout << error << "\n";
            }
            else
            {
                std::cout << "created " << descs[0]->descriptiveName << "\n";

                if (plugin_instance->hasEditor())
                {
                    pluginEditor.reset(plugin_instance->createEditor()); // Assign to unique_ptr
                    if (pluginEditor)
                    {
                        addAndMakeVisible(pluginEditor.get());                                                  // Add safely
                        pluginEditor->setBounds(30, 10, pluginEditor->getWidth(), pluginEditor->getHeight()); // Set position
                    }
                }
                else
                {
                    std::cout << "no editor\n";
                }
            }
        }
        else
        {
            std::cout << "no plugins found\n";
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
