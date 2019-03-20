#include "InputParser.h"
#include <iostream>
#include <vector>
#include <array>
#include "Vector2i.h"
#include <set>
#include <optional>
#include <numeric>

struct Cell {
	enum CellType {
		Ground,
		Wall,
	};
	
	CellType type;
};

struct Unit {
	enum UnitType {
		Guard,
		Elf
	};
	
	UnitType unitType;
	SVector2i pos;
	int attackPower = 3;
	int hp = 200;
};

struct Board {
	void Draw() {
		std::cout << std::endl;
		std::vector<std::string> lines;
		lines.resize(height);
		for (auto& line : lines) {
			line.resize(width);
		}
		
		for (std::size_t i=0; i<grid.size(); ++i) {
			const auto coord = IndexToCoord(i);
			lines[coord.y][coord.x] = (grid[i].type == Cell::Ground) ? '.' : '#';
		}
		
		for (const auto& unit : units) {
			lines[unit.pos.y][unit.pos.x] = (unit.unitType == Unit::Guard) ? 'G' : 'E';
			
			lines[unit.pos.y] += " ";
			lines[unit.pos.y] += (unit.unitType == Unit::Guard) ? "G(" : "E(";
			lines[unit.pos.y] += std::to_string(unit.hp);
			lines[unit.pos.y] += "), ";
		}
		
		for (const auto& line : lines) {
			std::cout << line << std::endl;
		}
	}
	
	std::vector<int> GetDistanceMap(const Unit& unit) const {
		std::vector<int> distanceMap;
		distanceMap.resize(grid.size());
		std::fill(distanceMap.begin(), distanceMap.end(), -1);
		
		std::vector<SVector2i> queue;
		distanceMap[CoordToIndex(unit.pos)] = 0;
		queue.push_back(unit.pos);
		
		while (!queue.empty()) {
			const auto coord = queue.back();
			const auto index = CoordToIndex(coord);
			queue.pop_back();
			
			const auto distance = distanceMap[index];
			const auto neighbors = GetNeighborCoords(coord);
			for (const auto& neighborPos : neighbors) {
				if (IsOpenGround(neighborPos)) {
					const auto neighborIndex = CoordToIndex(neighborPos);
					const auto currentDistance = distanceMap[neighborIndex];

					if (distance+1 < currentDistance || currentDistance == -1) {
						distanceMap[neighborIndex] = distance+1;
						queue.push_back(neighborPos);
					}
				}
			}
		}
		
		/*
		for (int y=0; y<height; ++y) {
			for (int x=0; x<width; ++x) {
				std::cout << distanceMap[x+y*width] << '\t';
			}
			std::cout << std::endl;
		}
		 */
		
		return distanceMap;
	};
	
	bool IsValidCoord(const SVector2i& coord) const {
		if (coord.x < 0 || coord.y < 0 || coord.x >= width || coord.y >= height) {
			return false;
		} else {
			return true;
		}
	}
	
	std::vector<SVector2i> GetNeighborCoords(const SVector2i& coord) const {
		std::vector<SVector2i> neighbors = {
			{coord.x, coord.y-1},
			{coord.x-1, coord.y},
			{coord.x+1, coord.y},
			{coord.x, coord.y+1}
		};
		return neighbors;
	}
	
	bool IsOpenGround(const SVector2i& pos) const {
		if (!IsValidCoord(pos)) {
			return false;
		}
		const auto index = CoordToIndex(pos);
		if (grid[index].type == Cell::Wall) {
			return false;
		}
		if (units.end() != std::find_if(units.begin(), units.end(), [&](const Unit& unit){
			return unit.pos == pos;
		})) {
			return false;
		}
		return true;
	}
	
	std::optional<int> GetTargetUnitIndex(const Unit& unit) {
		std::vector<int> targets;
		for (int i=0; i<units.size(); ++i) {
			const auto& other = units[i];
			if (other.unitType == unit.unitType) {
				continue;
			}
			
			if (std::abs(other.pos.x - unit.pos.x)+std::abs(other.pos.y - unit.pos.y) == 1) {
				targets.push_back(i);
			}
		}
		
		if (targets.empty()) {
			return {};
		}
		
		const auto selectedTarget = std::min_element(targets.begin(), targets.end(), [&](const int indexA, const int indexB){
			const auto hpA = units[indexA].hp;
			const auto hpB = units[indexB].hp;
			if (hpA == hpB) {
				return indexA < indexB;
			} else {
				return hpA < hpB;
			}
		});
		
		return *selectedTarget;
	}
	
	void MoveUnit(Unit& unit) {
		const auto& distanceMap = GetDistanceMap(unit);
		
		std::vector<std::pair<SVector2i, int>> validTargets;
		for (const auto& otherUnit : units) {
			if (otherUnit.unitType == unit.unitType) {
				continue;
			}
			
			const auto neighbors = GetNeighborCoords(otherUnit.pos);
			for (const auto& neighbor : neighbors) {
				if (!IsOpenGround(neighbor)) {
					continue;
				}
			
				const auto distance = distanceMap[CoordToIndex(neighbor)];
				if (distance == -1) {
					continue;
				}
				validTargets.push_back(std::make_pair(neighbor, distance));
			}
		}
		if (validTargets.empty()) {
			return;
		}
		
		std::sort(validTargets.begin(), validTargets.end(), [](const std::pair<SVector2i, int>& a, const std::pair<SVector2i, int>& b){
			if (a.second == b.second) {
				return a.first < b.first;
			}
			return a.second < b.second;
		});
		
		const auto& selectedTarget = validTargets.front();
		SVector2i targetPos = selectedTarget.first;
		while (distanceMap[CoordToIndex(targetPos)] > 1) {
			auto neighbors = GetNeighborCoords(targetPos);
			
			const auto targetIter = std::min_element(neighbors.begin(), neighbors.end(), [&](const SVector2i& a, const SVector2i& b){
				const auto distanceA = distanceMap[CoordToIndex(a)];
				const auto distanceB = distanceMap[CoordToIndex(b)];
				if (distanceA == distanceB) {
					return a < b;
				} else {
					if (distanceA == -1) {
						return false;
					} else if (distanceB == -1) {
						return true;
					}
					return distanceA < distanceB;
				}
			});
			targetPos = *targetIter;
		}
		
		
		unit.pos = targetPos;
	}
	
	
	void UpdateUnit(Unit& unit) {
		if (!TryAttack(unit)) {
			MoveUnit(unit);
			TryAttack(unit);
		}
	}
	
	bool TryAttack(Unit& unit) {
		const auto targetIndex = GetTargetUnitIndex(unit);
		if (!targetIndex.has_value()) {
			return false;
		}
		
		Attack(unit, units[targetIndex.value()]);
		return true;
	}
	
	void Attack(Unit& source, Unit& target) {
		target.hp -= source.attackPower;
	}
	
	bool Tick() {
		std::sort(units.begin(), units.end(), [&](const Unit& a, const Unit& b){
			return CoordToIndex(a.pos) < CoordToIndex(b.pos);
		});
		
		for (int i=0; i<units.size(); ++i) {
			auto& unit = units[i];
			UpdateUnit(unit);
			
			if (GetNumUnitsLeft(Unit::UnitType::Elf) == 0||
				GetNumUnitsLeft(Unit::UnitType::Guard)== 0) {
				return false;
			}
			
			for (int n=0; n<units.size(); ++n) {
				if (units[n].hp <= 0) {
					units.erase(units.begin() + n);
					if (n < i) {
						i--;
					}
					break;
				}
			}
		}
		
		return true;
	}
	
	SVector2i IndexToCoord(std::size_t index) const {
		return { static_cast<int>(index % width), static_cast<int>(index / width) };
	}
	
	std::size_t CoordToIndex(const SVector2i& coord) const {
		return coord.y * width + coord.x;
	}
	
	int GetNumUnitsLeft(const Unit::UnitType unitType) const {
		return std::reduce(units.begin(), units.end(), 0, [unitType](const int acc, const Unit& unit){
			if (unit.unitType == unitType) {
				return acc + 1;
			} else {
				return acc;
			}
		});
	}

	
	const int width;
	const int height;
	std::vector<Cell> grid;
	std::vector<Unit> units;
};

struct BattleResult {
	int battleOutcome;
	Unit::UnitType winner;
	Board board;
};

BattleResult SimulateBattle(Board board) {
	int numRounds = 0;
	while (true) {
		if (!board.Tick()) {
			break;
		}
		
		numRounds++;
	}
	
	const auto hpLeft = std::reduce(board.units.begin(), board.units.end(), 0, [](const int acc, const Unit& unit){
		return acc + unit.hp;
	});
	
	const auto winner = board.units.front().unitType;
	
	std::cout << "num rounds: " << numRounds << std::endl;
	std::cout << "hp left: " << hpLeft << std::endl;
	std::cout << "winner: " << (winner == Unit::Elf ? "Elf" : "Guard") << std::endl;
	
	return {numRounds * hpLeft, winner, board};
}

Board InitBoard(int elfAttackPower = 3) {
	auto inputLines = utils::getInputLines("inputs/day15.txt");
	
	const int width = inputLines.front().size();
	const int height = inputLines.size();
	Board board{width, height};
	board.grid.reserve(width*height);
	
	for (std::size_t y=0; y<inputLines.size(); ++y) {
		const auto& line = inputLines[y];
		for (std::size_t x=0; x<line.size(); ++x) {
			const auto& token = line[x];
			if (token == '#') {
				board.grid.push_back(Cell{Cell::CellType::Wall});
			} else {
				board.grid.push_back(Cell{Cell::CellType::Ground});
			}
			
			if (token == 'G'){
				board.units.push_back(Unit{Unit::UnitType::Guard, SVector2i{(int)x,(int)y}});
			} else if (token == 'E'){
				board.units.push_back(Unit{Unit::UnitType::Elf, SVector2i{(int)x,(int)y}, elfAttackPower});
			}
		}
	}
	
	return board;
}

int main(int argc, char* argv[]) {
	{
		// Part 1
		const auto battleResult = SimulateBattle(InitBoard());
		std::cout << "Part 1: " << battleResult.battleOutcome << std::endl;
	}

	{
		// Part 2
		const auto battleResult = [](){
			int attackPower = 4;
			while (true) {
				const auto initialBoard = InitBoard(attackPower);
				const auto battleResult = SimulateBattle(initialBoard);
				
				const auto numElvesBefore = initialBoard.GetNumUnitsLeft(Unit::UnitType::Elf);
				const auto numElvesAfter = battleResult.board.GetNumUnitsLeft(Unit::UnitType::Elf);
				
				if (numElvesBefore == numElvesAfter) {
					return battleResult;
				} else {
					std::cout << "Num elves: " << numElvesAfter << "/" << numElvesBefore << std::endl;
				}
				attackPower++;
				// slow but works
			}
		}();
		
		std::cout << "Part 2: " << battleResult.battleOutcome << std::endl;
	}
	
	return 0;
}
