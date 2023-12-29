#include "Object.h"

void OperatorAssign()
{
	/* �������ݣ�
	 * ����ֵ��
	 * []��ȡ
	 * dot��ȡ����
	 * �±긳ֵ�� struct��ֵ��array��ֵ��
	 * array����struct, 
	 * struct����array */

	/* ����struct֮��ĸ�ֵ�ʹ��� */
	{
		auto child_struct = object::makeStruct(
			StringPair<NumberT>{ "Child_Age", NumberT{ 10 } },
			StringPair<StringT>{ "Child_Name", StringT{ "Erth" } }
		);

		auto struct_maker = object::makeStruct(
			StringPair<NumberT>{ "Age", NumberT{ 123 } },
			StringPair<StringT>{ "Name", StringT{ "Hello World" } },
			StringPair<BooleanT>{ "Male", BooleanT{ true }},
			StringPair<StructT>{"Child", * child_struct}
		);

		object obj_struct = object::structure(struct_maker);
		object obj_struct_2 = object::null();

		object obj_index = object::string("Name");

		object obj_string = object::string(String{ "Hello World Double" });

		object obj_refer = obj_struct.dot(obj_index);
		obj_struct_2 = obj_struct;

		obj_refer = obj_string;
	}

	/* ����array֮��ĸ�ֵ�ʹ��� */
	 {
		 auto child_array = object::makeArray(NumberT{ 10 }, StringT{ "Erth" });
		 auto array_maker = object::makeArray(NumberT{ 35 }, StringT{ "Array Maker" }, *child_array);

		 object array_obj_1 = object::array(array_maker);
		 object array_obj_2 = object::null();

		 object array_index = object::number(2);
		 object array_string = object::string("Array Double");
		 object array_refer = array_obj_1[array_index];

		 array_obj_2 = array_obj_1;
		 array_refer = array_string;
	 }

	 /* ����struct �� ���� array */
	 {
		 auto array_maker = object::makeArray(NumberT{ 10 }, StringT{ "Erth" });
		 
		 auto struct_maker = object::makeStruct(
			 StringPair<NumberT>{ "Age", NumberT{ 123 } },
			 StringPair<StringT>{ "Name", StringT{ "Hello World" } },
			 StringPair<BooleanT>{ "Male", BooleanT{ true }},
			 StringPair<ArrayT>{"Child", * array_maker}
		 );

		 object obj_struct = object::structure(struct_maker);

		 object obj_struct_2 = object::null();
		 object obj_index = object::string("Name");
		 object obj_string = object::string(String{ "Hello World Double" });

		 object obj_refer = obj_struct.dot(obj_index);
		 obj_struct_2 = obj_struct;

		 obj_refer = obj_string;
	 }

	 /* ����array ���� struct */
	 {
		 auto struct_maker = object::makeStruct(
			 StringPair<NumberT>{ "Child_Age", NumberT{ 10 } },
			 StringPair<StringT>{ "Child_Name", StringT{ "Erth" } }
		 );
		 auto array_maker = object::makeArray(NumberT{ 35 }, StringT{ "Array Maker" }, *struct_maker);

		 object array_obj_1 = object::array(array_maker);
		 object array_obj_2 = object::null();

		 object array_index = object::number(2);
		 object array_string = object::string("Array Double");
		 object array_refer = array_obj_1[array_index];

		 array_obj_2 = array_obj_1;
		 array_refer = array_string;
	 }
}

void operatorAddTest()
{
	/* ���Լӷ����� */
	object obj_1 = object::boolean(true);
	object obj_2 = object::number(11456.3);
	object obj_3 = object::string("Hello World");

	// object result_1 = obj_1 + obj_2; // boolean + number -> error : bool���������ڼӷ������
	object result_2 = obj_2 + obj_2;    // number + number -> number
	// object result_3 = obj_2 + obj_3; // number + string -> error �� ��ͬ����֮�䲻����ʹ�üӷ������
	object result_4 = obj_3 + obj_3;	// string + string -> string
}

int main() 
{
	operatorAddTest();
	return 0;
}