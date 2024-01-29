#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>

class ParFileReader {
private:
    std::multimap<std::string, std::vector<std::string>> data;
    std::vector<std::string> paramNames;

    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first) {
            return str;
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

public:
    ParFileReader() {
        // Initialize parameter names here
        paramNames = {
            "Jobname", "Print Input Files", "Frequency and Bandwidth Definitions",
            "Transient Solution Definition", "Output Units Option", "Structural Output Quantity",
            "Acoustic Output Quantity", "Print Coupling Matrix", "Print Power Flows",
            "Subsystem Printing", "Derived Quantity Printing"
        };
    }

    void readFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return; // Early return on failure to open file
        }

        std::string line;
        size_t lineNumber = 0;
        while (std::getline(file, line)) {
            // Skip empty lines or comments
            if (line.empty() || line[0] == '!') continue;

            // Split line data and trim each token
            std::vector<std::string> lineData = split(trim(line), ',');
            for (auto& token : lineData) {
                token = trim(token); // Trim each token
            }

            // Assign data to corresponding parameter name or to an additional parameter
            std::string key = (lineNumber < paramNames.size()) ? paramNames[lineNumber] : "Additional Parameter";
            data.insert({ key, lineData });

            lineNumber++;
        }
    }

    void displayParameters() const {
        for (const auto& pair : data) {
            std::cout << pair.first << ": ";
            for (const auto& value : pair.second) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    }

    const std::multimap<std::string, std::vector<std::string>>& getData() const {
        return data;
    }
};