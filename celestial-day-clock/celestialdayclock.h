#ifndef CELESTIAL_DAY_CLOCK_H
#define CELESTIAL_DAY_CLOCK_H

#include "celestialtimepiece.h"
#include "numeric_limits.h"
#include <ctime>
#include <string>
#include <vector>

/* A generic clock that keeps track of a celestial body's time of day using the new time type from
   the custom numeric limits template */
class CelestialDayClock : public CelestialTimepiece {
public:
	static const char delimiter = ':';
	static const char anteChar = 'A';
	static const char postChar = 'P';
	static const char meridiemChar = 'M';
	static constexpr int maxHoursMin = 2;
	static constexpr int radix = numeric_limits<std::time_t>::radices[0];
	static constexpr int radixMax = radix - 1;
	static constexpr int secondaryRadix = numeric_limits<std::time_t>::radices[1];
	static constexpr int secondaryRadixMax = secondaryRadix - 1;

	CelestialDayClock(int h, int m);

	void setHours(int h);
	int getHours() const { return hours; }

	void setMinutesDigit1(int m);
	int getMinutesDigit1() const { return minutesDigit1; }

	void setMinutesDigit2(int m);
	int getMinutesDigit2() const { return minutesDigit2; }

	void setSecondsDigit1(int s);
	int getSecondsDigit1() const { return secondsDigit1; }

	void setSecondsDigit2(int s);
	int getSecondsDigit2() const { return secondsDigit2; }

	void setBodyMaximums(int h, int m);
	std::vector<int> getBodyMaximums() const;

	std::string getTimeMilitary() const;

	int getStandardHours() const;

	std::string getMeridiemIndicator() const;

	std::string getTime() const;

	std::vector<std::string> getTimes() override;

	bool checkTimeReset();

	void tick() override;

private:
	int maxHours = 0;
	int maxMinutes = 0;
	int hours = 0;
	int minutesDigit1 = 0;
	int minutesDigit2 = 0;
	int secondsDigit1 = 0;
	int secondsDigit2 = 0;

	int clamp(const int value, const int max) const;

	void tickMinutes();
};

#endif