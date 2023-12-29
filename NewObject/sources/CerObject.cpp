#include "CerObjectManager.h"
#include "CerObject.h"

namespace ScriptC
{
	namespace Obj
	{

		CerObject::CerObject()
		{
			auto manager = CerObjectManager::getInstance();
			auto refer = manager->createRef();
			m_refer = manager->joinRef(refer);
		}

		CerObject::CerObject(const CerObject& copy)
		{
			auto manager = CerObjectManager::getInstance();
			auto refer = manager->joinRef(copy.m_refer);
			m_refer = refer;
		}

		CerObject::CerObject(CerObject&& move)
		{
			auto manager = CerObjectManager::getInstance();
			m_refer = std::move(move.m_refer);
		}

		CerObject::~CerObject()
		{
			auto manager = CerObjectManager::getInstance();
			manager->leaveRef(m_refer);
		}

		CerObject CerObject::null()
		{
			auto manager = CerObjectManager::getInstance();

			CerObject ret;
			ret.setRefer(manager->createRef());
			return ret;
		}

		CerObject CerObject::undef(UndefT undef_var)
		{
			auto manager = CerObjectManager::getInstance();

			CerObject ret;
			ret.setRefer(manager->createRef(undef_var));
			return ret;
		}

		CerObject CerObject::boolean(BooleanT bool_var)
		{
			auto manager = CerObjectManager::getInstance();

			CerObject ret;
			ret.setRefer(manager->createRef(bool_var));
			return ret;
		}

		CerObject CerObject::number(NumberT number_var)
		{
			auto manager = CerObjectManager::getInstance();

			CerObject ret;
			ret.setRefer(manager->createRef(number_var));
			return ret;
		}

		CerObject CerObject::string(StringT string_var)
		{
			auto manager = CerObjectManager::getInstance();

			CerObject ret;
			ret.setRefer(manager->createRef(string_var));
			return ret;
		}

		CerObject CerObject::function(FunctionT function_var)
		{
			auto manager = CerObjectManager::getInstance();

			CerObject ret;
			ret.setRefer(manager->createRef(function_var));
			return ret;
		}

		CerObject CerObject::array(ArrayMaker& array_maker)
		{
			auto manager = CerObjectManager::getInstance();

			CerObject ret;
			ret.setRefer(manager->createRef(*array_maker));
			return ret;
		}

		CerObject CerObject::structure(StructMaker& struct_maker)
		{
			auto manager = CerObjectManager::getInstance();

			CerObject ret;
			ret.setRefer(manager->createRef(*struct_maker));
			return ret;
		}

		void CerObject::setRefer(ObjectRefer refer)
		{
			auto manager = CerObjectManager::getInstance();

			auto temp_refer = manager->joinRef(refer);
			manager->leaveRef(m_refer);
			m_refer = temp_refer;
		}

		ObjectRefer CerObject::getRefer()
		{
			return m_refer;
		}

		/* 运算符实现 */

		CerObject& CerObject::operator=(CerObject& index)
		{
			auto manager = CerObjectManager::getInstance();

			if (this == &index)
				return *this;

			ObjectStruct value = manager->assigment(index.m_refer);
			m_refer = manager->setReferStruct(m_refer, std::move(value));
			return *this;
		}

		/* 杂项运算符 */
		CerObject CerObject::operator[](CerObject& index)
		{
			auto manager = CerObjectManager::getInstance();

			ifNotThrow("Operator[] this type need “array”", this->getType() == ObjectType::array);
			ifNotThrow("Operator[] index type need “number”", index.getType() == ObjectType::number);

			
			CerObject ret_object = CerObject::null();

			auto & this_array = this->m_refer->m_array;
			auto& index_number = index.m_refer->m_number;
			auto finder = this_array[index_number];
			
			ret_object.setRefer(finder);
			return ret_object;
		}

		CerObject CerObject::dot(CerObject& index)
		{
			auto manager = CerObjectManager::getInstance();

			ifNotThrow("Operator dot this type need “structure”", this->getType() == ObjectType::structure);
			ifNotThrow("Operator dot index type need “string”", index.getType() == ObjectType::string);


			CerObject ret_object = CerObject::null();

			auto& this_string = this->m_refer->m_struct;
			auto& index_string = index.m_refer->m_string;
			auto finder = this_string[index_string];

			ret_object.setRefer(finder);
			return ret_object;
		}

		ObjectType CerObject::getType()
		{
			return m_refer->m_type;
		}

		String CerObject::getTypeStr()
		{
			switch (getType())
			{
			case ScriptC::Obj::null:
				return String{ "null" };
				break;
			case ScriptC::Obj::undef:
				return String{ "undef" };
				break;
			case ScriptC::Obj::boolean:
				return String{ "boolean" };
				break;
			case ScriptC::Obj::number:
				return String{ "number" };
				break;
			case ScriptC::Obj::string:
				return String{ "string" };
				break;
			case ScriptC::Obj::function:
				return String{ "function" };
				break;
			case ScriptC::Obj::array:
				return String{ "array" };
				break;
			case ScriptC::Obj::structure:
				return String{ "structure" };
				break;
			default:
				break;
			}
		}

		String CerObject::replaceMapString(String src, Map<String, String> map)
		{
			for (auto& i : map)
			{
				auto nofind = src.npos;
				auto finder = src.find(i.first);
				if (finder == nofind)
					continue;
				else
				{
					String temp = src.substr(0, finder);
					temp += i.second;
					temp += src.substr(finder + i.first.size());
					src = temp;

				}
			}

			return src;
		}


		void CerObject::ifNotThrow(String info, BooleanT cond)
		{
			if (!cond)
			{
				CerObjectManager::throwError(info);
			}
		}
		CerObject CerObject::operator+(CerObject& cond)
		{
			auto manager = CerObjectManager::getInstance();

			auto left_type = this->getType();
			auto right_type = cond.getType();

			auto left_refer = this->getRefer();
			auto right_refer = cond.getRefer();
			
			String error_str = R"("+" operator on "{Left}" and "{Right}" are not allowed)";
			Map<String, String> replace_map =
			{
				{ "{Left}", this->getTypeStr() },
				{ "{Right}", cond.getTypeStr() },
			};
			error_str = replaceMapString(error_str, replace_map);

			ifNotThrow(error_str, left_type == right_type && 
				(left_type == ObjectType::number || left_type == ObjectType::string));

			CerObject result = this->null();
			if (left_type == ObjectType::number)
			{
				NumberT result_number = left_refer->m_number + right_refer->m_number;
				CerObject result_object = this->number(result_number);
				result = result_object;
			}
			else if (left_type == ObjectType::string)
			{
				String result_string = left_refer->m_string + right_refer->m_string;
				CerObject result_object = this->string(result_string);
				result = result_object;
			}

			return result;
		}
}
}