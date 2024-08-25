#ifndef GLOBALS_H
#define GLOBALS_H

#include <map>
#include <string>
#include <unordered_map>

enum PlanetChoice {
	Mercury = 1,
	Venus,
	Earth,
	Mars,
	Jupiter,
	Saturn,
	Uranus,
	Neptune,
	MaxChoice = Neptune
};

extern const std::map<PlanetChoice, std::string> planetNames;

struct CelestialDay { const int hours; const int minutes; };

extern const std::unordered_map<PlanetChoice, CelestialDay> planetDayLengths;

#endif