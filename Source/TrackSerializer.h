#pragma once

#include <JuceHeader.h>

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

    juce::File file = juce::File("track.json");
    void save()
    {
        json stepArray;

        // for (const auto& step : steps) {
        //     stepArray.push_back({ { "startStep", step.startStep },
        //         { "pitch", step.pitch },
        //         { "length", step.length } });
        // }

        // juce::FileOutputStream outputStream(file);
        // if (outputStream.openedOk()) {
        //     outputStream.writeText(stepArray.dump(4), false, false, nullptr);
        // }
    }
};