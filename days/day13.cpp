#include "InputParser.h"
#include <iostream>
#include <assert.h>
#include <tuple>
#include <optional>

struct SVector2i {
	int x;
	int y;
	
	bool operator==(const SVector2i& other) const {
		return x == other.x && y == other.y;
	}
	
	void operator+=(const SVector2i& other) {
		x += other.x;
		y += other.y;
	}
};

struct SCart {
	SVector2i pos;
	SVector2i direction;
	int intersectionCount=0;
	
	void TurnLeft() {
		direction = SVector2i{direction.y, -direction.x};
	}
	
	void TurnRight() {
		direction = SVector2i{-direction.y, direction.x};
	}
	
	char ToChar() const {
		if (direction == SVector2i{-1,0}) {
			return '<';
		} else if (direction == SVector2i{1,0}) {
			return '>';
		} else if (direction == SVector2i{0,1}) {
			return 'v';
		} else if (direction == SVector2i{0,-1}) {
			return '^';
		} else {
			assert(false);
			return 'O';
		}
	}
};

std::tuple<std::vector<SCart>, std::vector<std::string>> ParseInput() {
	auto track = utils::getInputLines("inputs/day13.txt");
	std::vector<SCart> carts;
	for (std::size_t y=0; y<track.size(); ++y) {
		auto& line = track[y];
		for (std::size_t x=0; x<line.size(); ++x) {
			const auto coord = SVector2i{static_cast<int>(x), static_cast<int>(y)};
			
			auto& c = line[x];
			if (c == '>') {
				line[x] = '-';
				carts.push_back(SCart{coord, SVector2i{1,0}});
			} else if (c == '<') {
				line[x] = '-';
				carts.push_back(SCart{coord, SVector2i{-1,0}});
			} else if (c == '^') {
				line[x] = '|';
				carts.push_back(SCart{coord, SVector2i{0,-1}});
			} else if (c == 'v') {
				line[x] = '|';
				carts.push_back(SCart{coord, SVector2i{0,1}});
			}
		}
	}
	
	return {carts, track};
}

std::optional<SVector2i> Tick(std::vector<SCart>& carts, const std::vector<std::string>& track, const bool removeOnCollision = false) {
	const auto trackSizeX = track.front().size();
	std::sort(carts.begin(), carts.end(), [trackSizeX](const SCart& a, const SCart& b){
		const auto indexA = a.pos.y * trackSizeX + a.pos.x;
		const auto indexB = b.pos.y * trackSizeX + b.pos.x;
		return indexA < indexB;
	});
	
	std::vector<SVector2i> positionsToRemove;
	
	for (auto& cart : carts) {
		if (std::find(positionsToRemove.begin(), positionsToRemove.end(), cart.pos) != positionsToRemove.end()) {
			continue;
		}
		cart.pos += cart.direction;
		
		const auto collideIter = std::find_if(carts.begin(), carts.end(), [&cart](const auto& otherCart){
			if (&otherCart == &cart) {
				return false;
			}
			return (otherCart.pos == cart.pos);
		});
		if (collideIter != carts.end()) {
			const auto& collisionPos = cart.pos;
			if (removeOnCollision) {
				positionsToRemove.push_back(collisionPos);
				continue;
			} else {
				return collisionPos;
			}
		}
		
		if (track[cart.pos.y][cart.pos.x] == '\\') {
			if (cart.direction.y == 0) {
				cart.TurnRight();
			} else {
				cart.TurnLeft();
			}
		} else if (track[cart.pos.y][cart.pos.x] == '/') {
			if (cart.direction.y == 0) {
				cart.TurnLeft();
			} else {
				cart.TurnRight();
			}
		} else if (track[cart.pos.y][cart.pos.x] == '+') {
			const auto decision = cart.intersectionCount % 3;
			if (decision  == 0) {
				cart.TurnLeft();
			} else if (decision == 2) {
				cart.TurnRight();
			}
			cart.intersectionCount++;
		}
	}
	
	if (removeOnCollision) {
		for (const auto& collisionPos : positionsToRemove) {
			carts.erase(std::remove_if(carts.begin(), carts.end(), [collisionPos](const auto& cart){
				return cart.pos == collisionPos;
			}), carts.end());
		}
	}
	
	return {};
}

int main(int argc, char* argv[]) {
	const auto [initialCarts, track] = ParseInput();
	
	{
		// Part 1
		auto carts = initialCarts;
		while (true) {
			const auto collision = Tick(carts, track);
			if (collision.has_value()) {
				std::cout << "Part 1: " << collision.value().x << "," << collision.value().y << std::endl;
				break;
			}
		}
	}
	
	{
		// Part 2
		auto carts = initialCarts;
		while (true) {
			const bool removeOnCollision = true;
			Tick(carts, track, removeOnCollision);
			if (carts.size() == 1) {
				std::cout << "Part 2: " << carts.front().pos.x << "," << carts.front().pos.y << std::endl;
				break;
			}
		}
	}
	
	
	/*
	
	*/
	
	//const auto [initialState, formulas] = ParseInput();
	
	{
	//	const auto sum = CalcSum(20, initialState, formulas);
	//	std::cout << "Part 1: " << sum << std::endl;
	}
	
	{
	//	std::cout << "Part 2: " << sum << std::endl;
	}
	
	return 0;
}

