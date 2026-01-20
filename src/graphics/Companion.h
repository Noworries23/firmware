#pragma once

#include <Arduino.h>
#include <stdint.h>

namespace graphics {

enum class CompanionMood {
    NEUTRAL,
    HAPPY,
    EXCITED,
    SAD,
    SLEEPING,
    LISTENING
};

class Companion {
public:
    Companion();

    void update();
    void setMood(CompanionMood mood, uint32_t durationMs = 0);
    CompanionMood getMood() const { return currentMood; }

    bool isBlinking() const { return blinking; }
    int getYOffset() const { return yOffset; }

    static Companion& getInstance();

private:
    CompanionMood currentMood = CompanionMood::NEUTRAL;
    CompanionMood baseMood = CompanionMood::NEUTRAL;
    uint32_t moodUntil = 0;

    bool blinking = false;
    int yOffset = 0;

    uint32_t lastBlink = 0;
    uint32_t nextBlinkAt = 0;
};

} // namespace graphics
