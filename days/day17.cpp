#include "InputParser.h"
#include "Vector2i.h"
#include <iostream>
#include <numeric>
#include <sstream>
#include <stack>
#include <tuple>

std::vector<SVector2i> ParseInput() {
	const auto inputLines = utils::getInputLines("inputs/day17.txt");
	std::vector<SVector2i> clayVeins;
	for (const auto& line : inputLines) {
		char axis1;
		int coord1;
		char axis2;
		int coord2a;
		int coord2b;
		std::sscanf(line.c_str(), "%c=%d, %c=%d..%d", &axis1, &coord1, &axis2, &coord2a, &coord2b);
		if (axis1 == 'x') {
			for (int y=coord2a; y<=coord2b; ++y) {
				clayVeins.push_back({coord1, y});
			}
		} else if (axis1 == 'y') {
			for (int x=coord2a; x<=coord2b; ++x) {
				clayVeins.push_back({x, coord1});
			}
		}
	}
	
	return clayVeins;
}

enum Tile {
	Sand,
	Clay,
	SettledWater,
	Flowing,
	Spring
};

char ToChar(const Tile tile) {
	switch (tile) {
		case Sand: return '.';
		case Clay: return '#';
		case SettledWater: return '~';
		case Flowing: return '|';
		case Spring: return '+';
	}
}

struct Bounds {
	SVector2i GetSize() const { return SVector2i{1,1}+max-min; }
	
	SVector2i min{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
	SVector2i max{std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};
};

struct Ground {
	Ground(const Bounds& bounds, const SVector2i& spring, const std::vector<SVector2i>& clayVeins)
		: bounds(bounds)
		, spring(spring)
		, size(bounds.max + SVector2i{1,1}) {

		tiles.resize(size.x * size.y);
		std::fill(tiles.begin(), tiles.end(), Tile::Sand);
		
		for (const auto& vein : clayVeins) {
			const auto index = GetIndex(vein);
			tiles[index] = Tile::Clay;
		}
		
		tiles[GetIndex(this->spring)] = Tile::Spring;
	}
	
	int GetIndex(const SVector2i& coord) const {
		return coord.x + coord.y*size.x;
	}
	
	void Draw() const {
		for (int y=0; y<size.y; ++y) {
			std::stringstream stream;
			for (int x=0; x<size.x; ++x) {
				const auto index = GetIndex({x,y});
				const auto tile = tiles[index];
				stream << ToChar(tile);
			}
			std::cout << stream.str() << std::endl;
		}
	}
	
	Tile Get(const SVector2i& coord) const {
		const auto index = GetIndex(coord);
		return tiles[index];
	}
	
	void Set(const SVector2i& coord, const Tile tile) {
		const auto index = GetIndex(coord);
		tiles[index] = tile;
	}
	
	const Bounds bounds;
	const SVector2i spring;
	const SVector2i size;
	std::vector<Tile> tiles;
	std::vector<SVector2i> movingWater;
};

bool IsSolid(const Tile tile) {
	return tile == Tile::Clay || tile == Tile::SettledWater;
};

bool IsFree(const Tile tile) {
	return tile == Tile::Flowing || tile == Tile::Sand;
};


int main(int argc, char* argv[]) {
	
	const auto clayVeins = ParseInput();
	const auto bounds = std::reduce(clayVeins.begin(), clayVeins.end(), Bounds{clayVeins.front(), clayVeins.front()}, [](const Bounds acc, const SVector2i& vein){
		return Bounds{
			{std::min(acc.min.x, vein.x), std::min(acc.min.y, vein.y)},
			{std::max(acc.max.x, vein.x+1), std::max(acc.max.y, vein.y+1)},
		};
	});
	
	const SVector2i springCoord{500,0};
	Ground ground(bounds, springCoord, clayVeins);
	
	std::stack<SVector2i> queue;
	queue.push(springCoord);
	
	while (!queue.empty()) {
		const auto coord = queue.top();
		queue.pop();
		
		if (coord.y >= ground.size.y-1) {
			continue;
		}
		
		const auto tile = ground.Get({coord.x, coord.y+1});
		if (tile == Tile::Sand) {
			if (coord.y < ground.size.y-1) {
				queue.push({coord.x, coord.y+1});
			}
			
			ground.Set({coord.x, coord.y+1}, Tile::Flowing);
		} else if (IsSolid(tile)) {
			const auto fillHorizontal = [&](const SVector2i& start, int xDir) -> std::tuple<bool, int> {
				SVector2i testCoord = start;
				while (true) {
					const auto tile = ground.Get({testCoord.x, testCoord.y});
					if (IsSolid(tile)) {
						return {true, testCoord.x-xDir};
					}
					
					const auto tileBelow = ground.Get({testCoord.x, testCoord.y+1});
					if (IsFree(tileBelow)) {
						queue.push(testCoord);
						return {false, testCoord.x};
					}
					
					testCoord.x += xDir;
				}
			};
			
			const auto [wallLeft, minX] = fillHorizontal(coord, -1);
			const auto [wallRight, maxX] = fillHorizontal(coord, 1);
			
			if (wallLeft && wallRight) {
				queue.push({coord.x, coord.y-1});
			}
			
			for (int x=minX; x<=maxX; ++x) {
				ground.Set({x, coord.y}, (wallLeft && wallRight) ? Tile::SettledWater : Tile::Flowing);
			}
		}
	}
	
	int countFlowing = 0;
	int countSettled = 0;
	for (int y=bounds.min.y; y<bounds.max.y; ++y) {
		for (int x=0; x<ground.size.x; ++x) {
			const auto tile = ground.Get(SVector2i{x,y});
			if (tile == Tile::Flowing) {
				countFlowing++;
			} else if (tile == Tile::SettledWater) {
				countSettled++;
			}
		}
	}
	
	ground.Draw();
	
	std::cout << "Part 1: " << (countFlowing+countSettled) << std::endl; // 39649
	std::cout << "Part 2: " << (countSettled) << std::endl; // 28872
	
	return 0;
}

