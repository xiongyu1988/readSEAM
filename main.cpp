
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string MATFIL, SUBFIL, JNCFIL, EXCFIL, PARFIL, UNIFIL, FNCFIL;

const std::string FILEIN{ "seamInputFiles/seam.in" };

int main()
{
    std::ifstream file(FILEIN); 
    std::vector<std::vector<std::string>> filePaths(5); 

    if (file) {
        std::string line;
        while (getline(file, line)) {
            // Check and store .mat files
            if (line.find(".mat") != std::string::npos) {
                filePaths[0].push_back(line);
            }
            // Check and store .sub files
            else if (line.find(".sub") != std::string::npos) {
                filePaths[1].push_back(line);
            }
            // Check and store .jun files
            else if (line.find(".jun") != std::string::npos) {
                filePaths[2].push_back(line);
            }
            // Check and store .exc files
            else if (line.find(".exc") != std::string::npos) {
                filePaths[3].push_back(line);
            }
            // Check and store .par files
            else if (line.find(".par") != std::string::npos) {
                filePaths[4].push_back(line);
            }
        }
    }
    else {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    for (int i = 0; i < filePaths.size(); ++i) {
        for (const auto& path : filePaths[i]) {
            std::cout << path << std::endl;
        }
    }

    //ISEAM();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
