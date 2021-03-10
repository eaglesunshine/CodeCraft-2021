#pragma once
#include "Definition.h"

class ProcessFile {
public:
	ServersAll Servers;
	VirtualMachinesAll VirtualMachines;
	RequestsAll Requests;

public:
	bool ProcessInput(string fild_name);
	bool ProcessInputCin();
};