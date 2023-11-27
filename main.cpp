#include "LetManager.h"
#include "LetObject.h"
#include "LetArrayMotion.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace AutoMem::Obj;

std::string Path = R"(C:\Users\30651\Desktop\file.bin)";


int main() {
	LetTools tools;
	auto_c var;

	std::string obj;
	obj = tools.ObjTToStr(LetObject::ObjT::boolean);
	std::cout << obj << std::endl;
	obj = tools.ObjGToStr(LetObject::ObjG::Dynamic);
	std::cout << obj << std::endl;

	LetObject::ObjT objt = tools.StrToObjT("String");
	LetObject::ObjG objg = tools.StrToObjG("Static");
	return 0;
}