#include "InputParser.h"

#include <vector>
#include <iostream>
#include <numeric>
#include <list>
#include <array>

std::list<int>::const_iterator GetIterator(const std::list<int>& marbles, std::list<int>::const_iterator currentIter, const int offset) {
	auto iter{currentIter};
	if (offset > 0) {
		for (int i=0; i<offset; ++i) {
			iter = iter == marbles.cend() ? marbles.cbegin() : iter;
			iter++;
		}
	} else {
		for (int i=0; i<-offset; ++i) {
			iter = iter == marbles.cbegin() ? marbles.cend() : iter;
			iter--;
		}
	}
	
	return iter;
}

unsigned int CalcBestScore(const int numPlayers, const int finalMarbleScore) {
	std::vector<int> playerScores(numPlayers);
	std::list<int> marbles{0};
	
	auto currentIterator = marbles.cbegin();
	for (int marble=1; marble<=finalMarbleScore; ++marble) {
		const auto currentPlayerIndex = marble % numPlayers;
		if (marble % 23 == 0) {
			const auto iter = GetIterator(marbles, currentIterator, -7);
			playerScores[currentPlayerIndex] += marble + *iter;
			currentIterator = marbles.erase(iter);
		} else {
			const auto iter = GetIterator(marbles, currentIterator, 2);
			currentIterator = marbles.insert(iter, marble);
		}
	}
	
	const auto bestScore = std::max_element(playerScores.begin(), playerScores.end());
	return *bestScore;
}

int main(int argc, char* argv[]) {
	const auto inputString = utils::getInputLines("inputs/day09.txt").front();
	
	int numPlayers;
	int finalMarbleScore;
	std::sscanf(inputString.c_str(), "%d players; last marble is worth %d points", &numPlayers, &finalMarbleScore);
	
	const auto bestScore = CalcBestScore(numPlayers, finalMarbleScore);
	std::cout << "Part 1: " << bestScore << std::endl;
	
	const auto bestScorePart2 = CalcBestScore(numPlayers, finalMarbleScore*100);
	std::cout << "Part 2: " << bestScorePart2 << std::endl;
	
	return 0;
}

