#include "BeginnerRenderer.h"
#include "graphics/Companion.h"
#include "mesh/StatsCollector.h"
#include "graphics/img/companion_bitmaps.h"
#include "PowerStatus.h"
#include "NodeDB.h"
#include "main.h"

namespace graphics {

namespace BeginnerRenderer {

void drawBeginnerFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y) {
    display->clear();

    // Update companion state
    Companion::getInstance().update();

    drawCompanion(display, x, y);
    drawStatsBar(display, x, y);
    drawNotification(display, x, y);
}

void drawCompanion(OLEDDisplay *display, int16_t x, int16_t y) {
    Companion& companion = Companion::getInstance();
    CompanionMood mood = companion.getMood();

    const uint8_t* face;
    switch (mood) {
        case CompanionMood::HAPPY: face = companion_happy; break;
        case CompanionMood::EXCITED: face = companion_excited; break;
        case CompanionMood::SAD: face = companion_sad; break;
        case CompanionMood::SLEEPING: face = companion_neutral; break;
        case CompanionMood::LISTENING: face = companion_listening; break;
        default: face = companion_neutral; break;
    }

    int drawX = x + (display->width() - COMPANION_WIDTH) / 2;
    int drawY = y + (display->height() - COMPANION_HEIGHT) / 2 - 5 + companion.getYOffset();

    display->drawXbm(drawX, drawY, COMPANION_WIDTH, COMPANION_HEIGHT, face);

    // Blinking overlay
    if (companion.isBlinking() && mood != CompanionMood::SLEEPING) {
        display->setColor(BLACK);
        display->fillRect(drawX + 12, drawY + 18, 8, 4);
        display->fillRect(drawX + 28, drawY + 18, 8, 4);
        display->setColor(WHITE);
    }
}

void drawStatsBar(OLEDDisplay *display, int16_t x, int16_t y) {
    int barY = y + display->height() - 10;

    // Happiness
    int happiness = (int)(statsCollector.getHappiness() * 100);
    display->drawXbm(x + 5, barY, 8, 8, imgHeart);
    display->drawString(x + 15, barY - 2, String(happiness));

    // Energy
    int energy = powerStatus ? powerStatus->getBatteryChargePercent() : 0;
    display->drawXbm(x + 45, barY, 8, 8, imgLightning);
    display->drawString(x + 55, barY - 2, String(energy));

    // Friends
    int friends = statsCollector.getSociability();
    display->drawXbm(x + 85, barY, 8, 8, imgFriends);
    display->drawString(x + 95, barY - 2, String(friends));
}

void drawNotification(OLEDDisplay *display, int16_t x, int16_t y) {
    // Show recent activity or notifications
    if (statsCollector.hasRecentActivity(5000)) {
        display->setFont(ArialMT_Plain_10);
        display->setTextAlignment(TEXT_ALIGN_CENTER);

        CompanionMood mood = Companion::getInstance().getMood();
        if (mood == CompanionMood::EXCITED) {
            display->drawString(x + display->width() / 2, y + 2, "New Friend!");
            const char* name = statsCollector.getLastFriendName();
            if (name && name[0]) {
                display->drawString(x + display->width() / 2, y + 14, "\"" + String(name) + "\"");
            }
            float dist = statsCollector.getMaxDistanceKm();
            if (dist > 0) {
                display->drawString(x + display->width() / 2, y + 26, String(dist, 1) + "km away");
            }
        } else if (mood == CompanionMood::HAPPY) {
            display->drawString(x + display->width() / 2, y + 5, "Message Received!");
        }
    }
}

} // namespace BeginnerRenderer

} // namespace graphics
