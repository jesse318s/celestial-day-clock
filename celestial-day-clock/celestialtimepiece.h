#ifndef CELESTIAL_TIMEPIECE_H
#define CELESTIAL_TIMEPIECE_H

class CelestialTimepiece {
public:
	virtual ~CelestialTimepiece() {};

	virtual void tick() = 0;
};

#endif
