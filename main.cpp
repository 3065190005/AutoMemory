#include "LetManager.h"
#include "LetObject.h"
#include "LetArrayMotion.h"

#include <iostream>
using namespace AutoMem::Obj;


int main() {

	LetTools tools;
	auto_c list,var,ptr;

	list["Value1"] << "Hello World";
	var << "This is Var";
	LetObject::reference(&ptr, &var);
	ptr << "AAA";
	tools.IsRef(ptr, &var);

	return 0;
}