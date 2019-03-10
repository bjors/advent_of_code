#include <string>
#include <cctype>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <numeric>
#include <tuple>
#include <limits>
#include "InputParser.h"
#include <unordered_map>

struct SCoord {
  int x;
  int y;

  SCoord operator-(const SCoord& other) const {
    return SCoord{x - other.x, y - other.y};
  }

  SCoord operator+(const SCoord& other) const {
    return SCoord{x + other.x, y + other.y};
  }
};

SCoord ParseCoord(const std::string& line) {
  std::istringstream stream{line};
  int x;
  char dummy;
  int y;
  stream >> x;
  stream >> dummy;
  stream >> y;
  return SCoord{x,y};
}

std::vector<SCoord> ParseCoords(const std::vector<std::string>& inputLines) {
  std::vector<SCoord> coords;
  coords.reserve(inputLines.size());
  for (const auto& line : inputLines) {
    coords.push_back(ParseCoord(line));
  }
  return coords;
}

std::tuple<SCoord, SCoord> CalcBounds(const std::vector<SCoord>& coords) {
  SCoord minCoord(coords.front());
  SCoord maxCoord(coords.front());
  for (const auto& coord : coords) {
    minCoord.x = std::min(minCoord.x, coord.x);
    minCoord.y = std::min(minCoord.y, coord.y);

    maxCoord.x = std::max(maxCoord.x, coord.x);
    maxCoord.y = std::max(maxCoord.y, coord.y);
  }
  return {minCoord, maxCoord};
}

struct SCell {
  bool IsInitialized() const {
    return mClosestDistance != std::numeric_limits<int>::max();
  }
  bool IsTie() const {
    return IsInitialized() && mClosestCoord == -1;
  }
  int mClosestCoord = -1;
  int mClosestDistance = std::numeric_limits<int>::max();
};

struct SGrid {
  std::vector<SCell> mCells;
  SCoord mSize;
};

int GetDistance(const SCoord& a, const SCoord& b) {
  return std::abs(a.x-b.x) + std::abs(a.y-b.y);
}

SGrid CreateGrid(const std::vector<SCoord>& coords) {
  const auto [minCoord, maxCoord] = CalcBounds(coords);

  const auto gridSize = SCoord{1,1} + maxCoord - minCoord;

  std::vector<SCell> grid;
  grid.resize(gridSize.x*gridSize.y);

  for (const auto& coord : coords) {
    const auto translatedCoord = coord - minCoord;
    for (int y=0; y<gridSize.y; ++y) {
      for (int x=0; x<gridSize.x; ++x) {
        auto& cell = grid[x + y*gridSize.x];
        const auto distance = GetDistance(SCoord{x,y}, translatedCoord);
        if (!cell.IsInitialized() || distance < cell.mClosestDistance) {
          cell.mClosestDistance = distance;
          cell.mClosestCoord = translatedCoord.x + translatedCoord.y*gridSize.x;
        } else if (distance == cell.mClosestDistance) {
          cell.mClosestCoord = -1;
        }
      }
    }
  }
  return {grid, gridSize};
}

int GetLargestCount(const SGrid& grid) {
  std::unordered_map<int, int> countPerId;
  for (int y=0; y<grid.mSize.y; ++y) {
    for (int x=0; x<grid.mSize.x; ++x) {
      auto& currentCell = grid.mCells[x + y*grid.mSize.x];
      if (currentCell.IsTie()) {
        continue;
      }
      auto& count = countPerId[currentCell.mClosestCoord];
      if (x==0 || y==0 || x==grid.mSize.x-1 || y==grid.mSize.y-1 || count == -1) {
        count = -1;
      } else {
        count++;
      }
    }
  }

  const auto maxIter = std::max_element(countPerId.begin(), countPerId.end(), [](const std::pair<int,int>& left, const std::pair<int,int>& right){
    return left.second < right.second;
  });

  return maxIter->second;
}

int GetSizeOfSafeRegion(const std::vector<SCoord>& coords) {
  const auto [minCoord, maxCoord] = CalcBounds(coords);

  int regionSize = 0;
  for (int y=minCoord.y; y<=maxCoord.y; ++y) {
    for (int x=minCoord.x; x<=maxCoord.x; ++x) {
      const auto totalDistance = std::reduce(coords.begin(), coords.end(), 0, [x, y](const int acc, const SCoord& coord){
        return acc + GetDistance(SCoord{x,y}, coord);
      });
      if (totalDistance < 10000) {
        regionSize++;
      }
    } 
  }

  return regionSize;
}

int main(int argc, char* argv[]) {
  const auto inputLines = utils::getInputLines("inputs/day06.txt");

  const auto coords = ParseCoords(inputLines);
  const auto grid = CreateGrid(coords);
  const auto largestCount = GetLargestCount(grid);
  std::cout << "Part 1: " << largestCount << std::endl;

  const auto safeRegionSize = GetSizeOfSafeRegion(coords);
  std::cout << "Part 2: " << safeRegionSize << std::endl;

  return 0;
}
