#include "orrerytimepiece.h"

OrreryTimepiece::~OrreryTimepiece() { deleteClocks(); }

void OrreryTimepiece::add(const std::string& label, CelestialDayClock* clock) {
	if (clock == nullptr) throw std::invalid_argument("Cannot add a null clock");

	for (const auto& [existingLabel, existingClock] : clocks) {
		if (existingClock == nullptr)
			throw std::runtime_error("Null clock pointer encountered in add");

		if (label == existingLabel) {
			std::cerr << "Clock with label " << label << " already exists" << std::endl;
			return;
		}
	}

	clocks.emplace_back(label, clock);
}

CelestialDayClock& OrreryTimepiece::getClock(const std::string& searchLabel) {
	for (const auto& [label, clock] : clocks) {
		if (clock == nullptr)
			throw std::runtime_error("Null clock pointer encountered in getClock");

		if (searchLabel == label) return *clock;
	}

	throw std::runtime_error("Clock with label " + searchLabel + " not found");
}

void OrreryTimepiece::clear() {
	deleteClocks();
	clocks.clear();
}

std::vector<std::string> OrreryTimepiece::getTimesMilitary() const {
	std::vector<std::string> times;

	for (const auto& [label, clock] : clocks) {
		if (clock == nullptr)
			throw std::runtime_error("Null clock pointer encountered in getTimesMilitary");

		times.emplace_back(label + clock->getTimeMilitary());
	}

	return times;
}

std::vector<std::string> OrreryTimepiece::getTimes() {
	std::vector<std::string> times;

	for (const auto& [label, clock] : clocks) {
		if (clock == nullptr)
			throw std::runtime_error("Null clock pointer encountered in getTimes");

		times.emplace_back(label + clock->getTime());
	}

	return times;
}

void OrreryTimepiece::tick() {
	for (std::pair<std::string, CelestialDayClock*>& clock : clocks) {
		if (clock.second == nullptr)
			throw std::runtime_error("Null clock pointer encountered in tick");

		clock.second->tick();
	}
}

void OrreryTimepiece::deleteClocks() {
	for (std::pair<std::string, CelestialDayClock*>& clock : clocks) {
		if (clock.second != nullptr) {
			delete clock.second;
			clock.second = nullptr;
		}
	}
}