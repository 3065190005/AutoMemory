#ifndef _SCRIPTC_CEROBJECTDEF_
#define _SCRIPTC_CEROBJECTDEF_

// #define USEUTF8

#include <list>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace ScriptC
{
	namespace Obj
	{
		
		/*****************
		* �ڴ�鶨��
		******************/

		/* ������������ */
#ifndef USEUTF8 // �Ƿ�ʹ��utf-8
		using String = std::string;
#else
		using String = std::wstring;
#endif
		/* C++�������Ͷ��� */
		using USize = size_t;
		using LDouble = long double;
		using Boolean = bool;
		using VoidPtr = void*;
		
		/* List���Ͷ��� */
		template<typename T>
		using List = std::list<T>;

		/* Vector���Ͷ��� */
		template<typename T>
		using Vector = std::vector<T>;

		/* Map���Ͷ��� */
		template<typename Fir, typename Sec>
		using Map = std::map<Fir, Sec>;

		/* Pair ���Ͷ��� */
		template<typename Fir, typename Sec>
		using Pair = std::pair<Fir, Sec>;

		/* StringPair���Ͷ��� */
		template<typename T>
		using StringPair = Pair<String, T>;

		/* �ڴ������ */
		enum ObjectType
		{
			null,
			undef,
			boolean,
			number,
			string,
			function,
			array,
			structure,
		};

		/* function���ͽṹ�屾�� */
		struct FunctionStruct
		{
			String m_name;
			USize m_param_size;
		};

		/* ԭ���ڴ�� */
		struct ObjectStruct
		{
		public:
			using ObjectList = List<ObjectStruct>;
			using ObjectRefer = ObjectList::iterator;
			using StructPair = Pair<String, ObjectRefer>;

			using ArrayType = Vector<ObjectRefer>;
			using StructType = Map<String, ObjectRefer>;

			using NumberT = LDouble;
			using BooleanT = Boolean;
			using UndefT = VoidPtr;
			using StringT = String;
			using FunctionT = FunctionStruct;
			using ArrayT = ArrayType;
			using StructT = StructType;

			UndefT m_undef;
			BooleanT m_bool;
			NumberT m_number;
			StringT m_string;
			FunctionT m_function;
			ArrayT m_array;
			StructT m_struct;

		public:
			USize m_ref{ 0 };
			ObjectType m_type{ ObjectType::null };
		};

		/* �����ڴ��� */
		using ObjectList = ObjectStruct::ObjectList;

		/* �ڴ��������� */
		using ObjectRefer = ObjectStruct::ObjectRefer;

		/* �����ض��� */
		using UndefT = ObjectStruct::UndefT;
		using BooleanT = ObjectStruct::BooleanT;
		using NumberT = ObjectStruct::NumberT;
		using StringT = ObjectStruct::StringT;
		using FunctionT = ObjectStruct::FunctionT;
		using ArrayT = ObjectStruct::ArrayT;
		using StructT = ObjectStruct::StructT;
		using StructPair = ObjectStruct::StructPair;

		/* ������������callback */
		using DropCallBack = std::function<BooleanT(ObjectRefer ref)>;

		/* throw����callback */
		using ErrorCallBack = std::function<void(String)>;

		/* �ڴ������� */
		struct MemeryStruct
		{
		public:
			ObjectList m_null;
			ObjectList m_undef;
			ObjectList m_boolean;
			ObjectList m_number;
			ObjectList m_string;
			ObjectList m_function;
			ObjectList m_array;
			ObjectList m_structure;
		};

		/*****************
		* debug���Ͷ���
		******************/

		using MemeryInfo = Map<String, USize>;
	}
}


#endif