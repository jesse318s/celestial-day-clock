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

std::vector<std::string> OrreryTimepiece::getTimesMilitary() const {
	std::vector<std::string> times;

	for (const auto& [label, clock] : clocks) {
		times.emplace_back(label + clock->getTimeMilitary());
	}

	return times;
}

std::vector<std::string> OrreryTimepiece::getTimes() const {
	std::vector<std::string> times;

	for (const auto& [label, clock] : clocks) {
		times.emplace_back(label + clock->getTime());
	}

	return times;
}

void OrreryTimepiece::tick() {
	for (std::pair<std::string, CelestialDayClock*>& clock : clocks) {
		clock.second->tick();
	}
}