#pragma once
#ifndef _OBJECT_MARCO_CERVER
#define _OBJECT_MARCO_CERVER


#define DebugObjectLog false
#define DebugObjectTitle true

#if DebugObjectLog && GlobalDebugOpend
#define objlog(var)\
std::cout << var
#else
#define objlog(var)
#endif

#define ClassCalcAll(type_class)\
ClassAssignAll(type_class)\
ClassAssignInOutAll(type_class)\

#define ClassAssignAll(type_class)\
ClassAssignInterface(type_class)\
ClassAssignFloat(type_class)\
ClassAssignLong(type_class)\
ClassAssignBool(type_class)\
ClassAssignShort(type_class)\
ClassAssignInt(type_class)\
ClassAssignVoid(type_class)

#define ClassAssignInOutAll(type_class)\
ClassAssignOut(type_class)\
ClassAssignIn(type_class)\
ClassAssignInConst(type_class)

#define ClassAssignInterface(type_class)\
type_class& assign(Object* node) {\
	Object::operator=(node);\
	return *this;\
}

// ���� ���� = double
#define ClassAssignFloat(type_class) \
virtual type_class& operator=(ObjectFloat& node)\
{ return assign((Object*)&node); }

// ���� ���� = long
#define ClassAssignLong(type_class) \
virtual type_class& operator=(ObjectLong& node)\
{ return assign((Object*)&node); }

// ���� ���� = bool
#define ClassAssignBool(type_class) \
virtual type_class& operator=(ObjectBool& node)\
{ return assign((Object*)&node); }

// ���� ���� = short
#define ClassAssignShort(type_class)\
virtual type_class& operator=(ObjectShort& node)\
{ return assign((Object*)&node); }

// ���� ���� = int
#define ClassAssignInt(type_class)\
virtual type_class& operator=(ObjectInt& node)\
{ return assign((Object*)&node); }

// ���� ���� = void
#define ClassAssignVoid(type_class)\
virtual type_class& operator=(ObjectVoid& node) \
{ return assign((Object*)&node); }

// ���� ���� = string
#define ClassAssignString(type_class)\
virtual type_class& operator=(ObjectString& node) \
{ return assign((Object*)&node); }

// ���� ���� >> ��ȡֵ �� ��������
#define ClassAssignOut(type_class)\
virtual type_class& operator>>(long long& val)\
{ val = Get_Element(); return *this; }\
virtual type_class& operator>>(double& val)\
{ val = Get_Element(); return *this; }

// ���� ���� << ����ֵ �������� ��ȡ ֵ
#define ClassAssignIn(type_class)\
virtual type_class& operator<<(long long& val)\
{ Set_Element(val); return *this; }\
virtual type_class& operator<<(double& val)\
{ Set_Element(val); return *this; }

// ���� ���� << ����ֵ �������� ��ȡ ֵ
#define ClassAssignInConst(type_class)\
virtual type_class& operator<<(long long&& val)\
{ Set_Element(val); return *this; }\
virtual type_class& operator<<(double&& val)\
{ Set_Element(val); return *this; }

#endif