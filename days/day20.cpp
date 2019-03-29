#include "InputParser.h"
#include "Vector2i.h"
#include <iostream>
#include <map>
#include <algorithm>

enum Direction {
	West,
	South,
	East,
	North
};

SVector2i Translate(const SVector2i& coord, const Direction direction) {
	switch (direction) {
		case West: return coord + SVector2i{-1,0};
		case East: return coord + SVector2i{1,0};
		case South: return coord + SVector2i{0,1};
		case North: return coord + SVector2i{0,-1};
	}
}

Direction GetDirection(const char c) {
	switch (c) {
		case 'E': return Direction::East;
		case 'W': return Direction::West;
		case 'S': return Direction::South;
		case 'N': return Direction::North;
	}
	assert(false);
}

std::map<SVector2i, int> distanceMap;

int GenerateMap(const std::string& inputString, int stringIndex = 0, const SVector2i startCoord={0,0}, const int startDistance=0) {
	auto coord = startCoord;
	int distance = startDistance;
	
	while (true) {
		const char c = inputString[stringIndex++];
		
		if (c == '^') {
			continue;
		} else if (c == '|') {
			coord = startCoord;
			distance = startDistance;
		} else if (c == '(') {
			stringIndex = GenerateMap(inputString, stringIndex, coord, distance);
		} else if (c == ')' || c == '$') {
			return stringIndex;
		} else {
			const auto direction = GetDirection(c);
			const auto newCoord = Translate(coord, direction);
			distance++;
			if (distanceMap.find(newCoord) == distanceMap.end()) {
				distanceMap[newCoord] = distance;
			} else {
				distanceMap[newCoord] = std::min(distanceMap[newCoord], distance);
			}
			coord = newCoord;
		}
	}
};

int main(int argc, char* argv[]) {
	const auto inputString = utils::getInputLines("inputs/day20.txt").front();
	
	distanceMap[{0,0}] = 0;

	GenerateMap(inputString);

	const auto maxDist = std::max_element(distanceMap.begin(), distanceMap.end(), [](const std::pair<SVector2i, int>& a, const std::pair<SVector2i, int>& b){
		return a.second < b.second;
	})->second;
	const auto atLeast1000doors = std::count_if(distanceMap.begin(), distanceMap.end(), [](const std::pair<SVector2i, int>& iter){
		return iter.second >= 1000;
	});
	
	std::cout << "Part 1: " << maxDist << std::endl;
	std::cout << "Part 2: " << atLeast1000doors << std::endl;
	
	return 0;
}
