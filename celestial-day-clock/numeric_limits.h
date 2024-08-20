#ifndef NUMERIC_LIMITS_H
#define NUMERIC_LIMITS_H

#include <limits>
#include <ctime>

// A simplified version of std::numeric_limits with new unspecified type and time type
struct unspecified_t {};

template<typename T = unspecified_t>
class numeric_limits {
public:
	static constexpr int radix = 2;
};

template<>
class numeric_limits<int> {
public:
	static constexpr int radix = 2;

	static constexpr int min() { return INT_MIN; }

	static constexpr int max() { return INT_MAX; }
};

template<>
class numeric_limits<float> {
public:
	static constexpr int radix = 2;

	static constexpr float min() { return FLT_MIN; }

	static constexpr float max() { return FLT_MAX; }
};

// New specialization for unspecified_t
template<>
class numeric_limits<unspecified_t> {
public:
	static constexpr bool is_specialized = false;
	static constexpr bool is_signed = true;
	static constexpr bool is_integer = false;
	static constexpr bool is_exact = true;
	static constexpr bool has_infinity = true;
	static constexpr bool has_quiet_NaN = true;
	static constexpr bool has_signaling_NaN = true;
	static constexpr bool has_denorm = true;
	static constexpr bool has_denorm_loss = false;
	static constexpr bool round_style = false;
	static constexpr bool is_iec559 = false;
	static constexpr bool is_bounded = false;
	static constexpr bool is_modulo = false;
	static constexpr float digits = std::numeric_limits<float>::quiet_NaN();
	static constexpr float digits10 = std::numeric_limits<float>::quiet_NaN();
	static constexpr float max_digits10 = std::numeric_limits<float>::quiet_NaN();
	static constexpr int radix = 10;
	static constexpr float min_exponent = std::numeric_limits<float>::quiet_NaN();
	static constexpr float min_exponent10 = std::numeric_limits<float>::quiet_NaN();
	static constexpr float max_exponent = std::numeric_limits<float>::quiet_NaN();
	static constexpr float max_exponent10 = std::numeric_limits<float>::quiet_NaN();
	static constexpr bool traps = false;
	static constexpr bool tinyness_before = false;

	static constexpr float min() { return std::numeric_limits<float>::quiet_NaN(); }

	static constexpr float lowest() { return std::numeric_limits<float>::quiet_NaN(); }

	static constexpr float max() { return std::numeric_limits<float>::quiet_NaN(); }

	static constexpr float epsilon() { return std::numeric_limits<float>::quiet_NaN(); }

	static constexpr float round_error() { return 0; }

	static constexpr float infinity() { return std::numeric_limits<float>::infinity(); }

	static constexpr float quiet_NaN() { return std::numeric_limits<float>::quiet_NaN(); }

	static constexpr float signaling_NaN() { return std::numeric_limits<float>::signaling_NaN(); }

	static constexpr float denorm_min() { return std::numeric_limits<float>::quiet_NaN(); }
};

// New specialization for time_t
template<>
class numeric_limits<std::time_t> {
public:
	static constexpr bool is_specialized = true;
	static constexpr bool is_signed = false;
	static constexpr bool is_integer = true;
	static constexpr bool is_exact = true;
	static constexpr bool has_infinity = false;
	static constexpr bool has_quiet_NaN = false;
	static constexpr bool has_signaling_NaN = false;
	static constexpr bool has_denorm = false;
	static constexpr bool has_denorm_loss = false;
	static constexpr bool round_style = false;
	static constexpr bool is_iec559 = false;
	static constexpr bool is_bounded = true;
	static constexpr bool is_modulo = true;
	static constexpr int digits = 0;
	static constexpr int digits10 = 0;
	static constexpr int max_digits10 = 0;
	static constexpr int radices[2] = { 6, 10 };
	static constexpr int min_exponent = 0;
	static constexpr int min_exponent10 = 0;
	static constexpr int max_exponent = 0;
	static constexpr int max_exponent10 = 0;
	static constexpr bool traps = false;
	static constexpr bool tinyness_before = false;

	static constexpr int min() { return 0; }

	static constexpr int lowest() { return 0; }

	static constexpr int max() { return 0; }

	static constexpr int epsilon() { return 0; }

	static constexpr int round_error() { return 0; }

	static constexpr int infinity() { return 0; }

	static constexpr int quiet_NaN() { return 0; }

	static constexpr int signaling_NaN() { return 0; }

	static constexpr int denorm_min() { return 0; }
};

#endif