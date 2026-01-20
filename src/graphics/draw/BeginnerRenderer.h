#pragma once

#include <OLEDDisplay.h>
#include <OLEDDisplayUi.h>

namespace graphics {

namespace BeginnerRenderer {
    void drawBeginnerFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
    void drawCompanion(OLEDDisplay *display, int16_t x, int16_t y);
    void drawStatsBar(OLEDDisplay *display, int16_t x, int16_t y);
    void drawNotification(OLEDDisplay *display, int16_t x, int16_t y);
}

} // namespace graphics
