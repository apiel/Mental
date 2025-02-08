#pragma once

#include <JuceHeader.h>

class TrackListener {
public:
    virtual void onMidiClockTick(int clockCounter, bool isQuarterNote, int sampleNum) = 0;
};

class TrackEmitter {
private:
    juce::ListenerList<TrackListener> listeners;
    int clockCounter = 0;

public:
    static TrackEmitter& get()
    {
        static TrackEmitter instance;
        return instance;
    }

    void sendClockTick(int sampleNum)
    {
        clockCounter++;
        // Clock events are sent at a rate of 24 pulses per quarter note
        // (24/4 = 6)
        bool isQuarterNote = clockCounter % 6 == 0;
        listeners.call([this, isQuarterNote, sampleNum](TrackListener& l) { 
            l.onMidiClockTick(clockCounter, isQuarterNote, sampleNum);
        });
    }

    void subscribe(TrackListener* listener) { listeners.add(listener); }
};
