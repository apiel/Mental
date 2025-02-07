#pragma once

struct StepCondition {
    const char* name;
    bool (*conditionMet)(uint8_t loopCounter);
} stepConditions[] = {
    { "---", [](uint8_t loopCounter) { return true; } },
    { "Pair", [](uint8_t loopCounter) { return loopCounter % 2 == 0; } },
    { "4th", [](uint8_t loopCounter) { return loopCounter % 4 == 0; } },
    { "6th", [](uint8_t loopCounter) { return loopCounter % 6 == 0; } },
    { "8th", [](uint8_t loopCounter) { return loopCounter % 8 == 0; } },
    { "Impair", [](uint8_t loopCounter) { return loopCounter % 2 == 1; } },
    { "1%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) == 0; } },
    { "2%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 2; } },
    { "5%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 5; } },
    { "10%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 10; } },
    { "20%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 20; } },
    { "30%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 30; } },
    { "40%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 40; } },
    { "50%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 50; } },
    { "60%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 60; } },
    { "70%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 70; } },
    { "80%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 80; } },
    { "90%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 90; } },
    { "95%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 95; } },
    { "98%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 98; } },
    { "99%", [](uint8_t loopCounter) { return (juce::Random::getSystemRandom().nextInt (100)) < 99; } },
};

struct StepMotion {
    const char* name;
    std::function<int8_t(uint8_t loopCounter)> get;
} stepMotions[] = {
    { "---", [](uint8_t loopCounter) { return 0; } },
    { "0,1", [](uint8_t loopCounter) { uint8_t semitone[2] = { 0, 1 }; return semitone[loopCounter % 2]; } },
    { "1,0", [](uint8_t loopCounter) { uint8_t semitone[2] = { 1, 0 }; return semitone[loopCounter % 2]; } },
    { "0,2", [](uint8_t loopCounter) { uint8_t semitone[2] = { 0, 2 }; return semitone[loopCounter % 2]; } },
    { "2,0", [](uint8_t loopCounter) { uint8_t semitone[2] = { 2, 0 }; return semitone[loopCounter % 2]; } },
    { "0,3", [](uint8_t loopCounter) { uint8_t semitone[2] = { 0, 3 }; return semitone[loopCounter % 2]; } },
    { "3,0", [](uint8_t loopCounter) { uint8_t semitone[2] = { 3, 0 }; return semitone[loopCounter % 2]; } },
    { "0,4", [](uint8_t loopCounter) { uint8_t semitone[2] = { 0, 4 }; return semitone[loopCounter % 2]; } },
    { "4,0", [](uint8_t loopCounter) { uint8_t semitone[2] = { 4, 0 }; return semitone[loopCounter % 2]; } },
    { "0,5", [](uint8_t loopCounter) { uint8_t semitone[2] = { 0, 5 }; return semitone[loopCounter % 2]; } },
    { "5,0", [](uint8_t loopCounter) { uint8_t semitone[2] = { 5, 0 }; return semitone[loopCounter % 2]; } },
    { "0,6", [](uint8_t loopCounter) { uint8_t semitone[2] = { 0, 6 }; return semitone[loopCounter % 2]; } },
    { "6,0", [](uint8_t loopCounter) { uint8_t semitone[2] = { 6, 0 }; return semitone[loopCounter % 2]; } },
    { "0,12", [](uint8_t loopCounter) { uint8_t semitone[2] = { 0, 12 }; return semitone[loopCounter % 2]; } },
    { "12,0", [](uint8_t loopCounter) { uint8_t semitone[2] = { 12, 0 }; return semitone[loopCounter % 2]; } },
    { "0,1,2", [](uint8_t loopCounter) { uint8_t semitone[3] = { 0, 1, 2 }; return semitone[loopCounter % 3]; } },
    { "2,1,0", [](uint8_t loopCounter) { uint8_t semitone[3] = { 2, 1, 0 }; return semitone[loopCounter % 3]; } },
    { "0,2,4", [](uint8_t loopCounter) { uint8_t semitone[3] = { 0, 2, 4 }; return semitone[loopCounter % 3]; } },
    { "4,2,0", [](uint8_t loopCounter) { uint8_t semitone[3] = { 4, 2, 0 }; return semitone[loopCounter % 3]; } },
    { "0,3,6", [](uint8_t loopCounter) { uint8_t semitone[3] = { 0, 3, 6 }; return semitone[loopCounter % 3]; } },
    { "6,3,0", [](uint8_t loopCounter) { uint8_t semitone[3] = { 6, 3, 0 }; return semitone[loopCounter % 3]; } },
    { "0,4,8", [](uint8_t loopCounter) { uint8_t semitone[3] = { 0, 4, 8 }; return semitone[loopCounter % 3]; } },
    { "8,4,0", [](uint8_t loopCounter) { uint8_t semitone[3] = { 8, 4, 0 }; return semitone[loopCounter % 3]; } },
    { "0,6,12", [](uint8_t loopCounter) { uint8_t semitone[3] = { 0, 6, 12 }; return semitone[loopCounter % 3]; } },
    { "12,6,0", [](uint8_t loopCounter) { uint8_t semitone[3] = { 12, 6, 0 }; return semitone[loopCounter % 3]; } },
    { "0,12,24", [](uint8_t loopCounter) { uint8_t semitone[3] = { 0, 12, 24 }; return semitone[loopCounter % 3]; } },
    { "24,12,0", [](uint8_t loopCounter) { uint8_t semitone[3] = { 24, 12, 0 }; return semitone[loopCounter % 3]; } },
    { "0,1,2,3", [](uint8_t loopCounter) { uint8_t semitone[4] = { 0, 1, 2, 3 }; return semitone[loopCounter % 4]; } },
    { "3,2,1,0", [](uint8_t loopCounter) { uint8_t semitone[4] = { 3, 2, 1, 0 }; return semitone[loopCounter % 4]; } },
    { "0,2,4,6", [](uint8_t loopCounter) { uint8_t semitone[4] = { 0, 2, 4, 6 }; return semitone[loopCounter % 4]; } },
    { "6,4,2,0", [](uint8_t loopCounter) { uint8_t semitone[4] = { 6, 4, 2, 0 }; return semitone[loopCounter % 4]; } },
    { "0,3,6,9", [](uint8_t loopCounter) { uint8_t semitone[4] = { 0, 3, 6, 9 }; return semitone[loopCounter % 4]; } },
    { "9,6,3,0", [](uint8_t loopCounter) { uint8_t semitone[4] = { 9, 6, 3, 0 }; return semitone[loopCounter % 4]; } },
    { "0,4,8,12", [](uint8_t loopCounter) { uint8_t semitone[4] = { 0, 4, 8, 12 }; return semitone[loopCounter % 4]; } },
    { "12,8,4,0", [](uint8_t loopCounter) { uint8_t semitone[4] = { 12, 8, 4, 0 }; return semitone[loopCounter % 4]; } },
    { "0,6,12,18", [](uint8_t loopCounter) { uint8_t semitone[4] = { 0, 6, 12, 18 }; return semitone[loopCounter % 4]; } },
    { "18,12,6,0", [](uint8_t loopCounter) { uint8_t semitone[4] = { 18, 12, 6, 0 }; return semitone[loopCounter % 4]; } },
    { "0,2,1,3", [](uint8_t loopCounter) { uint8_t semitone[4] = { 0, 2, 1, 3 }; return semitone[loopCounter % 4]; } },
    { "3,1,2,0", [](uint8_t loopCounter) { uint8_t semitone[4] = { 3, 1, 2, 0 }; return semitone[loopCounter % 4]; } },
    { "0,4,2,6", [](uint8_t loopCounter) { uint8_t semitone[4] = { 0, 4, 2, 6 }; return semitone[loopCounter % 4]; } },
    { "6,2,4,0", [](uint8_t loopCounter) { uint8_t semitone[4] = { 6, 2, 4, 0 }; return semitone[loopCounter % 4]; } },
    { "0,6,3,9", [](uint8_t loopCounter) { uint8_t semitone[4] = { 0, 6, 3, 9 }; return semitone[loopCounter % 4]; } },
    { "9,3,6,0", [](uint8_t loopCounter) { uint8_t semitone[4] = { 9, 3, 6, 0 }; return semitone[loopCounter % 4]; } },
    { "0,8,4,12", [](uint8_t loopCounter) { uint8_t semitone[4] = { 0, 8, 4, 12 }; return semitone[loopCounter % 4]; } },
    { "12,4,8,0", [](uint8_t loopCounter) { uint8_t semitone[4] = { 12, 4, 8, 0 }; return semitone[loopCounter % 4]; } },
    { "0,12,6,18", [](uint8_t loopCounter) { uint8_t semitone[4] = { 0, 12, 6, 18 }; return semitone[loopCounter % 4]; } },
    { "18,6,12,0", [](uint8_t loopCounter) { uint8_t semitone[4] = { 18, 6, 12, 0 }; return semitone[loopCounter % 4]; } },
};

uint8_t STEP_CONDITIONS_COUNT = sizeof(stepConditions) / sizeof(stepConditions[0]);
uint8_t STEP_MOTIONS_COUNT = sizeof(stepMotions) / sizeof(stepMotions[0]);

struct Step {
    int startStep;
    int pitch;
    int length;
    float velocity = 1.0f;
    int condition = 0;
    int motion = 0;
};