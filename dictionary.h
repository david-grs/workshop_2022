#pragma once

#include <set>
#include <string>
#include <string_view>

class Dictionary
{
public:
	explicit Dictionary(const std::vector<std::string>& words) :
        mWords(words.begin(), words.end())
	{}

	bool Contains(std::string_view word) const
	{
		return mWords.find({word.data(), word.size()}) != mWords.end();
	}

private:
	std::set<std::string> mWords;
};

