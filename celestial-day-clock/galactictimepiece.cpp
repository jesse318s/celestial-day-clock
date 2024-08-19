#include "galactictimepiece.h"

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

	stopTicking();
	timepieces.emplace_back(label, timepiece);
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

	const auto tickRange = [](auto start, const auto end) {
		for (auto& itr = start; itr != end; ++itr) {
			if (itr->second == nullptr)
				throw std::runtime_error("Null timepiece pointer encountered in tick");

			itr->second->tick();
		}
		};

	std::future<void> firstHalf =
		std::async(std::launch::async, tickRange, timepieces.begin(), mid);
	std::future<void> secondHalf =
		std::async(std::launch::async, tickRange, mid, timepieces.end());

	firstHalf.get();
	secondHalf.get();
}

void GalacticTimepiece::startTicking() {
	if (running) return;

	running = true;

	auto tickingTask = [this]() {
		auto nextTick = std::chrono::steady_clock::now() + std::chrono::seconds(1);

		while (running) {
			const auto start = std::chrono::steady_clock::now();
			tick();
			const auto end = std::chrono::steady_clock::now();
			const auto tickDuration =
				std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

			if (tickDuration > 1000)
				std::cerr << "Tick duration exceeded 1000 milliseconds" << std::endl;

			std::this_thread::sleep_until(nextTick);
			nextTick += std::chrono::seconds(1);
		}
		};

	tickingFuture = std::async(std::launch::async, tickingTask);
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