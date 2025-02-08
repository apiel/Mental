#pragma once

#include <JuceHeader.h>

class ClockListener {
public:
    virtual void onMidiClockTick(int clockCounter, bool isQuarterNote) = 0;
};

class ClockEmitter {
private:
    juce::ListenerList<ClockListener> listeners;
    int clockCounter = 0;

public:
    static ClockEmitter& get()
    {
        static ClockEmitter instance;
        return instance;
    }

    void sendClockTick()
    {
        clockCounter++;
        // Clock events are sent at a rate of 24 pulses per quarter note
        // (24/4 = 6)
        bool isQuarterNote = clockCounter % 6 == 0;
        listeners.call([this, isQuarterNote](ClockListener& l) { 
            l.onMidiClockTick(clockCounter, isQuarterNote);
        });
    }

    void subscribe(ClockListener* listener) { listeners.add(listener); }
};
