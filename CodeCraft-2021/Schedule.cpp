#include "Schedule.h"

#include <algorithm>
#include <iterator>
#include <iostream>

void Schedule::readInput(string file_name) {
	if (file_name.size() == 0) {
		this->ProcessData.ProcessInputCin();
	}
	else {
		this->ProcessData.ProcessInput(file_name);
	}

	//输出预处理：需要输出多少的数据
	this->DaySchedules.day_num = this->ProcessData.Requests.day_num;
	this->DaySchedules.schedules = vector<DaySchdule>(this->DaySchedules.day_num);
	for (int i = 0; i < this->ProcessData.Requests.day_num; i++) {
		this->DaySchedules.schedules[i].request_add_num = this->ProcessData.Requests.requests[i].request_add_num;
	}
}

string Schedule::getNewServer(int need_cpu_num, int need_mem_num) {
	//暴力方法：每个虚拟机任务都使用一台新服务器部署
	int min_cpu = 0, min_mem = 0;
	for (auto it = this->ProcessData.Servers.cpuMap.begin(); it != this->ProcessData.Servers.cpuMap.end(); it++) {
		if (need_cpu_num <= it->first) {
			min_cpu = it->first;
			break;
		}
	}
	for (auto it = this->ProcessData.Servers.memMap.begin(); it != this->ProcessData.Servers.memMap.end(); it++) {
		if (need_mem_num <= it->first) {
			min_mem = it->first;
			break;
		}
	}
	//求交集
	set<string> valid_server_types;
	set_intersection(this->ProcessData.Servers.cpuMap[min_cpu].begin(), this->ProcessData.Servers.cpuMap[min_cpu].end(), 
		this->ProcessData.Servers.memMap[min_mem].begin(), this->ProcessData.Servers.memMap[min_mem].end(),
		inserter(valid_server_types, valid_server_types.begin())); // 求交集
	//返回交集中的第一个元素
	auto it = valid_server_types.begin();
	return *it;
}

void Schedule::schedule_1() {
	for (int i = 0; i < this->ProcessData.Requests.day_num; i++) {
		//保存虚拟机ID对应的服务器type
		int old_count= this->ServersInfo.size();
		vector<int> virtual_ids;
		map<int, int> is_double_map;
		map<int, string> virtual_id_to_type;
		for (int j = 0; j < this->ProcessData.Requests.requests[i].request_num; j++) {
			//只管创建，不管删除
			if (this->ProcessData.Requests.requests[i].day_requests[j].is_add) {
				//add的虚拟机需要的资源信息
				int virtual_machine_id = this->ProcessData.Requests.requests[i].day_requests[j].virtual_machine_id;
				int need_cpu_num = this->ProcessData.VirtualMachines.virtual_machines[this->ProcessData.Requests.requests[i].day_requests[j].type].cpu_num;
				int need_mem_num = this->ProcessData.VirtualMachines.virtual_machines[this->ProcessData.Requests.requests[i].day_requests[j].type].mem_num;
				int is_double = this->ProcessData.VirtualMachines.virtual_machines[this->ProcessData.Requests.requests[i].day_requests[j].type].is_double;

				//为add的虚拟机匹配服务器
				string need_server_type = this->getNewServer(need_cpu_num, need_mem_num);
				virtual_ids.emplace_back(virtual_machine_id);
				virtual_id_to_type[virtual_machine_id] = need_server_type;
				is_double_map[virtual_machine_id] = is_double;
				
				//增加今日的购买信息
				this->DaySchedules.schedules[i].PurchaseList[need_server_type] += 1;
			}
		}

		//指定服务器ID进行部署：因为购买是一次性做入服务器ID
		int add_count = virtual_id_to_type.size();
		map<string, vector<int>> typeMap;
		for (auto it = virtual_ids.begin(); it != virtual_ids.end(); it++) {
			typeMap[virtual_id_to_type[*it]].push_back(*it);
		}

		map<string, vector<int>> IDMap;
		for (auto it = typeMap.begin(); it != typeMap.end(); it++) {
			for (int id : it->second) {
				//增加服务器到机房
				ServerRemain server;
				server.type = virtual_id_to_type[id];
				int server_id = this->ServersInfo.size();
				this->ServersInfo[server_id] = server;

				IDMap[server.type].push_back(server_id);
			}
		}

		//对add的虚拟机指定服务器部署
		map<int, Migration> MigrationMap;
		for (auto it = typeMap.begin(); it != typeMap.end(); it++) {
			string type = it->first;
			for (int k = 0; k < it->second.size(); k++) {
				int virtual_machine_id = typeMap[type][k];
				int server_id = IDMap[type][k];

				Migration migration;
				migration.virtual_machine_id = virtual_machine_id;
				migration.dest_machine_id = server_id;
				if (is_double_map[virtual_machine_id] == 0)
					migration.dest_node = "A";
				MigrationMap[virtual_machine_id] = migration;
			}
		}
		//按顺序输出
		for (int id : virtual_ids) {
			this->DaySchedules.schedules[i].RequestAddList.emplace_back(MigrationMap[id]);
		}
	}
}

void Schedule::output() {
	for (int i = 0; i < this->ProcessData.Requests.day_num; i++) {
		//今天需要购买的服务器信息
		cout << "(purchase," << int(this->DaySchedules.schedules[i].PurchaseList.size()) << ")" << endl;
		for (auto it = this->DaySchedules.schedules[i].PurchaseList.begin(); it != this->DaySchedules.schedules[i].PurchaseList.end(); it++) {
			cout << "(" << it->first << ", " << it->second << ")" << endl;
		}
		//今天需要迁移的虚拟机信息
		cout << "(migration, 0)" << endl;
		//按输入中请求的顺序，输出对于当前这一天的每一个创建请求
		for (auto it = this->DaySchedules.schedules[i].RequestAddList.begin(); it != this->DaySchedules.schedules[i].RequestAddList.end(); it++) {
			if ((*it).dest_node == "") {	//双节点部署
				cout << "(" << (*it).dest_machine_id << ")" << endl;
			}
			else {	//单节点部署
				cout << "(" << (*it).dest_machine_id << "," << (*it).dest_node << ")" << endl;
			}
		}
	}
}