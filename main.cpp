
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>

#include "DirFileReader.h"
#include "ParFileReader.h"
#include "MatFileReader.h"
#include "SubFileReader.h"
#include "ExcFileReader.h"
#include "JunFileReader.h"

std::string UNIFIL, FNCFIL;

int main()
{
	DirFileReader dirReader;
	dirReader.setFileFolder("C:/Projects/readInputFiles_SEAM/readSEAM/seamInputFiles/");

	dirReader.readDirFile(dirReader.getFileFolder() + "seam.in");
	dirReader.displayFilePaths();

	std::cout << "=========================================================\n";
	std::cout << std::endl;

	ParFileReader parReader;
	parReader.readFromFile(dirReader.getFileFolder() + "seam.par");
	parReader.displayParameters();

	std::cout << "=========================================================\n";
	std::cout << std::endl;

	MatFileReader matReader;
	matReader.readMatFile(dirReader.getFileFolder() + "seam.mat");
	matReader.displayMaterials();

	std::cout << "=========================================================\n";
	std::cout << std::endl;

	SubFileReader subReader;
	subReader.readSubFile(dirReader.getFileFolder() + "seam.sub");
	subReader.displaySubsystems();

	std::cout << "=========================================================\n";
	std::cout << std::endl;

	ExcFileReader excReader;
	excReader.readExcFile(dirReader.getFileFolder() + "seam.exc");
	excReader.displayExcitations();

	std::cout << "=========================================================\n";
	std::cout << std::endl;

	JunFileReader junReader;
	junReader.readExcFile(dirReader.getFileFolder() + "seam.jun");
	junReader.displayJunctions();

	return 0;
}
