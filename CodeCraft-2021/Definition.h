#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

//����������
struct Server {
	string type;		//�ͺ�
	int cpu_num;		//CPU����
	int mem_num;		//�ڴ��С
	double mech_cost;	//Ӳ���ɱ�
	double day_cost;	//ÿ���ܺĳɱ�
};

struct ServersAll {
	int type_num = 0;	//��������������
	map<string, Server> servers;
	//��cpu��С��������ķ�����type
	map<int, set<string>> cpuMap;
	//��mem��С��������ķ�����tpye
	map<int, set<string>> memMap;
};

//���������
struct VirtualMachine {
	string type;		//�ͺ�
	int cpu_num;		//CPU����
	int mem_num;		//�ڴ��С
	int is_double;		//�Ƿ�˫�ڵ㲿��
};

struct VirtualMachinesAll {
	int type_num = 0;	//�������������
	map<string, VirtualMachine> virtual_machines;
};

//��������
struct Request {
	int is_add;				//�������ͣ�0-del��1-add
	string type;			//������ͺ�(ע��add����Ҫtype��Ϣ��delֻ��Ҫid����)
	int virtual_machine_id;	//�����ID
};

struct DayRequest {
	int request_num = 0;
	int request_add_num = 0;
	vector<Request> day_requests;
};

struct RequestsAll {
	int day_num = 0;	//��������
	map<int, DayRequest> requests;
};

struct Migration {
	int virtual_machine_id;
	int dest_machine_id;
	string dest_node="";
};

struct DaySchdule {
	//������Ҫ����ķ�������Ϣ
	int mach_tpye_num = 0;
	map<string, int> PurchaseList;
	//������ҪǨ�Ƶ��������Ϣ
	int migration_num = 0;
	vector<Migration> MigrationList;
	//�������������˳��������ڵ�ǰ��һ���ÿһ����������
	int request_add_num = 0;
	vector<Migration> RequestAddList;
};

struct DaySchedulesAll {
	int day_num = 0;
	vector<DaySchdule> schedules;
};

//ʵʱ������Դ��Ϣ
struct ServerRemain {
	string type;		//�ͺ�
	int remian_cpu_num;		//ʣ��CPU����
	int remain_mem_num;		//ʣ���ڴ��С
	map<int, VirtualMachine> A_virtual_machines;	//A�ڵ��ϱ�����������Ϣ��<�����ID��������ͺ���Ϣ>
	map<int, VirtualMachine> B_virtual_machines;	//B�ڵ��ϱ�����������Ϣ��<�����ID��������ͺ���Ϣ>
};