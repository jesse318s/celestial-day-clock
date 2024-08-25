#ifndef CELESTIAL_TIMEPIECE_H
#define CELESTIAL_TIMEPIECE_H

#include <vector>
#include <string>

// ADT for a celestial timepiece that provides the current times and ticks to the next times
class CelestialTimepiece {
public:
	virtual ~CelestialTimepiece() {};

	virtual std::vector<std::string> getTimes() = 0;

	virtual void tick() = 0;
};

#endif
