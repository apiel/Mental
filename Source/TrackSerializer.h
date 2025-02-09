#pragma once

#include <JuceHeader.h>

#include "AudioTrack.h"
#include "TrackComponent.h"

#include "libs/nlohmann/json.hpp"
using json = nlohmann::json;

class TrackSerializer {
public:
    TrackSerializer()
    {
    }
    ~TrackSerializer()
    {
    }

    void save(juce::String filePath, AudioTrack& track, TrackComponent& trackComponent)
    {
        juce::File file = juce::File(filePath);
        json stepArray;

        for (const auto& step : track.steps) {
            stepArray.push_back({ { "startStep", step.startStep },
                { "pitch", step.pitch },
                { "length", step.length },
                { "velocity", step.velocity },
                { "condition", step.condition }, // might want to save as string
                { "motion", step.motion } }); // might want to save as string
        }

        json root;
        root["steps"] = stepArray;
        root["color"] = ("#" + trackComponent.color.toDisplayString(false)).toStdString();
        root["name"] = trackComponent.name.toStdString();

        juce::FileOutputStream outputStream(file);
        if (outputStream.openedOk()) {
            outputStream.setPosition(0); // Ensure writing starts from the beginning
            outputStream.truncate(); // Truncate the file to remove old contents
            outputStream.writeText(root.dump(4), false, false, nullptr);
        }
    }

    void load(juce::String filePath, AudioTrack& track, TrackComponent& trackComponent)
    {
        juce::File file = juce::File(filePath);
        if (file.exists()) {
            std::string jsonStr = file.loadFileAsString().toStdString();

            json root = json::parse(jsonStr);
            trackComponent.name = root["name"].get<std::string>();
            std::string colorString = root["color"].get<std::string>(); // Read from JSON
            if (colorString.length() == 7) { // Ensure it's in #RRGGBB format
                int r = std::stoi(colorString.substr(1, 2), nullptr, 16);
                int g = std::stoi(colorString.substr(3, 2), nullptr, 16);
                int b = std::stoi(colorString.substr(5, 2), nullptr, 16);
                trackComponent.setColor(juce::Colour(r, g, b));
            }

            if (root["steps"].is_array()) {
                track.steps.clear();
                for (const auto& step : root["steps"]) {
                    Step newStep;
                    newStep.startStep = step["startStep"].get<int>();
                    newStep.pitch = step["pitch"].get<int>();
                    newStep.length = step["length"].get<int>();
                    newStep.velocity = step["velocity"].get<float>();
                    newStep.condition = step["condition"].get<int>();
                    newStep.motion = step["motion"].get<int>();
                    track.steps.add(newStep);
                }
            }
        }
    }
};