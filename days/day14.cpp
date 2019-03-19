#include <iostream>
#include <vector>
#include <array>
#include <sstream>

void CreateNewRecipes(std::vector<int>& board, std::array<int, 2>& elfs) {
	const int newScore = board[elfs[0]] + board[elfs[1]];
	if (newScore < 10) {
		board.push_back(newScore);
	} else {
		board.push_back(1);
		board.push_back(newScore-10);
	}
	
	for (auto& elf : elfs) {
		elf = (elf + (board[elf] + 1)) % board.size();
	}
}

void Draw(const std::vector<int>& board, const std::array<int, 2>& elfs) {
	for (std::size_t i=0; i<board.size(); ++i) {
		const auto& score = board[i];
		if (i == elfs[0]) {
			std::cout << "(" << score << ")\t";
		} else if (i == elfs[1]) {
			std::cout << "[" << score << "]\t";
		} else {
			std::cout << " " << score << " \t";
		}
		
	}
	std::cout << std::endl;
}

std::tuple<std::vector<int>, std::array<int, 2>> Setup() {
	std::vector<int> board;
	board.push_back(3);
	board.push_back(7);
	
	std::array<int, 2> elfs = {{0,1}};
	
	return {board, elfs};
}

int FindMatch(const int matchToFind) {
	auto [board, elfs] = Setup();
	int nextIndexToTest = 0;
	
	std::vector<int> matchVector = [](int matchToFind){
		std::vector<int> matchVector;
		while (matchToFind > 0) {
			matchVector.insert(matchVector.begin(), matchToFind % 10);
			matchToFind /= 10;
		}
		return matchVector;
	}(matchToFind);
	const auto matchSize = matchVector.size();
	while (true) {
		CreateNewRecipes(board, elfs);
		if (board.size() < matchSize) {
			continue;
		}
		
		while (nextIndexToTest+5 < board.size()) {
			const bool found = [&, board = std::ref(board)](){
				for (int i=0; i<matchSize; ++i) {
					if (board.get()[nextIndexToTest+i] != matchVector[i]) {
						return false;
					}
				}
				return true;
			}();
			
			if (found) {
				return nextIndexToTest;
			}
			
			nextIndexToTest++;
		}
	}
	
	return -1;
}

int main(int argc, char* argv[]) {
	{
		// Part 1
		auto [board, elfs] = Setup();
		
		const int numRecipes = 894501;
		while (board.size() < numRecipes+10) {
			CreateNewRecipes(board, elfs);
		}
		
		std::stringstream result;
		for (int i=numRecipes; i<numRecipes+10; ++i) {
			result << board[i];
		}
		
		std::cout << "Part 1: " << result.str() << std::endl;
	}
	
	{
		// Part 2
		const auto numRecipes = FindMatch(894501);
		std::cout << "Part 2: " << numRecipes << std::endl;
	}
	
	return 0;
}
