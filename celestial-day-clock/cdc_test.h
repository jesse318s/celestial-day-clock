#ifndef CDC_TEST_H
#define CDC_TEST_H

#include <string>

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
	const std::string zeroTimeUnitString = "00";
	const std::string oneTimeUnitString = "01";
	const std::string zeroTimeString = std::string(1, '0') + cdc_test::delimiter +
		zeroTimeUnitString + cdc_test::delimiter +
		zeroTimeUnitString;
	const std::string oneSecondTimeString = std::string(1, '0') + cdc_test::delimiter +
		zeroTimeUnitString + cdc_test::delimiter +
		oneTimeUnitString;
	constexpr int senaryRadix = 6;
	constexpr int senaryRadixMax = senaryRadix - 1;
	constexpr int decimalRadix = 10;
	constexpr int decimalRadixMax = decimalRadix - 1;
	constexpr int hours = 4;
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

#endif