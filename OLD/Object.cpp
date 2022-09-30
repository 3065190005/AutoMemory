#include "Object.h"
#include <sstream>
using namespace Cervice::Obj;

/**********************
* class Object
* 所有变量的基类
* 负责调度内存使用和回收
**********************/

unsigned long Object::m_MemoryLength = 0;
std::list<MemPoint*> Cervice::Obj::Object::AllBlock_Free;
std::list<MemPoint*> Cervice::Obj::Object::AllBlock_Free_dynamic;
std::list<MemPoint*> Cervice::Obj::Object::AllBlock_Malloc;
Ptr Cervice::Obj::Object::NullPtr = Cervice::Obj::Object::AllBlock_Malloc.end();
std::vector<Object*> Cervice::Obj::Object::AllObject_Pool;

void Cervice::Obj::Object::InitMemoryPool(int length)
{
	if (Object::AllBlock_Free.empty() || length != 0) {
		for (auto i = 0; i < length; i++) {
			Object::AllBlock_Free.emplace_back(new MemPoint{ (unsigned int)i,8,0,0,new char[8]{0} });
			Object::AllBlock_Free_dynamic.emplace_back(new MemPoint{ (unsigned int)i,32,0,1,new char[32]{0} });
			m_MemoryLength = length;
		}
	}
	else {
		throw("Global Memory is already init");
	}
}

Cervice::Obj::Object::Object(ElementType type) :
	m_option((size_t)ElementOption::TO_INTEGER)
{
	if (AllBlock_Free.empty()) {
		InitMemoryPool(10240);
	}

	m_class_type = ElementType::T_VOID;
	create(type);
}

Object::~Object() {
	destory();
}

Object* Cervice::Obj::Object::createObj(Object* copy) {
	if (copy == nullptr) {
		return createObj(ElementType::T_VOID);
	}

	copy->m_pool_ref++;
	if (copy->m_val != NullPtr) {
		(*copy->m_val)->ref++;
	}

	return copy;
}

Object* Cervice::Obj::Object::createObjNoPtr(Object* copy) {
	if (copy == nullptr) {
		return createObj(ElementType::T_VOID);
	}

	Object* ret = createObj(copy->m_class_type,copy->getOption());
	*ret = copy;
	return ret;
}

Object* Cervice::Obj::Object::createObj(ElementType type, ElementOption option)
{
	for (auto& i : AllObject_Pool) {
		if (i->getType() == type &&
			i->getOption() == option &&
			i->m_pool_ref == 0) {
			i->m_pool_ref++;
			i->create(i->getType());
			return i;
		}
	}

	Object* ret = nullptr;

	if(type == Cervice::Obj::ElementType::T_VOID)
		ret = new void_c;
	else if (type == Cervice::Obj::ElementType::T_BOOL)
		ret = new bool_c;
	else if (type == Cervice::Obj::ElementType::T_WORD)
		ret = new short_c;
	else if (type == Cervice::Obj::ElementType::T_DWORD)
		ret = new int_c;
	else if (type == Cervice::Obj::ElementType::T_NUMBER &&
		option == Cervice::Obj::ElementOption::TO_INTEGER)
		ret = new long_c;
	else if (type == Cervice::Obj::ElementType::T_NUMBER &&
		option == Cervice::Obj::ElementOption::TO_FLOAT)
		ret = new float_c;
	else if (type == Cervice::Obj::ElementType::T_STRING)
		ret = new string_c;
	else if (type == Cervice::Obj::ElementType::T_ARRAY)
		ret = new array_c;

	if(ret == nullptr)
		ret = new void_c;
	
	ret->m_pool_ref++;
	AllObject_Pool.push_back(ret);

	return ret;
}

void Cervice::Obj::Object::releaseObj(Object** ptr)
{
	if ((*ptr) == nullptr)
		throw("cant releasedObj with nullptr");

	(*ptr)->m_pool_ref--;
	if ((*ptr)->m_pool_ref == 0 && (*ptr)->m_val != NullPtr) {
		(*ptr)->destory();
	}
	*ptr = nullptr;

	return;
}

void Object::Free(Ptr _ptr) {
	(*_ptr)->ref--;
	if ((*_ptr)->ref == 0) {
		if((*_ptr)->type == 0)
			AllBlock_Free.emplace_front(*_ptr);
		else
			AllBlock_Free_dynamic.emplace_front(*_ptr);
		AllBlock_Malloc.erase(_ptr);
		m_val = NullPtr;
	}
	return;
}

Ptr Object::Malloc(bool dynamic) {
	std::list<MemPoint*>* list;
	if (dynamic)
		list = &AllBlock_Free_dynamic;
	else
		list = &AllBlock_Free;

	Ptr space = list->begin();
	(*space)->ref++;
	AllBlock_Malloc.emplace_back(*space);
	list->pop_front();
	Ptr ret = AllBlock_Malloc.begin();
	std::advance(ret, AllBlock_Malloc.size() - 1);
	return ret;
}

Ptr Object::searchBlocks(Ptr* one) {
	(*(*one))->ref++;
	return *one;
}

void Object::create(ElementType type) {
	if (type == ElementType::T_VOID) {
		m_val = NullPtr;
		return;
	}

	objlog("get new mem index : " << index << " len : " << len << std::endl);
	this->m_val = Malloc(ElementType::T_STRING == type);

#if DebugObjectTitle
	std::stringstream ss;
	std::string str,out;
	ss << AllBlock_Malloc.size();
	ss >> str;
	out = "title AutoObject Memery Block Size " + str;
	str = out;
	system(str.c_str());
#else
	system("title runTime");
#endif // DebugObjectLog
}

void Object::destory() {

	if(m_val == NullPtr)
		return;

	if (this->getType() == ElementType::T_ARRAY) {
		for (auto& i : this->m_number_array) {
			releaseObj(&i.second);
		}
		m_number_array.clear();

		for (auto& i : this->m_str_array) {
			releaseObj(&i.second);
		}
		m_str_array.clear();
	}

	Free(m_val);
}

void Object::ReleaseMemoryPool()
{
	for (auto& i : AllObject_Pool) {
		delete i;
		i = nullptr;
	}
	for (auto& i : AllBlock_Free) {
		if (i != nullptr && i->block != nullptr) {
			delete i->block;
			i->block = nullptr;
			delete i;
			i = nullptr;
		}
	}

	for (auto& i : AllBlock_Free_dynamic) {
		if (i != nullptr && i->block != nullptr) {
			delete i->block;
			i->block = nullptr;
			delete i;
			i = nullptr;
		}
	}

	for (auto& i : AllBlock_Malloc) {
		if (i != nullptr && i->block != nullptr) {
			delete i->block;
			i->block = nullptr;
			delete i;
			i = nullptr;
		}
	}
}

char* Object::getEleIndex(Ptr index)
{
	if (index == NullPtr) {
		throw("Memery index is bugger than max size");
	}

	return (*index)->block;
}

Object* Cervice::Obj::Object::operator=(Object* node)
{
	int len = (int)node->getType();
	int space = (int)this->getType();

	if (len == 0) {
		setZero();
		return this;
	}

	auto selfType = this->getType();
	auto nodeType = node->getType();
	auto targType = ElementType::T_STRING;

	if (selfType == targType || nodeType == targType) {
		throw("string can not assign to number");
	}

	char* sources = node->get();
	char* target = this->get();
	setZero();
	memcpy(target, sources, len > space ? space : len);
	return this;
}

Object* Cervice::Obj::Object::operator=(Object& node)
{
	int len = (int)node.getType();

	if (len == 0) {
		setZero();
		return this;
	}

	auto selfType = this->getType();
	auto nodeType = node.getType();
	auto targType = ElementType::T_STRING;

	if (selfType == targType || nodeType == targType) {
		throw("string can not assign to number");
	}

	int space = (int)this->getType();
	char* sources = node.get();
	char* target = this->get();
	setZero();
	memcpy(target, sources, len > space ? space : len);
	return this;
}

Object* Cervice::Obj::Object::operator+(Object* node)
{
	auto selfType = this->getType();
	auto nodeType = node->getType();
	auto targType = ElementType::T_STRING;

	if (selfType == targType || nodeType == targType) {
		throw("string can not add to number");
	}

	auto type = this->m_class_type;
	auto typeOption = this->m_option;

	auto vartype = node->m_class_type;
	auto varOption = node->m_option;

	// 整形或浮点加减法
	if (type <= ElementType::T_NUMBER && vartype <= ElementType::T_NUMBER) {
		if (typeOption == (size_t)ElementOption::TO_INTEGER && varOption == (size_t)ElementOption::TO_INTEGER) {
			long long int left  = 0;
			long long int right = 0;
			long long int result = 0;
			
			memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
			memcpy((char*)&right, node->get(), (*(node->m_val))->lenght);
			result = left + right;
			Object* resultObj = createObj(ElementType::T_NUMBER);
			resultObj->set((char*)&result);
			return resultObj;
		}
		if (typeOption == (size_t)ElementOption::TO_FLOAT || varOption == (size_t)ElementOption::TO_FLOAT) {
			int count = 0;
			if (typeOption == (size_t)ElementOption::TO_FLOAT)
				count+= 1;
			if (varOption == (size_t)ElementOption::TO_FLOAT)
				count+= 10;
			
			double left;
			double right;
			long long int other;

			if(count == 1){
				memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&other, node->get(), (*(node->m_val))->lenght);
				double result = left + other;
				Object* resultObj = createObj(ElementType::T_NUMBER,ElementOption::TO_FLOAT);
				resultObj->set((char*)&result);
				return resultObj;
			}
			if (count == 10) {
				memcpy((char*)&other, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&left, node->get(), (*(node->m_val))->lenght);
				long long int result = left + other;
				Object* resultObj = createObj(ElementType::T_NUMBER);
				resultObj->set((char*)&result);
				return resultObj;
			}
			if (count == 11) {
				memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&right, node->get(), (*(node->m_val))->lenght);
				double result = left + right;
				Object* resultObj = createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
				resultObj->set((char*)&result);
				return resultObj;
			}
		}
	}

	throw("can not take operator +");
}

Object* Cervice::Obj::Object::operator-(Object* node)
{

	auto selfType = this->getType();
	auto nodeType = node->getType();
	auto targType = ElementType::T_STRING;

	if (selfType == targType || nodeType == targType) {
		throw("string can not Sub to number");
	}

	auto type = this->m_class_type;
	auto typeOption = this->m_option;

	auto vartype = node->m_class_type;
	auto varOption = node->m_option;

	// 整形或浮点加减法
	if (type <= ElementType::T_NUMBER && vartype <= ElementType::T_NUMBER) {
		if (typeOption == (size_t)ElementOption::TO_INTEGER && varOption == (size_t)ElementOption::TO_INTEGER) {
			long long int left = 0;
			long long int right = 0;
			long long int result = 0;

			memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
			memcpy((char*)&right, node->get(), (*(node->m_val))->lenght);
			result = left - right;
			Object* resultObj = createObj(ElementType::T_NUMBER);
			resultObj->set((char*)&result);
			return resultObj;
		}
		if (typeOption == (size_t)ElementOption::TO_FLOAT || varOption == (size_t)ElementOption::TO_FLOAT) {
			int count = 0;
			if (typeOption == (size_t)ElementOption::TO_FLOAT)
				count += 1;
			if (varOption == (size_t)ElementOption::TO_FLOAT)
				count += 10;

			double left;
			double right;
			long long int other;

			if (count == 1) {
				memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&other, node->get(), (*(node->m_val))->lenght);
				double result = left - other;
				Object* resultObj = createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
				resultObj->set((char*)&result);
				return resultObj;
			}
			if (count == 10) {
				memcpy((char*)&other, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&left, node->get(), (*(node->m_val))->lenght);
				long long int result = left - other;
				Object* resultObj = createObj(ElementType::T_NUMBER);
				resultObj->set((char*)&result);
				return resultObj;
			}
			if (count == 11) {
				memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&right, node->get(), (*(node->m_val))->lenght);
				double result = left - right;
				Object* resultObj = createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
				resultObj->set((char*)&result);
				return resultObj;
			}
		}
	}
	throw("can not take operator -");
}

Object* Cervice::Obj::Object::operator*(Object* node)
{
	auto selfType = this->getType();
	auto nodeType = node->getType();
	auto targType = ElementType::T_STRING;

	if (selfType == targType || nodeType == targType) {
		throw("string can not Mul to number");
	}

	auto type = this->m_class_type;
	auto typeOption = this->m_option;

	auto vartype = node->m_class_type;
	auto varOption = node->m_option;

	// 整形或浮点加减法
	if (type <= ElementType::T_NUMBER && vartype <= ElementType::T_NUMBER) {
		if (typeOption == (size_t)ElementOption::TO_INTEGER && varOption == (size_t)ElementOption::TO_INTEGER) {
			long long int left = 0;
			long long int right = 0;
			long long int result = 0;

			memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
			memcpy((char*)&right, node->get(), (*(node->m_val))->lenght);
			result = left * right;
			Object* resultObj = createObj(ElementType::T_NUMBER);
			resultObj->set((char*)&result);
			return resultObj;
		}
		if (typeOption == (size_t)ElementOption::TO_FLOAT || varOption == (size_t)ElementOption::TO_FLOAT) {
			int count = 0;
			if (typeOption == (size_t)ElementOption::TO_FLOAT)
				count += 1;
			if (varOption == (size_t)ElementOption::TO_FLOAT)
				count += 10;

			double left;
			double right;
			long long int other;

			if (count == 1) {
				memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&other, node->get(), (*(node->m_val))->lenght);
				double result = left * other;
				Object* resultObj = createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
				resultObj->set((char*)&result);
				return resultObj;
			}
			if (count == 10) {
				memcpy((char*)&other, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&left, node->get(), (*(node->m_val))->lenght);
				long long int result = left * other;
				Object* resultObj = createObj(ElementType::T_NUMBER);
				resultObj->set((char*)&result);
				return resultObj;
			}
			if (count == 11) {
				memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&right, node->get(), (*(node->m_val))->lenght);
				double result = left * right;
				Object* resultObj = createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
				resultObj->set((char*)&result);
				return resultObj;
			}
		}
	}

	throw("can not take operator *");
}

Object* Cervice::Obj::Object::operator/(Object* node)
{
	auto selfType = this->getType();
	auto nodeType = node->getType();
	auto targType = ElementType::T_STRING;

	if (selfType == targType || nodeType == targType) {
		throw("string can not Div to number");
	}

	auto type = this->m_class_type;
	auto typeOption = this->m_option;

	auto vartype = node->m_class_type;
	auto varOption = node->m_option;

	// 整形或浮点加减法
	if (type <= ElementType::T_NUMBER && vartype <= ElementType::T_NUMBER) {
		if (typeOption == (size_t)ElementOption::TO_INTEGER && varOption == (size_t)ElementOption::TO_INTEGER) {
			long long int left = 0;
			long long int right = 0;
			long long int result = 0;

			memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
			memcpy((char*)&right, node->get(), (*(node->m_val))->lenght);
			if (right == 0) {
				throw("divisor can not be Zero(0) ");
			}
			result = left / right;
			Object* resultObj = createObj(ElementType::T_NUMBER);
			resultObj->set((char*)&result);
			return resultObj;
		}
		if (typeOption == (size_t)ElementOption::TO_FLOAT || varOption == (size_t)ElementOption::TO_FLOAT) {
			int count = 0;
			if (typeOption == (size_t)ElementOption::TO_FLOAT)
				count += 1;
			if (varOption == (size_t)ElementOption::TO_FLOAT)
				count += 10;

			double left;
			double right;
			long long int other;

			if (count == 1) {
				memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&other, node->get(), (*(node->m_val))->lenght);
				if (other == 0) {
					throw("divisor can not be Zero(0) ");
				}
				double result = left / other;
				Object* resultObj = createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
				resultObj->set((char*)&result);
				return resultObj;
			}
			if (count == 10) {
				memcpy((char*)&other, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&right, node->get(), (*(node->m_val))->lenght);
				if (right == 0.0) {
					throw("divisor can not be Zero(0) ");
				}
				long long int result = other / right;
				Object* resultObj = createObj(ElementType::T_NUMBER);
				resultObj->set((char*)&result);
				return resultObj;
			}
			if (count == 11) {
				memcpy((char*)&left, this->get(), (*(this->m_val))->lenght);
				memcpy((char*)&right, node->get(), (*(node->m_val))->lenght);
				if (right == 0.0) {
					throw("divisor can not be Zero(0) ");
				}
				double result = left / right;
				Object* resultObj = createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
				resultObj->set((char*)&result);
				return resultObj;
			}
		}
	}
	throw("can not take opeartor /");
}

Object* Cervice::Obj::Object::operator[](Object* node){
	if (node->getType() != ElementType::T_STRING &&
		node->getOption() != ElementOption::TO_FLOAT &&
		node->getOption() != ElementOption::TO_INTEGER) {
		throw("index must be number or string");
	}

	if (this->getType() != ElementType::T_ARRAY) {
		throw("this must be array that can take index");
	}

	if (node->getOption() == ElementOption::TO_FLOAT) {
		double index = *(double*)node->get();
		auto elemn = m_number_array.find(index);
		if (elemn != m_number_array.end()) {
			return elemn->second;
		}
		m_number_array.insert({ index,createObj(ElementType::T_VOID) });
		return m_number_array.find(index)->second;
	}
	else if (node->getOption() == ElementOption::TO_INTEGER) {
		long long int index = *(long long int*)node->get();
		auto elemn = m_number_array.find(index);
		if (elemn != m_number_array.end()) {
			return elemn->second;
		}
		m_number_array.insert({ index,createObj(ElementType::T_VOID) });
		return m_number_array.find(index)->second;
	}
	else if (node->getType() == ElementType::T_STRING) {
		std::string index;
		index.append(node->get());
		auto elemn = m_str_array.find(index);
		if (elemn != m_str_array.end()) {
			return elemn->second;
		}
		m_str_array.insert({ index,createObj(ElementType::T_VOID) });
		return m_str_array.find(index)->second;
	}

	return createObj(ElementType::T_VOID);
}

ElementType Object::getType() {
	return m_class_type;
}

ElementOption Cervice::Obj::Object::getOption()
{
	return (ElementOption)m_option;
}

std::map<double, Object*>* Cervice::Obj::Object::getNumberArray()
{
	throw("can not call getNumberArray with Object");
	return nullptr;
}

std::map<std::string, Object*>* Cervice::Obj::Object::getStrArray()
{
	throw("can not call getStrArray with Object");
	return nullptr;
}



/*************************************
* class ObjectVoid : public Object
* 继承Object的void类	不占用字节
* 负责void数据相关的实现
*************************************/

Cervice::Obj::ObjectVoid::ObjectVoid():
	Object(ElementType::T_VOID)
{
}


Cervice::Obj::ObjectVoid::~ObjectVoid()
{
}


/*************************************
* class ObjectLong : public Object
* 继承Object的long类	占用8字结
* 负责long数据相关的实现
*************************************/

ObjectLong::ObjectLong() :
	Object(ElementType::T_NUMBER) 
{
	m_class_type = ElementType::T_NUMBER;
	objlog( "init new ObjectInteger" << std::endl);
}
Cervice::Obj::ObjectLong::ObjectLong(long long num):
	Object(ElementType::T_NUMBER) 
{	
	m_class_type = ElementType::T_NUMBER;
	Set_Element(num);
	objlog( "init new ObjectInteger with " << num << std::endl);
}

ObjectLong::~ObjectLong()
{
	objlog( "delete ObjectInteger" << std::endl);
}

void ObjectLong::set(char* point) {
	
	char* mem = getEleIndex(m_val);
	*(long long int*)mem = *(long long int*)point;
}

char* ObjectLong::get(){
	char* mem = getEleIndex(m_val);
	return mem;
}

void Cervice::Obj::ObjectLong::setZero()
{
	Set_Element(0x0000000000000000);
}

void Cervice::Obj::ObjectLong::fullMem()
{
	Set_Element(0xffffffffffffffff);
}

void ObjectLong::Set_Element(long long int number) {
	this->set((char*)(&number));
}

long long int ObjectLong::Get_Element() {
	return *(long long*)(this->get());
}


/*********************************
* class ObjectBool : public Object
* 继承Object的bool类 占用1字节
* 负责bool数据相关的实现
**********************************/

ObjectBool::ObjectBool() : 
	Object(ElementType::T_BOOL) 
{
	m_class_type = ElementType::T_BOOL;
	objlog( "init new ObjectBool" << std::endl);
}
Cervice::Obj::ObjectBool::ObjectBool(bool bo) :
	Object(ElementType::T_BOOL)
{
	m_class_type = ElementType::T_BOOL;
	Set_Element(bo);
	std::string typ = bo == false ? "false" : "true";
	objlog( "init new ObjectBool with " << typ << std::endl);
}

ObjectBool::~ObjectBool() 
{
	objlog( "delete ObjectBool" << std::endl);
}

void ObjectBool::set(char* point) {
	char* mem = getEleIndex(m_val);
	*(bool*)mem = *(bool*)point;
}

char* ObjectBool::get() {
	char* mem = getEleIndex(m_val);
	return mem;
}

void Cervice::Obj::ObjectBool::setZero()
{
	Set_Element(0x00);
}

void Cervice::Obj::ObjectBool::fullMem()
{
	Set_Element(0xFF);
}

void ObjectBool::Set_Element(bool byte) {
	this->set((char*)(&byte));
}

bool ObjectBool::Get_Element() {
	return *(bool*)(this->get());
}

/***********************************
* class ObjectShort : public Object
* 继承Object的short类 占用2字节
* 负责short数据相关的实现
***********************************/

ObjectShort::ObjectShort() :
	Object(ElementType::T_WORD)
{
	m_class_type = ElementType::T_WORD;
	objlog( "init new ObjectShort" << std::endl);
}
Cervice::Obj::ObjectShort::ObjectShort(short shr):
	Object(ElementType::T_WORD)
{
	m_class_type = ElementType::T_WORD;
	Set_Element(shr);
	objlog( "init new ObjectShort with "<< shr << std::endl);
}

ObjectShort::~ObjectShort()
{
	objlog( "delete ObjectShort" << std::endl);
};

void ObjectShort::set(char* point) {
	char* mem = getEleIndex(m_val);
	*(short*)mem = *(short*)point;
}

char* ObjectShort::get() {
	char* mem = getEleIndex(m_val);
	return mem;
}

void Cervice::Obj::ObjectShort::setZero()
{
	Set_Element(0x0000);
}

void Cervice::Obj::ObjectShort::fullMem()
{
	Set_Element(0xFFFF);
}

void ObjectShort::Set_Element(short byte) {
	this->set((char*)(&byte));
}

short ObjectShort::Get_Element() {
	return *(short*)(this->get());
}

/******************************
* class ObjectInt : public Object
* 继承Object的int类 占用4字节
* 负责int数据相关的实现
******************************/

ObjectInt::ObjectInt() :
	Object(ElementType::T_DWORD)
{
	m_class_type = ElementType::T_DWORD;
	objlog( "init new ObjectInt" << std::endl);
}
Cervice::Obj::ObjectInt::ObjectInt(int num) :
	Object(ElementType::T_DWORD)
{
	m_class_type = ElementType::T_DWORD;
	objlog( "init new ObjectInt with " << num << std::endl);
}

ObjectInt::~ObjectInt()
{
	objlog( "delete ObjectInt" << std::endl);
};

void ObjectInt::set(char* point) {
	char* mem = getEleIndex(m_val);
	*(int*)mem = *(int*)point;
}

char* ObjectInt::get() {
	char* mem = getEleIndex(m_val);
	return mem;
}

void Cervice::Obj::ObjectInt::setZero()
{
	Set_Element(0x00000000);
}

void Cervice::Obj::ObjectInt::fullMem()
{
	Set_Element(0xFFFFFFFF);
}

void ObjectInt::Set_Element(int byte) {
	this->set((char*)(&byte));
}

int ObjectInt::Get_Element() {
	return *(int*)(this->get());
}

/******************************
* class ObjectFloat : public Object
* 继承Object的double类 占用8字节
* 负责double数据相关的实现
******************************/

ObjectFloat::ObjectFloat() :
	Object(ElementType::T_NUMBER)
{
	m_option = (size_t)ElementOption::TO_FLOAT;
	m_class_type = ElementType::T_NUMBER;
	objlog( "init new ObjectFloat" << std::endl);
}

Cervice::Obj::ObjectFloat::ObjectFloat(double num) :
	Object(ElementType::T_NUMBER)
{
	m_option = (size_t)ElementOption::TO_FLOAT;
	m_class_type = ElementType::T_NUMBER;
	objlog( "init new ObjectFloat with " << num << std::endl);
}

Cervice::Obj::ObjectFloat::~ObjectFloat()
{
	objlog( "delete ObjectFloat" << std::endl);
};

void Cervice::Obj::ObjectFloat::set(char* point) {
	char* mem = getEleIndex(m_val);
	*(double*)mem = *(double*)point;
}

char* Cervice::Obj::ObjectFloat::get() {
	char* mem = getEleIndex(m_val);
	return mem;
}

void Cervice::Obj::ObjectFloat::setZero()
{
	Set_Element(0x0000000000000000);
}

void Cervice::Obj::ObjectFloat::fullMem()
{
	Set_Element(0xFFFFFFFFFFFFFFFF);
}

void ObjectFloat::Set_Element(double byte) {
	this->set((char*)(&byte));
}

double ObjectFloat::Get_Element() {
	return *(double*)(this->get());
}

/******************************
* class ObjectString: public Object
* 字符串 类型实现
******************************/

Cervice::Obj::ObjectString::ObjectString() :
	Object(ElementType::T_STRING)
{
	m_option = (size_t)ElementOption::TO_OTHER;
	m_class_type = ElementType::T_STRING;
	objlog("init new ObjectString" << std::endl);
}

Cervice::Obj::ObjectString::ObjectString(const char* str):
	Object(ElementType::T_STRING)
{
	size_t len = strnlen_s(str, m_MemoryLength);
	setZero();
	m_option = len+1;
	(*m_val)->lenght = m_option;
	char* temp_block = (*m_val)->block;
	(*m_val)->block = new char[m_option];
	strcpy_s(temp_block, m_option, (*m_val)->block);
	delete temp_block;
	temp_block = nullptr;
	(*m_val)->block[m_option] = '\0';
	objlog("init new ObjectString " << str << std::endl);
}

Cervice::Obj::ObjectString::~ObjectString()
{
}

void Cervice::Obj::ObjectString::set(char* point)
{
	size_t len = strnlen_s(point, SIZE_MAX-1);;
	
	if ((*(this->m_val))->lenght <= len) {
		setZero();
		m_option = len+1;
		size_t Length = (*m_val)->lenght;
		while (true) {
			if (Length <= m_option) {
				if (Length > 1024) Length += 1024;
				else Length *= 2;
			}
			else {
				break;
			}
			(*m_val)->lenght = Length;
		}
		char* temp_block = (*m_val)->block;
		(*m_val)->block = new char[Length] {0};
		strcpy_s(temp_block, m_option, (*m_val)->block);
		delete temp_block;
		temp_block = nullptr;
		(*m_val)->block[m_option] = '\0';
	}

	setZero();
	char* mem = getEleIndex(m_val);
	memcpy(mem, point, len);
}

char* Cervice::Obj::ObjectString::get()
{
	char* mem = getEleIndex(m_val);
	return mem;
}

void Cervice::Obj::ObjectString::setZero()
{
	char* mem = get();
	memset(mem, 0, (*(this->m_val))->lenght);
}

void Cervice::Obj::ObjectString::fullMem()
{
	char* mem = get();
	memset(mem, 1, (*(this->m_val))->lenght-1);
	mem[(*(this->m_val))->lenght - 1] = '\0';
}

void Cervice::Obj::ObjectString::Set_Element(char* byte)
{
	set(byte);
}

std::string Cervice::Obj::ObjectString::Get_Element()
{
	std::string str;
	str.append(get());
	return str;
}


Object* Cervice::Obj::ObjectString::operator=(Object* node)
{
	auto selfType = this->getType();
	auto nodeType = node->getType();
	auto targType = ElementType::T_STRING;

	if (selfType != targType || nodeType != targType) {
		throw("string can only assign to string");
	}

	this->set(node->get());
	return this;
}

Object* Cervice::Obj::ObjectString::operator=(Object& node)
{
	auto selfType = this->getType();
	auto nodeType = node.getType();
	auto targType = ElementType::T_STRING;

	if (selfType != targType || nodeType != targType) {
		throw("string can only assign to string");
	}

	this->set(node.get());
	return this;
}


Object* Cervice::Obj::ObjectString::operator+(Object* node)
{
	auto selfType = this->getType();
	auto nodeType = node->getType();
	auto targType = ElementType::T_STRING;

	if (selfType != targType || nodeType != targType) {
		throw("string can only add to string");
	}

	std::string str;
	str.append(this->Get_Element());
	str.append(node->get());
	this->set((char*)(str.c_str()));
	return this;
}


/******************************
* class ObjectArray: public Object
* 数组类型实现
******************************/


Cervice::Obj::ObjectArray::ObjectArray():
	Object(ElementType::T_ARRAY)
{
	m_option = (size_t)ElementOption::TO_OTHER;
	m_class_type = ElementType::T_ARRAY;
	objlog("init new ObjectArray" << std::endl);
}

Cervice::Obj::ObjectArray::~ObjectArray()
{
}

Object* Cervice::Obj::ObjectArray::operator=(Object* node)
{
	if (this->getType() != node->getType()) {
		throw("array only assign with another array");
	}

	for (auto& i : m_number_array)
		releaseObj(&i.second);

	for (auto& i : m_str_array)
		releaseObj(&i.second);

	m_number_array.clear();
	m_str_array.clear();

	for (auto& i : *(node->getNumberArray())) {
		m_number_array[i.first] = createObjNoPtr(i.second);
	}

	for (auto& i : *(node->getStrArray())) {
		m_str_array[i.first] = createObjNoPtr(i.second);
	}

	return this;
}

Object* Cervice::Obj::ObjectArray::operator=(Object& node)
{
	if (this->getType() != node.getType()) {
		throw("array only assign with another array");
	}

	for (auto& i : m_number_array)
		releaseObj(&i.second);

	for (auto& i : m_str_array)
		releaseObj(&i.second);

	m_number_array.clear();
	m_str_array.clear();

	for (auto& i : *(node.getNumberArray())) {
		m_number_array[i.first] = createObjNoPtr(i.second);
	}

	for (auto& i : *(node.getStrArray())) {
		m_str_array[i.first] = createObjNoPtr(i.second);
	}

	return this;
}

Object* Cervice::Obj::ObjectArray::operator+(Object* node)
{
	Object* ret = createObjNoPtr(this);
	for (auto& i : *(node->getNumberArray())) {
		if((*ret->getNumberArray()).find(i.first) != m_number_array.end())
			(*ret->getNumberArray())[i.first] = createObjNoPtr(i.second);
	}

	for (auto& i : *(node->getStrArray())) {
		if ((*ret->getStrArray()).find(i.first) != m_str_array.end())
			(*ret->getStrArray())[i.first] = createObjNoPtr(i.second);
	}

	return ret;
}

void Cervice::Obj::ObjectArray::setZero()
{
	for (auto& i : m_number_array) {
		i.second->setZero();
	}

	for (auto& i : m_str_array) {
		i.second->setZero();
	}
}

void Cervice::Obj::ObjectArray::fullMem()
{
	for (auto& i : m_number_array) {
		i.second->fullMem();
	}

	for (auto& i : m_str_array) {
		i.second->fullMem();
	}
}

void Cervice::Obj::ObjectArray::Set_Element(char* byte)
{
	throw("error array can not take Set_Element");
}

std::string Cervice::Obj::ObjectArray::Get_Element()
{
	throw("error array can not take Get_Element");
}

void Cervice::Obj::ObjectArray::set(char* point)
{
	if (m_number_array.find(0) != m_number_array.end()) {
		m_number_array.find(0)->second->set(point);
	}
}

char* Cervice::Obj::ObjectArray::get()
{
	if (m_number_array.find(0) != m_number_array.end()) {
		return m_number_array.find(0)->second->get();
	}
	return nullptr;
}

std::map<double, Object*>* Cervice::Obj::ObjectArray::getNumberArray()
{
	return &m_number_array;
}

std::map<std::string, Object*>* Cervice::Obj::ObjectArray::getStrArray()
{
	return &m_str_array;
}