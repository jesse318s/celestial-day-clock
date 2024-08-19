#include "celestialdayclock.h"

void CelestialDayClock::setHours(int h) { hours = clamp(h, maxHours); }

void CelestialDayClock::setMinutesDigit1(int m) { minutesDigit1 = clamp(m, radixMax); }

void CelestialDayClock::setMinutesDigit2(int m) { minutesDigit2 = clamp(m, secondaryRadixMax); }

void CelestialDayClock::setSecondsDigit1(int s) { secondsDigit1 = clamp(s, radixMax); }

void CelestialDayClock::setSecondsDigit2(int s) { secondsDigit2 = clamp(s, secondaryRadixMax); }

void CelestialDayClock::setBodyMaximums(int h, int m) {
	constexpr int halfMaxBodyMinutes = (radixMax / 2 * secondaryRadix + secondaryRadixMax) - 1;

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
	if (maxMinutes == 0 && hours >= maxHours / 2 && hours != maxHours)
		return { ' ', postChar, meridiemChar };

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
		minutesDigit1 == radixMax && minutesDigit2 == secondaryRadixMax;
	const bool areMinutesMax = (hours == maxHours / 2 || hours >= maxHours) &&
		minutesDigit1 * secondaryRadix + minutesDigit2 >= maxMinutes - 1;
	const bool areSecondsMax = secondsDigit1 == radixMax && secondsDigit2 == secondaryRadixMax;
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

int CelestialDayClock::clamp(const int value, const int max) const {
	if (value > max) return max;

	if (value < 0) return 0;

	return value;
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