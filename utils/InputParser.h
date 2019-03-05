#pragma once

#include <vector>
#include <string>

namespace utils {
	std::vector<int> getInputValues(const char* fileName);
	std::vector<std::string> getInputLines(const char* fileName);
}
