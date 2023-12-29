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

			// 将其拷贝构造和赋值构造成为私有函数, 禁止外部拷贝和赋值
			CerObjectManager(const CerObjectManager& signal) {};
			const CerObjectManager& operator=(const CerObjectManager& signal) {};

		public:
			static CerObjectManager* getInstance();

			/* 创建不同类型的Ref */
			ObjectRefer createRef();
			ObjectRefer createRef(UndefT);
			ObjectRefer createRef(BooleanT);
			ObjectRefer createRef(NumberT);
			ObjectRefer createRef(StringT);
			ObjectRefer createRef(FunctionT);
			ObjectRefer createRef(ArrayT);
			ObjectRefer createRef(StructT);

			/* 释放一个Ref */
			ObjectRefer leaveRef(ObjectRefer);

			/* 加入一个Ref */
			ObjectRefer joinRef(ObjectRefer);

			/* 回收一个Ref */
			ObjectRefer destoryRef(ObjectRefer);

			/* 获取指定类型的块流 */
			ObjectList* getListByType(ObjectType);

			/* array的赋值方式 */
			ArrayT arrayCopy(ArrayT);

			/* struct的复制方式 */
			StructT structCopy(StructT);

			void dropDead();
			void setCallBack(DropCallBack);

			/* 获得内存集合的方法 */
			const MemeryStruct& getMemerys() const;
			const ObjectList& getDeadMemery() const;

		private:
			static CerObjectManager* m_instance;
			MemeryStruct m_memerys;
			ObjectList m_dead_memery;

			DropCallBack m_callback;
		};
	}
}

#endif

