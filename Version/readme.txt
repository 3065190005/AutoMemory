## Illustrate
### 引用实现
引用和原变量分别有两个block,原变量指针存放于引用block->ptr中
各种操作符直接操作的是原变量，如果参数或本体是引用类型，则直接对指针指向的本体进行操作
引用只能通过拷贝、移动、reference静态函数、来赋值和移动
移动构造函数 直接移动block,原来变量的blokc将为空
复制构造函数 将block->ref+1新老变量使用同一个block



## Version
v1		增加引用接口【未实现】
v2		实现引用接口，以及引用和变量之间的配合
v3		增加函数用来获取变量的两个数组内容
v4		修复无小数浮点类型转换字符串保留小数点问题
v5		增加运算符 <<(左移) >>(右移) ~(按位取反) !(单目非) %(取模)
v6		增加运算符 <(小于) >(大于) <=(小于等于) >=(大于等于) ==(恒等) !=(不等) &(按位与) ^(按位异或) |(按位或) &&(逻辑与) ||(逻辑或)
		增加关键字numberT 代替 long double
		修复bool到numberT数字丢失,修复逻辑运算符 (>= <= == != && ||)对null和undef的处理
		修复移动构造函数原变量保留类型bug
v7		增加成员函数 getIndex 获取变量的所有下标
v8		对属性进行_this递归函数get/set Attribute
		获取自身无递归Attribute属性 getSelfAttribute;
v9		当空余空间不足时自动以块为单位进行步长扩展
		降低初始内存和步长分别为4096/1024
v9_1	修改当赋值为null时清除数组内部所有元素
		添加字符串对比静态成员，只允许使用数组原类型
		增加LetArrayMotion类，处理数组函数
		增加Max和Min来获取最大值和最小值
		增加数组的所有支持操作
		修复getIndexs函数数字下标返回错误问题
		修复boolean对number的转换错误问题
v9_2	修复operator+( LetObject* node)缺少括号完成任意一位是数字越过第二个条件bug导致数字和字符串相加bug
v9_3    修复 + - * / 对于bool数据与double读取一致导致读取错误的bug
v9_4	更换命名空间 添加letTools类整合printLetObject到相关类
		
v9_5	[*] LetObject::Tools::printLetObject -> LetObject::Tools::print
	[+] LetObject::Tools::println
	[+] LetObject::Tools::StrCondition

v9_6	[+] LetObject::Tools::Swap 交换两个元素的值，并且不获取原来的值
v9_7	[+] LetObject::Tools::IsRef 检测Ref是否引用某个值

v9_8	[+] LetObject::Tools::ToBin 保存内存到本地bin文件
	[+] LetObject::Tools::FromBin 从本地bin文件中读取到内存

v9_9	[+] LetObject::Tools::ObjTToStr
	[+] LetObject::Tools::ObjGToStr
	[+] LetObject::Tools::StrToObjT
	[+] LetObject::Tools::StrToObjG
	修改 print 和 println 更好的数据显示

v9_10	bin文件将以大端模式进行存储

v9_11	使用宏控制LetObject的缩写
	LetArrayMotion::ArrayCmp 添加跳过指定字符串函数
	添加新的Attribute -> function


