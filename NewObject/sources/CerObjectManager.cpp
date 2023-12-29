#include "CerObjectManager.h"
#include <algorithm>

#ifndef _SCRIPTC_GLOBAL_DEFINE_
    #define CLASS_NAME "_class_name"			        // 接口变量的默认接口变量名
    #define CLASS_PARENT "_class_parent"		        // 接口变量的默认父接口变量名
    #define CLASS_AUTO_GC_CALL "_class_auto_gc_call"	// 特殊函数Gc是否开启调用变量名
#endif


namespace ScriptC
{
	namespace Obj
	{
		CerObjectManager* CerObjectManager::m_instance = nullptr;

		DropCallBack CerObjectManager::m_deadcall = [](ObjectRefer) {return true; };
		ErrorCallBack CerObjectManager::m_errorcall = [](String str) {throw(str); };

		CerObjectManager* Obj::CerObjectManager::getInstance()
		{
			if (m_instance == nullptr)
			{
				auto temp = new (std::nothrow) CerObjectManager();
				m_instance = temp;
			}

			return m_instance;
		}

		void CerObjectManager::throwError(String str)
		{
			m_errorcall(str);
		}

		void CerObjectManager::setDropCallBack(DropCallBack callback)
		{
			getInstance();

			m_deadcall = callback;
		}

		void CerObjectManager::setErrorCallBack(ErrorCallBack callback)
		{
			m_errorcall = callback;
		}

		/* 创建不同类型的 Ref */
		ObjectRefer CerObjectManager::createRef()
		{
			ObjectStruct null_ref;
			null_ref.m_type = ObjectType::null;

			m_memerys.push_front(null_ref);
			return m_memerys.begin();
		}

		ObjectRefer CerObjectManager::createRef(UndefT undef_var)
		{
			ObjectStruct undef_ref;
			undef_ref.m_undef = undef_var;
			undef_ref.m_type = ObjectType::undef;

			m_memerys.push_front(undef_ref);
			return m_memerys.begin();
		}

		ObjectRefer CerObjectManager::createRef(BooleanT bool_var)
		{
			ObjectStruct bool_ref;
			bool_ref.m_bool = bool_var;
			bool_ref.m_type = ObjectType::boolean;

			m_memerys.push_front(bool_ref);
			return m_memerys.begin();
		}

		ObjectRefer CerObjectManager::createRef(NumberT number_var)
		{
			ObjectStruct number_ref;
			number_ref.m_number = number_var;
			number_ref.m_type = ObjectType::number;

			m_memerys.push_front(number_ref);
			return m_memerys.begin();
		}

		ObjectRefer Obj::CerObjectManager::createRef(StringT string_var)
		{
			ObjectStruct string_ref;
			string_ref.m_string = string_var;
			string_ref.m_type = ObjectType::string;

			m_memerys.push_front(string_ref);
			return m_memerys.begin();
		}

		ObjectRefer Obj::CerObjectManager::createRef(FunctionT function_var)
		{
			ObjectStruct function_ref;
			function_ref.m_function = function_var;
			function_ref.m_type = ObjectType::function;

			m_memerys.push_front(function_ref);
			return m_memerys.begin();
		}

		ObjectRefer Obj::CerObjectManager::createRef(ArrayT array_var)
		{
			ObjectStruct array_ref;

			// array_ref.m_array = array_var;
			array_ref.m_array = arrayCopy(array_var);
			array_ref.m_type = ObjectType::array;

			m_memerys.push_front(array_ref);
			return m_memerys.begin();
		}

		ObjectRefer CerObjectManager::createRef(StructT struct_var)
		{
			ObjectStruct struct_ref;

			// struct_ref.m_struct = struct_var;
			struct_ref.m_struct = structCopy(struct_var);
			struct_ref.m_type = ObjectType::structure;

			m_memerys.push_front(struct_ref);
			return m_memerys.begin();
		}


		ObjectRefer CerObjectManager::createRef(StructT, ObjectRefer refer)
		{
			return refer;
		}

		/* 创建不同类型的 ObjectStruct */
		ObjectStruct CerObjectManager::createStruct()
		{
			ObjectStruct null_struct;
			null_struct.m_type = ObjectType::null;
			return null_struct;
		}

		ObjectStruct CerObjectManager::createStruct(UndefT undef_var)
		{
			ObjectStruct undef_struct;
			undef_struct.m_undef = undef_var;
			undef_struct.m_type = ObjectType::undef;
			return undef_struct;
		}

		ObjectStruct CerObjectManager::createStruct(BooleanT bool_var)
		{
			ObjectStruct bool_struct;
			bool_struct.m_bool = bool_var;
			bool_struct.m_type = ObjectType::boolean;
			return bool_struct;
		}

		ObjectStruct CerObjectManager::createStruct(NumberT number_var)
		{
			ObjectStruct number_struct;
			number_struct.m_number = number_var;
			number_struct.m_type = ObjectType::number;
			return number_struct;
		}

		ObjectStruct CerObjectManager::createStruct(StringT string_var)
		{
			ObjectStruct string_struct;
			string_struct.m_string = string_var;
			string_struct.m_type = ObjectType::string;
			return string_struct;
		}

		ObjectStruct CerObjectManager::createStruct(FunctionT function_var)
		{
			ObjectStruct function_struct;
			function_struct.m_function = function_var;
			function_struct.m_type = ObjectType::function;
			return function_struct;
		}

		ObjectStruct CerObjectManager::createStruct(ArrayT array_var, ObjectRefer refer)
		{
			ObjectStruct array_struct;

			// array_struct.m_array = array_var;
			array_struct.m_array = arrayCopy(array_var);
			array_struct.m_type = ObjectType::array;
			return array_struct;
		}

		ObjectStruct CerObjectManager::createStruct(StructT struct_var, ObjectRefer refer)
		{
			ObjectStruct struct_struct;

			// struct_struct.m_struct = struct_var;
			struct_struct.m_struct = structCopy(struct_var);

			struct_struct.m_type = ObjectType::structure;
			return struct_struct;
		}

		/* Array类型的Copy */
		ArrayT Obj::CerObjectManager::arrayCopy(ArrayT array_var)
		{
			ArrayT ret_array;
			
			for (auto& i : array_var)
			{
				switch (i->m_type)
				{
				case ScriptC::Obj::null:
					ret_array.push_back(createRef());
					break;
				case ScriptC::Obj::undef:
					ret_array.push_back(createRef(i->m_undef));
					break;
				case ScriptC::Obj::boolean:
					ret_array.push_back(createRef(i->m_bool));
					break;
				case ScriptC::Obj::number:
					ret_array.push_back(createRef(i->m_number));
					break;
				case ScriptC::Obj::string:
					ret_array.push_back(createRef(i->m_string));
					break;
				case ScriptC::Obj::function:
					ret_array.push_back(createRef(i->m_function));
					break;
				case ScriptC::Obj::array:
					ret_array.push_back(createRef(i->m_array));
					break;
				case ScriptC::Obj::structure:
					ret_array.push_back(createRef(i->m_struct, i));
					break;
				default:
					throwError("unrealized or unknow");
					break;
				}
			}

			return ret_array;
		}

		StructT Obj::CerObjectManager::structCopy(StructT struct_var)
		{
			return struct_var;
		}

		ObjectRefer CerObjectManager::leaveRef(ObjectRefer refer)
		{
			if(refer->m_ref > 0)
				refer->m_ref--;

			switch (refer->m_type)
			{
			case ScriptC::Obj::array:
				for (auto& i : refer->m_array)
					i = leaveRef(i);
				break;
			case ScriptC::Obj::structure:
				for (auto& i : refer->m_struct)
					i.second = leaveRef(i.second);
				break;
			default:
				break;
			}

			return destoryRef(refer);
		}

		ObjectRefer CerObjectManager::destoryRef(ObjectRefer refer)
		{
			if (refer->m_ref > 0)
				return refer;

			auto& memery_list = m_memerys;

			m_deadcall(refer);

			m_dead_memery.emplace_front(std::move(*refer));
			memery_list.erase(refer);
			return m_dead_memery.begin();
		}

		ObjectRefer Obj::CerObjectManager::joinRef(ObjectRefer refer)
		{
			refer->m_ref++;

			switch (refer->m_type)
			{
			case ScriptC::Obj::array:
				for (auto& i : refer->m_array)
					i = joinRef(i);
				break;
			case ScriptC::Obj::structure:
				for (auto& i : refer->m_struct)
					i.second = joinRef(i.second);
				break;
			default:
				break;
			}

			return refer;
		}


		const ObjectList& Obj::CerObjectManager::getMemerys() const
		{
			return m_memerys;
		}

		const ObjectList& Obj::CerObjectManager::getDeadMemery() const
		{
			return m_dead_memery;
		}

		ObjectStruct CerObjectManager::assigment(ObjectRefer refer)
		{
			ObjectStruct ret_struct;
			switch (refer->m_type)
			{
			case ScriptC::Obj::null:
				return createStruct();
			case ScriptC::Obj::undef:
				return createStruct(refer->m_undef);
				break;
			case ScriptC::Obj::boolean:
				return createStruct(refer->m_bool);
				break;
			case ScriptC::Obj::number:
				return createStruct(refer->m_number);
				break;
			case ScriptC::Obj::string:
				return createStruct(refer->m_string);
				break;
			case ScriptC::Obj::function:
				return createStruct(refer->m_function);
				break;
			case ScriptC::Obj::array:
				return createStruct(refer->m_array, refer);
				break;
			case ScriptC::Obj::structure:
				return createStruct(refer->m_struct, refer);
				break;
			default:
				throwError("assigment == deafult");
				break;
			}

			return ret_struct;
		}

		ObjectRefer CerObjectManager::setReferStruct(ObjectRefer refer, ObjectStruct&& object_struct)
		{
			USize ref = refer->m_ref;

			ObjectRefer temp = joinRef(createRef());
			*temp = std::move(*refer);

			while (temp->m_ref != 0)
				temp = leaveRef(temp);
			
			*refer = object_struct;
			refer = joinRef(refer);
			refer->m_ref = ref;
			return refer;
		}

		void Obj::CerObjectManager::dropDead()
		{
			while (m_dead_memery.begin() != m_dead_memery.end())
				m_dead_memery.erase(m_dead_memery.begin());
		}
	}
}