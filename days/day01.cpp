#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <numeric>

#include "InputParser.h"

int accumulateFrequencies(const std::vector<int>& frequencyChanges) {
	return std::accumulate(std::begin(frequencyChanges), std::end(frequencyChanges), 0);
}

int findFirstDuplicateAccumulation(const std::vector<int>& frequencyChanges) {
	std::set<int> foundFrequencies;

	int frequency = 0;
	foundFrequencies.insert(frequency);

	while (true) {
		for (const auto change : frequencyChanges) {
			frequency += change;
			if (foundFrequencies.find(frequency) == foundFrequencies.end()) {
				foundFrequencies.insert(frequency);
			} else {
				return frequency;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	const auto frequencyChanges = utils::getInputValues("inputs/day01.txt");

	const int finalFrequency = accumulateFrequencies(frequencyChanges);
	std::cout << "Part 1: " << finalFrequency << std::endl;

	const int firstDuplicate = findFirstDuplicateAccumulation(frequencyChanges);
	std::cout << "Part 2: " << firstDuplicate << std::endl;
	
	return 0;
}