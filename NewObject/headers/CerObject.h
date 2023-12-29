#pragma once
#ifndef _SCRIPTC_CEROBJECT_
#define _SCRIPTC_CEROBJECT_

#include "CerObjectDef.h"
#include "CerObjectManager.h"

namespace ScriptC 
{
	namespace Obj 
	{
		class CerObjectManager;

		/* ArrayT 管理类 */
		class ArrayMaker;

		/* StructT 管理类 */
		class StructMaker;

		class CerObject
		{
			void setRefer(ObjectRefer);

		public:
			~CerObject();
			CerObject();
			CerObject(const CerObject& signal);
			CerObject(CerObject&& signal);

			static CerObject null();
			static CerObject undef(UndefT);
			static CerObject boolean(BooleanT);
			static CerObject number(NumberT);
			static CerObject string(StringT);
			static CerObject function(FunctionT);
			static CerObject array(ArrayMaker&);
			static CerObject structure(StructMaker&);

			template<typename... Args>
			static ArrayMaker makeArray(Args&&... value);

			template<typename... T>
			static StructMaker makeStruct(StringPair<T>... pack);

		private:
			ObjectRefer m_refer;

		};

		/* ArrayT 管理类 */
		class ArrayMaker
		{
		public:
			ArrayMaker(ArrayT&& array_var) :
				m_struct{ array_var }
			{}
			~ArrayMaker()
			{
				auto manager = CerObjectManager::getInstance();
				for (auto& i : m_struct)
					manager->leaveRef(i);
				m_struct.clear();
			}

			ArrayMaker(ArrayMaker&& move) noexcept
			{
				ArrayT temp;
				temp = m_struct;
				m_struct = move.m_struct;
				move.m_struct = temp;
			}
			ArrayMaker& operator=(ArrayMaker&& move) noexcept
			{
				ArrayT temp;
				temp = m_struct;
				m_struct = move.m_struct;
				move.m_struct = temp;

				return *this;
			}

			ArrayT* operator->() { return &m_struct; }
			ArrayT& operator*() { return m_struct; }

		private:
			ArrayMaker(const ArrayMaker&) noexcept = delete;
			ArrayMaker& operator = (const ArrayMaker&) noexcept = delete;

		private:
			ArrayT m_struct;
		};


		template<typename... Args>
		inline ArrayMaker CerObject::makeArray(Args&&... value)
		{
			auto manager = CerObjectManager::getInstance();
			ArrayT array_var;

			auto op = { (array_var.push_back(manager->joinRef(manager->createRef(value))),0)... };
			return ArrayMaker(std::move(array_var));
		}


		/* StructT 管理类 */
		class StructMaker
		{
		public:
			StructMaker(StructT&& array_var) :
				m_struct{ array_var }
			{}
			~StructMaker()
			{
				auto manager = CerObjectManager::getInstance();
				for (auto& i : m_struct)
					manager->leaveRef(i.second);
				m_struct.clear();
			}

			StructMaker(StructMaker&& move) noexcept
			{
				StructT temp;
				temp = m_struct;
				m_struct = move.m_struct;
				move.m_struct = temp;
			}
			StructMaker& operator=(StructMaker&& move) noexcept
			{
				StructT temp;
				temp = m_struct;
				m_struct = move.m_struct;
				move.m_struct = temp;

				return *this;
			}

			StructT* operator->() { return &m_struct; }
			StructT& operator*() { return m_struct; }
			ObjectRefer& operator[](String& str) { return m_struct[str]; }
			ObjectRefer& operator[](String&& str) { return m_struct[str]; }

		private:
			StructMaker(const StructMaker&) noexcept = delete;
			StructMaker& operator = (const StructMaker&) noexcept = delete;

		private:
			StructT m_struct;
		};

		template<typename ...T>
		inline StructMaker CerObject::makeStruct(StringPair<T> ...pack)
		{
			auto manager = CerObjectManager::getInstance();
			StructT struct_var;

			auto op = { (struct_var[pack.first] = manager->joinRef(manager->createRef(pack.second)),0)... };
			return StructMaker(std::move(struct_var));
		}
	}
}

#endif

