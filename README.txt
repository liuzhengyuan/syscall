包含如下文件：
 README.txt： 		本文件
 kylincpu_syscall.c 	增加kylincpu系统调用的内核模块的源文件
 Makefile           	增加kylincpu系统调用的内核模块的Makefile文件
 test.c 		用来测试新增的kylincpu系统调用的测试程序的源文件
 klinux-cpuinfo.patch	klinux上获取cpuinfo的接口.

kylincpu系统调用：
	该系统调用的功能是在内核中打印出"hello kylin, i am in system call"字符串

新增kylincpu系统调用：
  1. 在sys_call_table系统调用表中选择一个未使用系统调用号，示例中选择系统调用号是250.
  2. 在sys_call_talbe系统调用表中索引是250的位置添加系统调用例程sys_kylincpu.
  3. sys_kylincpu例程实现系统调用功能. 
  4. 内核许打上klinux-cpuinfo.patch重新编译内核和模块


编译：
  在本目录下执行make命令，则会编译出模块kylincpu_syscall.ko。 (系统中必须安装有/lib/modules/xxxx/build/目录下安装有内核头文件和gcc编译环境，2a系统的内核头文件默认已经安装在此目录下，如果没有可以修改Make文件中的内核目录到实际头文件目录)

测试程序：
	测试程序使用syscall函数调用250号系统调用。

测试：
	1. 编译测试程序
			gcc -o test test.c
	2. 执行测试程序
			./test

结果：
	使用dmesg命令查看，系统打印出字符串"hello world, i am in system call"
	test 程序运行之后输出cpu信息
