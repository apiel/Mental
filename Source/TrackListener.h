#pragma once

#include <JuceHeader.h>

class TrackListener {
public:
    virtual void onMidiClockTick(int clockCounter, bool isQuarterNote) = 0;
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

    void sendClockTick()
    {
        clockCounter++;
        // Clock events are sent at a rate of 24 pulses per quarter note
        // (24/4 = 6)
        bool isQuarterNote = clockCounter % 6 == 0;
        listeners.call([this, isQuarterNote](TrackListener& l) { 
            l.onMidiClockTick(clockCounter, isQuarterNote);
        });
    }

    void subscribe(TrackListener* listener) { listeners.add(listener); }
};
