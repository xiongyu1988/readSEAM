#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

// SEAM JUNCTION FILE TEMPLATE (REV 3.0)
// 
// FIELD NAME                DESCRIPTION
// 
//   SUB#:     Subsystem identification number
//             or for jnc_type 41 = -1 means spring 
//                                = -2 means mass   
//                                = -3 means resistance
//                                = -4 means acoustic layer
//                                = -5 means bending plate
//             
//   EC:       Edge code (optional, * indicates default)
//             Masses and springs:    -2=Mass,   -1=Spring
//             Transmission Coefficient:  -6       (only for jnc_type45)
//             Structural subsystems: 1=Middle, *2=End,  3=Corner
//             Acoustical subsystems: 1=Middle, *2=Wall, 3=Edge, 4=Corner
//             Layers:  layer mat # 
//             A second digit point correction specifier can be used with
//             subsystems: 0=No point corrections
//                        *1=Impedance and coupling factor point corrections
//                         Blank=Coupling factor point corrections
//   CONSTANT: Mass, spring, or optional subsystem constant, layer's thickness
//   JNC#:     Junction number (optional)
//             Any order, -1 < JNC# < 100000
//   TYPE:     Junction type
//             11=Point translation, 12=Point rotation, 13=Point pressure
//             21=Line translation,  22=Line rotation,  23=Line pressure
//             31=Arc translation,   32=Arc rotation,   33=Arc pressure
//             4=Area,               41=Area w/Layers,  45=Area w/trans_coef
//   DIM:      Junction dimension
//             Number of points for point junctions, length for line and
//             arc junctions, area for area junctions
// 
// Example of junction input (Note that the example is offset to the
// right by one character):
// 
//// SUB#   EC   CONSTANT                  COMMENTS
//// JNC#  TYPE    DIM           (START THIS LINE WITH AN "=")
////-----X-----X---------X
//    43    21            First subsystem, end connection
//     7    11            Second subsystem, middle connection
//     0    -1  3.89E+08  Spring, specified as spring_constant/length
//    44    21            Third subsystem isolated by spring, end connection
//    45    21            Fourth subsystem, end connection
//     0    -2     0.083  Mass, specified as mass/length
//=   12    21       2.6  Junction #12, line translation, 2.6 units long
// // SUB#   EC   CONSTANT                  COMMENTS
// JNC#  TYPE    DIM           (START THIS LINE WITH AN "=")
//-----X-----X---------X
// SUB#   EC   CONSTANT                  COMMENTS
// JNC#  TYPE    DIM           (START THIS LINE WITH AN "=")
//-----X-----X---------X

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