## Illustrate
### ����ʵ��
���ú�ԭ�����ֱ�������block,ԭ����ָ����������block->ptr��
���ֲ�����ֱ�Ӳ�������ԭ��������������������������ͣ���ֱ�Ӷ�ָ��ָ��ı�����в���
����ֻ��ͨ���������ƶ���reference��̬����������ֵ���ƶ�
�ƶ����캯�� ֱ���ƶ�block,ԭ��������blokc��Ϊ��
���ƹ��캯�� ��block->ref+1���ϱ���ʹ��ͬһ��block



## Version
v1		�������ýӿڡ�δʵ�֡�
v2		ʵ�����ýӿڣ��Լ����úͱ���֮������
v3		���Ӻ���������ȡ������������������
v4		�޸���С����������ת���ַ�������С��������
v5		��������� <<(����) >>(����) ~(��λȡ��) !(��Ŀ��) %(ȡģ)
v6		��������� <(С��) >(����) <=(С�ڵ���) >=(���ڵ���) ==(���) !=(����) &(��λ��) ^(��λ���) |(��λ��) &&(�߼���) ||(�߼���)
		���ӹؼ���numberT ���� long double
		�޸�bool��numberT���ֶ�ʧ,�޸��߼������ (>= <= == != && ||)��null��undef�Ĵ���
		�޸��ƶ����캯��ԭ������������bug
v7		���ӳ�Ա���� getIndex ��ȡ�����������±�
v8		�����Խ���_this�ݹ麯��get/set Attribute
		��ȡ�����޵ݹ�Attribute���� getSelfAttribute;
v9		������ռ䲻��ʱ�Զ��Կ�Ϊ��λ���в�����չ
		���ͳ�ʼ�ڴ�Ͳ����ֱ�Ϊ4096/1024
v9_1	�޸ĵ���ֵΪnullʱ��������ڲ�����Ԫ��
		����ַ����ԱȾ�̬��Ա��ֻ����ʹ������ԭ����
		����LetArrayMotion�࣬�������麯��
		����Max��Min����ȡ���ֵ����Сֵ
		�������������֧�ֲ���
		�޸�getIndexs���������±귵�ش�������
		�޸�boolean��number��ת����������
v9_2	�޸�operator+( LetObject* node)ȱ�������������һλ������Խ���ڶ�������bug�������ֺ��ַ������bug
v9_3    �޸� + - * / ����bool������double��ȡһ�µ��¶�ȡ�����bug
v9_4	���������ռ� ���letTools������printAuto_c�������
		
v9_5	[*] LetObject::Tools::printAuto_c -> LetObject::Tools::print
	[+] LetObject::Tools::println
	[+] LetObject::Tools::StrCondition

v9_6	[+] LetObject::Tools::Swap ��������Ԫ�ص�ֵ�����Ҳ���ȡԭ����ֵ
v9_7	[+] LetObject::Tools::IsRef ���Ref�Ƿ�����ĳ��ֵ

v9_8	[+] LetObject::Tools::ToBin �����ڴ浽����bin�ļ�
	[+] LetObject::Tools::FromBin �ӱ���bin�ļ��ж�ȡ���ڴ�

v9_9	[+] LetObject::Tools::ObjTToStr
	[+] LetObject::Tools::ObjGToStr
	[+] LetObject::Tools::StrToObjT
	[+] LetObject::Tools::StrToObjG
	�޸� print �� println ���õ�������ʾ



