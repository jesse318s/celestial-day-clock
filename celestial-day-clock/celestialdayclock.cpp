#include "celestialdayclock.h"
#include "numeric_limits.h"
#include <string>

void CelestialDayClock::setHours(int h) {
	if (h > maxHours) h = maxHours;

	if (h < 0) h = 0;

	hours = h;
}
int CelestialDayClock::getHours() const { return hours; }

void CelestialDayClock::setMinutesDigit1(int m) {
	if (m > radix - 1) m = radix - 1;

	if (m < 0) m = 0;

	minutesDigit1 = m;
}
int CelestialDayClock::getMinutesDigit1() const { return minutesDigit1; }

void CelestialDayClock::setMinutesDigit2(int m) {
	if (m > secondaryRadix - 1) m = secondaryRadix - 1;

	if (m < 0) m = 0;

	minutesDigit2 = m;
}
int CelestialDayClock::getMinutesDigit2() const { return minutesDigit2; }

void CelestialDayClock::setSecondsDigit1(int s) {
	if (s > radix - 1) s = radix - 1;

	if (s < 0) s = 0;

	secondsDigit1 = s;
}
int CelestialDayClock::getSecondsDigit1() const { return secondsDigit1; }

void CelestialDayClock::setSecondsDigit2(int s) {
	if (s > secondaryRadix - 1) s = secondaryRadix - 1;

	if (s < 0) s = 0;

	secondsDigit2 = s;
}
int CelestialDayClock::getSecondsDigit2() const { return secondsDigit2; }

void CelestialDayClock::setBodyMaximums(int h, int m) {
	constexpr int halfMaxBodyMinutes = ((radix - 1) / 2 * secondaryRadix + (secondaryRadix - 1)) - 1;

	if (h < maxHoursMin) h = maxHoursMin;

	maxHours = h;

	m = m / 2;

	if (m < 0) m = 0;

	if (m % 2 == 1) --m;

	if (m > halfMaxBodyMinutes) m = halfMaxBodyMinutes;

	maxMinutes = m;

	if (maxHours % 2 == 1) {
		--maxHours;
		maxMinutes += radix * secondaryRadix / 2;
	}

	if (maxMinutes > 0) ++maxHours;
}
std::string CelestialDayClock::getBodyMaximums() const {
	const bool trulyOddMaxHours = maxHours % 2 == 1 && maxMinutes >= radix * secondaryRadix / 2;
	int bodyMaxHours = 0;
	int bodyMaxMinutes = 0;

	if (trulyOddMaxHours || maxMinutes == 0) bodyMaxHours = maxHours;
	else bodyMaxHours = maxHours - 1;

	if (bodyMaxHours % 2 == 1) bodyMaxMinutes = (maxMinutes - radix * secondaryRadix / 2) * 2;
	else bodyMaxMinutes = maxMinutes * 2;

	return std::to_string(bodyMaxHours) + delimiter
		+ (bodyMaxMinutes < secondaryRadix ? "0" : "") + std::to_string(bodyMaxMinutes) + delimiter
		+ "00";
}

std::string CelestialDayClock::getTimeMilitary() const {
	return std::to_string(hours) + delimiter +
		std::to_string(minutesDigit1) + std::to_string(minutesDigit2) + delimiter +
		std::to_string(secondsDigit1) + std::to_string(secondsDigit2);
}

int CelestialDayClock::getStandardHours() const {
	if (maxMinutes == 0 && hours == 0) return maxHours / 2;

	if (maxMinutes == 0 && hours == maxHours) return 0;

	if (maxMinutes == 0 && hours > maxHours / 2) return hours - maxHours / 2;

	if (maxMinutes != 0 && hours > maxHours / 2) return hours - maxHours / 2 - 1;

	return hours;
}

std::string CelestialDayClock::getMeridiemIndicator() const {
	if (maxMinutes == 0 && hours >= maxHours / 2 && hours != maxHours) return
	{ ' ', postChar, meridiemChar };

	if (maxMinutes != 0 && hours > maxHours / 2) return { ' ', postChar, meridiemChar };

	return { ' ', anteChar, meridiemChar };
}

std::string CelestialDayClock::getTime() const {
	return std::to_string(getStandardHours()) + delimiter +
		std::to_string(minutesDigit1) + std::to_string(minutesDigit2) + delimiter +
		std::to_string(secondsDigit1) + std::to_string(secondsDigit2) + getMeridiemIndicator();
}

bool CelestialDayClock::checkTimeReset() {
	const bool areHoursMax = hours >= maxHours - 1 &&
		minutesDigit1 == radix - 1 && minutesDigit2 == secondaryRadix - 1;
	const bool areMinutesMax = (hours == maxHours / 2 || hours >= maxHours) &&
		minutesDigit1 * secondaryRadix + minutesDigit2 >= maxMinutes - 1;
	const bool areSecondsMax = secondsDigit1 == radix - 1 && secondsDigit2 == secondaryRadix - 1;
	bool isReset = false;

	if (maxMinutes == 0 && areHoursMax && areSecondsMax) {
		hours = 0;
		minutesDigit1 = 0;
		minutesDigit2 = 0;
		secondsDigit1 = 0;
		secondsDigit2 = 0;

		isReset = true;
	}

	if (maxMinutes != 0 && areMinutesMax && areSecondsMax) {
		hours = hours == maxHours / 2 ? maxHours / 2 + 1 : 0;
		minutesDigit1 = 0;
		minutesDigit2 = 0;
		secondsDigit1 = 0;
		secondsDigit2 = 0;

		isReset = true;
	}

	return isReset;
}

void CelestialDayClock::tick() {
	if (checkTimeReset()) return;

	++secondsDigit2;

	if (secondsDigit2 >= secondaryRadix) {
		secondsDigit2 = 0;
		++secondsDigit1;

		if (secondsDigit1 >= radix) {
			secondsDigit1 = 0;
			tickMinutes();
		}
	}
}

void CelestialDayClock::tickMinutes() {
	++minutesDigit2;

	if (minutesDigit2 >= secondaryRadix) {
		minutesDigit2 = 0;
		++minutesDigit1;

		if (minutesDigit1 >= radix) {
			minutesDigit1 = 0;
			++hours;
		}
	}
}