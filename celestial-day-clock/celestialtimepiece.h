#ifndef CELESTIAL_TIMEPIECE_H
#define CELESTIAL_TIMEPIECE_H

#include <vector>
#include <string>

// Interface for a celestial timepiece that can provide the current times and tick to the next time
class CelestialTimepiece {
public:
	virtual ~CelestialTimepiece() {};

	virtual std::vector<std::string> getTimes() = 0;

	virtual void tick() = 0;
};

#endif
