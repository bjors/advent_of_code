#include <string>
#include <cctype>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <numeric>
#include "InputParser.h"

bool ShouldReact(const char a, const char b) {
	const bool sameType = std::tolower(a) == std::tolower(b);
	if (sameType) {
		const bool isUpperA = std::isupper(a);
		const bool isUpperB = std::isupper(b);
		return isUpperA != isUpperB;
	} else {
		return false;
	}
}

std::stack<char> RemoveReactingUnits(const std::string& inputString) {
	std::stack<char> stack;
	for (const auto unit : inputString) {
		if (stack.empty() || !ShouldReact(unit, stack.top())) {
			stack.push(unit);
		} else {
			stack.pop();
		}
	}
	return stack;
}

std::string GenerateAlphabet() {
	std::stringstream alphabetStream;
	for (auto c='a'; c<='z'; c++) {
		alphabetStream << c;
	}
	return alphabetStream.str();
}

std::string RemoveTypeFromString(const std::string& inputString, const char c) {
	std::string reducedString;
	std::copy_if(inputString.begin(), inputString.end(), std::back_inserter(reducedString), [c](const char inputC){
		return std::tolower(inputC) != c;
	});
	return reducedString;
}

std::size_t FindShortestPolymer(const std::string& inputString) {
	const auto alphabet = GenerateAlphabet();

	std::vector<std::size_t> reducedSizes;
	std::transform(alphabet.begin(), alphabet.end(), std::back_inserter(reducedSizes), [inputString](const char c){
		const auto reducedString = RemoveTypeFromString(inputString, c);
		const auto reducedSize = RemoveReactingUnits(reducedString).size();
		return reducedSize;
	});
		
	const auto minSize = std::min_element(reducedSizes.begin(), reducedSizes.end());

	return *minSize;
}

int main(int argc, char* argv[]) {
	const auto inputString = utils::getInputLines("inputs/day05.txt").front();

	const auto reactedPolymer = RemoveReactingUnits(inputString);
	std::cout << "Part 1: " << reactedPolymer.size() << std::endl;
	
	const auto shortestPolymer = FindShortestPolymer(inputString);
	std::cout << "Part 2: " << shortestPolymer << std::endl;

	return 0;
}
