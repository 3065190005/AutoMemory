#include "AutoMem/LetManager.h"
#include "AutoMem/LetObject.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace AutoMem::Obj;

std::string Path = R"(C:\Users\30651\Desktop\file.bin)";

int main() {
	LetTools tools;
	LetObject var, read;

	var[0]["Child"] << "This is Child";
	var["str"] << "this is str index";
	var[114514] << "this is number index";

	std::string file_bin;
	tools.ToBin(var, file_bin);

	tools.FromBin(read, file_bin);

	tools.print(read);
	return 0;
}