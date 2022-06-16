#pragma once

#include <random>
#include <unordered_set>
#include <sstream>
#include <vector>
#include <string>

inline std::string CreateRandomString()
{
    static std::random_device rd;
    static std::uniform_int_distribution<int> charDist(48, 127);
    static std::lognormal_distribution<float> lengthDist(2.19, 0.25);
    std::size_t len = (int)(std::floor(lengthDist(rd))) + 1;
    std::string out;
    out.reserve(len);
    for(std::size_t i = 0; i < len; i++)
        out.push_back(charDist(rd));

    return out;
}

inline std::vector<std::string> CreateUniqueRandomStrings(std::size_t len)
{
    std::unordered_set<std::string> strings;
    strings.reserve(len);

    while (strings.size() < len)
        strings.insert(CreateRandomString());

    return std::vector<std::string>(strings.begin(), strings.end());
}

