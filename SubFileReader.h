#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class SubsystemInfo {
public:
	std::string subsystemId;
	std::string type; 
	std::string subsystemName;
	std::string subsystemMatID;
	std::vector<double> properties;

	SubsystemInfo(std::string id, std::string tp, std::string name, std::string matID)
		: subsystemId(std::move(id)), type(std::move(tp)), subsystemName(std::move(name)), subsystemMatID(std::move(matID)) {}
};

class SubFileReader {
private:
	std::map<std::string, SubsystemInfo> info;

public:


	void readSubFile(const std::string& filename) {
		std::ifstream file(filename);
		std::string line;

		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filename << std::endl;
			return;
		}

		std::string currentSubsystemId;
		std::string currentType;
		std::string currentName;
		std::string currentMatID;
		int lineCount = 1;
		while (getline(file, line)) {
			std::istringstream iss(line);

			if (line[0] == '!') {
				continue;
			}
			if (line[0] == '(') {
				continue;
			}
			if (line[0] == ')') {
				continue;
			}

			if (!line.empty() && line[0] != '(' && line[0] != '!' && lineCount == 1) {

				// Read subsystem ID and type
				iss >> currentSubsystemId >> currentType >> currentName >> currentMatID;
				info.insert({ currentSubsystemId, SubsystemInfo(currentSubsystemId, currentType, currentName, currentMatID) });
				lineCount = 2;
				continue;
			}
			if (!line.empty() && line[0] != '(' && line[0] != '!' && lineCount == 2) {
				// Read properties
				double prop;
				while (iss >> prop) {
					auto it = info.find(currentSubsystemId);
					if (it != info.end()) {
						it->second.properties.push_back(prop);
					}
				}
				lineCount = 1;
				continue;
			}
		}

		file.close();
	}

	void displaySubsystems() {
		for (const auto& pair : info) {
			const SubsystemInfo& inf = pair.second;
			std::cout << "Subsystem ID: " << inf.subsystemId << std::endl;
			std::cout << "Type: " << inf.type << std::endl;
			std::cout << "Subsystem Name: " << inf.subsystemName << std::endl;
			std::cout << "MAT#: " << inf.subsystemMatID << std::endl;
			std::cout << "Properties: ";
			for (double prop : inf.properties) {
				std::cout << prop << " ";
			}
			std::cout << std::endl << std::endl;
		}
	}
};

