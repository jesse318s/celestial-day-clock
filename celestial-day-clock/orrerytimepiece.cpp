#include "orrerytimepiece.h"

OrreryTimepiece::~OrreryTimepiece() {
	for (std::pair<std::string, CelestialDayClock*>& clock : clocks) {
		delete clock.second;
	}
}

void OrreryTimepiece::clear() {
	for (std::pair<std::string, CelestialDayClock*>& clock : clocks) {
		delete clock.second;
	}

	clocks.clear();
}

std::vector<std::string> OrreryTimepiece::getAllTimesMilitary() const {
	std::vector<std::string> times;

	for (const std::pair<std::string, CelestialDayClock*>& clock : clocks) {
		times.emplace_back(clock.first + clock.second->getTimeMilitary());
	}

	return times;
}

std::vector<std::string> OrreryTimepiece::getAllTimes() const {
	std::vector<std::string> times;

	for (const std::pair<std::string, CelestialDayClock*>& clock : clocks) {
		times.emplace_back(clock.first + clock.second->getTime());
	}

	return times;
}

void OrreryTimepiece::tick() {
	for (std::pair<std::string, CelestialDayClock*>& clock : clocks) {
		clock.second->tick();
	}
}