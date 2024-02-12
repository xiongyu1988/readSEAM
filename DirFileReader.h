#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

class DirFileReader {
public:
    // Constructor
    DirFileReader() = default;

    void setFileFolder(const std::string& path) {
        fileFolder = path;
    }

    const std::string& getFileFolder() const {
        return fileFolder;
    }

    void setMATFIL(const std::string& path) { MATFIL = path; }
    void setSUBFIL(const std::string& path) { SUBFIL = path; }
    void setJNCFIL(const std::string& path) { JNCFIL = path; }
    void setEXCFIL(const std::string& path) { EXCFIL = path; }
    void setPARFIL(const std::string& path) { PARFIL = path; }

    const std::string& getMATFIL() const { return MATFIL; }
    const std::string& getSUBFIL() const { return SUBFIL; }
    const std::string& getJNCFIL() const { return JNCFIL; }
    const std::string& getEXCFIL() const { return EXCFIL; }
    const std::string& getPARFIL() const { return PARFIL; }

    void readDirFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Unable to open the input file: " << filename << std::endl;
            return;
        }
        std::string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] != 'C') { // Checks for an empty line or non-C starting lines
                continue;
            }

            std::string filename = extractFilename(line);
            std::string fullPath = getFileFolder() + filename;
            std::ifstream testFile(fullPath);

            if (!testFile) {
                std::cerr << "Error: File does not exist - " << line << std::endl;
                return;
            }

            if (line.find(".mat") != std::string::npos) {
                setMATFIL(line);
            }
            else if (line.find(".sub") != std::string::npos) {
                setSUBFIL(line);
            }
            else if (line.find(".jun") != std::string::npos) {
                setJNCFIL(line);
            }
            else if (line.find(".exc") != std::string::npos) {
                setEXCFIL(line);
            }
            else if (line.find(".par") != std::string::npos) {
                setPARFIL(line);
            }
            else {
                std::cerr << "Error: Unsupported file type - " << line << std::endl;
            }
        }

    }

    void displayFilePaths() {
        std::cout << "MAT file path: " << getMATFIL() << std::endl;
        std::cout << "SUB file path: " << getSUBFIL() << std::endl;
        std::cout << "JNC file path: " << getJNCFIL() << std::endl;
        std::cout << "EXC file path: " << getEXCFIL() << std::endl;
        std::cout << "PAR file path: " << getPARFIL() << std::endl;
    }

    std::string extractFilename(const std::string& fullPath) {
        size_t lastSlashPos = fullPath.find_last_of("/\\");
        if (lastSlashPos != std::string::npos) {
            return fullPath.substr(lastSlashPos + 1);
        }
        return fullPath;
    }

private:
    std::string fileFolder;
    std::string MATFIL;
    std::string SUBFIL;
    std::string JNCFIL;
    std::string EXCFIL;
    std::string PARFIL;

};
