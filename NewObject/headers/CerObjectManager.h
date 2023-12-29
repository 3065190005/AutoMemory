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

			// ���俽������͸�ֵ�����Լ���Ϊ˽�к���, ��ֹ�ⲿ�����͸�ֵ
			CerObjectManager(const CerObjectManager& signal) {};
			const CerObjectManager& operator=(const CerObjectManager& signal) {};

		public:
			static CerObjectManager* getInstance();
			static void throwError(String);
			static void setDropCallBack(DropCallBack);
			static void setErrorCallBack(ErrorCallBack);

			/* ������ͬ���͵�Ref */
			ObjectRefer createRef();
			ObjectRefer createRef(UndefT);
			ObjectRefer createRef(BooleanT);
			ObjectRefer createRef(NumberT);
			ObjectRefer createRef(StringT);
			ObjectRefer createRef(FunctionT);
			ObjectRefer createRef(ArrayT);
			ObjectRefer createRef(StructT);
			ObjectRefer createRef(StructT, ObjectRefer);



			/* ������ͬ���͵�StructT */
			ObjectStruct createStruct();
			ObjectStruct createStruct(UndefT);
			ObjectStruct createStruct(BooleanT);
			ObjectStruct createStruct(NumberT);
			ObjectStruct createStruct(StringT);
			ObjectStruct createStruct(FunctionT);
			ObjectStruct createStruct(ArrayT, ObjectRefer);
			ObjectStruct createStruct(StructT, ObjectRefer );

			/* �ͷ�һ��Ref */
			ObjectRefer leaveRef(ObjectRefer);

			/* ����һ��Ref */
			ObjectRefer joinRef(ObjectRefer);

			/* ����һ��Ref */
			ObjectRefer destoryRef(ObjectRefer);

			/* array�Ŀ�����ʽ */
			ArrayT arrayCopy(ArrayT);

			/* struct�ĸ��Ʒ�ʽ */
			StructT structCopy(StructT);

			/* ͨ��ObjectRefer����ObjectStruct */
			ObjectStruct assigment(ObjectRefer);

			/* ͨ��ObjectStruct��ֵObjectRefer �Զ� joinRef */
			ObjectRefer setReferStruct(ObjectRefer, ObjectStruct&&);

			/* �ӵ������������� */
			void dropDead();

			/* ����ڴ漯�ϵķ��� */
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

