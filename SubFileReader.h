#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

// SEAM SUBSYSTEM FILE TEMPLATE (REV 3.0)
//
// FIELD NAME          DESCRIPTION
// 
//   SUB#:     Subsystem number
//             Any order, 0 < SUB# < 100000
//   NAME:     Subsystem name
//             Up to 19 characters can be used
//   TYPE:     Type of subsystem
//// Geometric parameters:
//_type_____L1_________L2_________L3_________L4_________L5_________L6____Comments
// INPL   length     width     t_mass
//          0        area      t_mass                                   2-D only
// I1D    length     width     t_mass
// I2D    length     width     t_mass
//          0        area      t_mass                                   2-D only
//________________________________________________________________________________
// BEND   length     width     t_mass  k_stiff(opt) fluid_mat# t_fluid 
//          0        area      t_mass  k_stiff(opt) fluid_mat# t_fluid  2-D only
// B1D    length     width     t_mass  k_stiff(opt)
// B1DG   length     width     t_mass  conduct(opt) k_stiff(opt)
// B2D    length     width     t_mass  k_stiff(opt)
//          0        area      t_mass  k_stiff(opt)                     2-D only
// B2DG   length     width     t_mass  conduct(opt) k_stiff(opt)
//          0        area      t_mass  conduct(opt) k_stiff(opt)        2-D only
// BSUP   sup_space  area      t_mass  k_stiff(opt)
//________________________________________________________________________________
// ACST    len_max   len       len_min                                 rectangular
//         len_max    0         X-area  abs_area(opt)                  1-D to 3-D duct
//          0       area_surf len_min abs_area(opt)                  layer
//          0         0         volume  abs_area(opt)                  3-D volume
// A1D     len_max   len       len_min #abs_points(opt)               rect duct
//         len_max    0         X-area  #abs_points(opt)               irregular duct
// A2D     len_max  length    len_min abs_perimeter(opt)             rect layer
//          0       area_surf len_min abs_perimeter(opt)             irreg layer
// A3D     len_max   len       len_min                                rect box
//         len_max    0         X-area  abs_area(opt)                  duct
//          0       area_surf len_min abs_area(opt)                  layer
//          0         0         volume  abs_area(opt)                  volume
//________________________________________________________________________________
// PIP     length   radius    t_mass  rho-c(opt)

//________________________________________________________________________________
//
// Example of a formatted subsystem input:
//234567890123456789012345678901234567890123456789012345678901234567890
// id#             sub_type            sub_name      mat#
//-----X-------------------X-------------------X---------X
//   43                BEND          back_plate         3
//             L1        L2        L3        L4        L5        L6
//XXXXXX---------X---------X---------X---------X---------X---------X
//            1.5       1.0    0.0063       6.0
//
// Example of a free format subsystem input with comma delimiters:
// 43, BEND, back_plate, 3
// 1.5, 1.0, 0.0063, 6.0
//
//the beginning of subsystem records
//((subdata
// id#             sub_type            sub_name      mat#
//-----X-------------------X-------------------X---------X
//             L1        L2        L3        L4        L5        L6
//XXXXXX---------X---------X---------X---------X---------X---------X
//))
// the end of subsystem records

class SubsystemInfo {
public:
	std::string subsystemId;
	std::string type; 
	std::string subsystemName;
	std::string subsystemMatID;
	std::vector<double> properties;

	SubsystemInfo(std::string id, std::string tp, std::string name, std::string matID)
		: subsystemId(std::move(id)), type(std::move(tp)), subsystemName(std::move(name)), subsystemMatID(std::move(matID)) {}
};

class SubFileReader {
private:
	std::map<std::string, SubsystemInfo> info;

public:


	void readSubFile(const std::string& filename) {
		std::ifstream file(filename);
		std::string line;

		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filename << std::endl;
			return;
		}

		std::string currentSubsystemId;
		std::string currentType;
		std::string currentName;
		std::string currentMatID;
		int lineCount = 1;
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

			if (!line.empty() && line[0] != '(' && line[0] != '!' && lineCount == 1) {

				// Read subsystem ID and type
				iss >> currentSubsystemId >> currentType >> currentName >> currentMatID;
				info.insert({ currentSubsystemId, SubsystemInfo(currentSubsystemId, currentType, currentName, currentMatID) });
				lineCount = 2;
				continue;
			}
			if (!line.empty() && line[0] != '(' && line[0] != '!' && lineCount == 2) {
				// Read properties
				double prop;
				while (iss >> prop) {
					auto it = info.find(currentSubsystemId);
					if (it != info.end()) {
						it->second.properties.push_back(prop);
					}
				}
				lineCount = 1;
				continue;
			}
		}

		file.close();
	}

	void displaySubsystems() {
		for (const auto& pair : info) {
			const SubsystemInfo& inf = pair.second;
			std::cout << "Subsystem ID: " << inf.subsystemId << std::endl;
			std::cout << "Type: " << inf.type << std::endl;
			std::cout << "Subsystem Name: " << inf.subsystemName << std::endl;
			std::cout << "MAT#: " << inf.subsystemMatID << std::endl;
			std::cout << "Properties: ";
			for (double prop : inf.properties) {
				std::cout << prop << " ";
			}
			std::cout << std::endl << std::endl;
		}
	}
};

