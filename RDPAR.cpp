#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "filemanager.h"

// SUBROUTINE TO READ SEAM PARAMETER FILE AND SAVE VARIABLES IN COMMON.

//std::vector<std::string> split(const std::string& s, char delimiter) {
//    std::vector<std::string> tokens;
//    std::string token;
//    std::istringstream tokenStream(s);
//    while (std::getline(tokenStream, token, delimiter)) {
//        tokens.push_back(token);
//    }
//    return tokens;
//}
//
//std::string trim(const std::string& str) {
//    size_t first = str.find_first_not_of(' ');
//    if (std::string::npos == first) {
//        return str;
//    }
//    size_t last = str.find_last_not_of(' ');
//    return str.substr(first, (last - first + 1));
//}
//
//FileManager fileManager;
//
//
//std::string fileAddress = fileManager.getPARFIL();
//
//std::vector<std::vector<std::string>> readParFile(const std::string& filename) {
//    std::vector<std::vector<std::string>> data;
//    std::ifstream file(filename);
//    std::string line;
//    while (std::getline(file, line)) {
//        // Ignore comment lines
//        if (line.empty() || line[0] == '!') continue;
//
//        // Trim line and split it into tokens
//        std::vector<std::string> lineData = split(trim(line), ',');
//        data.push_back(lineData);
//    }
//    return data;
//}

// line 1 JOBNAME


// LINE 2 ECHO PRINT


// LINE 3 FREQUENCY AND BANDWIDTH

// LINE 4 TRANSIENT PARAMETERS

// LINE 5 OUTPUT UNITS

// LINE 6 STRUCTURAL MOTION QUANTITY AND REFERENCE


// LINE 7 ACOUSTIC SPACE QUANTITY AND REFERENCE

// LINE 8 COUPLING MATRIX PRINT FLAG AND COUPLING INFORMATION TO SET COUPLING MULTIPLICATION FACTOR (TAUMUL)

// LINE 9 POWER FLOW PRINT FLAG

// LINE 10 SELECTED SUBSYSTEM PRINT FLAG

// LINE 11 SELECTED JUNCTION PRINT FLAG

// LINE 12 (OPTIONAL) POWER, ENERGY, & INTENSITY REFERENCES

// LINE 13  dB RESPONSE TABLE ACCURACY, lin RESPONSE TABLE ACCURACY

// SUCCESSFUL READ OF PARAM FILE