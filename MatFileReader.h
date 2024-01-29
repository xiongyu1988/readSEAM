#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class SubsystemMaterial {
public:
	std::string subsystemId;
	std::string type; // Material type
	std::vector<double> properties;

	SubsystemMaterial(std::string id, std::string tp)
		: subsystemId(std::move(id)), type(std::move(tp)) {}
};

class MatFileReader {
private:
	std::map<std::string, SubsystemMaterial> materials;

public:


	void readMatFile(const std::string& filename) {
		std::ifstream file(filename);
		std::string line;

		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filename << std::endl;
			return;
		}

		std::string currentSubsystemId;
		std::string currentType;
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
				iss >> currentSubsystemId >> currentType;
				materials.insert({ currentSubsystemId, SubsystemMaterial(currentSubsystemId, currentType) });
				lineCount = 2;
				continue;
			}
			if (!line.empty() && line[0] != '(' && line[0] != '!' && lineCount == 2) {
				// Read properties
				double prop;
				while (iss >> prop) {
					auto it = materials.find(currentSubsystemId);
					if (it != materials.end()) {
						it->second.properties.push_back(prop);
					}
				}
				lineCount = 1;
				continue;
			}
		}

		file.close();
	}

	void displayMaterials() {
		for (const auto& pair : materials) {
			const SubsystemMaterial& mat = pair.second;
			std::cout << "Subsystem ID: " << mat.subsystemId << std::endl;
			std::cout << "Type: " << mat.type << std::endl;
			std::cout << "Properties: ";
			for (double prop : mat.properties) {
				std::cout << prop << " ";
			}
			std::cout << std::endl << std::endl;
		}
	}
};
