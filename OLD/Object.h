#pragma once
#ifndef _OBJECT_CERVICE_
#define _OBJECT_CERVICE_

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include "ObjectMacro.h"

namespace Cervice {
	namespace Obj {

		class Object;
		class ObjectVoid;
		class ObjectLong;
		class ObjectBool;
		class ObjectShort;
		class ObjectInt;
		class ObjectFloat;
		class ObjectString;
		class ObjectArray;

		typedef ObjectVoid void_c;
		typedef ObjectLong long_c;
		typedef ObjectBool bool_c;
		typedef ObjectShort short_c;
		typedef ObjectInt int_c;
		typedef ObjectFloat float_c;
		typedef ObjectString string_c;
		typedef ObjectArray array_c;

		struct _MemPoint {
			unsigned index = 0;
			size_t lenght = 0;
			unsigned ref = 0;
			unsigned type = 0;
			char* block = nullptr;
		};

		typedef _MemPoint MemPoint;
		typedef std::list<MemPoint*>::iterator Ptr;

		// 创建内存大小及类型
		enum class ElementType {
			T_VOID = 0,
			T_BOOL = 1,
			T_WORD = 2,
			T_DWORD = 4,
			T_NUMBER = 8,
			T_STRING = 9,
			T_ARRAY = 10
		};

		// 子标识
		enum class ElementOption {
			TO_INTEGER = 1,
			TO_FLOAT	= 2,
			TO_OTHER = 3,
		};

		/**************
		* class Object 
		* 基类实现
		**************/

		class Object {
		protected:
			static unsigned long m_MemoryLength;				// 总内存长度
			static std::list<MemPoint*> AllBlock_Free;			// 剩余空间固定长度8
			static std::list<MemPoint*> AllBlock_Free_dynamic;	// 剩余空间固定长度8
			static std::list<MemPoint*> AllBlock_Malloc;		// 使用空间
			static std::vector<Object*> AllObject_Pool;			// 池
			static Ptr NullPtr;									// list.end()

		public:
			static void InitMemoryPool(int length);	// 初始化总内存大小
			static void ReleaseMemoryPool();		// 释放内存

			// 映射池创建元素
			static Object* createObj(ElementType type, ElementOption option = ElementOption::TO_INTEGER);
			static Object* createObj(Object* copy);
			static Object* createObjNoPtr(Object* copy);

			// 映射池销毁元素
			static void releaseObj(Object**);

			Object(ElementType type);
			~Object();

		protected:
			void create(ElementType type);		// 创建分配内存对应映射表元素
			void destory();						// 删除释放内存对应映射表元素

			char* getEleIndex(Ptr index); // 获取数组指针

		public:
			virtual Object* operator=(Object* node);	// 重载 指针 =
			virtual Object* operator=(Object& node);	// 重载 引用 =
			virtual Object* operator+(Object* node);	// 重载 指针 +
			virtual Object* operator-(Object* node);	// 重载 指针 -
			virtual Object* operator*(Object* node);	// 重载 指针 *
			virtual Object* operator/(Object* node);	// 重载 指针 /

			virtual Object* operator[](Object* node);	// 重载 指针 []

		public:
			virtual void set(char* point) = 0;
			virtual char* get() = 0;


			virtual void setZero() = 0;			// 清空内存
			virtual void fullMem() = 0;			// 设置最大值

			ElementType getType();
			ElementOption getOption();

			virtual std::map<double, Object*>* getNumberArray();
			virtual std::map<std::string, Object*>* getStrArray();

		private:
			void Free(Ptr);
			Ptr Malloc(bool dynamic);
			Ptr searchBlocks(Ptr*);

		protected:
			Ptr m_val;					// 映射表句柄
			ElementType m_class_type;	// Object类型
			unsigned int m_pool_ref;	// 引用下标

			std::map<double, Object*> m_number_array;	// 数组容器 数字下标
			std::map<std::string, Object*> m_str_array;	// 数组容器 字符下标

			size_t m_option;		// 0 = number, 1 = float
		};

		using ObjectPtr = Object**;

		/******************************
		* class ObjectVoid: public Object
		* void 类型实现
		******************************/

		class ObjectVoid : public Object {
		public:
			ObjectVoid();
			~ObjectVoid();

		private:
			void set(char* point) override {};				// 禁止设置void变量
			char* get() override { return nullptr; };		// 禁止获取void变量

			void setZero() override {};			// 清空内存
			void fullMem() override {};			// 设置最大值
		};

		/******************************
		* class ObjectLong: public Object
		* long 类型实现
		******************************/

		class ObjectLong : public Object {
		public:
			ObjectLong();
			ObjectLong(long long);
			~ObjectLong();

		public:
			ClassCalcAll(ObjectLong);			// 符号重载

		public:
			void set(char* point) override;		// 设置long变量
			char* get()override;				// 获取long变量

		public:
			void setZero() override;			// 清空内存
			void fullMem() override;			// 设置最大值

		public:
			void Set_Element(long long int number);	// 设置变量 封装
			long long int Get_Element();			// 获取变量 封装
		};

		/******************************
		* class ObjectBool: public Object
		* bool 类型实现
		******************************/

		class ObjectBool : public Object {
		public:
			ObjectBool();
			ObjectBool(bool);
			~ObjectBool();

		public:
			ClassCalcAll(ObjectBool);			// 符号重载

		public:
			void set(char* point) override;		// 设置bool变量
			char* get()override;				// 获取bool变量

		public:

			void setZero() override;			// 清空内存
			void fullMem() override;			// 设置最大值

		public:
			void Set_Element(bool byte);		// 设置bool变量 封装
			bool Get_Element();					// 获取bool变量 封装

		};

		/******************************
		* class ObjectShort: public Object
		* word 双字节 类型实现
		******************************/

		class ObjectShort : public Object {
		public:
			ObjectShort();
			ObjectShort(short);
			~ObjectShort();

		public:
			ClassCalcAll(ObjectShort);			// 符号重载

		public:
			void set(char* point) override;		// 设置short变量
			char* get()override;				// 获取short变量

		public:

			void setZero() override;			// 清空内存
			void fullMem() override;			// 设置最大值

		public:
			void Set_Element(short byte);		// 设置short变量 封装
			short Get_Element();				// 获取short变量 封装

		};

		/******************************
		* class ObjectInt: public Object
		* dword 双字节 类型实现
		******************************/

		class ObjectInt : public Object {
		public:
			ObjectInt();
			ObjectInt(int);
			~ObjectInt();

		public:
			ClassCalcAll(ObjectInt);			// 符号重载

		public:
			void set(char* point) override;		// 设置int变量
			char* get()override;				// 获取int变量

		public:

			void setZero() override;			// 清空内存
			void fullMem() override;			// 设置最大值

		public:
			void Set_Element(int byte);		// 设置int变量 封装
			int Get_Element();				// 获取int变量 封装

		};

		/******************************
		* class ObjectFloat: public Object
		* 浮点 双字节 类型实现
		******************************/

		class ObjectFloat : public Object {
		public:
			ObjectFloat();
			ObjectFloat(double);
			~ObjectFloat();

		public:
			ClassCalcAll(ObjectFloat);			// 符号重载

		public:
			void set(char* point) override;		// 设置double变量
			char* get()override;				// 获取double变量

		public:

			void setZero() override;			// 清空内存
			void fullMem() override;			// 设置最大值

		public:
			void Set_Element(double byte);		// 设置double变量 封装
			double Get_Element();				// 获取double变量 封装

		};

		/******************************
		* class ObjectString: public Object
		* 字符串 类型实现
		******************************/

		class ObjectString : public Object {
		public:
			ObjectString();
			ObjectString(const char * str);
			~ObjectString();

		public:
			Object* operator=(Object* node) override;
			Object* operator=(Object& node) override;
			Object* operator+(Object* node) override;

		public:
			void set(char* point) override;		// 设置double变量
			char* get()override;				// 获取double变量

		public:

			void setZero() override;			// 清空内存
			void fullMem() override;			// 设置最大值

		public:
			void Set_Element(char* byte);	// 设置double变量 封装
			std::string Get_Element();			// 获取double变量 封装

		};

		/******************************
		* class ObjectArray: public Object
		* 数组类型实现
		******************************/

		class ObjectArray : public Object {
		public:
			ObjectArray();
			~ObjectArray();

		public:
			Object* operator=(Object* node) override;
			Object* operator=(Object& node) override;
			Object* operator+(Object* node) override;

		public:
			void set(char* point) override;		// 返回下标数字0的set
			char* get()override;				// 返回下标数字0的get

			virtual std::map<double, Object*>* getNumberArray() override;		// 获取m_array_number
			virtual std::map<std::string, Object*>* getStrArray() override;		// 获取m_array_str

		public:

			void setZero() override;			// 清空内存
			void fullMem() override;			// 设置最大值

		private:
			void Set_Element(char* byte);		// 数组禁用Set_Element
			std::string Get_Element();			// 数组禁用Get_Element

		};

	};

};

#endif