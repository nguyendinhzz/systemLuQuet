#ifndef STATIONMANAGER_H
#define STATIONMANAGER_H

#include <Arduino.h>
#include <vector>

class Station {
public:
    String idNode;
    int deviceCount;
    String status;
    unsigned long timestamp;
    Station(String idNode, const int& deviceCount,const String& status);
};

class StationManager {
private:
    std::vector<Station> stations;
public:
    void addStation(const Station& station);
    void updateStation(String idNode,const int& deviceCount, const String& status);
    std::vector<Station> getStations() const;
    void updateTime(String idNode);
    void resetStations(String idNode);
    void checkTimeoutStaion();
};

void scanStations(StationManager &stationManager);

#endif // STATIONMANAGER_H
