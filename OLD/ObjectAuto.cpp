#include "Object.h"
#include "ObjectAuto.h"

using namespace Cervice::Obj;

/************************
* class ObjectAuto
* 对各个Object类型之间做兼容
* 包括 数字，字符，数组（可能）
*************************/

std::map<Object*, std::vector<Object*>> ObjectAuto::m_array_map;

Cervice::Obj::ObjectAuto::ObjectAuto(Object* ptr):
	m_ptr(nullptr),
	m_type_isnum(eleType::none),
	m_parent(nullptr)
{
	if (ptr == nullptr) {
		m_ptr = Object::createObj(ElementType::T_VOID);
		return;
	}

	m_ptr = Object::createObjNoPtr(ptr);
}

Cervice::Obj::ObjectAuto::~ObjectAuto()
{
	for (auto i = m_array_map.begin(); i != m_array_map.end(); i++) {
		if (i->first == m_ptr) {
			m_array_map.erase(i);
			break;
		}
	}

	if (m_ptr != nullptr) {
		RemoveTable(m_ptr,m_parent);
		Object::releaseObj(&m_ptr);
	}
}

// copy
Cervice::Obj::ObjectAuto::ObjectAuto(const ObjectAuto& right)
{
	RemoveTable(m_ptr,m_parent);
	RemoveTable(right.m_ptr,right.m_parent);

	if (m_ptr) {
		Object::releaseObj(&m_ptr);
	}

	m_ptr = Object::createObjNoPtr(right.getPtr());

	m_type_isnum = right.m_type_isnum;
	m_index_num = right.m_index_num;
	m_index_str = right.m_index_str;
	m_parent = right.m_parent;

	AddTable(m_ptr,m_parent);
}

// move
Cervice::Obj::ObjectAuto::ObjectAuto(ObjectAuto&& right)
{
	RemoveTable(m_ptr,m_parent);
	RemoveTable(right.m_ptr,right.m_parent);

	if (m_ptr) {
		Object::releaseObj(&m_ptr);
	}


	m_ptr = Object::createObjNoPtr(right.getPtr());

	m_type_isnum = right.m_type_isnum;
	m_index_num = right.m_index_num;
	m_index_str = right.m_index_str;
	m_parent = right.m_parent;

	AddTable(m_ptr,m_parent);
}

ObjectAuto& Cervice::Obj::ObjectAuto::operator=(ObjectAuto &&right)
{
	return operator=(right);
}

ObjectAuto& Cervice::Obj::ObjectAuto::operator=(ObjectAuto& right)
{
	auto leftPtr = this->getPtr();
	auto rightPtr = right.getPtr();

	auto leftType = leftPtr->getType();
	auto rightType = rightPtr->getType();

	if (m_type_isnum != eleType::none) {
		for (auto& i : m_array_map) {
			if (i.first != m_parent)
				continue;
			for (auto& j : i.second)
			{
				if (j != m_ptr)
					continue;

				if (m_type_isnum == eleType::number) {
					auto numbers = i.first->getNumberArray();
					auto finder = numbers->find(m_index_num);
					if (finder != numbers->end()) {
						if (finder->second->getType() == right.getType() &&
							finder->second->getOption() == right.getOption()) 
						{
							*(finder->second) = rightPtr;
						}
						Object::releaseObj(&(finder->second));
					}
					(*numbers)[m_index_num] = Object::createObjNoPtr(rightPtr);
					break;
				}
				else {
					auto strings = i.first->getStrArray();
					auto finder = strings->find(m_index_str);
					if (finder != strings->end()) {
						if (finder->second->getType() == right.getType() &&
							finder->second->getOption() == right.getOption())
						{
							*(finder->second) = rightPtr;
						}
						Object::releaseObj(&(finder->second));
					}
					(*strings)[m_index_str] = Object::createObjNoPtr(rightPtr);
					break;
				}
			}
			break;
		}
	}

	if (leftType == rightType) {
		*leftPtr = rightPtr;
		return *this;
	}

	RemoveTable(m_ptr,m_parent);
	if (m_ptr) {
		Object::releaseObj(&m_ptr);
	}

	m_ptr = Object::createObjNoPtr(right.getPtr());
	AddTable(m_ptr,m_parent);
	return *this;
}

ObjectAuto Cervice::Obj::ObjectAuto::operator+(ObjectAuto& right)
{
	auto leftPtr = this->getPtr();
	auto rightPtr = right.getPtr();

	auto leftType = leftPtr->getType();
	auto rightType = rightPtr->getType();

	auto leftOption = leftPtr->getOption();
	auto rightOption = rightPtr->getOption();


	if (rightType == ElementType::T_VOID || leftType == ElementType::T_VOID) {
		throw("ObjectAuto : void can not be adder");
	}

	m_ss.str("");m_ss.clear();
	if (leftType != rightType)
	{
		if (leftType == ElementType::T_STRING) {

			// 将右值非字符串转换为字符串和左值相加
			std::string leftStr, rightStr;
			leftStr.append(leftPtr->get());
			if (rightOption == ElementOption::TO_INTEGER) {
				long long int numb = *((long long int*)rightPtr->get());
				m_ss << numb;
				m_ss >> rightStr;
				leftStr += rightStr;
			}
			else if (rightOption == ElementOption::TO_FLOAT) {
				double numb = *((double*)rightPtr->get());
				m_ss << numb;
				m_ss >> rightStr;
				leftStr += rightStr;
			}

			Object* ret = Object::createObj(ElementType::T_STRING);
			ret->set((char*)leftStr.c_str());
			ObjectAuto retVec(ret);
			Object::releaseObj(&ret);
			return std::move(retVec);
		}
		else if (rightType == ElementType::T_STRING)
		{
			bool changeWrong = true;
			m_ss.str("");m_ss.clear();
			// 尝试将右值字符串转换为左值类型
			std::string rightStr;
			rightStr.append(rightPtr->get());
			m_ss << rightStr;
			if (leftOption == ElementOption::TO_FLOAT) {
				double LeftNum, Rightnum;
				LeftNum = *((double*)leftPtr->get());
				m_ss >> Rightnum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
					auto result = LeftNum + Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
				m_ss.str("");m_ss.clear();
			}
			else if (leftOption == ElementOption::TO_INTEGER) {
				long long int LeftNum, Rightnum;
				LeftNum = *((long long int*)leftPtr->get());
				m_ss >> Rightnum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_INTEGER);
					auto result = LeftNum + Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
				m_ss.str("");m_ss.clear();
			}

			// 尝试 int + string 右值转换int失败
			// 解决 左值转换成string加上右值
			if (changeWrong) {
				// 将左值非字符串转换为字符串和右值相加

				std::string leftStr, rightStr;
				rightStr.append(rightPtr->get());
				if (leftOption == ElementOption::TO_INTEGER) {
					long long int numb = *((long long int*)leftPtr->get());
					m_ss << numb;
					m_ss >> leftStr;
					leftStr += rightStr;
				}
				else if (leftOption == ElementOption::TO_FLOAT) {
					double numb = *((double*)leftPtr->get());
					m_ss.str("");m_ss.clear();
					m_ss << numb;
					m_ss >> leftStr;
					leftStr += rightStr;
				}

				Object* ret = Object::createObj(ElementType::T_STRING);
				ret->set((char*)leftStr.c_str());
				ObjectAuto retVec(ret);
				Object::releaseObj(&ret);
				return std::move(retVec);
			}
		}
		
		else if (leftType == ElementType::T_ARRAY) {
			Object* ret = nullptr;
			if (!ret) {
				for (auto& i : *(m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}
			if (!ret) {
				for (auto& i : *(m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}

			if (!ret)
				throw("can not take array with 0 element");

			ret = Object::createObjNoPtr(ret);
			ObjectAuto retLeft(ret);
			ObjectAuto retVec = retLeft + right;
			Object::releaseObj(&ret);
			return std::move(retVec);

		}
		else if (rightType == ElementType::T_ARRAY) {
			Object* ret = nullptr;
			if (!ret) {
				for (auto& i : *(right.m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}
			if (!ret) {
				for (auto& i : *(right.m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}

			if (!ret)
				throw("can not take array with 0 element");

			ret = Object::createObjNoPtr(ret);
			ObjectAuto retRight(ret);
			ObjectAuto retVec = *this + retRight;
			Object::releaseObj(&ret);
			return std::move(retVec);
		}
	}

	Object* ret = nullptr;
	ret = (*leftPtr + rightPtr);
	ObjectAuto retVec(ret);
	Object::releaseObj(&ret);
	return std::move(retVec);
}

ObjectAuto Cervice::Obj::ObjectAuto::operator+(ObjectAuto&& right)
{
	return operator+(right);
}

ObjectAuto Cervice::Obj::ObjectAuto::operator-(ObjectAuto& right)
{
	auto leftPtr = this->getPtr();
	auto rightPtr = right.getPtr();

	auto leftType = leftPtr->getType();
	auto rightType = rightPtr->getType();

	auto leftOption = leftPtr->getOption();
	auto rightOption = rightPtr->getOption();


	if (rightType == ElementType::T_VOID || leftType == ElementType::T_VOID) {
		throw("ObjectAuto : void can not be subber");
	}

	m_ss.str("");m_ss.clear();
	if (leftType != rightType)
	{
		if (leftType == ElementType::T_STRING) {

			bool changeWrong = true;

			// 尝试将左值字符串转换为右值类型
			std::string leftStr;
			leftStr.append(leftPtr->get());
			m_ss << leftStr;
			if (rightOption == ElementOption::TO_FLOAT) {
				double LeftNum, Rightnum;
				Rightnum = *((double*)rightPtr->get());
				m_ss >> LeftNum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
					auto result = LeftNum - Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
			else if (rightOption == ElementOption::TO_INTEGER) {
				long long int LeftNum, Rightnum;
				Rightnum = *((long long int*)rightPtr->get());
				m_ss >> LeftNum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_INTEGER);
					auto result = LeftNum - Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
		}
		else if (rightType == ElementType::T_STRING)
		{
			bool changeWrong = true;

			// 尝试将右值字符串转换为左值类型
			std::string rightStr;
			rightStr.append(rightPtr->get());
			m_ss << rightStr;
			if (leftOption == ElementOption::TO_FLOAT) {
				double LeftNum, Rightnum;
				LeftNum = *((double*)leftPtr->get());
				m_ss >> Rightnum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
					auto result = LeftNum - Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
			else if (leftOption == ElementOption::TO_INTEGER) {
				long long int LeftNum, Rightnum;
				LeftNum = *((long long int*)leftPtr->get());
				m_ss >> Rightnum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_INTEGER);
					auto result = LeftNum - Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
		}

		else if (leftType == ElementType::T_ARRAY) {
			Object* ret = nullptr;
			if (!ret) {
				for (auto& i : *(m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}
			if (!ret) {
				for (auto& i : *(m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}

			if (!ret)
				throw("can not take array with 0 element");

			ret = Object::createObjNoPtr(ret);
			ObjectAuto retLeft(ret);
			ObjectAuto retVec = retLeft - right;
			Object::releaseObj(&ret);
			return std::move(retVec);

		}
		else if (rightType == ElementType::T_ARRAY) {
			Object* ret = nullptr;
			if (!ret) {
				for (auto& i : *(right.m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}
			if (!ret) {
				for (auto& i : *(right.m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}

			if (!ret)
				throw("can not take array with 0 element");

			ret = Object::createObjNoPtr(ret);
			ObjectAuto retRight(ret);
			ObjectAuto retVec = *this - retRight;
			Object::releaseObj(&ret);
			return std::move(retVec);
		}

		throw("string can not be subber");
	}

	Object* ret = nullptr;
	ret = (*leftPtr - rightPtr);
	ObjectAuto retVec(ret);
	Object::releaseObj(&ret);
	return std::move(retVec);
}

ObjectAuto Cervice::Obj::ObjectAuto::operator-(ObjectAuto&& right)
{
	return operator-(right);
}

ObjectAuto Cervice::Obj::ObjectAuto::operator*(ObjectAuto& right)
{
	auto leftPtr = this->getPtr();
	auto rightPtr = right.getPtr();

	auto leftType = leftPtr->getType();
	auto rightType = rightPtr->getType();

	auto leftOption = leftPtr->getOption();
	auto rightOption = rightPtr->getOption();


	if (rightType == ElementType::T_VOID || leftType == ElementType::T_VOID) {
		throw("ObjectAuto : void can not be subber");
	}

	m_ss.str("");m_ss.clear();
	if (leftType != rightType)
	{
		if (leftType == ElementType::T_STRING) {

			bool changeWrong = true;

			// 尝试将左值字符串转换为右值类型
			std::string leftStr;
			leftStr.append(leftPtr->get());
			m_ss << leftStr;
			if (rightOption == ElementOption::TO_FLOAT) {
				double LeftNum, Rightnum;
				Rightnum = *((double*)rightPtr->get());
				m_ss >> LeftNum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
					auto result = LeftNum * Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
			else if (rightOption == ElementOption::TO_INTEGER) {
				long long int LeftNum, Rightnum;
				Rightnum = *((long long int*)rightPtr->get());
				m_ss >> LeftNum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_INTEGER);
					auto result = LeftNum * Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
		}
		else if (rightType == ElementType::T_STRING)
		{
			bool changeWrong = true;

			// 尝试将右值字符串转换为左值类型
			std::string rightStr;
			rightStr.append(rightPtr->get());
			m_ss << rightStr;
			if (leftOption == ElementOption::TO_FLOAT) {
				double LeftNum, Rightnum;
				LeftNum = *((double*)leftPtr->get());
				m_ss >> Rightnum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
					auto result = LeftNum * Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
			else if (leftOption == ElementOption::TO_INTEGER) {
				long long int LeftNum, Rightnum;
				LeftNum = *((long long int*)leftPtr->get());
				m_ss >> Rightnum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_INTEGER);
					auto result = LeftNum * Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
		}

		else if (leftType == ElementType::T_ARRAY) {
			Object* ret = nullptr;
			if (!ret) {
				for (auto& i : *(m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}
			if (!ret) {
				for (auto& i : *(m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}

			if (!ret)
				throw("can not take array with 0 element");

			ret = Object::createObjNoPtr(ret);
			ObjectAuto retLeft(ret);
			ObjectAuto retVec = retLeft * right;
			Object::releaseObj(&ret);
			return std::move(retVec);

		}
		else if (rightType == ElementType::T_ARRAY) {
			Object* ret = nullptr;
			if (!ret) {
				for (auto& i : *(right.m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}
			if (!ret) {
				for (auto& i : *(right.m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}

			if (!ret)
				throw("can not take array with 0 element");

			ret = Object::createObjNoPtr(ret);
			ObjectAuto retRight(ret);
			ObjectAuto retVec = *this * retRight;
			Object::releaseObj(&ret);
			return std::move(retVec);
		}

		throw("string can not be Muller");
	}

	Object* ret = nullptr;
	ret = (*leftPtr * rightPtr);
	ObjectAuto retVec(ret);
	Object::releaseObj(&ret);
	return std::move(retVec);
}

ObjectAuto Cervice::Obj::ObjectAuto::operator*(ObjectAuto&& right)
{
	return operator*(right);
}

ObjectAuto Cervice::Obj::ObjectAuto::operator/(ObjectAuto& right)
{
	auto leftPtr = this->getPtr();
	auto rightPtr = right.getPtr();

	auto leftType = leftPtr->getType();
	auto rightType = rightPtr->getType();

	auto leftOption = leftPtr->getOption();
	auto rightOption = rightPtr->getOption();


	if (rightType == ElementType::T_VOID || leftType == ElementType::T_VOID) {
		throw("ObjectAuto : void can not be subber");
	}

	m_ss.str("");m_ss.clear();
	if (leftType != rightType)
	{
		if (leftType == ElementType::T_STRING) {

			bool changeWrong = true;

			// 尝试将左值字符串转换为右值类型
			std::string leftStr;
			leftStr.append(leftPtr->get());
			m_ss << leftStr;
			if (rightOption == ElementOption::TO_FLOAT) {
				double LeftNum, Rightnum;
				Rightnum = *((double*)rightPtr->get());
				m_ss >> LeftNum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
					if (Rightnum == 0.0) {
						throw("muller can not be zero");
					}
					auto result = LeftNum / Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
			else if (rightOption == ElementOption::TO_INTEGER) {
				long long int LeftNum, Rightnum;
				Rightnum = *((long long int*)rightPtr->get());
				m_ss >> LeftNum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_INTEGER);
					if (Rightnum == 0) {
						throw("muller can not be zero");
					}
					auto result = LeftNum / Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
		}
		else if (rightType == ElementType::T_STRING)
		{
			bool changeWrong = true;

			// 尝试将右值字符串转换为左值类型
			std::string rightStr;
			rightStr.append(rightPtr->get());
			m_ss << rightStr;
			if (leftOption == ElementOption::TO_FLOAT) {
				double LeftNum, Rightnum;
				LeftNum = *((double*)leftPtr->get());
				m_ss >> Rightnum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
					if (Rightnum == 0.0) {
						throw("muller can not be zero");
					}
					auto result = LeftNum / Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
			else if (leftOption == ElementOption::TO_INTEGER) {
				long long int LeftNum, Rightnum;
				LeftNum = *((long long int*)leftPtr->get());
				m_ss >> Rightnum;
				changeWrong = m_ss.fail();
				if (changeWrong == false) {
					Object* ret = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_INTEGER);
					if (Rightnum == 0) {
						throw("muller can not be zero");
					}
					auto result = LeftNum / Rightnum;
					ret->set((char*)&result);
					ObjectAuto retVec(ret);
					Object::releaseObj(&ret);
					return std::move(retVec);
				}
			}
		}

		else if (leftType == ElementType::T_ARRAY) {
			Object* ret = nullptr;
			if (!ret) {
				for (auto& i : *(m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}
			if (!ret) {
				for (auto& i : *(m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}

			if (!ret)
				throw("can not take array with 0 element");

			ret = Object::createObjNoPtr(ret);
			ObjectAuto retLeft(ret);
			ObjectAuto retVec = retLeft / right;
			Object::releaseObj(&ret);
			return std::move(retVec);

		}
		else if (rightType == ElementType::T_ARRAY) {
			Object* ret = nullptr;
			if (!ret) {
				for (auto& i : *(right.m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}
			if (!ret) {
				for (auto& i : *(right.m_ptr->getNumberArray())) {
					ret = i.second;
					break;
				}
			}

			if (!ret)
				throw("can not take array with 0 element");

			ret = Object::createObjNoPtr(ret);
			ObjectAuto retRight(ret);
			ObjectAuto retVec = *this / retRight;
			Object::releaseObj(&ret);
			return std::move(retVec);
		}

		throw("string can not be Divver");
	}

	Object* ret = nullptr;
	ret = (*leftPtr / rightPtr);
	ObjectAuto retVec(ret);
	Object::releaseObj(&ret);
	return std::move(retVec);
}

ObjectAuto Cervice::Obj::ObjectAuto::operator/(ObjectAuto&& right)
{
	return operator/(right);
}

ObjectAuto Cervice::Obj::ObjectAuto::operator[](ObjectAuto& right)
{
	if (!(right.getOption() == ElementOption::TO_INTEGER ||
		right.getOption() == ElementOption::TO_FLOAT ||
		right.getType() == ElementType::T_STRING))
	{
		throw("array index must be number or string");
	}

	if (this->m_ptr->getType() != ElementType::T_ARRAY) {
		Object::releaseObj(&m_ptr);
		m_ptr = Object::createObj(ElementType::T_ARRAY);
	}

	Object* ret = Object::createObjNoPtr(right.m_ptr);
	ObjectAuto retVec((*m_ptr)[ret]);
	Object::releaseObj(&ret);

	if (right.getOption() == ElementOption::TO_INTEGER) {
		retVec.m_type_isnum = eleType::number;
		retVec.m_index_num = *(long long int*)right.m_ptr->get();
	}
	else if (right.getOption() == ElementOption::TO_FLOAT) {
		retVec.m_type_isnum = eleType::number;
		retVec.m_index_num = *(double*)right.m_ptr->get();
	}
	else {
		retVec.m_type_isnum = eleType::string;
		retVec.m_index_str.clear();
		retVec.m_index_str.append(right.m_ptr->get());
	}

	// 加入数组映射关系
	std::vector<Object*> arrMapChild;
	retVec.m_parent = m_ptr;
	if (m_array_map.find(m_ptr) == m_array_map.end()) {
		m_array_map[m_ptr] = arrMapChild;
	}
	m_array_map[m_ptr].push_back(retVec.m_ptr);
	return std::move(retVec);
}

// 针对原始变量

void Cervice::Obj::ObjectAuto::operator<<(std::string& str)
{
	if (!m_ptr) {
		throw ("Auto Ptr is nullptr");
	}

	auto type = m_ptr->getType();

	if (type == ElementType::T_STRING) {
		m_ptr->setZero();
		m_ptr->set((char*)str.c_str());
		return;
	}

	Object::releaseObj(&m_ptr);
	m_ptr = Object::createObj(ElementType::T_STRING);
	m_ptr->set((char*)str.c_str());
	return;
}

void Cervice::Obj::ObjectAuto::operator<<(std::string&& str)
{
	if (!m_ptr) {
		throw ("Auto Ptr is nullptr");
	}

	auto type = m_ptr->getType();

	if (type == ElementType::T_STRING) {
		m_ptr->setZero();
		m_ptr->set((char*)str.c_str());
		return;
	}

	Object::releaseObj(&m_ptr);
	m_ptr = Object::createObj(ElementType::T_STRING);
	m_ptr->set((char*)str.c_str());
	return;
}

void Cervice::Obj::ObjectAuto::operator<<(long long int num)
{
	if (!m_ptr) {
		throw ("Auto Ptr is nullptr");
	}

	auto type = m_ptr->getType();
	auto option = m_ptr->getOption();

	if (type != ElementType::T_VOID && 
		(option == ElementOption::TO_INTEGER || 
		option == ElementOption::TO_FLOAT)) {
		m_ptr->setZero();
		m_ptr->set((char*)&num);
		return;
	}

	Object::releaseObj(&m_ptr);
	m_ptr = Object::createObj(ElementType::T_NUMBER,ElementOption::TO_INTEGER);
	m_ptr->set((char*)&num);
	return;
}

void Cervice::Obj::ObjectAuto::operator<<(int num)
{
	operator<<((long long int)num);
	return;
}

void Cervice::Obj::ObjectAuto::operator<<(double num)
{
	if (!m_ptr) {
		throw ("Auto Ptr is nullptr");
	}

	auto type = m_ptr->getType();
	auto option = m_ptr->getOption();

	if (type != ElementType::T_VOID &&
		(option == ElementOption::TO_INTEGER ||
		option == ElementOption::TO_FLOAT)) {
		m_ptr->setZero();
		m_ptr->set((char*)&num);
		return;
	}

	Object::releaseObj(&m_ptr);
	m_ptr = Object::createObj(ElementType::T_NUMBER, ElementOption::TO_FLOAT);
	m_ptr->set((char*)&num);
	return;
}

bool Cervice::Obj::ObjectAuto::operator>>(std::string& num)
{
	if (m_ptr && m_ptr->getType() == ElementType::T_STRING) {
		num.clear();
		num.append(m_ptr->get());
		return true;
	}
	
	num.clear();
	return false;
}

bool Cervice::Obj::ObjectAuto::operator>>(double& num)
{
	if (m_ptr && m_ptr->getOption() == ElementOption::TO_INTEGER) {
		num = *((long long int*)m_ptr->get());
		return true;
	}

	else if (m_ptr && m_ptr->getOption() == ElementOption::TO_FLOAT) {
		num = *((double*)m_ptr->get());
		return true;
	}
	return false;
}

bool Cervice::Obj::ObjectAuto::operator>>(long long int& num)
{
	if (m_ptr && m_ptr->getOption() == ElementOption::TO_INTEGER) {
		num = *((long long int*)m_ptr->get());
		return true;
	}

	else if (m_ptr && m_ptr->getOption() == ElementOption::TO_FLOAT) {
		num = *((double*)m_ptr->get());
		return true;
	}
	return false;
}

Object* Cervice::Obj::ObjectAuto::getPtr() const
{
	return m_ptr;
}

ElementType Cervice::Obj::ObjectAuto::getType()
{
	return m_ptr->getType();
}

ElementOption Cervice::Obj::ObjectAuto::getOption()
{
	return m_ptr->getOption();
}

void Cervice::Obj::ObjectAuto::RemoveTable(Object* ptr, Object* parent)
{
	if (parent == nullptr)
		return;

	for (auto& parent : m_array_map) {
		if (parent.first != m_parent)
			continue;
		for (auto child = parent.second.begin(); child != parent.second.end(); child ++) {
			if (*child != ptr)
				continue;
			
			parent.second.erase(child);
			break;
		}

		break;
	}
}

void Cervice::Obj::ObjectAuto::AddTable(Object* ptr, Object* parent)
{
	if (parent == nullptr)
		return;

	for (auto& parent : m_array_map) {
		if (parent.first != m_parent)
			continue;

		parent.second.push_back(ptr);
		break;
	}
	return;
}
