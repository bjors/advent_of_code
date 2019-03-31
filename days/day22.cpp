#include "InputParser.h"
#include "Vector2i.h"
#include <array>
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <vector>

constexpr auto modulo = 20183;

/*
constexpr auto depth = 510;
constexpr auto targetX = 10;
constexpr auto targetY = 10;
 */

constexpr auto depth = 11991;
constexpr auto targetX = 6;
constexpr auto targetY = 797;

constexpr auto width = targetX*20; // <-- @todo: should calculate extra rows/columns lazily instead
constexpr auto height = targetY*2;


enum RegionType {
	Rocky,
	Wet,
	Narrow
};

struct Region {
	void SetGeologic(const long long geologic) {
		this->geologic = geologic;
		erosion = (geologic + depth) % modulo;
		type = static_cast<RegionType>(erosion % 3);
	}
	char ToChar() const {
		switch (type) {
			case RegionType::Rocky: return '.';
			case RegionType::Wet: return '=';
			case RegionType::Narrow: return '|';
		}
	}
	int GetRisk() const {
		return static_cast<int>(type);
	}
	long long geologic;
	long long erosion;
	RegionType type;
};

using Cave = std::array<Region, width*height>;

const auto getIndex = [](const int x, const int y) {
	return x + y*width;
};



enum Tool {
	Torch,
	ClimbingGear,
	Neither
};

std::vector<Tool> GetAllowedTools(const RegionType regionType) {
	switch (regionType) {
		case RegionType::Rocky: return {{Tool::Torch, Tool::ClimbingGear}};
		case RegionType::Wet: return {{Tool::ClimbingGear, Tool::Neither}};
		case RegionType::Narrow: return {{Tool::Torch, Tool::Neither}};
	}
}

bool IsAllowedTool(const RegionType regionType, const Tool tool) {
	const auto allowedTools = GetAllowedTools(regionType);
	return std::find(allowedTools.begin(), allowedTools.end(), tool) != allowedTools.end();
}

struct AStarNode {
	std::shared_ptr<AStarNode> cameFrom;
	SVector2i coord;
	Tool tool;
	float f;
	float g;
};

bool InBounds(const SVector2i& coord) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < width && coord.y < height;
}

std::shared_ptr<AStarNode> GetNode(const SVector2i& coord, const Tool& tool) {
	const auto key = std::make_pair(coord, tool);
	static std::map<decltype(key),std::shared_ptr<AStarNode>> map;
	auto iter = map.find(key);
	if (iter != map.end()) {
		return iter->second;
	} else if (InBounds(coord)) {
		auto node = std::make_shared<AStarNode>();
		node->coord = coord;
		node->tool = tool;
		node->f = -1;
		node->g = -1;
		map[key] = node;
		return node;
	} else {
		return {};
	}
}

std::vector<std::shared_ptr<AStarNode>> GetNeighbors(const Cave& cave, const AStarNode& node) {
	std::vector<std::shared_ptr<AStarNode>> neighbors;
	
	const std::array<SVector2i, 4> directions = {SVector2i{-1,0}, SVector2i{1,0}, SVector2i{0,-1}, SVector2i{0,1}};
	for (const auto& dir : directions) {
		const auto coord = node.coord + dir;
		if (!InBounds(coord)) {
			continue;
		}
		
		const auto caveIndex = getIndex(coord.x, coord.y);
		const auto& region = cave[caveIndex];
		
		if (!IsAllowedTool(region.type, node.tool)) {
			continue;
		}
			
		const auto neighbor = GetNode(coord, node.tool);
		neighbors.push_back(neighbor);
	}
	
	constexpr std::array<Tool, 3> allTools = {{Tool::ClimbingGear, Tool::Neither, Tool::Torch}};
	for (const auto tool : allTools) {
		if (tool == node.tool) {
			continue;
		}
		if (!IsAllowedTool(cave[getIndex(node.coord.x, node.coord.y)].type, tool)) {
			continue;
		}
		neighbors.push_back(GetNode(node.coord, tool));
	}
	
	return neighbors;
}

int Heuristic(const std::shared_ptr<AStarNode>& start, const std::shared_ptr<AStarNode>& goal) {
	return ((goal->tool == start->tool) ? 0 : 7) + std::abs(goal->coord.x-start->coord.x) + std::abs(goal->coord.y-start->coord.y);
}

int AStar(const Cave& cave) {
	const auto goalNode = GetNode(SVector2i{targetX,targetY}, Tool::Torch);
	const auto startNode = GetNode(SVector2i{0,0}, Tool::Torch);
	startNode->g = 0;
	startNode->f = Heuristic(startNode, goalNode);
	
	std::set<std::shared_ptr<AStarNode>> closedList;
	std::set<std::shared_ptr<AStarNode>> openList = {startNode};
	
	while (!openList.empty()) {
		const auto bestOpenNodeIter = std::min_element(openList.begin(), openList.end(), [](const std::shared_ptr<AStarNode>& nodeA, const std::shared_ptr<AStarNode>& nodeB){
			return nodeA->g < nodeB->g;
		});
		const auto current = *bestOpenNodeIter;
		openList.erase(bestOpenNodeIter);
		
		if (current == goalNode) {
			/*
			std::vector<AStarNode> path;
			auto node = current;
			while (node) {
				path.push_back(*node.get());
				node = node->cameFrom;
			}
			*/
			
			return current->g;
		}
		
		closedList.insert(current);
		
		const auto neighbors = GetNeighbors(cave, *current.get());
		for (const auto& neighbor : neighbors) {
			if (closedList.find(neighbor) != closedList.end()) {
				continue;
			}
			const int cost = (current->tool != neighbor->tool) ? 7 : 1;
			const int tentativeG = current->g + cost;
			const bool inOpenList = openList.find(neighbor) != openList.end();
			if (!inOpenList || tentativeG < neighbor->g) {
				if (inOpenList) {
					openList.erase(neighbor);
				}
				
				neighbor->cameFrom = current;
				neighbor->g = tentativeG;
				neighbor->f = tentativeG + Heuristic(neighbor, goalNode);
				openList.insert(neighbor);
			}
		}
	}
	
	return -1;
}

int main(int argc, char* argv[]) {
	Cave cave;

	int totalRisk = 0;
	for (int y=0; y<height; ++y) {
		for (int x=0; x<width; ++x) {
			const auto index = getIndex(x,y);
			auto& region = cave[index];
			
			if (x == targetX && y == targetY) {
				region.SetGeologic(0);
			} else if (y == 0) {
				region.SetGeologic(x*16807);
			} else if (x == 0) {
				region.SetGeologic(y*48271);
			} else {
				const auto& r0 = cave[getIndex(x-1, y)];
				const auto& r1 = cave[getIndex(x, y-1)];
				region.SetGeologic(r0.erosion * r1.erosion);
			}
			
			if (x <= targetX && y <= targetY) {
				totalRisk += region.GetRisk();
			}
		}
	}
	
	std::cout << "Part 1: " << totalRisk << std::endl; // 5622
	
	const auto totalCost = AStar(cave);
	std::cout << "Part 2: " << totalCost << std::endl; // 1089
	
	return 0;
}
