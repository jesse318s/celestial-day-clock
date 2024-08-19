#ifndef CELESTIAL_TIMEPIECE_H
#define CELESTIAL_TIMEPIECE_H

#include <vector>
#include <string>

class CelestialTimepiece {
public:
	virtual ~CelestialTimepiece() {};

	virtual std::vector<std::string> getTimes() = 0;

	virtual void tick() = 0;
};

#endif
