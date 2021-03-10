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

	//���Ԥ������Ҫ������ٵ�����
	this->DaySchedules.day_num = this->ProcessData.Requests.day_num;
	this->DaySchedules.schedules = vector<DaySchdule>(this->DaySchedules.day_num);
	for (int i = 0; i < this->ProcessData.Requests.day_num; i++) {
		this->DaySchedules.schedules[i].request_add_num = this->ProcessData.Requests.requests[i].request_add_num;
	}
}

string Schedule::getNewServer(int need_cpu_num, int need_mem_num) {
	//����������ÿ�����������ʹ��һ̨�·���������
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
	//�󽻼�
	set<string> valid_server_types;
	set_intersection(this->ProcessData.Servers.cpuMap[min_cpu].begin(), this->ProcessData.Servers.cpuMap[min_cpu].end(), 
		this->ProcessData.Servers.memMap[min_mem].begin(), this->ProcessData.Servers.memMap[min_mem].end(),
		inserter(valid_server_types, valid_server_types.begin())); // �󽻼�
	//���ؽ����еĵ�һ��Ԫ��
	auto it = valid_server_types.begin();
	return *it;
}

void Schedule::schedule_1() {
	for (int i = 0; i < this->ProcessData.Requests.day_num; i++) {
		//���������ID��Ӧ�ķ�����type
		int old_count= this->ServersInfo.size();
		vector<int> virtual_ids;
		map<int, int> is_double_map;
		map<int, string> virtual_id_to_type;
		for (int j = 0; j < this->ProcessData.Requests.requests[i].request_num; j++) {
			//ֻ�ܴ���������ɾ��
			if (this->ProcessData.Requests.requests[i].day_requests[j].is_add) {
				//add���������Ҫ����Դ��Ϣ
				int virtual_machine_id = this->ProcessData.Requests.requests[i].day_requests[j].virtual_machine_id;
				int need_cpu_num = this->ProcessData.VirtualMachines.virtual_machines[this->ProcessData.Requests.requests[i].day_requests[j].type].cpu_num;
				int need_mem_num = this->ProcessData.VirtualMachines.virtual_machines[this->ProcessData.Requests.requests[i].day_requests[j].type].mem_num;
				int is_double = this->ProcessData.VirtualMachines.virtual_machines[this->ProcessData.Requests.requests[i].day_requests[j].type].is_double;

				//Ϊadd�������ƥ�������
				string need_server_type = this->getNewServer(need_cpu_num, need_mem_num);
				virtual_ids.emplace_back(virtual_machine_id);
				virtual_id_to_type[virtual_machine_id] = need_server_type;
				is_double_map[virtual_machine_id] = is_double;
				
				//���ӽ��յĹ�����Ϣ
				this->DaySchedules.schedules[i].PurchaseList[need_server_type] += 1;
			}
		}

		//ָ��������ID���в�����Ϊ������һ�������������ID
		int add_count = virtual_id_to_type.size();
		map<string, vector<int>> typeMap;
		for (auto it = virtual_ids.begin(); it != virtual_ids.end(); it++) {
			typeMap[virtual_id_to_type[*it]].push_back(*it);
		}

		map<string, vector<int>> IDMap;
		for (auto it = typeMap.begin(); it != typeMap.end(); it++) {
			for (int id : it->second) {
				//���ӷ�����������
				ServerRemain server;
				server.type = virtual_id_to_type[id];
				int server_id = this->ServersInfo.size();
				this->ServersInfo[server_id] = server;

				IDMap[server.type].push_back(server_id);
			}
		}

		//��add�������ָ������������
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
		//��˳�����
		for (int id : virtual_ids) {
			this->DaySchedules.schedules[i].RequestAddList.emplace_back(MigrationMap[id]);
		}
	}
}

void Schedule::output() {
	for (int i = 0; i < this->ProcessData.Requests.day_num; i++) {
		//������Ҫ����ķ�������Ϣ
		cout << "(purchase," << int(this->DaySchedules.schedules[i].PurchaseList.size()) << ")" << endl;
		for (auto it = this->DaySchedules.schedules[i].PurchaseList.begin(); it != this->DaySchedules.schedules[i].PurchaseList.end(); it++) {
			cout << "(" << it->first << ", " << it->second << ")" << endl;
		}
		//������ҪǨ�Ƶ��������Ϣ
		cout << "(migration, 0)" << endl;
		//�������������˳��������ڵ�ǰ��һ���ÿһ����������
		for (auto it = this->DaySchedules.schedules[i].RequestAddList.begin(); it != this->DaySchedules.schedules[i].RequestAddList.end(); it++) {
			if ((*it).dest_node == "") {	//˫�ڵ㲿��
				cout << "(" << (*it).dest_machine_id << ")" << endl;
			}
			else {	//���ڵ㲿��
				cout << "(" << (*it).dest_machine_id << "," << (*it).dest_node << ")" << endl;
			}
		}
	}
}