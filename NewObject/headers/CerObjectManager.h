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

			// ���俽������͸�ֵ�����Ϊ˽�к���, ��ֹ�ⲿ�����͸�ֵ
			CerObjectManager(const CerObjectManager& signal) {};
			const CerObjectManager& operator=(const CerObjectManager& signal) {};

		public:
			static CerObjectManager* getInstance();

			/* ������ͬ���͵�Ref */
			ObjectRefer createRef();
			ObjectRefer createRef(UndefT);
			ObjectRefer createRef(BooleanT);
			ObjectRefer createRef(NumberT);
			ObjectRefer createRef(StringT);
			ObjectRefer createRef(FunctionT);
			ObjectRefer createRef(ArrayT);
			ObjectRefer createRef(StructT);

			/* �ͷ�һ��Ref */
			ObjectRefer leaveRef(ObjectRefer);

			/* ����һ��Ref */
			ObjectRefer joinRef(ObjectRefer);

			/* ����һ��Ref */
			ObjectRefer destoryRef(ObjectRefer);

			/* ��ȡָ�����͵Ŀ��� */
			ObjectList* getListByType(ObjectType);

			/* array�ĸ�ֵ��ʽ */
			ArrayT arrayCopy(ArrayT);

			/* struct�ĸ��Ʒ�ʽ */
			StructT structCopy(StructT);

			void dropDead();
			void setCallBack(DropCallBack);

			/* ����ڴ漯�ϵķ��� */
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

