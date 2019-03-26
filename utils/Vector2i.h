#pragma once

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
	
	bool operator<(const SVector2i& other) const {
		return (x+y*10000) < (other.x+other.y*10000);
	}
	
	SVector2i operator-(const SVector2i& other) const {
		return SVector2i{x-other.x, y-other.y};
	}
	SVector2i operator+(const SVector2i& other) const {
		return SVector2i{x+other.x, y+other.y};
	}
};
