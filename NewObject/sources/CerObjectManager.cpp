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
		CerObjectManager* Obj::CerObjectManager::getInstance()
		{
			if (m_instance == nullptr)
			{
				auto temp = new (std::nothrow) CerObjectManager();
				m_instance = temp;
				m_instance->m_callback = [&](ObjectRefer) {return true; };
			}

			return m_instance;
		}


		ObjectRefer CerObjectManager::createRef()
		{
			ObjectStruct null_ref;
			null_ref.m_type = ObjectType::null;

			m_memerys.m_null.push_front(null_ref);
			return m_memerys.m_null.begin();
		}

		ObjectRefer CerObjectManager::createRef(UndefT undef_var)
		{
			ObjectStruct undef_ref;
			undef_ref.m_undef = undef_var;
			undef_ref.m_type = ObjectType::undef;

			m_memerys.m_undef.push_front(undef_ref);
			return m_memerys.m_undef.begin();
		}

		ObjectRefer CerObjectManager::createRef(BooleanT bool_var)
		{
			ObjectStruct bool_ref;
			bool_ref.m_bool = bool_var;
			bool_ref.m_type = ObjectType::boolean;

			m_memerys.m_boolean.push_front(bool_ref);
			return m_memerys.m_boolean.begin();
		}

		ObjectRefer CerObjectManager::createRef(NumberT number_var)
		{
			ObjectStruct number_ref;
			number_ref.m_number = number_var;
			number_ref.m_type = ObjectType::number;

			m_memerys.m_number.push_front(number_ref);
			return m_memerys.m_number.begin();
		}

		ObjectRefer Obj::CerObjectManager::createRef(StringT string_var)
		{
			ObjectStruct string_ref;
			string_ref.m_string = string_var;
			string_ref.m_type = ObjectType::string;

			m_memerys.m_string.push_front(string_ref);
			return m_memerys.m_string.begin();
		}

		ObjectRefer Obj::CerObjectManager::createRef(FunctionT function_var)
		{
			ObjectStruct function_ref;
			function_ref.m_function = function_var;
			function_ref.m_type = ObjectType::function;

			m_memerys.m_function.push_front(function_ref);
			return m_memerys.m_function.begin();
		}

		ObjectRefer Obj::CerObjectManager::createRef(ArrayT array_var)
		{
			ObjectStruct array_ref;

			// array_ref.m_array = array_var;
			array_ref.m_array = arrayCopy(array_var);
			array_ref.m_type = ObjectType::array;

			m_memerys.m_array.push_front(array_ref);
			return m_memerys.m_array.begin();
		}

		ObjectRefer Obj::CerObjectManager::createRef(StructT struct_var)
		{
			ObjectStruct struct_ref;

			// struct_ref.m_struct = struct_var;
			struct_ref.m_struct = structCopy(struct_var);
			struct_ref.m_type = ObjectType::structure;

			m_memerys.m_structure.push_front(struct_ref);
			return m_memerys.m_structure.begin();
		}

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
					ret_array.push_back(createRef(i->m_struct));
					break;
				default:
					throw("unrealized or unknow");
					break;
				}
			}

			return ret_array;
		}

		StructT Obj::CerObjectManager::structCopy(StructT struct_var)
		{
			for (auto& i : struct_var)
			{
				i.second->m_ref++;

				if (i.second->m_type == ObjectType::structure)
					structCopy(i.second->m_struct);
			}

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

			auto memery_list = getListByType(refer->m_type);

			m_callback(refer);

			m_dead_memery.emplace_front(std::move(*refer));
			memery_list->erase(refer);
			return m_dead_memery.begin();
		}

		ObjectList* CerObjectManager::getListByType(ObjectType type)
		{
			switch (type)
			{
			case ScriptC::Obj::null:
				return &m_memerys.m_null;
				break;
			case ScriptC::Obj::undef:
				return &m_memerys.m_undef;
				break;
			case ScriptC::Obj::boolean:
				return &m_memerys.m_boolean;
				break;
			case ScriptC::Obj::number:
				return &m_memerys.m_number;
				break;
			case ScriptC::Obj::string:
				return &m_memerys.m_string;
				break;
			case ScriptC::Obj::function:
				return &m_memerys.m_function;
				break;
			case ScriptC::Obj::array:
				return &m_memerys.m_array;
				break;
			case ScriptC::Obj::structure:
				return &m_memerys.m_structure;
				break;
			default:
				throw("Error getListByType switch deafult");
				break;
			}
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
				break;
			default:
				break;
			}

			return refer;
		}


		const MemeryStruct& Obj::CerObjectManager::getMemerys() const
		{
			return m_memerys;
		}

		const ObjectList& Obj::CerObjectManager::getDeadMemery() const
		{
			return m_dead_memery;
		}

		void Obj::CerObjectManager::setCallBack(DropCallBack callback)
		{
			m_callback = callback;
		}

		void Obj::CerObjectManager::dropDead()
		{
			while (m_dead_memery.begin() != m_dead_memery.end())
				m_dead_memery.erase(m_dead_memery.begin());
		}
	}
}