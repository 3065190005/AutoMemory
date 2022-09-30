#include "LetManager.h"
#include "LetObject.h"
#include "LetArrayMotion.h"

#include <iostream>
using namespace Cervice::Obj;
void printAuto_c(auto_c& value)
{
	if (value.getType() == LetObject::ObjT::array) {
		for (auto& i : value.getStrArray()) {
			if (i.second.getType() == LetObject::ObjT::array) {
				printAuto_c(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::number) {
				std::cout << LetObject::cast<long double>(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::boolean) {
				std::cout << std::boolalpha << LetObject::cast<bool>(i.second);
			}
			else {
				std::cout << LetObject::cast<std::string>(i.second);
			}
		}

		for (auto& i : value.getNumArray()) {
			if (i.second.getType() == LetObject::ObjT::array) {
				printAuto_c(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::boolean) {
				std::cout << std::boolalpha << i.first << " " << LetObject::cast<bool>(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::number) {
				std::cout << LetObject::cast<long double>(i.second);
			}
			else {
				std::cout << LetObject::cast<std::string>(i.second);
			}
		}
	}
	else if (value.getType() == LetObject::ObjT::number) {
		std::cout << LetObject::cast<long double>(value);
	}
	else if (value.getType() == LetObject::ObjT::string) {
		std::cout << LetObject::cast<std::string>(value);
	}
	else if (value.getType() == LetObject::ObjT::boolean) {
		std::cout << std::boolalpha << LetObject::cast<bool>(value);
	}

}


int main() {
	auto_c va1,va2,va3;
	va1[0] << "A";
	va1[1] << "B";
	va1[2] << "C";
	va1[3][0] << "YESYES";
	va1[3][1] << "NONO";
	va1["HE"] << "D";
	va1["HD"] << "E";
	va1["HF"] << "F";
	va1["Gg"]["G"] << "GG";
	va1["Gg"]["g"] << "gg";

	auto lecV = va1.getIndexs();
	return 0;
}