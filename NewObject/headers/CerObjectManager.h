#ifndef _SCRIPTC_CEROBJECTMANAGER_
#define _SCRIPTC_CEROBJECTMANAGER_

#include "CerObjectDef.h"

namespace ScriptC 
{
	namespace Obj
	{
		class CerObjectManager
		{
		private:
			CerObjectManager() {};
			~CerObjectManager() {};

			// 将其拷贝构造和赋值构造以及成为私有函数, 禁止外部拷贝和赋值
			CerObjectManager(const CerObjectManager& signal) {};
			const CerObjectManager& operator=(const CerObjectManager& signal) {};

		public:
			static CerObjectManager* getInstance();
			static void throwError(String);
			static void setDropCallBack(DropCallBack);
			static void setErrorCallBack(ErrorCallBack);

			/* 创建不同类型的Ref */
			ObjectRefer createRef();
			ObjectRefer createRef(UndefT);
			ObjectRefer createRef(BooleanT);
			ObjectRefer createRef(NumberT);
			ObjectRefer createRef(StringT);
			ObjectRefer createRef(FunctionT);
			ObjectRefer createRef(ArrayT);
			ObjectRefer createRef(StructT);
			ObjectRefer createRef(StructT, ObjectRefer);



			/* 创建不同类型的StructT */
			ObjectStruct createStruct();
			ObjectStruct createStruct(UndefT);
			ObjectStruct createStruct(BooleanT);
			ObjectStruct createStruct(NumberT);
			ObjectStruct createStruct(StringT);
			ObjectStruct createStruct(FunctionT);
			ObjectStruct createStruct(ArrayT, ObjectRefer);
			ObjectStruct createStruct(StructT, ObjectRefer );

			/* 释放一个Ref */
			ObjectRefer leaveRef(ObjectRefer);

			/* 加入一个Ref */
			ObjectRefer joinRef(ObjectRefer);

			/* 回收一个Ref */
			ObjectRefer destoryRef(ObjectRefer);

			/* array的拷贝方式 */
			ArrayT arrayCopy(ArrayT);

			/* struct的复制方式 */
			StructT structCopy(StructT);

			/* 通过ObjectRefer创建ObjectStruct */
			ObjectStruct assigment(ObjectRefer);

			/* 通过ObjectStruct赋值ObjectRefer 自动 joinRef */
			ObjectRefer setReferStruct(ObjectRefer, ObjectStruct&&);

			/* 扔掉所有死亡变量 */
			void dropDead();

			/* 获得内存集合的方法 */
			const ObjectList& getMemerys() const;
			const ObjectList& getDeadMemery() const;

		private:
			static CerObjectManager* m_instance;

			ObjectList m_memerys;
			ObjectList m_dead_memery;

			static DropCallBack m_deadcall;
			static ErrorCallBack m_errorcall;
		};
	}
}

#endif

