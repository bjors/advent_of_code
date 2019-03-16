#include "InputParser.h"
#include <iostream>
#include <array>

int CalculatePowerLevel(const int x, const int y, const int serial) {
	const auto rackId = x + 10;
	const auto powerLevel = (((((rackId * y) + serial) * rackId) / 100) % 10) - 5;
	return powerLevel;
}

using Grid = std::array<int, 300*300>;

Grid CreateGrid(const int serial) {
	Grid grid;
	for (int y=0; y<300; ++y) {
		for (int x=0; x<300; ++x) {
			const int index = x + y*300;
			const auto powerLevel = CalculatePowerLevel(x+1, y+1, serial);
			grid[index] = powerLevel;
		}
	}
	return grid;
}

int GetPowerOfSquare(const Grid& grid, const int startX, const int startY, int size) {
	int totalPower = 0;
	for (int y=startY; y<startY+size; ++y) {
		for (int x=startX; x<startX+size; ++x) {
			totalPower += grid[x + y*300];
		}
	}
	return totalPower;
}

std::tuple<int, int> FindBest3x3Square(const Grid& grid) {
	int highestPower = 0;
	int bestIndex = 0;
	const int size = 3;
	for (int y=0; y<300-size; ++y) {
		for (int x=0; x<300-size; ++x) {
			const int power = GetPowerOfSquare(grid, x, y, size);
			if (power > highestPower) {
				highestPower = power;
				bestIndex = x+y*300;
			}
		}
	}
	return {(bestIndex % 300)+1, (bestIndex / 300)+1};
}

std::tuple<int, int, int> FindBestSquareAnySize(const Grid& grid) {
	int highestPower = 0;
	int bestIndex = 0;
	int bestSize = 0;
	for (int y=0; y<300; ++y) {
		for (int x=0; x<300; ++x) {
			for (int size=1; size<=300-std::max(x,y); size++) {
				// slow, shouldn't recalculate entire square, just add one row/column
				const int power = GetPowerOfSquare(grid, x, y, size);
				if (power > highestPower) {
					highestPower = power;
					bestIndex = x+y*300;
					bestSize = size;
				}
			}
		}
	}
	return {(bestIndex % 300)+1, (bestIndex / 300)+1, bestSize};
}

int main(int argc, char* argv[]) {
	const int serial = 6548;
	const auto grid = CreateGrid(serial);
	
	const auto [bestX, bestY] = FindBest3x3Square(grid);
	std::cout << "Part 1: " << bestX << "," << bestY << std::endl;
	
	const auto [bestX2, bestY2, bestSize] = FindBestSquareAnySize(grid);
	std::cout << "Part 2: " << bestX2 << "," << bestY2 << "," << bestSize << std::endl;
	
	return 0;
}

