#include "StatsCollector.h"
#include "NodeDB.h"
#include "gps/GeoCoord.h"
#include "main.h"

namespace meshtastic {

StatsCollector::StatsCollector() {
    lastActivityMs = millis();
}

void StatsCollector::recordMessageSent() {
    messagesSent++;
    lastActivityMs = millis();
}

void StatsCollector::recordAckReceived() {
    acksReceived++;
    lastActivityMs = millis();
}

void StatsCollector::recordNakReceived() {
    naksReceived++;
    lastActivityMs = millis();
}

void StatsCollector::recordRelay() {
    relaysPerformed++;
    lastActivityMs = millis();
}

void StatsCollector::setLastFriendName(const char* name) {
    if (name) {
        strncpy(lastFriendName, name, sizeof(lastFriendName) - 1);
        lastFriendName[sizeof(lastFriendName) - 1] = '\0';
    }
}

float StatsCollector::getHappiness() const {
    if (messagesSent == 0) return 1.0f; // Happy by default
    float successRate = (float)acksReceived / (float)messagesSent;
    return successRate;
}

int StatsCollector::getSociability() const {
    return nodeDB->getNumMeshNodes();
}

void StatsCollector::updateMaxDistance() {
    float maxD = 0.0f;
    meshtastic_NodeInfoLite *ourNode = nodeDB->getMeshNode(nodeDB->getNodeNum());
    if (!ourNode || !nodeDB->hasValidPosition(ourNode)) {
        maxDistanceKm = 0.0f;
        return;
    }

    for (size_t i = 0; i < nodeDB->getNumMeshNodes(); i++) {
        const meshtastic_NodeInfoLite *n = nodeDB->getMeshNodeByIndex(i);
        if (n && n->num != ourNode->num && nodeDB->hasValidPosition(n)) {
            float d = GeoCoord::latLongToMeter(
                ourNode->position.latitude_i * 1e-7, ourNode->position.longitude_i * 1e-7,
                n->position.latitude_i * 1e-7, n->position.longitude_i * 1e-7
            ) / 1000.0f;
            if (d > maxD) maxD = d;
        }
    }
    maxDistanceKm = maxD;
}

bool StatsCollector::hasRecentActivity(uint32_t timeoutMs) const {
    return (millis() - lastActivityMs) < timeoutMs;
}

} // namespace meshtastic

meshtastic::StatsCollector& statsCollector = meshtastic::StatsCollector::getInstance();
