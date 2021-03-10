#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

//服务器类型
struct Server {
	string type;		//型号
	int cpu_num;		//CPU核数
	int mem_num;		//内存大小
	double mech_cost;	//硬件成本
	double day_cost;	//每日能耗成本
};

struct ServersAll {
	int type_num = 0;	//服务器类型数量
	map<string, Server> servers;
	//按cpu大小递增排序的服务器type
	map<int, set<string>> cpuMap;
	//按mem大小递增排序的服务器tpye
	map<int, set<string>> memMap;
};

//虚拟机类型
struct VirtualMachine {
	string type;		//型号
	int cpu_num;		//CPU核数
	int mem_num;		//内存大小
	int is_double;		//是否双节点部署
};

struct VirtualMachinesAll {
	int type_num = 0;	//虚拟机类型数量
	map<string, VirtualMachine> virtual_machines;
};

//请求序列
struct Request {
	int is_add;				//请求类型，0-del，1-add
	string type;			//虚拟机型号(注：add才需要type信息，del只需要id即可)
	int virtual_machine_id;	//虚拟机ID
};

struct DayRequest {
	int request_num = 0;
	int request_add_num = 0;
	vector<Request> day_requests;
};

struct RequestsAll {
	int day_num = 0;	//请求天数
	map<int, DayRequest> requests;
};

struct Migration {
	int virtual_machine_id;
	int dest_machine_id;
	string dest_node="";
};

struct DaySchdule {
	//今天需要购买的服务器信息
	int mach_tpye_num = 0;
	map<string, int> PurchaseList;
	//今天需要迁移的虚拟机信息
	int migration_num = 0;
	vector<Migration> MigrationList;
	//按输入中请求的顺序，输出对于当前这一天的每一个创建请求
	int request_add_num = 0;
	vector<Migration> RequestAddList;
};

struct DaySchedulesAll {
	int day_num = 0;
	vector<DaySchdule> schedules;
};

//实时机房资源信息
struct ServerRemain {
	string type;		//型号
	int remian_cpu_num;		//剩余CPU核数
	int remain_mem_num;		//剩余内存大小
	map<int, VirtualMachine> A_virtual_machines;	//A节点上保活的虚拟机信息：<虚拟机ID，虚拟机型号信息>
	map<int, VirtualMachine> B_virtual_machines;	//B节点上保活的虚拟机信息：<虚拟机ID，虚拟机型号信息>
};