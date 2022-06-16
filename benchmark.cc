#include "util.h"
#include "dictionary.h"
#include "tsc_clock.h"

extern "C" 
{
#include <x86intrin.h> // __rdtsc
}

#include <iostream>
#include <vector>

void BenchmarkInDictionary(const Dictionary& dict, const std::vector<std::string>& wordsIn)
{
	bool allFound = true;

	const auto startTs = TSCClock::now();
	for (const std::string& word : wordsIn)
	{
		const bool isPresent = dict.Contains(word);
		allFound &= isPresent;
	}
	const auto endTs = TSCClock::now();

	if (!allFound)
		throw std::runtime_error("InDictionary consistency check failed");

	std::cout << "InDict:" << TSCClock::FromCycles(endTs - startTs).count() / (double)wordsIn.size() << "ns/it" << std::endl;
}

void BenchmarkNotInDictionary(const Dictionary& dict, const std::vector<std::string>& wordsNotIn)
{
	bool someFound = false;

	const auto startTs = TSCClock::now();
	for (const std::string& word : wordsNotIn)
	{
		const bool isPresent = dict.Contains(word);
		someFound |= isPresent;
	}
	const auto endTs = TSCClock::now();

	if (someFound)
		throw std::runtime_error("NotInDictionary consistency check failed");

	std::cout << "NotInDict:" << TSCClock::FromCycles(endTs - startTs).count() / (double)wordsNotIn.size() << "ns" << std::endl;
}

int main()
{
	TSCClock::Initialise();

	const int DictionarySize = 100'000;
	const std::vector<std::string> words = CreateUniqueRandomStrings(DictionarySize * 2);

	std::vector<std::string> wordsIn, wordsNotIn;
	wordsIn.assign(words.begin(), words.begin() + DictionarySize);
	wordsNotIn.assign(words.begin() + DictionarySize, words.end());

	Dictionary dict(wordsIn);
	BenchmarkInDictionary(dict, wordsIn);
	BenchmarkNotInDictionary(dict, wordsNotIn);
	
	return 0;
}
