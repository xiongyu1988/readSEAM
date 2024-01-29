
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>

#include "filemanager.h"
#include "ParFileReader.h"
#include "MatFileReader.h"
#include "SubFileReader.h"
#include "ExcFileReader.h"

std::string UNIFIL, FNCFIL;

int main()
{
	FileManager fileManager; // Instance of FileManager to store file path
	fileManager.setfileFolder("seamInputFiles\\");

	std::string fileName = "seam.in";
	const std::string FILEIN{ fileManager.getfileFolder() + fileName };

	std::ifstream file(FILEIN);

	if (file) {
		std::string line;
		while (getline(file, line)) {
			char cAddress = line[0];
			char cDrive = 'C';
			if (cAddress != cDrive) {
				continue;
			}

			if (line.find(".mat") != std::string::npos) {
				std::string filename = fileManager.extractFilename(line);
				std::ifstream testFile(fileManager.getfileFolder() + filename);
				if (testFile) {
					fileManager.setMATFIL(line);
				}
				else {
					if (line.find(".mat") != std::string::npos) {
						std::cerr << "Error: MAT file does not exist - " << line << std::endl;
					}
				}
			}
			else if (line.find(".sub") != std::string::npos) {
				std::string filename = fileManager.extractFilename(line);
				std::ifstream testFile(fileManager.getfileFolder() + filename);
				if (testFile) {
					fileManager.setSUBFIL(line);
				}
				else {
					if (line.find(".sub") != std::string::npos) {
						std::cerr << "Error: SUB file does not exist - " << line << std::endl;
					}
				}
			}
			else if (line.find(".jun") != std::string::npos) {
				std::string filename = fileManager.extractFilename(line);
				std::ifstream testFile(fileManager.getfileFolder() + filename);
				if (testFile) {
					fileManager.setJNCFIL(line);
				}
				else {
					if (line.find(".jun") != std::string::npos) {
						std::cerr << "Error: JUN file does not exist - " << line << std::endl;
					}
				}
			}
			else if (line.find(".exc") != std::string::npos) {
				std::string filename = fileManager.extractFilename(line);
				std::ifstream testFile(fileManager.getfileFolder() + filename);
				if (testFile) {
					fileManager.setEXCFIL(line);
				}
				else {
					if (line.find(".exc") != std::string::npos) {
						std::cerr << "Error: EXC file does not exist - " << line << std::endl;
					}
				}
			}
			else if (line.find(".par") != std::string::npos) {
				std::string filename = fileManager.extractFilename(line);
				std::ifstream testFile(fileManager.getfileFolder() + filename);
				if (testFile) {
					fileManager.setPARFIL(line);
				}
				else {
					if (line.find(".par") != std::string::npos) {
						std::cerr << "Error: PAR file does not exist - " << line << std::endl;
					}
				}
			}
			else {
				// If the file extension is not recognized, print a generic error message
				std::cerr << "Error: File does not exist or unsupported file type - " << line << std::endl;
			}
		}
	}
	else {
		std::cerr << "Unable to open the input file" << std::endl;
		return 1;
	}

	std::cout << "MAT file path: " << fileManager.getMATFIL() << std::endl;
	std::cout << "SUB file path: " << fileManager.getSUBFIL() << std::endl;
	std::cout << "JNC file path: " << fileManager.getJNCFIL() << std::endl;
	std::cout << "EXC file path: " << fileManager.getEXCFIL() << std::endl;
	std::cout << "PAR file path: " << fileManager.getPARFIL() << std::endl;

	std::cout << "=========================================================\n";
	std::cout << std::endl;

	ParFileReader parReader;
	std::string filename = fileManager.getfileFolder() + "seam.par";
	parReader.readFromFile(filename);
	parReader.displayParameters();

	std::cout << "=========================================================\n";
	std::cout << std::endl;

	MatFileReader matReader;
	matReader.readMatFile(fileManager.getfileFolder() + "seam.mat");  
	matReader.displayMaterials();

	std::cout << "=========================================================\n";
	std::cout << std::endl;

	SubFileReader subReader;
	subReader.readSubFile(fileManager.getfileFolder() + "seam.sub");
	subReader.displaySubsystems();

	std::cout << "=========================================================\n";
	std::cout << std::endl;

	ExcFileReader excReader;
	excReader.readExcFile(fileManager.getfileFolder() + "seam.exc");
	excReader.displayExcitations();

	//ISEAM();
	//GSEAM();
	//GSEAM INPUTS, PRINTS, AND STORES SUBSYSTEM, MATERIAL
	//MODEL DEFINITION, AND JUNCTION DATA FOR SEAM.

	//MATERIAL DATA

	return 0;
}
