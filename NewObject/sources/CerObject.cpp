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
			m_refer = manager->joinRef(copy.m_refer);
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
			manager->leaveRef(m_refer);
			m_refer = manager->joinRef(refer);
		}
	}
}