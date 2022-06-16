#pragma once

#include <cstdint>
#include <chrono>
#include <cmath>
#include <thread>

extern "C"
{
#include <x86intrin.h>
}

struct TSCClock
{
	using time_point = std::uint64_t;

	static time_point now() { return __rdtsc(); }

	static void Initialise();

	static std::chrono::nanoseconds FromCycles(std::uint64_t cycles)
	{
		return std::chrono::nanoseconds{std::lround(cycles / GetFrequencyGHz())};
	}

	template <class Duration>
	static std::uint64_t ToCycles(Duration duration)
	{
		return std::lround(GetFrequencyGHz() * std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
	}

	static double GetFrequencyGHz() { return GetFrequencyGHzImpl(); }

private:
	static double& GetFrequencyGHzImpl()
	{
		static double TSCFreqGHz = .0;
		return TSCFreqGHz;
	}
};

namespace Internal
{

inline double MeasureTSCFrequency()
{
	using SteadyClock = std::conditional_t<
			std::chrono::high_resolution_clock::is_steady,
			std::chrono::high_resolution_clock,
			std::chrono::steady_clock>;

	std::uint32_t unused;

	const auto startTs = SteadyClock::now();
	const auto startTSC = __rdtscp(&unused);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	const auto endTSC = __rdtscp(&unused);
	const auto endTs = SteadyClock::now();

	const auto durationNs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTs - startTs);
	return (endTSC - startTSC) / double(durationNs.count());
}

}

inline void TSCClock::Initialise()
{
	double& tscFreq = GetFrequencyGHzImpl();
	if (tscFreq != .0)
	{
		return;
	}

	double prevFreq = -1.0;
	for (int i = 0; i < 10; ++i)
	{
		tscFreq = Internal::MeasureTSCFrequency();

		if (tscFreq > prevFreq * 0.9999 && tscFreq < prevFreq * 1.0001)
		{
			break;
		}

		prevFreq = tscFreq;
	}
}
