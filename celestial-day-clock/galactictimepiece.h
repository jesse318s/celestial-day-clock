#ifndef GALACTIC_TIMEPIECE_H
#define GALACTIC_TIMEPIECE_H

#include "orrerytimepiece.h"
#include <vector>
#include <string>
#include <utility>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

class GalacticTimepiece {
public:
	GalacticTimepiece() : running(false) {}

	~GalacticTimepiece();

	size_t getSize() const { return timepieces.size(); }

	void add(const std::string& label, OrreryTimepiece* timepiece);

	void clear();

	std::vector<std::string> getTimesMilitary() const;

	std::vector<std::string> getTimes() const;

	void tick();

	void startTicking();

	void stopTicking();

private:
	std::vector<std::pair<std::string, OrreryTimepiece*>> timepieces;
	std::thread thread;
	std::mutex mtx;
	bool running;
};

#endif
