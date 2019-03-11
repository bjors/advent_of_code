#include "InputParser.h"

#include <iostream>
#include <cstdio>
#include <vector>
#include <memory>
#include <unordered_map>
#include <set>
#include <sstream>
#include <array>
#include <numeric>
#include <optional>

using DependencyGraph = std::unordered_map<char, std::set<char>>;

DependencyGraph ParseDependencyGraph(const std::vector<std::string>& inputLines) {
	DependencyGraph graph;
	for (const auto& line : inputLines) {
		char dependency;
		char step;
		std::sscanf(line.c_str(), "Step %c must be finished before step %c can begin.", &dependency, &step);
		graph[step].insert(dependency);
		graph[dependency];
	}
	return graph;
}

std::optional<char> FindNextNode(const DependencyGraph& graph, const std::set<char>& completedNodes, const std::set<char>& excludedNodes = {}) {
	std::vector<std::pair<char, std::set<char>>> availableNodes;
	std::copy_if(graph.begin(), graph.end(), std::back_inserter(availableNodes), [&](const std::pair<char, std::set<char>>& iter){
		const auto node = iter.first;
		if (completedNodes.find(node) != completedNodes.end()) {
			return false;
		}
		if (excludedNodes.find(node) != excludedNodes.end()) {
			return false;
		}
		
		const auto& deps = iter.second;
		const auto allDepsCompleted = std::all_of(deps.begin(), deps.end(), [&](const char& dependency){
			return completedNodes.find(dependency) != completedNodes.end();
		});
		
		return allDepsCompleted;
	});
	
	if (availableNodes.empty()) {
		return {};
	}
	
	std::sort(availableNodes.begin(), availableNodes.end());
	return availableNodes.front().first;
}

std::string CalculateCompletionOrder(const DependencyGraph& graph) {
	std::set<char> completedNodes;
	std::stringstream nodeOrder;
	while (graph.size() > completedNodes.size()) {
		const auto nextNode = FindNextNode(graph, completedNodes);
		completedNodes.insert(nextNode.value());
		nodeOrder << nextNode.value();
	}
	return nodeOrder.str();
}

struct SWorker {
	bool IsIdle() const {
		return mNode == -1;
	}
	char mNode = -1;
};

std::unordered_map<char, int> GetTimeToCompleteEachNode(const DependencyGraph& graph) {
	const int nodeBaseTime = 60;
	std::unordered_map<char, int> secondsLeftPerNode;
	for (const auto iter : graph) {
		const auto nodeId = iter.first;
		const int timeLeft = nodeBaseTime + 1 + nodeId - 'A';
		secondsLeftPerNode[nodeId] = timeLeft;
	}
	return secondsLeftPerNode;
}

constexpr int numWorkers = 5;
using Workers = std::array<SWorker, numWorkers>;

std::set<char> GetExcludedNodes(const Workers& workers) {
	std::set<char> excludedNodes;
	for (const auto& worker : workers) {
		if (!worker.IsIdle()) {
			excludedNodes.insert(worker.mNode);
		}
	}
	return excludedNodes;
}

int CalculateCompletionTimeMultipleWorkers(const DependencyGraph& graph) {
	auto secondsLeftPerNode = GetTimeToCompleteEachNode(graph);
	int secondsElapsed = 0;
	Workers workers;
	std::set<char> completedNodes;

	while (graph.size() > completedNodes.size()) {
		for (auto& worker : workers) {
			if (worker.IsIdle()) {
				continue;
			}
			
			auto& secondsLeft = secondsLeftPerNode[worker.mNode];
			if (secondsLeft > 0) {
				secondsLeft--;
			}
			
			if (secondsLeft == 0) {
				completedNodes.insert(worker.mNode);
				worker.mNode = -1;
			}
		}
		
		for (auto& worker : workers) {
			if (!worker.IsIdle()) {
				continue;
			}
			
			const auto nextNode = FindNextNode(graph, completedNodes, GetExcludedNodes(workers));
			if (!nextNode.has_value()) {
				break;
			}
		
			worker.mNode = nextNode.value();
		}
		
		secondsElapsed++;
	}

	return secondsElapsed - 1;
}

int main(int argc, char* argv[]) {
	const auto inputLines = utils::getInputLines("inputs/day07.txt");
	
	const auto graph = ParseDependencyGraph(inputLines);
	const auto completionOrder = CalculateCompletionOrder(graph);
	std::cout << "Part 1: " << completionOrder << std::endl;
	
	const auto completionTimeMultipleWorkers = CalculateCompletionTimeMultipleWorkers(graph);
	std::cout << "Part 2: " << completionTimeMultipleWorkers << std::endl;
	
	return 0;
}
