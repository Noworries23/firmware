#pragma once

#include <stdint.h>
#include <Arduino.h>

namespace meshtastic {

class StatsCollector {
public:
    StatsCollector();

    void recordMessageSent();
    void recordAckReceived();
    void recordNakReceived();
    void recordRelay();

    float getHappiness() const; // 0.0 to 1.0
    int getSociability() const;
    float getMaxDistanceKm() { return maxDistanceKm; }
    void updateMaxDistance();
    const char* getLastFriendName() const { return lastFriendName; }
    void setLastFriendName(const char* name);

    uint32_t getLastActivityMs() const { return lastActivityMs; }
    bool hasRecentActivity(uint32_t timeoutMs = 5000) const;

    static StatsCollector& getInstance() {
        static StatsCollector instance;
        return instance;
    }

private:
    uint32_t messagesSent = 0;
    uint32_t acksReceived = 0;
    uint32_t naksReceived = 0;
    uint32_t relaysPerformed = 0;

    uint32_t lastActivityMs = 0;
    float maxDistanceKm = 0.0f;
    char lastFriendName[32] = {0};
};

} // namespace meshtastic

extern meshtastic::StatsCollector& statsCollector;
