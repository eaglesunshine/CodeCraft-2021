#include "ProcessFile.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

bool ProcessFile::ProcessInput(string fild_name) {
	ifstream file;
	file.open(fild_name, ios::in);
	if (!file){
		cout << fild_name << " file not exist!!";
		return false;
	}

	string lineStr;
	int count = 0;

	//读取服务器信息
	getline(file, lineStr);
	this->Servers.type_num = stoi(lineStr);
	count = 0;
	while (getline(file, lineStr)) {	
		//去除括号
		lineStr = lineStr.substr(1, lineStr.size() - 2);

		stringstream ss(lineStr);
		string str;

		//(型号, CPU核数, 内存大小, 硬件成本, 每日能耗成本)
		int sub_count = 0;
		Server tmp_server;
		while (getline(ss, str, ',')){
			switch (sub_count)
			{
			case 0:
				tmp_server.type = str; break;
			case 1:
				tmp_server.cpu_num = stoi(str); break;
			case 2:
				tmp_server.mem_num = stoi(str); break;
			case 3:
				tmp_server.mech_cost = stof(str); break;
			case 4:
				tmp_server.day_cost = stod(str); break;
			default:
				break;
			}
			++sub_count;
		}
		this->Servers.servers[tmp_server.type] = tmp_server;
		this->Servers.cpuMap[tmp_server.cpu_num].insert(tmp_server.type);
		this->Servers.memMap[tmp_server.mem_num].insert(tmp_server.type);

		++count;
		if (count == this->Servers.type_num)
			break;
	}

	//读取虚拟机信息
	getline(file, lineStr);
	this->VirtualMachines.type_num = stoi(lineStr);
	count = 0;
	while (getline(file, lineStr)) {
		//去除括号
		lineStr = lineStr.substr(1, lineStr.size() - 2);

		stringstream ss(lineStr);
		string str;

		//(型号, CPU核数, 内存大小, 是否双节点部署)
		int sub_count = 0;
		VirtualMachine tmp_virtual_machine;
		while (getline(ss, str, ',')){
			switch (sub_count)
			{
			case 0:
				tmp_virtual_machine.type = str; break;
			case 1:
				tmp_virtual_machine.cpu_num = stoi(str); break;
			case 2:
				tmp_virtual_machine.mem_num = stoi(str); break;
			case 3:
				tmp_virtual_machine.is_double = stoi(str); break;
			default:
				break;
			}
			++sub_count;
		}
		this->VirtualMachines.virtual_machines[tmp_virtual_machine.type] = tmp_virtual_machine;

		++count;
		if (count == this->VirtualMachines.type_num)
			break;
	}

	//读取请求队列
	getline(file, lineStr);
	this->Requests.day_num = stoi(lineStr);
	for (int i = 0; i < this->Requests.day_num; i++) {
		getline(file, lineStr);
		this->Requests.requests[i].request_num = stoi(lineStr);
		count = 0;
		int add_count = 0;
		while (getline(file, lineStr)) {
			//去除括号
			lineStr = lineStr.substr(1, lineStr.size() - 2);

			stringstream ss(lineStr);
			string str;

			//(add, 虚拟机型号, 虚拟机ID)或(del, 虚拟机ID)
			int sub_count = 0;
			Request tmp_request;
			while (getline(ss, str, ',')) {
				switch (sub_count)
				{
				case 0:
					if (str == "add") {
						tmp_request.is_add = 1;
						++add_count;
					}
					else
						tmp_request.is_add = 0;
					break;
				case 1:
					if (tmp_request.is_add) {
						while (str.front() == ' ') str = str.substr(1, str.length());
						tmp_request.type = str;
					}
					else {
						tmp_request.virtual_machine_id = stoi(str);
					}
					break;
				case 2:
					tmp_request.virtual_machine_id = stoi(str); break;
				default:
					break;
				}
				++sub_count;
			}
			this->Requests.requests[i].day_requests.emplace_back(tmp_request);

			++count;
			if (count == this->Requests.requests[i].request_num)
				break;
		}
		this->Requests.requests[i].request_add_num = add_count;

	}
	
	//cout << "read file success!!" << endl;
	return true;
}

bool ProcessFile::ProcessInputCin() {
	string lineStr;
	int count = 0;

	//读取服务器信息
	getline(cin, lineStr);
	this->Servers.type_num = stoi(lineStr);
	count = 0;
	while (getline(cin, lineStr)) {
		//去除括号
		lineStr = lineStr.substr(1, lineStr.size() - 2);

		stringstream ss(lineStr);
		string str;

		//(型号, CPU核数, 内存大小, 硬件成本, 每日能耗成本)
		int sub_count = 0;
		Server tmp_server;
		while (getline(ss, str, ',')) {
			switch (sub_count)
			{
			case 0:
				tmp_server.type = str; break;
			case 1:
				tmp_server.cpu_num = stoi(str); break;
			case 2:
				tmp_server.mem_num = stoi(str); break;
			case 3:
				tmp_server.mech_cost = stof(str); break;
			case 4:
				tmp_server.day_cost = stod(str); break;
			default:
				break;
			}
			++sub_count;
		}
		this->Servers.servers[tmp_server.type] = tmp_server;
		this->Servers.cpuMap[tmp_server.cpu_num].insert(tmp_server.type);
		this->Servers.memMap[tmp_server.mem_num].insert(tmp_server.type);

		++count;
		if (count == this->Servers.type_num)
			break;
	}

	//读取虚拟机信息
	getline(cin, lineStr);
	this->VirtualMachines.type_num = stoi(lineStr);
	count = 0;
	while (getline(cin, lineStr)) {
		//去除括号
		lineStr = lineStr.substr(1, lineStr.size() - 2);

		stringstream ss(lineStr);
		string str;

		//(型号, CPU核数, 内存大小, 是否双节点部署)
		int sub_count = 0;
		VirtualMachine tmp_virtual_machine;
		while (getline(ss, str, ',')) {
			switch (sub_count)
			{
			case 0:
				tmp_virtual_machine.type = str; break;
			case 1:
				tmp_virtual_machine.cpu_num = stoi(str); break;
			case 2:
				tmp_virtual_machine.mem_num = stoi(str); break;
			case 3:
				tmp_virtual_machine.is_double = stoi(str); break;
			default:
				break;
			}
			++sub_count;
		}
		this->VirtualMachines.virtual_machines[tmp_virtual_machine.type] = tmp_virtual_machine;

		++count;
		if (count == this->VirtualMachines.type_num)
			break;
	}

	//读取请求队列
	getline(cin, lineStr);
	this->Requests.day_num = stoi(lineStr);
	for (int i = 0; i < this->Requests.day_num; i++) {
		getline(cin, lineStr);
		this->Requests.requests[i].request_num = stoi(lineStr);
		count = 0;
		int add_count = 0;
		while (getline(cin, lineStr)) {
			//去除括号
			lineStr = lineStr.substr(1, lineStr.size() - 2);

			stringstream ss(lineStr);
			string str;

			//(add, 虚拟机型号, 虚拟机ID)或(del, 虚拟机ID)
			int sub_count = 0;
			Request tmp_request;
			while (getline(ss, str, ',')) {
				switch (sub_count)
				{
				case 0:
					if (str == "add") {
						tmp_request.is_add = 1;
						++add_count;
					}
					else
						tmp_request.is_add = 0;
					break;
				case 1:
					if (tmp_request.is_add) {
						while (str.front() == ' ') str = str.substr(1, str.length());
						tmp_request.type = str;
					}
					else {
						tmp_request.virtual_machine_id = stoi(str);
					}
					break;
				case 2:
					tmp_request.virtual_machine_id = stoi(str); break;
				default:
					break;
				}
				++sub_count;
			}
			this->Requests.requests[i].day_requests.emplace_back(tmp_request);

			++count;
			if (count == this->Requests.requests[i].request_num)
				break;
		}
		this->Requests.requests[i].request_add_num = add_count;
		
	}

	//cout << "read cin success!!" << endl;
	return true;
}