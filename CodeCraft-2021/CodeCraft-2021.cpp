#include <iostream>
#include <string>

#include "Schedule.h"

using namespace std;

int main()
{
	Schedule Schedule;

	//input
	//Schedule.readInput("training-data/test.txt");	//本地调试：使用文件输入
	Schedule.readInput();		//云端测试：使用std::cin输入

	//process
	Schedule.schedule_1();

	//output
	Schedule.output();

    return 0;
}

