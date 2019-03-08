#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "InputParser.h"
#include <unordered_map>

struct SInterval {
	int mFirstMinute;
	int mLastMinute;
};
struct SGuard {
	std::vector<SInterval> mSleepIntervals;
	int mTotalMinutesSlept = 0;
	int mId;
};

std::unordered_map<int, SGuard> ParseSleepPattern(const std::vector<std::string>& lines) {
	std::unordered_map<int, SGuard> guards;
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int guardId;

	for (const auto& line : lines) {
		if (std::string::npos != line.rfind("begins shift")) {
			std::sscanf(line.c_str(), "[%d-%d-%d %d:%d] Guard #%d begins shift", &year,&month,&day,&hour,&minute,&guardId);
			guards[guardId].mId = guardId;
		} else if (std::string::npos != line.rfind("falls asleep")) {
			std::sscanf(line.c_str(), "[%d-%d-%d %d:%d] falls asleep", &year,&month,&day,&hour,&minute);
			guards[guardId].mSleepIntervals.push_back({hour*60+minute, -1});
		} else if (std::string::npos != line.rfind("wakes up")) {
			std::sscanf(line.c_str(), "[%d-%d-%d %d:%d] wakes up", &year,&month,&day,&hour,&minute);

			auto& interval = guards[guardId].mSleepIntervals.back();
			interval.mLastMinute = hour*60+minute;
			guards[guardId].mTotalMinutesSlept += interval.mLastMinute - interval.mFirstMinute;
		} 
	}

	return guards;
}

SGuard FindMostSleepyGuard(const std::unordered_map<int, SGuard>& guards) {
	const auto mostSleepyGuard = std::max_element(guards.begin(), guards.end(), [](const std::pair<int,SGuard>& left, const std::pair<int, SGuard>& right){
		return left.second.mTotalMinutesSlept < right.second.mTotalMinutesSlept;
	});
	return mostSleepyGuard->second;
}

int FindMostSleptMinute(const SGuard& guard) {
	std::unordered_map<int, int> countPerMinute;
	for (const auto& interval : guard.mSleepIntervals) {
		for (int i=interval.mFirstMinute; i<interval.mLastMinute; ++i) {
			countPerMinute[i]++;
		}
	}

	const auto mostSleptMinute = std::max_element(countPerMinute.begin(), countPerMinute.end(), [](const std::pair<int, int>& left, const std::pair<int, int>& right){
		return left.second < right.second;
	});

	return mostSleptMinute->first;
}

int main(int argc, char* argv[]) {
	auto lines = utils::getInputLines("inputs/day04.txt");
	std::sort(lines.begin(), lines.end());
	
	const auto guards = ParseSleepPattern(lines);
	const auto mostSleepyGuard = FindMostSleepyGuard(guards);
	const auto mostSleptMinute = FindMostSleptMinute(mostSleepyGuard);
	
	std::cout << "Part 1" << mostSleepyGuard.mId*mostSleptMinute << std::endl;
	
	
	
	return 0;
}
