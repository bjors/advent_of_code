#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <optional>

#include "InputParser.h"

int calcChecksum(const std::vector<std::string>& lines) {
	std::unordered_map<int, int> totalUniqueCounts;

	for (const auto line : lines) {
		std::unordered_map<char, int> characterCounts;
		for (const char ch : line) {
			characterCounts[ch]++;
		}

		std::set<int> uniqueCounts;
		for (const auto iter : characterCounts) {
			uniqueCounts.insert(iter.second);
		}

		for (const auto count : uniqueCounts) {
			totalUniqueCounts[count]++;
		}
	}

	const int checksum = totalUniqueCounts[2]*totalUniqueCounts[3];
	return checksum;
}

std::optional<std::string> findCommon(const std::string& lineA, const std::string& lineB) {
	auto mismatchResult = std::mismatch(lineA.begin(), lineA.end(), lineB.begin());
	if (mismatchResult.first == lineA.end()) {
		// strings are equal
		return {};
	}

	const auto beginA = std::string(lineA.begin(), mismatchResult.first);
	const auto trailA = std::string(++mismatchResult.first, lineA.end());
	const auto trailB = std::string(++mismatchResult.second, lineB.end());
	if (trailA != trailB) {
		// more than one character is different
		return {};
	}
		
	return beginA + trailA;
}

std::optional<std::string> findCommon(std::vector<std::string> lines) {
	std::sort(lines.begin(), lines.end());

	for (std::size_t i=1; i<lines.size(); ++i) {
		const auto& lineA = lines[i-1];
		const auto& lineB = lines[i];

		const auto result = findCommon(lineA, lineB);
		if (result.has_value()) {
			return result;
		}
	}

	return {};
}

int main(int argc, char* argv[]) { 
	const auto lines = utils::getInputLines("./inputs/day02.txt");

	const int checksum = calcChecksum(lines);
	std::cout << "Part 1: " << checksum << std::endl;

	const auto commonString = findCommon(lines);
	std::cout << "Part 2: " << commonString.value() << std::endl;
}