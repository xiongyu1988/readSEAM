#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

// SEAM EXCITATION FILE TEMPLATE (REV 3.0)
//
// FIELD NAME                        DESCRIPTION
// 
//   EXTYP:    Excitation type
//             0=Power, 1=Force, 2=Displacement, 3=Velocity,
//             4=Acceleration , 5 = Modal power
//             For CASE# 4 and 5, EXTYP=1 is pressure for acoustic spaces
//   CASE#:    Location of excitation or average excitation level
//             1=Middle of subsystem, 2=Edge of subsystem, 3=Junction,
//             4=Average rms level,  5=Average dB level
//             Cases 1-3 are point inputs, 4 and 5 are reverberant levels
//   S#/J#:    Subsystem or junction number
//   CONSTANT: Power or rms force, displacement, velocity, or
//             acceleration at 1 Hz
//             For CASE# 5, CONSTANT = dB level referenced to structural
//             or acoustic reference quantity specified in parameter file
//   EXPONENT: Frequency exponent
//             Power or rms = CONSTANT * f(Hz)**EXPONENT
//   EX_BW:    Flag for excitation standard deviation bandwidth
//             0=Pure tone (narrow band), 1=Analysis bandwidth specified
//             in parameter file
//
// Example of excitation input:
//
//EXTYP CASE# S#/J#  CONSTANT  EXPONENT EX_BW        COMMENTS
//-----X-----X-----X---------X---------X-----X 
//    1     2    10       1.0        0.     0 Point force on sub 10, edge
//
//((excdata
//EXTYP CASE# S#/J#  CONSTANT  EXPONENT EX_BW        COMMENTS
//-----X-----X-----X---------X---------X-----X
//))

class SubsystemExcitation {
public:
	std::string excitationType;
	std::string locExcitation; 
	std::string subJuncNum;
	std::string type;
	std::string freqExp;
	std::string flag;
	std::string comment;

	SubsystemExcitation(std::string EXTYP, std::string CASE, std::string SJ, std::string CONSTANT, std::string EXPONENT, std::string EX_BW, std::string COMMENT)
		: excitationType(std::move(EXTYP)), locExcitation(std::move(CASE)), subJuncNum(std::move(SJ)), type(std::move(CONSTANT)),
		  freqExp(std::move(EXPONENT)), flag(std::move(EX_BW)), comment(std::move(COMMENT)) {}
};

class ExcFileReader {
private:
	std::map<std::string, SubsystemExcitation> materials;

public:


	void readExcFile(const std::string& filename) {
		std::ifstream file(filename);
		std::string line;

		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filename << std::endl;
			return;
		}

		std::string currentExcitationType;
		std::string currentLocExcitation;
		std::string currentSubJuncNum;
		std::string currentType;
		std::string currentFreqExp;
		std::string currentFlag;
		std::string currentComment;

		while (getline(file, line)) {
			std::istringstream iss(line);

			if (line[0] == '!') {
				continue;
			}
			if (line[0] == '(') {
				continue;
			}
			if (line[0] == ')') {
				continue;
			}

			if (!line.empty() && line[0] != '(' && line[0] != '!') {

				iss >> currentExcitationType >> currentLocExcitation >> currentSubJuncNum >> currentType >> currentFreqExp >> currentFlag >> currentComment;
				materials.insert({ "1", SubsystemExcitation(currentExcitationType, currentLocExcitation, currentSubJuncNum, currentType,
															currentFreqExp, currentFlag, currentComment) });
				continue;
			}
		}

		file.close();
	}

	void displayExcitations() {
		for (const auto& pair : materials) {
			const SubsystemExcitation& exc = pair.second;
			std::cout << "EXTYP: " << exc.excitationType << std::endl;
			std::cout << "CASE#: " << exc.locExcitation << std::endl;
			std::cout << "S#/J#: " << exc.subJuncNum << std::endl;
			std::cout << "CONSTANT: " << exc.type << std::endl;
			std::cout << "EXPONENT: " << exc.freqExp << std::endl;
			std::cout << "EX_BW: " << exc.flag << std::endl;
			std::cout << "COMMENT: " << exc.comment << std::endl;
			std::cout << std::endl << std::endl;
		}
	}
};

