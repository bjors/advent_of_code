#include "InputParser.h"
#include <iostream>
#include <vector>
#include <array>
#include <tuple>
#include <numeric>

constexpr int FormulaSize = 5;

std::tuple<std::string, std::vector<std::string>> ParseInput() {
	const auto inputLines = utils::getInputLines("inputs/day12.txt");
	
	const auto startIndex = strlen("initial state: ");
	std::string initialState = inputLines.front().substr(startIndex);
	
	std::vector<std::string> formulas;
	for (std::size_t i=2; i<inputLines.size(); ++i) {
		const auto& line = inputLines[i];
		if (line[line.size()-1] == '.') {
			continue;
		}
		formulas.push_back(line.substr(0, FormulaSize));
	}
	
	return {initialState, formulas};
}

std::tuple<std::string, int> AddPadding(std::string input) {
	int offsetChange = 0;
	const auto paddingSize = FormulaSize;
	const auto firstIndex = input.find('#');
	if (firstIndex < paddingSize-1) {
		const int numToAdd = paddingSize-1-firstIndex;
		for (int i=0; i<numToAdd; ++i) {
			input.insert(input.begin(), '.');
		}
		offsetChange += numToAdd;
	}
	const auto lastIndex = input.rfind('#');
	const auto numberOfEmptyAtEnd = input.size() - lastIndex;
	if (numberOfEmptyAtEnd < paddingSize) {
		input.append(paddingSize-numberOfEmptyAtEnd, '.');
	}
	return {input, offsetChange};
}

bool MatchesFormula(const std::string& state, const std::size_t index, const std::vector<std::string>& formulas) {
	for (const auto& formula : formulas) {
		if (state.compare(index-2, FormulaSize, formula) == 0) {
			return true;
		}
	}
	return false;
}

std::tuple<std::string, int> NextGeneration(const std::string& prevState, const std::vector<std::string>& formulas, const int offset) {
	const auto [state, offsetChange] = AddPadding(prevState);
	const auto newOffset = offset + offsetChange;
	std::string newState = state;
	
	for (int i=2; i<state.size()-2; ++i) {
		if (MatchesFormula(state, i, formulas)) {
			newState[i] = '#';
		} else {
			newState[i] = '.';
		}
	}
		
	return {newState, newOffset};
}

std::string Trim(const std::string& input) {
	const auto startIndex = input.find('#');
	const auto endIndex = input.rfind('#');
	return input.substr(startIndex, endIndex-startIndex);
}

long long CalcSum(const long long numGenerations, const std::string& initialState, const std::vector<std::string>& formulas) {
	auto state = initialState;
	int offset = 0;
	long long generation = 0;
	for (; generation<numGenerations; ++generation) {
		const auto [newState, newOffset] = NextGeneration(state, formulas, offset);
		
		if (Trim(newState) == Trim(state)) {
			break;
		}
		
		state = newState;
		offset = newOffset;
	}
	
	long long sum = 0;
	for (std::size_t i=0; i<state.size(); ++i) {
		if (state[i] == '#') {
			sum += (numGenerations-generation)+i-offset;
		}
	}
	
	return sum;
}

int main(int argc, char* argv[]) {
	const auto [initialState, formulas] = ParseInput();
	
	{
		const auto sum = CalcSum(20, initialState, formulas);
		std::cout << "Sum: " << sum << std::endl;
	}

	{
		const auto sum = CalcSum(50000000000, initialState, formulas);
		std::cout << "Sum: " << sum << std::endl;
	}
	
	return 0;
}
