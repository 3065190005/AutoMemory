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
		* 内存块定义
		******************/

		/* 基础类型命名 */
#ifndef USEUTF8 // 是否使用utf-8
		using String = std::string;
#else
		using String = std::wstring;
#endif
		/* C++基础类型定义 */
		using USize = size_t;
		using LDouble = long double;
		using Boolean = bool;
		using VoidPtr = void*;
		
		/* List类型定义 */
		template<typename T>
		using List = std::list<T>;

		/* Vector类型定义 */
		template<typename T>
		using Vector = std::vector<T>;

		/* Map类型定义 */
		template<typename Fir, typename Sec>
		using Map = std::map<Fir, Sec>;

		/* Pair 类型定义 */
		template<typename Fir, typename Sec>
		using Pair = std::pair<Fir, Sec>;

		/* StringPair类型定义 */
		template<typename T>
		using StringPair = Pair<String, T>;

		/* 内存块类型 */
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

		/* function类型结构体本体 */
		struct FunctionStruct
		{
			String m_name;
			USize m_param_size;
		};

		/* 原子内存块 */
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

		/* 单个内存流 */
		using ObjectList = ObjectStruct::ObjectList;

		/* 内存流迭代器 */
		using ObjectRefer = ObjectStruct::ObjectRefer;

		/* 类型重定义 */
		using UndefT = ObjectStruct::UndefT;
		using BooleanT = ObjectStruct::BooleanT;
		using NumberT = ObjectStruct::NumberT;
		using StringT = ObjectStruct::StringT;
		using FunctionT = ObjectStruct::FunctionT;
		using ArrayT = ObjectStruct::ArrayT;
		using StructT = ObjectStruct::StructT;
		using StructPair = ObjectStruct::StructPair;

		/* 死亡变量回收callback */
		using DropCallBack = std::function<BooleanT(ObjectRefer ref)>;

		/* throw操作callback */
		using ErrorCallBack = std::function<void(String)>;

		/* 内存流集合 */
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
		* debug类型定义
		******************/

		using MemeryInfo = Map<String, USize>;
	}
}


#endif