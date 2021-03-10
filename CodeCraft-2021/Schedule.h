#pragma once
#include "ProcessFile.h"
#include <string>


class Schedule {
public:
	ProcessFile ProcessData;
	DaySchedulesAll DaySchedules;
	map<int, ServerRemain> ServersInfo;		//<������ID��������ʣ����Դ��Ϣ>
public:
	//����
	void readInput(string file_name = "");

	//����������
	void schedule_1();	//��Ǩ�Ƶ��ȣ���������˳����������𵽷������ϣ������͹���
	string getNewServer(int need_cpu_num, int need_mem_num);

	//���
	void output();
};