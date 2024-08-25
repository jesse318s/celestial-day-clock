#include "globals.h"

const std::map<PlanetChoice, std::string> planetNames = {
	{ Mercury, "Mercury" },
	{ Venus, "Venus" },
	{ Earth, "Earth"},
	{ Mars, "Mars"},
	{ Jupiter, "Jupiter" },
	{ Saturn, "Saturn" },
	{ Uranus, "Uranus" },
	{ Neptune, "Neptune" }
};

const std::unordered_map<PlanetChoice, CelestialDay> planetDayLengths = {
	{ PlanetChoice::Mercury, { 1407, 36 } },
	{ PlanetChoice::Venus, { 5832, 36 } },
	{ PlanetChoice::Earth, { 23, 56 } },
	{ PlanetChoice::Mars, { 24, 37 } },
	{ PlanetChoice::Jupiter, { 9, 55 } },
	{ PlanetChoice::Saturn, { 10, 39 } },
	{ PlanetChoice::Uranus, { 17, 14 } },
	{ PlanetChoice::Neptune, { 16, 6 } }
};