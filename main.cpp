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

	tools.ReadFromBin(var, Path);
	tools.println(var);

	return 0;
}