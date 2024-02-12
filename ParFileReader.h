#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>

// SEAM PARAMETER FILE TEMPLATE (REV 3.0)
// Format: The lines in this file specify options.  At least 11
//         non-comment lines are required as input.  Multiple entries
//         are delimited by commas and/or spaces.
// 
// LINE 1  Jobname (up to 50 characters) COL 50-> X
// Transmission loss through 1/8" aluminum panel
// 
// LINE 2  Print input files (0=No, 1=Yes) (four entries required)
// MATERIAL, SUBSYSTEM, JUNCTION, EXCITATION, IMPORT(opt)
//  1,1,1,1
// 
// LINE 3  Frequency and bandwidth definitions (five entries):
// F_LOWER, F_UPPER, INC_BW, ANALYS_BW, COUNT_BW
//     100.,    8000.,    0.1,         0.1
// 
// LINE 4  Transient solution definition (four entries):
// NSTEPS, NSTEP_ON, BETA, SAMPLE_RATE
//       1,        1,  0.6,      1.
// 
// LINE 5  Output units option (1=Linear, 2=DB)
// 2
// 
// LINE 6  Structural output quantity (2=Disp, 3=Vel, 4=Accel)
//         and reference
//         20 LOG (MOTION/REF) if dB, or (MOTION/REF) if linear
// 4, 3.94e-4
// 
// LINE 7  Acoustic output quantity (1=Press, 2=Disp, 3=Vel, 4=Accel) and
//         reference
// 1, 2.90e-9
// 
// LINE 8  Print coupling matrix   {W N(W) ETA}  (0=No, 1=Yes)
// 0
// 
// LINE 9  Print power flows (0=No, 1=Yes)
// 0
// 
// LINE 10  Subsystem printing
//          (1=None, 2=All, 3=Selected subsystems)
// 
// Note: The subsystem response table is printed if all (2) or selected
//       subsystems (3) is specified.  If selected subsystem printing (3)
//       is specified, then a list of selected subsystem numbers should
//       follow line 10, one number per line, terminated with a zero.
// 2
// 
// LINE 11  Derived quantity printing
//          (1=None, 2=All, 3=Selected junctions)
// 
// Note: The derived subsystem, junction, and excitation quantity tables
//       are printed if all (2) or selected junctions (3) is specified.
//       If selected junction printing is requested (3), then a list of
//       selected junction numbers should follow line 11, one number per
//       line, terminated with a zero.
// 1

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