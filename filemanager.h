#include <string>


class FileManager {
public:
    void setfileFolder(const std::string& path) {
        fileFolder = path;
    }
    // Setter for MATFIL, SUBFIL, JNCFIL, EXCFIL, PARFIL
    void setMATFIL(const std::string& path) {
        MATFIL = path;
    }
    void setSUBFIL(const std::string& path) {
        SUBFIL = path;
    }
    void setJNCFIL(const std::string& path) {
        JNCFIL = path;
    }
    void setEXCFIL(const std::string& path) {
        EXCFIL = path;
    }
    void setPARFIL(const std::string& path) {
        PARFIL = path;
    }

    const std::string& getfileFolder() const {
        return fileFolder;
    }
    // Getter for MATFIL, SUBFIL, JNCFIL, EXCFIL, PARFIL
    const std::string& getMATFIL() const {
        return MATFIL;
    }
    const std::string& getSUBFIL() const {
        return SUBFIL;
    }
    const std::string& getJNCFIL() const {
        return JNCFIL;
    }
    const std::string& getEXCFIL() const {
        return EXCFIL;
    }
    const std::string& getPARFIL() const {
        return PARFIL;
    }

    static std::string extractFilename(const std::string& fullPath) {
        std::string directory = "seamInputFiles\\";
        size_t pos = fullPath.find(directory);
        if (pos != std::string::npos) {
            // Add the length of directory to the found position to start from the end of the directory name
            return fullPath.substr(pos + directory.length());
        }
        // Return empty string if the directory is not found in the path
        return "";
    }

private:
    std::string fileFolder;

    std::string MATFIL; // Member variable to store .mat file path
    std::string SUBFIL; // Member variable to store .sub file path
    std::string JNCFIL; // Member variable to store .jnc file path
    std::string EXCFIL; // Member variable to store .exc file path
    std::string PARFIL; // Member variable to store .par file path
};