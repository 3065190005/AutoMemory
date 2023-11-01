#include "LetManager.h"
#include "LetObject.h"
#include "LetArrayMotion.h"

#include <iostream>
using namespace AutoMem::Obj;


int main() {

	LetTools tools;
	auto_c target;

	target["string"] << "Has string";
	bool ret = tools.AutoCmp({ LetTools ::Operator::Type,"Array"}, target);
		
	return ret;
}