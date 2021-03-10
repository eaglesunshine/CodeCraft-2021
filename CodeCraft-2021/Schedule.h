#pragma once
#include "ProcessFile.h"
#include <string>


class Schedule {
public:
	ProcessFile ProcessData;
	DaySchedulesAll DaySchedules;
	map<int, ServerRemain> ServersInfo;		//<服务器ID，服务器剩余资源信息>
public:
	//输入
	void readInput(string file_name = "");

	//服务器调度
	void schedule_1();	//不迁移调度：按照请求顺序将虚拟机部署到服务器上，不够就购买
	string getNewServer(int need_cpu_num, int need_mem_num);

	//输出
	void output();
};