#pragma once

// VsCode doesn't seems to recognize JUCE_PLUGINHOST_VST3, so I just define it myself.
#define JUCE_PLUGINHOST_VST3 1

#include <JuceHeader.h>

// https://forum.juce.com/t/juce-plugin-host/61888

class MainComponent : public juce::Component
{
public:
    MainComponent()
    {
        setSize(1200, 800);
        loadPlugin();
    }
    // ~MainComponent() override;

    void paint(juce::Graphics &g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

        g.setFont(juce::FontOptions(16.0f));
        g.setColour(juce::Colours::white);
        g.drawText("Hello World!", getLocalBounds(), juce::Justification::centred, true);
    }

    // void resized() override;

    juce::AudioPluginFormatManager pluginmanager;
    std::unique_ptr<juce::AudioPluginInstance> plugin_instance;
    void loadPlugin()
    {
        pluginmanager.addDefaultFormats();
        // the single .vst3 file may have multiple plugin types, so we need to deal with an array
        // of PluginDescriptions
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
                    auto *editor = plugin_instance->createEditor();
                    if (editor)
                    {
                        addAndMakeVisible(editor);
                        editor->setBounds(100, 100, editor->getWidth(), editor->getHeight()); // Set to desired position
                    }
                } else {
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
