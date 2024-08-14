#ifndef ORRERY_TIMEPIECE_H
#define ORRERY_TIMEPIECE_H

#include "celestialdayclock.h"
#include <vector>
#include <string>
#include <utility>

class OrreryTimepiece {
public:
	~OrreryTimepiece();

	size_t getSize() const { return clocks.size(); }

	void add(const std::string& label, CelestialDayClock* clock) {
		clocks.emplace_back(label, clock);
	}

	void clear();

	std::vector<std::string> getAllTimesMilitary() const;

	std::vector<std::string> getAllTimes() const;

	void tick();

private:
	std::vector<std::pair<std::string, CelestialDayClock*>> clocks;
};

#endif