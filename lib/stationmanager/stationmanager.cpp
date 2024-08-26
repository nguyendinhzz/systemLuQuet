#include "stationmanager.h"

// Constructor for Station class
Station::Station(String idNode,const int& deviceCount, const String& status)
    : idNode(idNode),deviceCount(deviceCount), status(status) {}

// Add a station
void StationManager::addStation(const Station& station) {
    stations.push_back(station);
}

// Update a station's status
void StationManager::updateStation(String idNode,const int& deviceCount, const String& status) {
    for (auto& station : stations) {
        if (station.idNode == idNode) {
            station.status = status;
            break;
        }
    }
}

std::vector<Station> StationManager::getStations() const {
    return stations;
}
void scanStations(StationManager &stationManager){
    
}
void StationManager::resetStations(String idNode) {
    for (auto& station : stations) {
        if (station.idNode == idNode) {
            station.status = "scan";
            return;
        }
    }
}
void StationManager::checkTimeoutStaion(){
    for(auto& station : stations){
        if(millis()-station.timestamp>40000){
            station.status="offline";
        }
    }
}
void StationManager::updateTime(String idNode){
    for(auto& station : stations){
        if(station.idNode==idNode){
            station.timestamp=millis();
        }
    }
}