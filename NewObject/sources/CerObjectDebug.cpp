#include "CerObjectDebug.h"
#include "CerObjectManager.h"

namespace ScriptC
{
	namespace Obj
	{
		MemeryInfo CerObjectDebug::debugObjectListInfo(const ObjectList& object_list)
		{
			auto manager = CerObjectManager::getInstance();
			auto& m_memerys = object_list;

			USize
				null_size{ 0 },
				undef_size{ 0 },
				boolean_size{ 0 },
				number_size{ 0 },
				string_size{ 0 },
				function_size{ 0 },
				array_size{ 0 },
				structure_size{ 0 },
				total_size{ 0 };

			MemeryInfo info;
			for (auto& i : m_memerys)
			{
				switch (i.m_type)
				{
				case ScriptC::Obj::null:
					null_size++;
					total_size++;
					break;
				case ScriptC::Obj::undef:
					undef_size++;
					total_size++;
					break;
				case ScriptC::Obj::boolean:
					boolean_size++;
					total_size++;
					break;
				case ScriptC::Obj::number:
					number_size++;
					total_size++;
					break;
				case ScriptC::Obj::string:
					string_size++;
					total_size++;
					break;
				case ScriptC::Obj::function:
					function_size++;
					total_size++;
					break;
				case ScriptC::Obj::array:
					array_size++;
					total_size++;
					break;
				case ScriptC::Obj::structure:
					structure_size++;
					total_size++;
					break;
				default:
					break;
				}
			}

			info["null"] = null_size;
			info["undef"] = undef_size;
			info["boolean"] = boolean_size;
			info["number"] = number_size;
			info["string"] = string_size;
			info["function"] = function_size;
			info["array"] = array_size;
			info["structure"] = structure_size;
			info["total"] = total_size;

			return info;
		}
		MemeryInfo Obj::CerObjectDebug::debugMemerysInfo()
		{
			auto manager = CerObjectManager::getInstance();
			return debugObjectListInfo(manager->getMemerys());
		}


		MemeryInfo Obj::CerObjectDebug::debugDeadMemeryInfo()
		{
			auto manager = CerObjectManager::getInstance();
			return debugObjectListInfo(manager->getDeadMemery());
		}

		void CerObjectDebug::debugPrintInfo(MemeryInfo info)
		{
			USize total{ 0 };
			for (auto& i : info)
			{
				debugPrint("%s : %ld\n", i.first.c_str(), i.second);
				total += i.second;
			}

			debugPrint("\n-------------\n\n", total);
		}
		void CerObjectDebug::debugPrintByRef(ObjectRefer refer)
		{
			switch (refer->m_type)
			{
			case ScriptC::Obj::null:
				debugPrint("null\n");
				break;
			case ScriptC::Obj::undef:
				debugPrint("undef : %p \n", refer->m_undef);
				break;
			case ScriptC::Obj::boolean:
				debugPrint("boolean : %d \n", refer->m_bool);
				break;
			case ScriptC::Obj::number:
				debugPrint("number : %lf \n", refer->m_number);
				break;
			case ScriptC::Obj::string:
				debugPrint("string : %s \n", refer->m_string.c_str());
				break;
			case ScriptC::Obj::function:
				debugPrint("function : [name :  %s , param_size : %d] \n",
					refer->m_function.m_name.c_str(),
					refer->m_function.m_param_size);
				break;
			case ScriptC::Obj::array:
				debugPrint("array : [ ");
				for (auto& i : refer->m_array)
				{
					debugPrintByRef(i);
					debugPrint(", ");
				}
				debugPrint("] \n");
				break;
			case ScriptC::Obj::structure:
				debugPrint("structure : [ \n,");
				for (auto& i : refer->m_struct)
				{
					debugPrint("%s : ",i.first.c_str());
					debugPrintByRef(i.second);
					debugPrint(", ");
				}
				debugPrint("] \n");
				break;
			default:
				break;
			}
		}
	}
}
