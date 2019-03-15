#include "InputParser.h"
#include <tuple>
#include <iostream>

struct SVector2i {
	int x;
	int y;
	
	SVector2i operator-(const SVector2i& other) const {
		return SVector2i{x-other.x, y-other.y};
	}
	
	SVector2i operator+(const SVector2i& other) const {
		return SVector2i{x+other.x, y+other.y};
	}
};

struct SPoint {
	SVector2i pos;
	SVector2i vel;
};

struct SBounds {
	SVector2i min;
	SVector2i max;
	
	SVector2i GetSize() const {
		return max - min;
	}
};

SBounds CalcBounds(const std::vector<SPoint>& points) {
	SBounds bounds;
	bounds.min.x = bounds.max.x = points.front().pos.x;
	bounds.min.y = bounds.max.y = points.front().pos.y;
	
	for (const auto& point : points) {
		bounds.min.x = std::min(bounds.min.x, point.pos.x);
		bounds.min.y = std::min(bounds.min.y, point.pos.y);
		
		bounds.max.x = std::max(bounds.max.x, point.pos.x);
		bounds.max.y = std::max(bounds.max.y, point.pos.y);
	}
	return bounds;
}


void DrawPoints(const std::vector<SPoint>& points) {
	const auto bounds = CalcBounds(points);
	const auto size = bounds.GetSize() + SVector2i{1,1};
	
	std::vector<std::string> lines(size.y);
	for (std::size_t y=0; y<lines.size(); ++y) {
		auto& line = lines[y];
		line.resize(size.x);
		std::fill(std::begin(line), std::end(line), '.');
	}
	
	for (const auto& point : points) {
		const auto pos = point.pos - bounds.min;
		lines[pos.y][pos.x] = '#';
	}
	
	for (const auto& line : lines) {
		std::cout <<  line << std::endl;
	}
}

std::tuple<std::vector<SPoint>, int> Simulate(std::vector<SPoint> points) {
	int numSeconds = 0;
	while (true) {
		const auto prevSize = CalcBounds(points).GetSize();
		const auto previousPoints = points;
		for (auto& point : points) {
			point.pos = point.pos + point.vel;
		}
		const auto newBounds = CalcBounds(points);
		const auto newSize = newBounds.GetSize();
		
		if (newSize.x > prevSize.x && newSize.y > prevSize.y) {
			return {previousPoints, numSeconds};
		}
		numSeconds++;
	}
}


int main(int argc, char* argv[]) {
	const auto inputLines = utils::getInputLines("inputs/day10.txt");
	
	std::vector<SPoint> points;
	for (const auto& line : inputLines) {
		SPoint point;
		std::sscanf(line.c_str(), "position=<%d, %d> velocity=<%d, %d>", &point.pos.x, &point.pos.y, &point.vel.x, &point.vel.y);
		points.push_back(point);
	}
	
	const auto [messagePoints, numSeconds] = Simulate(points);
	
	std::cout << "Part 1: " << std::endl;
	DrawPoints(messagePoints);

	std::cout << "Part 2: " << numSeconds << std::endl;
	
	return 0;
}
