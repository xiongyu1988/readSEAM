#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cctype> 
#include <cstdlib> 

// SEAM MATERIAL FILE TEMPLATE (REV 3.0)
//
// FIELD NAME          DESCRIPTION
//
//  MATL#:             Material definition number
//                     Any order, 0 < MATL# < 100000
//  NAME:              Material name (opt)
//                     Up to 19 characters can be used
//  TYPE:              Type of material
//
//__type_______________description______________________________________________
//
//  ISOELASTIC         linear, temperature-independent isotropic materials
//  GAS                gases
//  LIQUID             liquids and fluids
//  SOLIDWAVE          general isotropic materials with known longitudinal and
//                     shear wavespeeds
//  FIBER              porous materials used for acoustic absorption and noise
//                     control
//  FIBERZ             porous materials with known characteristic impedance
//                     and propagation constant
//____________________________________________________________________________
//
//  MP1:               First material parameter
//  MP2:               Second material parameter
//  MP3:               Third material parameter
//  MP4:               Forth material parameter
//  MP5:               Fifth material parameter
//  MP6:               Sixth material parameter
//
//     | ISOELASTIC | GAS | LIQUID | SOLIDWAVE    | FIBER      | FIBERZ
//_________________________________________________________________________
// MP1 | RHO        | RHO          | RHO          | RHO        | RHO      
// MP2 | E          | C            | C_LONG       | FIB_TYPE   | RE_Z     
// MP3 | G          | ETA          | C_SHEAR      | RHO_GAS    | -IM_Z 
// MP4 | NU         | ALPHA(opt)   | ETA          | C_GAS      | RE_B/OMEGA 
// MP5 | ETA        | DAMP_EXP(opt)| DAMP_EXP(opt)| R_FLOW(opt)| IM_B/OMEGA  
// MP6 | DAMP_EXP   | ABS_EXP(opt) |              | D(opt)     |
//
//__parameter__description_______________________________material types______
//
//  RHO:       Mass density                              all
//  C:         Speed of Sound                            GAS, LIQUID
//  C_LONG:    Longitudinal wave speed                   GAS, LIQUID,SOLIDWAVE
//  C_SHEAR:   Shear wave speed                          SOLIDWAVE
//  ETA:       Damping constant                          ISOELASTIC, GAS,
//                                                       LIQUID, SOLIDWAVE
//  DAMP EXP:  Damping Exponent                          ISOELASTIC, GAS,
//             Loss factor = ETA * f(Hz)**DAMP_EXP       LIQUID, SOLIDWAVE
//  PIVOT FRQ: Pivot frequency for frequency-dependent   SOLIDWAVE
//             damping (opt)
//  E:         Modulus of Elasticity (Young's Modulus)   ISOELASTIC
//  G:         Shear Modulus                             ISOELASTIC
//  NU:        Poisson's Ratio                           ISOELASTIC
//  ALFA:      Absorption Coefficient                    GAS, LIQUID
//  ABS EXP:   Absorption Exponent
//             Abs coefficient = ETA * f(Hz)**ABS_EXP    GAS, LIQUID
//  FIB TYPE:  Type of Fiber Material:                   FIBER
//             =1 for mineral wool
//             =2 for glass fiber
//  R_FLOW:    Flow Resistance                           FIBER
//  RHO_GAS:   Mass Density of the Gas within the        FIBER
//             fibrous material
//  D:         Fiber Diameter                            FIBER
//  RE_Z:      Real Part of the Characteristic Impedance FIBERZ
//  -IM_Z:     -1* the Imaginary Part of the             FIBERZ
//             Characteristic Impedance
//  RE_B/OMEGA:Real Part of the Propagation Constant     FIBERZ
//             divided by radian frequency
//  IM_B/OMEGA:Imaginary Part of the Propagation         FIBERZ
//             Constant divided by radian frequency
//_____________________________________________________________________________
//
// Note:
// - Units must be consistent
// - There are two lines for each subsystem record.
// - Data can be entered either as formatted records or in a free format with
//   comma "," delimiters between fields.
// - An exclamation mark (//) in column 1 indicates a comment line.
// - A blank line (first 60 characters blank) is taken as a comment line.
//
//
// Example of a formatted material input:
// mat#  mat_type            mat name (opt)     comments
//-----X-------------------X-------------------X
// 1011          ISOELASTIC               steel
//           MP1       MP2       MP3       MP4       MP5     MP6    comments
//XXXXXX---------X---------X---------X---------X---------X---------X
//        7.85e-6    2.07e8     8.0e7       0.3     #1061            panel_b
//(FREQVAL
// 1061         1         1
//             10     0.032
//           1000     0.032
//          10000      0.01
//)
//
// Example of a free format material input with comma delimiters:
// 1011, ISOELASTIC, steeL
//  7.85e-6,2.07e8,8.0e7, 0.3, #1061,, panel_b
//(FREQVAL
// 1061,1,1
//   10,  0.032
//   1000,0.032
//   10000,0.01
//
//
//
//
// the beginning of the material records
//((MATDATA
// mat#  mat_type            mat name (opt)     comments
//-----X-------------------X-------------------X
//           MP1       MP2       MP3       MP4       MP5     MP6    comments
//XXXXXX---------X---------X---------X---------X---------X---------X
//))


class SubsystemMaterial {
public:
	std::string subsystemId;
	std::string type;
	std::vector<double> properties;
	std::map<int, std::vector<std::pair<double, double>>> 	freqProperties;

	SubsystemMaterial(std::string id, std::string tp)
		: subsystemId(std::move(id)), type(std::move(tp)) {}
};

class MatFileReader {
private:
	std::map<std::string, SubsystemMaterial> materials;
	std::map<int, std::vector<std::pair<double, double>>> freqValTables;
	std::vector<std::pair<double, double>> 	freqValTables0;
public:

	bool isNumeric(const std::string& str) {
		char* end;
		std::strtod(str.c_str(), &end);
		return end != str.c_str() && *end == '\0';
	}

	void parseLine(const std::string& line) {
		std::istringstream iss(line);
		std::string field;
		std::vector<double> numbers;
		std::vector<std::string> tags;

		while (iss >> field) {
			if (isNumeric(field)) {
				numbers.push_back(std::strtod(field.c_str(), nullptr));
			}
			else {
				tags.push_back(field);
			}
		}
		std::cout << "Numbers: ";
		for (double num : numbers) {
			std::cout << num << " ";
		}
		std::cout << "\nTags: ";
		for (const std::string& tag : tags) {
			std::cout << tag << " ";
		}
		std::cout << std::endl;
	}

	void readMatFile(const std::string& filename) {
		std::ifstream file(filename);
		std::string line;

		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filename << std::endl;
			return;
		}

		bool inFreqValSection = false;
		int freqValIndex = 0;
		std::string currentSubsystemId;
		std::string currentType;
		int lineCount = 1;
		while (getline(file, line)) {
			std::istringstream iss(line);

			if (line[0] == '!') {
				continue;
			}
			if (line[0] == '(' && line[1] != 'F') {
				continue;
			}
			if (line[0] == ')') {
				continue;
			}

			if (!line.empty() && line[0] != '(' && line[0] != '!' && lineCount == 1) {

				iss >> currentSubsystemId >> currentType;
				materials.insert({ currentSubsystemId, SubsystemMaterial(currentSubsystemId, currentType) });
				lineCount = 2;
				continue;
			}
			std::vector<int> tempFreqValTableId;
			if (!line.empty() && line[0] != '(' && line[0] != '!' && lineCount == 2) {

				std::string field;
				std::vector<double> numbers;
				std::vector<std::pair<std::string, std::string>> tags;
				while (iss >> field) {
					auto it = materials.find(currentSubsystemId);
					if (isNumeric(field)) {
						it->second.properties.push_back(std::strtod(field.c_str(), nullptr));
					}
					else {
						tags.emplace_back(currentSubsystemId, field);
					}
				}
				lineCount = 1;
			}
			if (line.find("(FREQVAL") != std::string::npos) {
				inFreqValSection = true;
				std::string nextLine;
				getline(file, nextLine);
				std::istringstream issNextLine(nextLine);
				issNextLine >> std::ws;
				issNextLine >> freqValIndex;
				while (getline(file, nextLine)) {
					if (nextLine[0] == ')' && nextLine[1] != ')') {
						lineCount = 1;
						break;
					}
					double freq, val;
					std::istringstream issNextLineAgain(nextLine);
					issNextLineAgain >> freq >> val;
					freqValTables0.emplace_back(freq, val);

				}
				auto it = materials.find(currentSubsystemId);
				it->second.freqProperties.insert({ freqValIndex, freqValTables0 });
				freqValTables0.clear();
			}
		}

		file.close();
	}

	void displayMaterials() {
		for (const auto& pair : materials) {
			const SubsystemMaterial& mat = pair.second;
			std::cout << "Subsystem ID: " << mat.subsystemId << std::endl;
			std::cout << "Type: " << mat.type << std::endl;
			std::cout << "Properties: ";
			for (double prop : mat.properties) {
				std::cout << prop << " ";
			}
			if (!mat.freqProperties.empty()) {
				std::cout << std::endl;
				std::cout << "Frequency-dependent Properties: ";
				std::cout << std::endl;
				for (auto it = mat.freqProperties.begin(); it != mat.freqProperties.end(); it++)
				{
					std::cout << it->first << ": \n";
					std::cout << "   Freq" << "       " << "  Value" << std::endl;
					for (auto i : it->second) {
						std::cout << i.first << "       " << i.second << std::endl;
					}

				}
			}
			std::cout << std::endl << std::endl;
		}
	}
};
