#include "galactictimepiece.h"

GalacticTimepiece::~GalacticTimepiece() {
	stopTicking();

	for (std::pair<std::string, OrreryTimepiece*>& timepiece : timepieces) {
		delete timepiece.second;
	}
}

void GalacticTimepiece::clear() {
	stopTicking();

	for (std::pair<std::string, OrreryTimepiece*>& timepiece : timepieces) {
		delete timepiece.second;
	}

	timepieces.clear();
}

std::vector<std::string> GalacticTimepiece::getTimesMilitary() const {
	std::vector<std::string> times;

	for (const std::pair<std::string, OrreryTimepiece*>& timepiece : timepieces) {
		for (const std::string& time : timepiece.second->getTimesMilitary()) {
			times.emplace_back(timepiece.first + time);
		}
	}

	return times;
}

std::vector<std::string> GalacticTimepiece::getTimes() const {
	std::vector<std::string> times;

	for (const std::pair<std::string, OrreryTimepiece*>& timepiece : timepieces) {
		for (const std::string& time : timepiece.second->getTimes()) {
			times.emplace_back(timepiece.first + time);
		}
	}

	return times;
}

void GalacticTimepiece::tick() {
	std::lock_guard<std::mutex> lock(mtx);

	for (std::pair<std::string, OrreryTimepiece*>& timepiece : timepieces) {
		timepiece.second->tick();
	}
}

void GalacticTimepiece::startTicking() {
	running = true;

	thread = std::thread([this]() {
		auto nextTick = std::chrono::steady_clock::now() + std::chrono::seconds(1);
		auto start = std::chrono::steady_clock::now();
		auto end = std::chrono::steady_clock::now();
		auto tickDuration =
			std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

		while (running) {
			start = std::chrono::steady_clock::now();
			tick();
			end = std::chrono::steady_clock::now();
			tickDuration =
				std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

			if (tickDuration > 1000)
				std::cerr << "Tick duration exceeded 1000 milliseconds" << std::endl;

			std::this_thread::sleep_until(nextTick);
			nextTick += std::chrono::seconds(1);
		}
		});
}

void GalacticTimepiece::stopTicking() {
	running = false;

	if (thread.joinable()) thread.join();
}
