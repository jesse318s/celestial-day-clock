#ifndef ORRERY_TIMEPIECE_H
#define ORRERY_TIMEPIECE_H

#include "celestialtimepiece.h"
#include "celestialdayclock.h"
#include <vector>
#include <string>
#include <utility>

// A collection of CelestialDayClocks that keeps track of a star system's time
class OrreryTimepiece : public CelestialTimepiece {
public:
	~OrreryTimepiece();

	size_t getSize() const { return clocks.size(); }

	void add(const std::string& label, CelestialDayClock* clock) {
		clocks.emplace_back(label, clock);
	}

	void clear();

	std::vector<std::string> getTimesMilitary() const;

	std::vector<std::string> getTimes() const;

	void tick();

private:
	std::vector<std::pair<std::string, CelestialDayClock*>> clocks;

	void deleteClocks();
};

#endif
