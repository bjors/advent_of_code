#include "InputParser.h"
#include <vector>
#include <iostream>
#include <string>

using State = std::vector<std::string>;

struct Counts {
	int trees = 0;
	int lumber = 0;
};

std::vector<std::string> Tick(const State& state) {
	const auto countAcres = [](const int x, const int y, const State& state) -> Counts {
		Counts counts;
		
		for (int iy=y-1; iy<=y+1; iy++) {
			if (iy < 0 || iy >= state.size()) {
				continue;
			}
			
			const auto& line = state[iy];
			for (int ix=x-1; ix<=x+1; ix++) {
				if (ix==x && iy==y) {
					continue;
				}
				if (ix < 0 || ix >= line.length()) {
					continue;
				}
				
				if (line[ix] == '|') {
					counts.trees++;
				} else if (line[ix] == '#') {
					counts.lumber++;
				}
			}
		}
		return counts;
	};
	
	State newState = state;
	for (std::size_t y=0; y<state.size(); ++y) {
		const auto& line = state[y];
		for (std::size_t x=0; x<line.length(); ++x) {
			const auto c = line[x];
			
			const auto count = countAcres(x, y, state);
			if (c == '.') {
				if (count.trees >= 3) {
					newState[y][x] = '|';
				} else {
					newState[y][x] = '.';
				}
			} else if (c == '|') {
				if (count.lumber >= 3) {
					newState[y][x] = '#';
				} else {
					newState[y][x] = '|';
				}
			} else if (c == '#') {
				if (count.lumber >= 1 && count.trees >= 1) {
					newState[y][x] = '#';
				} else {
					newState[y][x] = '.';
				}
			}
			
		}
	}
	
	return newState;
}

void Draw(const State& state) {
	for (const auto& line : state) {
		std::cout << line << std::endl;
	}
	std::cout << std::endl;
}

Counts CountTreeAndLumber(const State& state) {
	Counts counts;
	for (const auto& line : state) {
		for (const auto& c : line) {
			if (c == '|') {
				counts.trees++;
			} else if (c == '#') {
				counts.lumber++;
			}
		}
	}
	return counts;
}

int main(int argc, char* argv[]) {
	const auto initialState = utils::getInputLines("inputs/day18.txt");
	
	{
		auto state = initialState;
		for (int i=0; i<10; ++i) {
			state = Tick(state);
		}
		const auto counts = CountTreeAndLumber(state);
		std::cout << "Part 1: " << counts.trees * counts.lumber << std::endl;
	}
	
	std::vector<State> history;
	int numIterations = 0;
	auto state = initialState;
	const int targetMinutes = 1000000000;
	while (true) {
		state = Tick(state);
		
		const auto iter = std::find(history.begin(), history.end(), state);
		if (history.end() != iter) {
			const auto cycleStart = std::distance(history.begin(), iter);
			const auto cycleLength = numIterations - cycleStart;
			
			const auto offset = (targetMinutes - cycleStart) % cycleLength;
			const auto historyIndex = cycleStart+offset;
			
			const auto counts = CountTreeAndLumber(history[historyIndex-1]);
			std::cout << "Part 2: " << counts.trees * counts.lumber << std::endl;
			break;
		} else {
			history.push_back(state);
			numIterations++;
		}
	}
	
	return 0;
}


