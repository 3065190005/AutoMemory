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

	std::cout << true + true << std::endl;
	auto_c str;
	auto_c boo;

	str << "Hello";
	boo << true;
	auto_c he;
	he = str + boo;
	return 0;
}