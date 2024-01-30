#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

struct Subsystem {
    int number;
    int edgeCode;
    double constant;
    std::string comment;
};

struct JunctionDetails {
    std::vector<Subsystem> subsystems;
    int type;
    double dimension;
    std::string comments;
};

class JunFileReader {
public:
    void readExcFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Skip comments and empty lines
            if (line.empty() || line[0] == '!') continue;

            // Parse junction data
            if (line[0] == '=') {
                parseJunctionDefinition(line);
            }
            else {
                parseSubsystem(line);
            }
        }

        file.close();
    }

    void displayJunctions() const {
        for (const auto& pair : junctions) {
            const auto& junctionNumber = pair.first;
            const auto& junction = pair.second;

            std::cout << "Junction Number: " << junctionNumber << std::endl;
            std::cout << "Type: " << junction.type << ", Dimension: " << junction.dimension << std::endl;
            std::cout << "Comments: " << junction.comments << std::endl;
            std::cout << "Subsystems:" << std::endl;

            for (const auto& subsystem : junction.subsystems) {
                std::cout << "  Subsystem Number: " << subsystem.number << std::endl;
                std::cout << "  Edge Code: " << subsystem.edgeCode << ", Constant: " << subsystem.constant << std::endl;
                std::cout << "  Comments: " << subsystem.comment << std::endl;
            }

            std::cout << std::endl; 
        }
    }

private:
    std::map<int, JunctionDetails> junctions;
    JunctionDetails currentJunction;

    void parseSubsystem(const std::string& line) {
        std::istringstream iss(line);
        Subsystem subsystem;
        iss >> subsystem.number >> subsystem.edgeCode;

        // Check if there is a constant before the comment
        if (iss.peek() != ' ' && iss.peek() != '\t') {
            iss >> subsystem.constant;
        }

        std::getline(iss >> std::ws, subsystem.comment); 
        currentJunction.subsystems.push_back(subsystem);
    }

    void parseJunctionDefinition(const std::string& line) {
        std::istringstream iss(line.substr(1)); // Skip '='
        int junctionNumber;
        iss >> junctionNumber >> currentJunction.type >> currentJunction.dimension;
        std::getline(iss, currentJunction.comments);
        junctions[junctionNumber] = currentJunction;
        currentJunction.subsystems.clear();
    }
};