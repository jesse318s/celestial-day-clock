#ifndef GALACTIC_TIMEPIECE_H
#define GALACTIC_TIMEPIECE_H

#include "celestialtimepiece.h"
#include "orrerytimepiece.h"
#include <vector>
#include <string>
#include <utility>
#include <future>
#include <mutex>
#include <chrono>
#include <iostream>
#include <thread>

// A collection of OrreryTimepieces that keeps track of a galaxy or galaxy group's time
class GalacticTimepiece : public CelestialTimepiece {
public:
	GalacticTimepiece() : running(false) {}

	~GalacticTimepiece();

	size_t getSize() const;

	void add(const std::string& label, OrreryTimepiece* timepiece);

	void clear();

	std::vector<std::string> getTimesMilitary();

	std::vector<std::string> getTimes();

	void tick();

	void startTicking();

	void stopTicking();

private:
	std::vector<std::pair<std::string, OrreryTimepiece*>> timepieces;
	std::future<void> tickingFuture;
	std::mutex mtx;
	bool running;
};

#endif
