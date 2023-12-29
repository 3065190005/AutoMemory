#include "Object.h"

void OperatorAssign()
{
	/* 测试内容：
	 * 创建值，
	 * []获取
	 * dot获取，，
	 * 下标赋值， struct赋值，array赋值，
	 * array包含struct, 
	 * struct包含array */

	/* 测试struct之间的赋值和创建 */
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

	/* 测试array之间的赋值和创建 */
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

	 /* 测试struct 和 包含 array */
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

	 /* 测试array 包含 struct */
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
	/* 测试加法运算 */
	object obj_1 = object::boolean(true);
	object obj_2 = object::number(11456.3);
	object obj_3 = object::string("Hello World");

	// object result_1 = obj_1 + obj_2; // boolean + number -> error : bool不能作用于加法运算符
	object result_2 = obj_2 + obj_2;    // number + number -> number
	// object result_3 = obj_2 + obj_3; // number + string -> error ： 不同类型之间不允许使用加法运算符
	object result_4 = obj_3 + obj_3;	// string + string -> string
}

int main() 
{
	operatorAddTest();
	return 0;
}