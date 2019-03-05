#include "InputParser.h"

#include <fstream>
#include <iostream>
#include <functional>

namespace utils {

	void readStream(const char* fileName, std::function<void(std::ifstream& stream)> func) {
		std::ifstream inputStream(fileName);
		if (inputStream.is_open()) {
			func(inputStream);
			inputStream.close();
		} else {
			std::cout << "Error: file not found!" << std::endl;
		}
	}

	std::vector<int> getInputValues(const char* fileName) {
		std::vector<int> inputValues;
		readStream(fileName, [&inputValues](std::ifstream& inputStream){
			while (!inputStream.eof()) {
				int value;
				inputStream >> value;
				inputValues.push_back(value);
			}
		});
		
		return inputValues;
	}

	std::vector<std::string> getInputLines(const char* fileName) {
		std::vector<std::string> lines;
		readStream(fileName, [&lines](std::ifstream& inputStream){
			std::string line;
			while (std::getline(inputStream, line)) {
				lines.push_back(line);
			}
		});

		return lines;
	}

}