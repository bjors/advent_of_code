#include <iostream>
#include "InputParser.h"
#include <string>
#include <sstream>
#include <array>
#include <optional>

constexpr int fabricSize = 1000;
using Fabric = std::array<int, fabricSize*fabricSize>;

struct SClaim {
	int id;
	int x;
	int y;
	int width;
	int height;
};

std::istream& operator>>(std::istream& in, SClaim& claim)
{
	char dummyChar;
	SClaim parsedValues;
	in >> dummyChar >> parsedValues.id;
	in >> dummyChar >> parsedValues.x;
	in >> dummyChar >> parsedValues.y;
	in >> dummyChar >> parsedValues.width;
	in >> dummyChar >> parsedValues.height;

	claim = parsedValues;
    return in;
}

std::vector<SClaim> GetClaims(const std::vector<std::string>& inputStrings) {
	std::vector<SClaim> claims;
	claims.reserve(inputStrings.size());
	for (const auto& line : inputStrings) {
		std::stringstream lineStream(line);
		SClaim claim;
		lineStream >> claim;
		claims.push_back(claim);
	}
	return claims;
}

Fabric FillFabric(const std::vector<SClaim>& claims) {
	Fabric fabric;
	fabric.fill(0);

	for (const auto& claim : claims) {
		for (int x=claim.x; x<claim.x+claim.width; ++x) {
			for (int y=claim.y; y<claim.y+claim.height; ++y) {
				const int index = x+y*fabricSize;
				fabric[index]++;
			}			
		}
	}
	
	return fabric;
}

int CountOverlaps(const Fabric& fabric) {
	int numOverlaps = 0;
	for (const auto& val : fabric) {
		if (val >= 2) {
			numOverlaps++;
		}
	}
	return numOverlaps;
}

bool IsOverlapping(const Fabric& fabric, const SClaim& claim) {
	for (int x=claim.x; x<claim.x+claim.width;++x) {
		for (int y=claim.y; y<claim.y+claim.height; ++y) {
			const int index = x+y*fabricSize;
			if (fabric[index] != 1) {
				return true;
			}			
		}
	}
	return false;
}

std::optional<int> GetIdOfFirstClaimWithNoOverlap(const Fabric& fabric, const std::vector<SClaim>& claims) {
	for (const auto& claim : claims) {
		if (!IsOverlapping(fabric, claim)) {
			return claim.id;
		}
	}
	return {};
}

int main(int argc, char* argv[]) {
	const auto lines = utils::getInputLines("inputs/day03.txt");
	
	const auto claims = GetClaims(lines);
	const auto fabric = FillFabric(claims);
	const auto numOverlaps = CountOverlaps(fabric);
	std::cout << "Part 1: " << numOverlaps << " squares overlap" << std::endl;
		
	const auto idWithNoOverlap = GetIdOfFirstClaimWithNoOverlap(fabric, claims);
	std::cout << "Part 2: " << idWithNoOverlap.value() << std::endl;

	return 0;
}
