#include "LetManager.h"
#include "LetObject.h"
#include "LetArrayMotion.h"

#include <iostream>
using namespace AutoMem::Obj;


int main() {

	LetTools tools;
	auto_c str;
	auto_c boo;

	str << "Hello";
	boo << true;
	auto_c he;
	he = str + boo;
	tools.print(he);
	return 0;
}