#include "Companion.h"
#include "PowerStatus.h"
#include "mesh/NodeDB.h"
#include "main.h"

namespace graphics {

Companion::Companion() {
    nextBlinkAt = millis() + 3000;
}

Companion& Companion::getInstance() {
    static Companion instance;
    return instance;
}

void Companion::update() {
    uint32_t now = millis();

    // Manage temporary moods
    if (moodUntil > 0 && now > moodUntil) {
        currentMood = baseMood;
        moodUntil = 0;
    }

    // Determine base mood from environment if not in a temporary mood
    if (moodUntil == 0) {
        if (powerStatus && powerStatus->getBatteryChargePercent() < 20 && !powerStatus->getIsCharging()) {
            baseMood = CompanionMood::SAD;
        } else {
            baseMood = CompanionMood::NEUTRAL;
        }
        currentMood = baseMood;
    }

    // Breathing animation (very slow oscillation)
    // Using a simplified integer oscillation to avoid frequent sin() calls if desired,
    // but sin() is fine on Heltec V3 (ESP32). I'll keep it but ensure it's correct.
    yOffset = (int)(sin(now / 1000.0) * 2.0);

    // Blinking logic
    if (!blinking && now > nextBlinkAt) {
        blinking = true;
        lastBlink = now;
    } else if (blinking && now > lastBlink + 150) {
        blinking = false;
        nextBlinkAt = now + 5000;
    }
}

void Companion::setMood(CompanionMood mood, uint32_t durationMs) {
    currentMood = mood;
    if (durationMs > 0) {
        moodUntil = millis() + durationMs;
    } else {
        baseMood = mood;
        moodUntil = 0;
    }
}

} // namespace graphics
