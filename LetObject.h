#pragma once
#ifndef _AUTOMEM_LETOBJECT__
#define _AUTOMEM_LETOBJECT__

#include "letMacro.h"
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <iostream>
#include <filesystem>

namespace AutoMem {
	namespace Obj {

		namespace {
			bool StaticB = true;
			bool DynamicB = false;
		}

		class LetArrayMotion;
		class LetManager;
		class LetObject;

		using longPtr = long long int*;
		using doublePtr = long double*;
		using boolPtr = bool*;
		using nature = int;
		using numberT = long double;

		class BlockArray;

		/*************************
		* LetObject
		* ������ �û�ʹ�ñ���ԭ��
		*************************/
		class LetObject
		{
		public: // ���Ͷ���
			enum class ObjT	// ��������
			{
				null = 0,
				undef,
				boolean,
				number,
				string,
				array,
			};

			enum class ObjG { // ʶ����
				none,
				Static,
				Dynamic
			};

			struct ObjS { // ���ʶ��
				ObjT type;
				ObjG group;
			};

		public:
			LetObject(LetObject&&);
			LetObject(const LetObject&);

			LetObject(bool voi, bool type);
			LetObject();

			~LetObject();
		private:

		private:
			void create(bool type);
			void released();

			void changeBlock(bool type);
			bool isLess(ObjT, ObjT);
			bool isLessEq(ObjT, ObjT);
			bool isSameAs(ObjT, ObjT);

		public:
			LetObject* operator<<(long long int);
			LetObject* operator<<(int);
			LetObject* operator<<(long double);
			LetObject* operator<<(double);
			LetObject* operator<<(bool);

			LetObject* operator<<(const char*);
			LetObject* operator<<(std::string);

			template<typename T>
			bool operator>>(T& type);

			template<>
			bool operator>><std::string>(std::string& val);

			std::unordered_map<numberT, BlockArray> getNumArray();
			std::unordered_map<std::string, BlockArray> getStrArray();
			std::unordered_map<numberT, BlockArray> * getNumArrayPtr();
			std::unordered_map<std::string, BlockArray>* getStrArrayPtr();
			std::vector<LetObject> getIndexs();

		public:

			// ���������
			LetObject* operator=(LetObject*);
			LetObject& operator[](LetObject*);

			// ��Ŀ�����
			LetObject operator!();
			LetObject operator~();

			// ���������
			LetObject operator+(LetObject*);
			LetObject operator-(LetObject*);
			LetObject operator*(LetObject*);
			LetObject operator/(LetObject*);
			LetObject operator%(LetObject*);

			// ��λ�������
			LetObject operator<<(LetObject*);
			LetObject operator>>(LetObject*);


			// �Ƚ������
			LetObject operator>(LetObject*);
			LetObject operator<(LetObject*);
			LetObject operator>=(LetObject*);
			LetObject operator<=(LetObject*);
			LetObject operator!=(LetObject*);
			LetObject operator==(LetObject*);

			// �߼������
			LetObject operator&(LetObject*);
			LetObject operator^(LetObject*);
			LetObject operator|(LetObject*);
			LetObject operator&&(LetObject*);
			LetObject operator||(LetObject*);

		public:
			LetObject* operator=(LetObject&&);

			LetObject* operator=(LetObject&);

			LetObject& operator[](LetObject&&);

			LetObject& operator[](LetObject&);

			LetObject& operator[](const char* cstr);

			template<typename T>
			LetObject& operator[](T val);

			template<>
			LetObject& operator[]<std::string>(std::string val);

			LetObject operator+(LetObject&);
			LetObject operator+(LetObject&&);

			LetObject operator-(LetObject&);
			LetObject operator-(LetObject&&);

			LetObject operator*(LetObject&);
			LetObject operator*(LetObject&&);

			LetObject operator/(LetObject&);
			LetObject operator/(LetObject&&);

			LetObject operator%(LetObject&);
			LetObject operator%(LetObject&&);

			LetObject operator<<(LetObject&);
			LetObject operator<<(LetObject&&);

			LetObject operator>>(LetObject&);
			LetObject operator>>(LetObject&&);

			LetObject operator>(LetObject&);
			LetObject operator>(LetObject&&);

			LetObject operator<(LetObject&);
			LetObject operator<(LetObject&&);

			LetObject operator>=(LetObject&);
			LetObject operator>=(LetObject&&);

			LetObject operator<=(LetObject&);
			LetObject operator<=(LetObject&&);

			LetObject operator!=(LetObject&);
			LetObject operator!=(LetObject&&);

			LetObject operator==(LetObject&);
			LetObject operator==(LetObject&&);

			LetObject operator&(LetObject&);
			LetObject operator&(LetObject&&);

			LetObject operator^(LetObject&);
			LetObject operator^(LetObject&&);

			LetObject operator|(LetObject&);
			LetObject operator|(LetObject&&);

			LetObject operator&&(LetObject&);
			LetObject operator&&(LetObject&&);

			LetObject operator||(LetObject&);
			LetObject operator||(LetObject&&);

		public:
			template<typename castT>
			static castT cast(LetObject&);

			template<typename castT>
			static castT cast(LetObject*);

			static bool reference(LetObject* src, LetObject* des);

			template<>
			static std::string cast<std::string>(LetObject&);

			template<>
			static std::string cast<std::string>(LetObject*);

			void swap(LetObject&);

			bool isRef(LetObject* value);

			char* block();
			size_t lens();

		public:

			ObjT getType();
			ObjG getGroup();
			ObjS getStruct();

			nature getAttribute();
			void setAttribute(nature);
			nature getSelfAttribute();

		private:
			void setType(ObjS typ);

		private:
			Block* m_block = nullptr;					// �ڴ��
			LetManager* m_letM = nullptr;				// ������
			ObjS m_objType = { ObjT::null,ObjG::none };	// ����

			std::unordered_map<numberT, BlockArray> m_num_array;					// �����±�
			std::unordered_map<std::string, BlockArray> m_str_array;				// �����±�

			friend class LetArrayMotion;
		private:
			nature Attribute = 0;

		};

		class BlockArray : public LetObject {
		public: BlockArray() :LetObject(false, false) {}
		};

#ifdef LETOBJECT_AUTO
		using LetObject = LetObject;
		using autoPtr = LetObject*;
#endif // LETOBJECT_AUTO

		/*************************
		* LetTools
		* ������ ������ع���
		*************************/
		class LetTools
		{
		public:
			LetTools() {};
			~LetTools() {};

			enum class Operator
			{
				None = 0,
				Has,
				Type,
				Group,
				Nature,
				SelfNature,
			};

		public:
			void print(LetObject& value);														// ��ӡ
			void println(LetObject& value);													// ��ӡ���س�
			bool AutoCmp(std::pair<Operator,std::string> condition, LetObject& target);		// ���ԶԱ�
			void Swap(LetObject& value1, LetObject& value2);										// ͨ��move ����������ֵ
			bool IsRef(LetObject& ref_value, LetObject *value);									// ��� ref_vlaue �Ƿ����� value
			bool ToBin(LetObject& ref_value, std::string& bin_str, bool has_header = true);	// ����Ϊ16����
			bool FromBin(LetObject& ref_value, std::string& bin_str, bool has_header = true);	// 16����ת��Ϊ����
			bool SaveToBin(LetObject& ref_value, std::filesystem::path);						// �����16���Ʊ��浽�ļ�bin��
			bool ReadFromBin(LetObject& ref_value, std::filesystem::path);						// ���ļ�bin�н�������
			std::string ObjTToStr(LetObject::ObjT objt);									// ת��ObjT �� Str
			std::string ObjGToStr(LetObject::ObjG objg);									// ת��ObjG �� Str
			LetObject::ObjT StrToObjT(std::string objt_str);									// ת��Str �� ObjT
			LetObject::ObjG StrToObjG(std::string objg_str);									// ת��Str �� ObjG

			std::string toBigEndian(const char*, size_t);		// ת��Ϊ���
			std::string toLittleEndian(const char*, size_t);	// ת��ΪС��

		private:
			bool compileFileHeader(std::string&);				// �����ļ�ͷ
			bool compileData(std::string&, LetObject&);			// ������������
			bool compileDataObjS(std::string&, LetObject&);		// ���ݿ��ʶ
			bool compileDataAttribute(std::string&, LetObject&);	// ���ݿ�����
			bool compileDataNumAry(std::string&, LetObject&);		// ��������
			bool compileDataStrAry(std::string&, LetObject&);		// �ַ�����
			bool compileDataValue(std::string&, LetObject&);		// ����ֵ

			bool isBigEndian();

			bool analysisFileHeader(std::string&);				// �����ļ�ͷ
			size_t analysisFileSize(std::string&);				// �����ļ���С
			bool analysisData(std::string&, LetObject&);			// �����ļ���С
			bool analysisDataObjS(std::string&, LetObject&);		// ���ݿ��ʶ
			bool analysisDataAttribute(std::string&, LetObject&);	// ���ݿ�����
			bool analysisDataNumAry(std::string&, LetObject&);		// ��������
			bool analysisDataStrAry(std::string&, LetObject&);		// �ַ�����
			bool analysisDataValue(std::string&, LetObject&);		// ��ȡ����ֵ
		};

		using ToolsCond = std::pair<LetTools::Operator, std::string>;

#include "LetObject.tpp"

	}
}

#endif
