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
	va1 << true;
	va2 << true;

	while (true) {
		va3 = va1 == va2;
		if (LetObject::cast<bool>(va3)) {
			printAuto_c(va3);
			continue;	
		}
		break;

	}
	auto lecV = va1.getIndexs();
	return 0;
}