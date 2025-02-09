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
};