#pragma once

#include <JuceHeader.h>

class TrackListener {
public:
    virtual void onMidiClockTick(int clockCounter, bool isQuarterNote, int sampleNum) = 0;
};

class TrackEmitter {
private:
    juce::ListenerList<TrackListener> listeners;

public:
    static TrackEmitter& get()
    {
        static TrackEmitter instance;
        return instance;
    }

    void sendClockTick(int clockCounter, bool isQuarterNote, int sampleNum)
    {
        listeners.call([clockCounter, isQuarterNote, sampleNum](TrackListener& l) { 
            l.onMidiClockTick(clockCounter, isQuarterNote, sampleNum);
        });
    }

    void subscribe(TrackListener* listener) { listeners.add(listener); }
};
