#ifndef _SCRIPTC_CEROBJECTDEBUG_
#define _SCRIPTC_CEROBJECTDEBUG_

#include "CerObjectDef.h"

namespace ScriptC
{
	namespace Obj
	{
		class CerObjectDebug
		{
		public:
			static MemeryInfo debugMemerysInfo();
			static MemeryInfo debugDeadMemeryInfo();
			static void debugPrintInfo(MemeryInfo);
			static void debugPrintByRef(ObjectRefer);

			template<typename... Types>
			static void debugPrint(String&& first, Types... args);
		};

		/* tpp实例化 */
		template<typename ...Types>
		inline void CerObjectDebug::debugPrint(String&& first, Types ...args)
		{
#ifndef USEUTF8 // 是否使用utf-8
			::printf_s(first.c_str(), (args)...);
#else
			::wprintf_s(first.c_str(), (args)...);
#endif

		}
	}
}


#endif

