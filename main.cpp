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


	auto_c null;
	auto_c undef(true, true);
	auto_c number;
	number << 123.4567897;
	auto_c string;
	string << "Hello World";

    var[0] << "0 123";
    var[1] << "1 456";
    var["Hello"] << "Hello Abcef";
    var["Bye"] << "Bye AAAA";
	var["This"] << "This is Var";
	var["World"] << "This is World Abdef";

	var["list"][0] << 123456;
	var["list"][1] << 123456.456789;
	var["list"]["ListHel"] << "List Hello World";
	var["list"]["listL"] << "List List List";

	tools.println(null);
	tools.println(undef);
	tools.println(number);
	tools.println(string);
	tools.println(var);


	return 0;
}