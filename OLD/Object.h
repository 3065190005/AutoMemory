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

		// �����ڴ��С������
		enum class ElementType {
			T_VOID = 0,
			T_BOOL = 1,
			T_WORD = 2,
			T_DWORD = 4,
			T_NUMBER = 8,
			T_STRING = 9,
			T_ARRAY = 10
		};

		// �ӱ�ʶ
		enum class ElementOption {
			TO_INTEGER = 1,
			TO_FLOAT	= 2,
			TO_OTHER = 3,
		};

		/**************
		* class Object 
		* ����ʵ��
		**************/

		class Object {
		protected:
			static unsigned long m_MemoryLength;				// ���ڴ泤��
			static std::list<MemPoint*> AllBlock_Free;			// ʣ��ռ�̶�����8
			static std::list<MemPoint*> AllBlock_Free_dynamic;	// ʣ��ռ�̶�����8
			static std::list<MemPoint*> AllBlock_Malloc;		// ʹ�ÿռ�
			static std::vector<Object*> AllObject_Pool;			// ��
			static Ptr NullPtr;									// list.end()

		public:
			static void InitMemoryPool(int length);	// ��ʼ�����ڴ��С
			static void ReleaseMemoryPool();		// �ͷ��ڴ�

			// ӳ��ش���Ԫ��
			static Object* createObj(ElementType type, ElementOption option = ElementOption::TO_INTEGER);
			static Object* createObj(Object* copy);
			static Object* createObjNoPtr(Object* copy);

			// ӳ�������Ԫ��
			static void releaseObj(Object**);

			Object(ElementType type);
			~Object();

		protected:
			void create(ElementType type);		// ���������ڴ��Ӧӳ���Ԫ��
			void destory();						// ɾ���ͷ��ڴ��Ӧӳ���Ԫ��

			char* getEleIndex(Ptr index); // ��ȡ����ָ��

		public:
			virtual Object* operator=(Object* node);	// ���� ָ�� =
			virtual Object* operator=(Object& node);	// ���� ���� =
			virtual Object* operator+(Object* node);	// ���� ָ�� +
			virtual Object* operator-(Object* node);	// ���� ָ�� -
			virtual Object* operator*(Object* node);	// ���� ָ�� *
			virtual Object* operator/(Object* node);	// ���� ָ�� /

			virtual Object* operator[](Object* node);	// ���� ָ�� []

		public:
			virtual void set(char* point) = 0;
			virtual char* get() = 0;


			virtual void setZero() = 0;			// ����ڴ�
			virtual void fullMem() = 0;			// �������ֵ

			ElementType getType();
			ElementOption getOption();

			virtual std::map<double, Object*>* getNumberArray();
			virtual std::map<std::string, Object*>* getStrArray();

		private:
			void Free(Ptr);
			Ptr Malloc(bool dynamic);
			Ptr searchBlocks(Ptr*);

		protected:
			Ptr m_val;					// ӳ�����
			ElementType m_class_type;	// Object����
			unsigned int m_pool_ref;	// �����±�

			std::map<double, Object*> m_number_array;	// �������� �����±�
			std::map<std::string, Object*> m_str_array;	// �������� �ַ��±�

			size_t m_option;		// 0 = number, 1 = float
		};

		using ObjectPtr = Object**;

		/******************************
		* class ObjectVoid: public Object
		* void ����ʵ��
		******************************/

		class ObjectVoid : public Object {
		public:
			ObjectVoid();
			~ObjectVoid();

		private:
			void set(char* point) override {};				// ��ֹ����void����
			char* get() override { return nullptr; };		// ��ֹ��ȡvoid����

			void setZero() override {};			// ����ڴ�
			void fullMem() override {};			// �������ֵ
		};

		/******************************
		* class ObjectLong: public Object
		* long ����ʵ��
		******************************/

		class ObjectLong : public Object {
		public:
			ObjectLong();
			ObjectLong(long long);
			~ObjectLong();

		public:
			ClassCalcAll(ObjectLong);			// ��������

		public:
			void set(char* point) override;		// ����long����
			char* get()override;				// ��ȡlong����

		public:
			void setZero() override;			// ����ڴ�
			void fullMem() override;			// �������ֵ

		public:
			void Set_Element(long long int number);	// ���ñ��� ��װ
			long long int Get_Element();			// ��ȡ���� ��װ
		};

		/******************************
		* class ObjectBool: public Object
		* bool ����ʵ��
		******************************/

		class ObjectBool : public Object {
		public:
			ObjectBool();
			ObjectBool(bool);
			~ObjectBool();

		public:
			ClassCalcAll(ObjectBool);			// ��������

		public:
			void set(char* point) override;		// ����bool����
			char* get()override;				// ��ȡbool����

		public:

			void setZero() override;			// ����ڴ�
			void fullMem() override;			// �������ֵ

		public:
			void Set_Element(bool byte);		// ����bool���� ��װ
			bool Get_Element();					// ��ȡbool���� ��װ

		};

		/******************************
		* class ObjectShort: public Object
		* word ˫�ֽ� ����ʵ��
		******************************/

		class ObjectShort : public Object {
		public:
			ObjectShort();
			ObjectShort(short);
			~ObjectShort();

		public:
			ClassCalcAll(ObjectShort);			// ��������

		public:
			void set(char* point) override;		// ����short����
			char* get()override;				// ��ȡshort����

		public:

			void setZero() override;			// ����ڴ�
			void fullMem() override;			// �������ֵ

		public:
			void Set_Element(short byte);		// ����short���� ��װ
			short Get_Element();				// ��ȡshort���� ��װ

		};

		/******************************
		* class ObjectInt: public Object
		* dword ˫�ֽ� ����ʵ��
		******************************/

		class ObjectInt : public Object {
		public:
			ObjectInt();
			ObjectInt(int);
			~ObjectInt();

		public:
			ClassCalcAll(ObjectInt);			// ��������

		public:
			void set(char* point) override;		// ����int����
			char* get()override;				// ��ȡint����

		public:

			void setZero() override;			// ����ڴ�
			void fullMem() override;			// �������ֵ

		public:
			void Set_Element(int byte);		// ����int���� ��װ
			int Get_Element();				// ��ȡint���� ��װ

		};

		/******************************
		* class ObjectFloat: public Object
		* ���� ˫�ֽ� ����ʵ��
		******************************/

		class ObjectFloat : public Object {
		public:
			ObjectFloat();
			ObjectFloat(double);
			~ObjectFloat();

		public:
			ClassCalcAll(ObjectFloat);			// ��������

		public:
			void set(char* point) override;		// ����double����
			char* get()override;				// ��ȡdouble����

		public:

			void setZero() override;			// ����ڴ�
			void fullMem() override;			// �������ֵ

		public:
			void Set_Element(double byte);		// ����double���� ��װ
			double Get_Element();				// ��ȡdouble���� ��װ

		};

		/******************************
		* class ObjectString: public Object
		* �ַ��� ����ʵ��
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
			void set(char* point) override;		// ����double����
			char* get()override;				// ��ȡdouble����

		public:

			void setZero() override;			// ����ڴ�
			void fullMem() override;			// �������ֵ

		public:
			void Set_Element(char* byte);	// ����double���� ��װ
			std::string Get_Element();			// ��ȡdouble���� ��װ

		};

		/******************************
		* class ObjectArray: public Object
		* ��������ʵ��
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
			void set(char* point) override;		// �����±�����0��set
			char* get()override;				// �����±�����0��get

			virtual std::map<double, Object*>* getNumberArray() override;		// ��ȡm_array_number
			virtual std::map<std::string, Object*>* getStrArray() override;		// ��ȡm_array_str

		public:

			void setZero() override;			// ����ڴ�
			void fullMem() override;			// �������ֵ

		private:
			void Set_Element(char* byte);		// �������Set_Element
			std::string Get_Element();			// �������Get_Element

		};

	};

};

#endif