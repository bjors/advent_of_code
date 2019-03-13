#include "InputParser.h"

#include <vector>
#include <iostream>
#include <numeric>

struct SNode {
	char mId;
	std::vector<SNode> mChildren;
	std::vector<int> mMetaData;
};



SNode ParseData(std::vector<int>::const_iterator& iter, char& nextId) {
	SNode node;
	node.mId = nextId++;
	node.mChildren.reserve(*iter++);
	node.mMetaData.reserve(*iter++);

	for (std::size_t i=0; i<node.mChildren.capacity(); ++i) {
		node.mChildren.push_back(ParseData(iter, nextId));
	}
	
	for (std::size_t i=0; i<node.mMetaData.capacity(); ++i) {
		node.mMetaData.push_back(*iter++);
	}
	
	return node;
}

SNode ParseTree(const std::vector<int>& inputValues) {
	char nextId = 'A';
	auto iter = inputValues.begin();
	const auto rootNode = ParseData(iter, nextId);
	return rootNode;
}

int AccumulatePart1(const SNode& node) {
	return std::reduce(node.mChildren.begin(), node.mChildren.end(), 0, [](const int acc, const SNode& child){
		return acc + AccumulatePart1(child);
	}) + std::reduce(node.mMetaData.begin(), node.mMetaData.end());
}

int AccumulatePart2(const SNode& node) {
	if (node.mChildren.empty()) {
		return std::reduce(node.mMetaData.begin(), node.mMetaData.end());
	} else {
		return std::reduce(node.mMetaData.begin(), node.mMetaData.end(), 0, [&node](const int acc, const int indexBase1){
			const bool childExists = indexBase1 >= 1 && indexBase1 <= node.mChildren.size();
			return childExists ? acc + AccumulatePart2(node.mChildren[indexBase1-1]) : acc;
		});
	}
}

int main(int argc, char* argv[]) {
	const auto inputValues = utils::getInputValues("inputs/day08.txt");
	
	const auto rootNode = ParseTree(inputValues);
	
	std::cout << "Part 1: " << AccumulatePart1(rootNode) << std::endl;
	std::cout << "Part 2: " << AccumulatePart2(rootNode) << std::endl;
	
	return 0;
}

