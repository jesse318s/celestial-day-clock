#include "numeric_limits.h"
#include "celestialdayclock.h"
#include "orrerytimepiece.h"
#include "galactictimepiece.h"
#include <iostream>
#include <cassert>
#include <string>
#include <chrono>
#include <utility>
#include <map>
#include <unordered_map>
#include <memory>
#include <thread>
#include <cstdlib>

namespace cdc_test {
	struct ClockUnitValues {
		int hours;
		int minutesDigit1;
		int minutesDigit2;
		int secondsDigit1;
		int secondsDigit2;
	};

	const std::string passed = "Test passed.";
	const std::string nanString = "nan";
	const std::string amIndicator = " AM";
	const std::string pmIndicator = " PM";
	const char delimiter = ':';
	const std::string zeroTimeString = std::string(1, '0') + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"00";
	const std::string oneSecondTimeString = std::string(1, '0') + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"01";
	constexpr int senaryRadix = 6;
	constexpr int senaryRadixMax = senaryRadix - 1;
	constexpr int decimalRadix = 10;
	constexpr int decimalRadixMax = decimalRadix - 1;
	constexpr int hours = 24;
	constexpr int minutes = 58;
	constexpr int expectedMinutes = senaryRadixMax * decimalRadix + senaryRadix;
	const ClockUnitValues time1 = { hours / 2 - 1,
	 senaryRadixMax, decimalRadixMax,
	 senaryRadixMax, decimalRadixMax };
	const ClockUnitValues time2 = { hours - 1,
	 senaryRadixMax, decimalRadixMax,
	 senaryRadixMax, decimalRadixMax };
	const ClockUnitValues time3 = { hours / 2,
	 senaryRadixMax / 2, senaryRadix + 1,
	 senaryRadixMax, decimalRadixMax };
	const ClockUnitValues time4 = { hours,
	 senaryRadixMax, decimalRadixMax,
	 senaryRadixMax, decimalRadixMax };
	const ClockUnitValues time5 = { hours + 1,
	 senaryRadixMax / 2, senaryRadix + 1,
	 senaryRadixMax, decimalRadixMax };
}

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

static const std::map<PlanetChoice, std::string> planetNames = {
	{ Mercury, "Mercury" },
	{ Venus, "Venus" },
	{ Earth, "Earth"},
	{ Mars, "Mars"},
	{ Jupiter, "Jupiter" },
	{ Saturn, "Saturn" },
	{ Uranus, "Uranus" },
	{ Neptune, "Neptune" }
};

struct CelestialDay { const int hours; const int minutes; };

static const std::unordered_map<PlanetChoice, CelestialDay> planetDayLengths = {
	{ PlanetChoice::Mercury, { 1407, 36 } },
	{ PlanetChoice::Venus, { 5832, 36 } },
	{ PlanetChoice::Earth, { 23, 56 } },
	{ PlanetChoice::Mars, { 24, 37 } },
	{ PlanetChoice::Jupiter, { 9, 55 } },
	{ PlanetChoice::Saturn, { 10, 39 } },
	{ PlanetChoice::Uranus, { 17, 14 } },
	{ PlanetChoice::Neptune, { 16, 6 } }
};

static void testSimplifiedNumericLimits();

static void testNewNumericLimits();

static void testCDCMilitaryTime();

static void testCDCStandardTime();

static void testOrreryTimepiece();

static void testGalacticTimepiece();

static void displayCDCMenu();

int main() {
	std::srand(static_cast<unsigned int>(std::time(0)));
	// Testing the new numeric_limits template and the dependent CelestialDayClock class
	testSimplifiedNumericLimits();
	testNewNumericLimits();
	testCDCMilitaryTime();
	testCDCStandardTime();
	testOrreryTimepiece();
	testGalacticTimepiece();
	// Demonstrating the use of the CelestialDayClock class
	displayCDCMenu();

	return 0;
}

static void testSimplifiedNumericLimits() {
	std::cout << "\nTesting simplified numeric limits..." << std::endl;
	assert(numeric_limits<int>::radix == std::numeric_limits<int>::radix);
	assert(numeric_limits<float>::radix == std::numeric_limits<float>::radix);
	assert(numeric_limits<int>::min() == std::numeric_limits<int>::min());
	assert(numeric_limits<float>::max() == std::numeric_limits<float>::max());
	std::cout << cdc_test::passed << std::endl;
}

static void testNewNumericLimits() {
	std::cout << "\nTesting numeric limits for unspecified type..." << std::endl;
	assert(numeric_limits<>::radix == cdc_test::decimalRadix);
	assert(std::to_string(numeric_limits<>::min()) == cdc_test::nanString);
	std::cout << cdc_test::passed << std::endl;
	std::cout << "\nTesting numeric limits for time type..." << std::endl;
	assert(numeric_limits<std::time_t>::radices[0] == cdc_test::senaryRadix);
	assert(numeric_limits<std::time_t>::min() == 0);
	std::cout << cdc_test::passed << std::endl;
}

static void setCDCTime(CelestialDayClock* clock, const cdc_test::ClockUnitValues& time) {
	clock->setHours(time.hours);
	clock->setMinutesDigit1(time.minutesDigit1);
	clock->setMinutesDigit2(time.minutesDigit2);
	clock->setSecondsDigit1(time.secondsDigit1);
	clock->setSecondsDigit2(time.secondsDigit2);
}

static void testMilitaryTime1(CelestialDayClock* clock) {
	std::cout << "\nTesting max hours for first half of day..." << std::endl;
	assert(clock->getBodyMaximums() ==
		std::to_string(cdc_test::hours) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"00");
	setCDCTime(clock, cdc_test::time1);
	assert(clock->getTimeMilitary() ==
		std::to_string(cdc_test::time1.hours) + cdc_test::delimiter +
		std::to_string(cdc_test::time1.minutesDigit1 * cdc_test::decimalRadix +
			cdc_test::time1.minutesDigit2) + cdc_test::delimiter +
		std::to_string(cdc_test::time1.secondsDigit1 * cdc_test::decimalRadix +
			cdc_test::time1.secondsDigit2));
	clock->tick();
	assert(clock->getTimeMilitary() ==
		std::to_string(cdc_test::time1.hours + 1) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"00");
	clock->tick();
	assert(clock->getTimeMilitary() ==
		std::to_string(cdc_test::time1.hours + 1) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"01");
	std::cout << cdc_test::passed << std::endl;
}

static void testMilitaryTime2(CelestialDayClock* clock) {
	std::cout << "\nTesting max hours for end of day..." << std::endl;
	setCDCTime(clock, cdc_test::time2);
	assert(clock->getTimeMilitary() ==
		std::to_string(cdc_test::time2.hours) + cdc_test::delimiter +
		std::to_string(cdc_test::time2.minutesDigit1 * cdc_test::decimalRadix +
			cdc_test::time2.minutesDigit2) + cdc_test::delimiter +
		std::to_string(cdc_test::time2.secondsDigit1 * cdc_test::decimalRadix +
			cdc_test::time2.secondsDigit2));
	clock->tick();
	assert(clock->getTimeMilitary() == cdc_test::zeroTimeString);
	clock->tick();
	assert(clock->getTimeMilitary() == cdc_test::oneSecondTimeString);
	std::cout << cdc_test::passed << std::endl;
}

static void testMilitaryTime3(CelestialDayClock* clock) {
	std::cout << "\nTesting max hours and max minutes for first half of day..." << std::endl;
	clock->setBodyMaximums(cdc_test::hours, cdc_test::minutes);
	assert(clock->getBodyMaximums() ==
		std::to_string(cdc_test::hours) + cdc_test::delimiter +
		std::to_string(cdc_test::expectedMinutes) + cdc_test::delimiter +
		"00");
	setCDCTime(clock, cdc_test::time3);
	assert(clock->getTimeMilitary() ==
		std::to_string(cdc_test::time3.hours) + cdc_test::delimiter +
		std::to_string(cdc_test::time3.minutesDigit1 * cdc_test::decimalRadix +
			cdc_test::time3.minutesDigit2) + cdc_test::delimiter +
		std::to_string(cdc_test::time3.secondsDigit1 * cdc_test::decimalRadix +
			cdc_test::time3.secondsDigit2));
	clock->tick();
	assert(clock->getTimeMilitary() ==
		std::to_string(cdc_test::time3.hours + 1) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"00");
	clock->tick();
	assert(clock->getTimeMilitary() ==
		std::to_string(cdc_test::time3.hours + 1) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"01");
	std::cout << cdc_test::passed << std::endl;
}

static void testMilitaryTime4(CelestialDayClock* clock) {
	std::cout << "\nTesting max hours and max minutes for last half of day..." << std::endl;
	setCDCTime(clock, cdc_test::time4);
	assert(clock->getTimeMilitary() ==
		std::to_string(cdc_test::time4.hours) + cdc_test::delimiter +
		std::to_string(cdc_test::time4.minutesDigit1 * cdc_test::decimalRadix +
			cdc_test::time4.minutesDigit2) + cdc_test::delimiter +
		std::to_string(cdc_test::time4.secondsDigit1 * cdc_test::decimalRadix +
			cdc_test::time4.secondsDigit2));
	clock->tick();
	assert(clock->getTimeMilitary() ==
		std::to_string(cdc_test::time4.hours + 1) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"00");
	clock->tick();
	assert(clock->getTimeMilitary() ==
		std::to_string(cdc_test::time4.hours + 1) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"01");
	std::cout << cdc_test::passed << std::endl;
}

static void testMilitaryTime5(CelestialDayClock* clock) {
	std::cout << "\nTesting max hours and max minutes for end of day..." << std::endl;
	setCDCTime(clock, cdc_test::time5);
	assert(clock->getTimeMilitary() ==
		std::to_string(cdc_test::time5.hours) + cdc_test::delimiter +
		std::to_string(cdc_test::time5.minutesDigit1 * cdc_test::decimalRadix +
			cdc_test::time5.minutesDigit2) + cdc_test::delimiter +
		std::to_string(cdc_test::time5.secondsDigit1 * cdc_test::decimalRadix +
			cdc_test::time5.secondsDigit2));
	clock->tick();
	assert(clock->getTimeMilitary() == cdc_test::zeroTimeString);
	clock->tick();
	assert(clock->getTimeMilitary() == cdc_test::oneSecondTimeString);
	std::cout << cdc_test::passed << std::endl;
}

static void testCDCMilitaryTime() {
	CelestialDayClock* clock = new CelestialDayClock(cdc_test::hours, 0);

	std::cout << "\n\nBegin celestial day clock military time test." << std::endl;
	testMilitaryTime1(clock);
	testMilitaryTime2(clock);
	testMilitaryTime3(clock);
	testMilitaryTime4(clock);
	testMilitaryTime5(clock);
	delete clock;
	std::cout << "\nEnd celestial day clock military time test." << std::endl;
}

static void testStandardTime1(CelestialDayClock* clock) {
	std::cout << "\nTesting max hours for first half of day..." << std::endl;
	assert(clock->getBodyMaximums() ==
		std::to_string(cdc_test::hours) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"00");
	setCDCTime(clock, cdc_test::time1);
	assert(clock->getTime() ==
		std::to_string(cdc_test::time1.hours) + cdc_test::delimiter +
		std::to_string(cdc_test::time1.minutesDigit1 * cdc_test::decimalRadix +
			cdc_test::time1.minutesDigit2) + cdc_test::delimiter +
		std::to_string(cdc_test::time1.secondsDigit1 * cdc_test::decimalRadix +
			cdc_test::time1.secondsDigit2) + cdc_test::amIndicator);
	clock->tick();
	assert(clock->getTime() ==
		std::to_string(cdc_test::time1.hours + 1) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"00" + cdc_test::pmIndicator);
	clock->tick();
	assert(clock->getTime() ==
		std::to_string(cdc_test::time1.hours + 1) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"01" + cdc_test::pmIndicator);
	std::cout << cdc_test::passed << std::endl;
}

static void testStandardTime2(CelestialDayClock* clock) {
	std::cout << "\nTesting max hours for end of day..." << std::endl;
	setCDCTime(clock, cdc_test::time2);
	assert(clock->getTime() ==
		std::to_string(cdc_test::time2.hours / 2) + cdc_test::delimiter +
		std::to_string(cdc_test::time2.minutesDigit1 * cdc_test::decimalRadix +
			cdc_test::time2.minutesDigit2) + cdc_test::delimiter +
		std::to_string(cdc_test::time2.secondsDigit1 * cdc_test::decimalRadix +
			cdc_test::time2.secondsDigit2) + cdc_test::pmIndicator);
	clock->tick();
	assert(clock->getTime() ==
		std::to_string(cdc_test::time2.hours / 2 + 1) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"00" + cdc_test::amIndicator);
	clock->tick();
	assert(clock->getTime() ==
		std::to_string(cdc_test::time2.hours / 2 + 1) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"01" + cdc_test::amIndicator);
	std::cout << cdc_test::passed << std::endl;
}

static void testStandardTime3(CelestialDayClock* clock) {
	std::cout << "\nTesting max hours and max minutes for first half of day..." << std::endl;
	clock->setBodyMaximums(cdc_test::hours, cdc_test::minutes);
	assert(clock->getBodyMaximums() ==
		std::to_string(cdc_test::hours) + cdc_test::delimiter +
		std::to_string(cdc_test::expectedMinutes) + cdc_test::delimiter +
		"00");
	setCDCTime(clock, cdc_test::time3);
	assert(clock->getTime() ==
		std::to_string(cdc_test::time3.hours) + cdc_test::delimiter +
		std::to_string(cdc_test::time3.minutesDigit1 * cdc_test::decimalRadix +
			cdc_test::time3.minutesDigit2) + cdc_test::delimiter +
		std::to_string(cdc_test::time3.secondsDigit1 * cdc_test::decimalRadix +
			cdc_test::time3.secondsDigit2) + cdc_test::amIndicator);
	clock->tick();
	assert(clock->getTime() == cdc_test::zeroTimeString + cdc_test::pmIndicator);
	clock->tick();
	assert(clock->getTime() == cdc_test::oneSecondTimeString + cdc_test::pmIndicator);
	std::cout << cdc_test::passed << std::endl;
}

static void testStandardTime4(CelestialDayClock* clock) {
	std::cout << "\nTesting max hours and max minutes for last half of day..." << std::endl;
	setCDCTime(clock, cdc_test::time4);
	assert(clock->getTime() ==
		std::to_string(cdc_test::time4.hours / 2 - 1) + cdc_test::delimiter +
		std::to_string(cdc_test::time4.minutesDigit1 * cdc_test::decimalRadix +
			cdc_test::time4.minutesDigit2) + cdc_test::delimiter +
		std::to_string(cdc_test::time4.secondsDigit1 * cdc_test::decimalRadix +
			cdc_test::time4.secondsDigit2) + cdc_test::pmIndicator);
	clock->tick();
	assert(clock->getTime() ==
		std::to_string(cdc_test::time4.hours / 2) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"00" + cdc_test::pmIndicator);
	clock->tick();
	assert(clock->getTime() ==
		std::to_string(cdc_test::time4.hours / 2) + cdc_test::delimiter +
		"00" + cdc_test::delimiter +
		"01" + cdc_test::pmIndicator);
	std::cout << cdc_test::passed << std::endl;
}

static void testStandardTime5(CelestialDayClock* clock) {
	std::cout << "\nTesting max hours and max minutes for end of day..." << std::endl;
	setCDCTime(clock, cdc_test::time5);
	assert(clock->getTime() ==
		std::to_string(cdc_test::time5.hours / 2) + cdc_test::delimiter +
		std::to_string(cdc_test::time5.minutesDigit1 * cdc_test::decimalRadix +
			cdc_test::time5.minutesDigit2) + cdc_test::delimiter +
		std::to_string(cdc_test::time5.secondsDigit1 * cdc_test::decimalRadix +
			cdc_test::time5.secondsDigit2) + cdc_test::pmIndicator);
	clock->tick();
	assert(clock->getTime() == cdc_test::zeroTimeString + cdc_test::amIndicator);
	clock->tick();
	assert(clock->getTime() == cdc_test::oneSecondTimeString + cdc_test::amIndicator);
	std::cout << cdc_test::passed << std::endl;
}

static void testCDCStandardTime() {
	CelestialDayClock* clock = new CelestialDayClock(cdc_test::hours, 0);

	std::cout << "\n\nBegin celestial day clock standard time test." << std::endl;
	testStandardTime1(clock);
	testStandardTime2(clock);
	testStandardTime3(clock);
	testStandardTime4(clock);
	testStandardTime5(clock);
	delete clock;
	std::cout << "\nEnd celestial day clock standard time test." << std::endl;
}

static void testOrreryTimepiece() {
	OrreryTimepiece* timepiece = new OrreryTimepiece();
	int cdcCount = 0;

	std::cout << "\n\nTesting orrery timepiece..." << std::endl;
	timepiece->add("0. ", new CelestialDayClock(cdc_test::hours, 0));
	++cdcCount;
	timepiece->add("1. ", new CelestialDayClock(cdc_test::hours, cdc_test::minutes));
	++cdcCount;
	assert(timepiece->getSize() == cdcCount);
	assert(timepiece->getTimesMilitary()[0] == "0. " + cdc_test::zeroTimeString);
	assert(timepiece->getClock("0. ").getTimeMilitary() ==
		cdc_test::zeroTimeString);
	assert(timepiece->getTimesMilitary()[1] == "1. " + cdc_test::zeroTimeString);
	timepiece->tick();
	assert(timepiece->getTimesMilitary()[0] == "0. " + cdc_test::oneSecondTimeString);
	assert(timepiece->getClock("0. ").getTimeMilitary() ==
		cdc_test::oneSecondTimeString);
	assert(timepiece->getTimesMilitary()[1] == "1. " + cdc_test::oneSecondTimeString);
	delete timepiece;
	std::cout << cdc_test::passed << std::endl;
}

static void testGalacticTimepiece() {
	GalacticTimepiece* timepiece = new GalacticTimepiece();
	OrreryTimepiece* orreryTimepiece0 = new OrreryTimepiece();
	OrreryTimepiece* orreryTimepiece1 = new OrreryTimepiece();
	int cdcCount = 0;

	std::cout << "\n\nTesting galactic timepiece..." << std::endl;
	orreryTimepiece0->add("0. ", new CelestialDayClock(cdc_test::hours, 0));
	++cdcCount;
	orreryTimepiece0->add("1. ", new CelestialDayClock(cdc_test::hours, cdc_test::minutes));
	++cdcCount;
	orreryTimepiece1->add("0. ", new CelestialDayClock(cdc_test::hours, 0));
	++cdcCount;
	orreryTimepiece1->add("1. ", new CelestialDayClock(cdc_test::hours, cdc_test::minutes));
	++cdcCount;
	timepiece->add("0. ", orreryTimepiece0);
	timepiece->add("1. ", orreryTimepiece1);
	assert(timepiece->getSize() == cdcCount);
	assert(timepiece->getTimepiece("0. ").getClock("0. ").getTimeMilitary() ==
		cdc_test::zeroTimeString);
	assert(timepiece->getTimesMilitary()[0] == "0. 0. " + cdc_test::zeroTimeString);
	assert(timepiece->getTimesMilitary().back() == "1. 1. " + cdc_test::zeroTimeString);
	timepiece->tick();
	assert(timepiece->getTimepiece("0. ").getClock("0. ").getTimeMilitary() ==
		cdc_test::oneSecondTimeString);
	assert(timepiece->getTimesMilitary()[0] == "0. 0. " + cdc_test::oneSecondTimeString);
	assert(timepiece->getTimesMilitary().back() == "1. 1. " + cdc_test::oneSecondTimeString);
	timepiece->startTicking();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	assert(timepiece->getTimesMilitary()[0] != "0. 0. " + cdc_test::oneSecondTimeString);
	assert(timepiece->getTimesMilitary().back() != "1. 1. " + cdc_test::oneSecondTimeString);
	delete timepiece;
	std::cout << cdc_test::passed << std::endl;
}

static void displayCelestialTimepiece(CelestialTimepiece* timepiecePtr) {
	const std::unique_ptr<CelestialTimepiece> timepiece(timepiecePtr);
	std::chrono::time_point<std::chrono::steady_clock> nextTick =
		std::chrono::steady_clock::now() + std::chrono::seconds(1);

	std::cout << std::endl;

	try {
		for (const std::string& time : timepiece->getTimes()) {
			std::cout << time << std::endl;
		}

		timepiece->tick();

		while (true) {
			std::this_thread::sleep_until(nextTick);
			nextTick += std::chrono::seconds(1);

			for (const std::string& time : timepiece->getTimes()) {
				std::cout << time << std::endl;
			}

			timepiece->tick();
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception in displayCelestialTimepiece: " << e.what() << std::endl;
	}
}

static void displayPlanetaryCDCMenu() {
	const std::string range =
		std::to_string(PlanetChoice::Mercury) + " and " + std::to_string(PlanetChoice::MaxChoice);
	int choice = 0;
	bool validChoice = false;

	while (!validChoice) {
		validChoice = true;
		std::cout << "\n\nChoose a planet to display its standard clock: " << std::endl;

		for (const auto& [planetChoice, planetName] : planetNames) {
			std::cout << planetChoice << ". " << planetName << std::endl;
		}

		std::cin >> choice;

		if (std::cin.fail() ||
			choice < PlanetChoice::Mercury || choice > PlanetChoice::MaxChoice) {
			validChoice = false;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid planet choice number. Please enter a choice number between "
				+ range << std::endl;
		}
	}

	const std::unordered_map<PlanetChoice, CelestialDay>::const_iterator itr =
		planetDayLengths.find(static_cast<PlanetChoice>(choice));

	if (itr != planetDayLengths.end())
		displayCelestialTimepiece(new CelestialDayClock(itr->second.hours, itr->second.minutes));
}

static OrreryTimepiece* createOrreryTimepiece() {
	OrreryTimepiece* timepiece = new OrreryTimepiece();
	CelestialDayClock* clock = nullptr;

	for (const auto& [planetChoice, celestialDay] : planetDayLengths) {
		clock = new CelestialDayClock(celestialDay.hours, celestialDay.minutes);
		clock->setHours(std::rand() % 2);
		clock->setMinutesDigit1(std::rand() % cdc_test::senaryRadix);
		clock->setMinutesDigit2(std::rand() % cdc_test::decimalRadix);
		clock->setSecondsDigit1(std::rand() % cdc_test::senaryRadix);
		clock->setSecondsDigit2(std::rand() % cdc_test::decimalRadix);
		timepiece->add(planetNames.at(planetChoice) + cdc_test::delimiter + ' ', clock);
	}

	return timepiece;
}

static GalacticTimepiece* createGalacticTimepiece() {
	GalacticTimepiece* timepiece = new GalacticTimepiece();
	OrreryTimepiece* orreryTimepiece = createOrreryTimepiece();

	timepiece->add("Star System A - ", orreryTimepiece);
	orreryTimepiece = createOrreryTimepiece();
	timepiece->add("Star System B - ", orreryTimepiece);

	return timepiece;
}

static void displayCDCMenu() {
	constexpr int planetChoice = 1;
	constexpr int orreryChoice = planetChoice + 1;
	constexpr int galacticChoice = orreryChoice + 1;
	const std::string range =
		std::to_string(planetChoice) + " and " + std::to_string(galacticChoice);
	int choice = 0;
	bool validChoice = false;

	while (!validChoice) {
		validChoice = true;
		std::cout << "\n\nChoose to display a planet clock or an orrery timepiece: " << std::endl;
		std::cout << std::to_string(planetChoice) + ". Display a planet clock" << std::endl;
		std::cout << std::to_string(orreryChoice) + ". Display an orrery timepiece" << std::endl;
		std::cout << std::to_string(galacticChoice) + ". Display a galactic timepiece" << std::endl;
		std::cin >> choice;

		if (std::cin.fail() || choice < planetChoice || choice > galacticChoice) {
			validChoice = false;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid menu choice number. Please enter a choice number between " +
				range << std::endl;
		}
	}

	if (choice == 1) displayPlanetaryCDCMenu();

	if (choice == 2) displayCelestialTimepiece(createOrreryTimepiece());

	if (choice == 3) displayCelestialTimepiece(createGalacticTimepiece());
}