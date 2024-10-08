#include "galactictimepiece.h"
#include <chrono>
#include <iostream>
#include <thread>

GalacticTimepiece::~GalacticTimepiece() {
	stopTicking();
	deleteTimepieces();
}

size_t GalacticTimepiece::getSize() const {
	size_t size = 0;

	for (const std::pair<std::string, OrreryTimepiece*>& timepiece : timepieces) {
		if (timepiece.second == nullptr)
			throw std::runtime_error("Null timepiece pointer encountered in getSize");

		size += timepiece.second->getSize();
	}

	return size;
}

void GalacticTimepiece::add(const std::string& label, OrreryTimepiece* timepiece) {
	if (timepiece == nullptr) throw std::invalid_argument("Cannot add a null timepiece");

	for (const auto& [existingLabel, existingTimepiece] : timepieces) {
		if (existingTimepiece == nullptr)
			throw std::runtime_error("Null timepiece pointer encountered in add");

		if (label == existingLabel) {
			std::cerr << "Timepiece with label " << label << " already exists" << std::endl;
			return;
		}
	}

	stopTicking();
	timepieces.emplace_back(label, timepiece);
}

OrreryTimepiece& GalacticTimepiece::getTimepiece(const std::string& searchLabel) {
	stopTicking();

	for (const auto& [label, timepiece] : timepieces) {
		if (timepiece == nullptr)
			throw std::runtime_error("Null timepiece pointer encountered in getTimepiece");

		if (searchLabel == label) return *timepiece;
	}

	throw std::runtime_error("Timepiece with label " + searchLabel + " not found");
}

void GalacticTimepiece::clear() {
	stopTicking();
	deleteTimepieces();
	timepieces.clear();
}

std::vector<std::string> GalacticTimepiece::getTimesMilitary() {
	std::vector<std::string> times;

	stopTicking();

	for (const auto& [label, timepiece] : timepieces) {
		if (timepiece == nullptr)
			throw std::runtime_error("Null timepiece pointer encountered in getTimesMilitary");

		for (const std::string& time : timepiece->getTimesMilitary()) {
			times.emplace_back(label + time);
		}
	}

	return times;
}

std::vector<std::string> GalacticTimepiece::getTimes() {
	std::vector<std::string> times;

	stopTicking();

	for (const auto& [label, timepiece] : timepieces) {
		if (timepiece == nullptr)
			throw std::runtime_error("Null timepiece pointer encountered in getTimes");

		for (const std::string& time : timepiece->getTimes()) {
			times.emplace_back(label + time);
		}
	}

	return times;
}

void GalacticTimepiece::tick() {
	std::lock_guard<std::mutex> lock(mtx);
	const std::vector<std::pair<std::string, OrreryTimepiece*>>::iterator mid =
		timepieces.begin() + timepieces.size() / 2;

	const auto tickRng = [](auto start, const auto end) {
		try {
			for (auto& itr = start; itr != end; ++itr) {
				if (itr->second == nullptr) throw std::runtime_error("Null timepiece pointer encountered in tick");

				itr->second->tick();
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Exception in tickRange: " << e.what() << std::endl;
			throw;
		}
		};

	std::future<void> firstHalf = std::async(std::launch::async, tickRng, timepieces.begin(), mid);
	std::future<void> secondHalf = std::async(std::launch::async, tickRng, mid, timepieces.end());

	try {
		firstHalf.get();
		secondHalf.get();
	}
	catch (const std::exception& e) {
		std::cerr << "Exception in tick: " << e.what() << std::endl;
		stopTicking();
	}
}

void GalacticTimepiece::startTicking() {
	if (running) return;

	running = true;

	auto runTicks = [this]() {
		constexpr auto oneSecondInNanoseconds =
			std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)).count();
		auto nextTick = std::chrono::steady_clock::now() + std::chrono::seconds(1);

		try {
			while (running) {
				const auto start = std::chrono::steady_clock::now();
				tick();
				const auto end = std::chrono::steady_clock::now();
				const auto tickDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

				if (tickDuration > oneSecondInNanoseconds)
					std::cerr << "Tick duration exceeded " << oneSecondInNanoseconds << " nanoseconds" << std::endl;

				std::this_thread::sleep_until(nextTick);
				nextTick += std::chrono::seconds(1);
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Exception in tickingTask: " << e.what() << std::endl;
			stopTicking();
		}
		};

	tickingFuture = std::async(std::launch::async, runTicks);
}

void GalacticTimepiece::stopTicking() {
	running = false;

	if (tickingFuture.valid()) tickingFuture.get();
}

void GalacticTimepiece::deleteTimepieces() {
	for (std::pair<std::string, OrreryTimepiece*>& timepiece : timepieces) {
		if (timepiece.second != nullptr) {
			delete timepiece.second;
			timepiece.second = nullptr;
		}
	}
}