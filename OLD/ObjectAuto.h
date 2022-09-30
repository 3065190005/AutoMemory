#pragma once
#ifndef _CERVER_OBJECTAUTO_
#define _CERVER_OBJECTAUTO_

#include <map>
#include <vector>
#include <sstream>

namespace Cervice {
	namespace Obj {

		class Object;
		class ObjectAuto;
		enum class ElementType;
		enum class ElementOption;

		/************************
		* class ObjectAuto
		* 对各个Object类型之间做兼容
		* 包括 数字，字符，数组（可能）
		*************************/

		typedef ObjectAuto auto_c;
		typedef ObjectAuto* autoPtr;

		class ObjectAuto
		{
		private:
			static std::map<Object*, std::vector<Object*>> m_array_map;
		public:
			ObjectAuto(Object * ptr = nullptr);
			~ObjectAuto();

			ObjectAuto(const ObjectAuto&);		// copy
			ObjectAuto(ObjectAuto&&);			// move

			ObjectAuto& operator= (ObjectAuto&);
			ObjectAuto& operator= (ObjectAuto&&);

			ObjectAuto operator+ (ObjectAuto&);
			ObjectAuto operator+ (ObjectAuto&&);

			ObjectAuto operator- (ObjectAuto&);
			ObjectAuto operator- (ObjectAuto&&);

			ObjectAuto operator* (ObjectAuto&);
			ObjectAuto operator* (ObjectAuto&&);

			ObjectAuto operator/ (ObjectAuto&);
			ObjectAuto operator/ (ObjectAuto&&);

			ObjectAuto operator[] (ObjectAuto&);

			void operator<< (std::string& str);
			void operator<< (std::string&& str);
			void operator<< (long long int num);
			void operator<< (int num);
			void operator<< (double num);

			bool operator>> (std::string &str);
			bool operator>> (double& num);
			bool operator>> (long long int& num);

			enum class eleType {
				none = 0,
				number,
				string,
			};

		public:
			Object* getPtr() const;
			ElementType getType();
			ElementOption getOption();

			void RemoveTable(Object* ptr,Object * parent);
			void AddTable(Object* ptr,Object* parent);

		private:
			Object* m_ptr = nullptr;
			std::stringstream m_ss;

			eleType m_type_isnum;
			double m_index_num;
			std::string m_index_str;
			Object* m_parent = nullptr;
		};
	}
}

#endif


