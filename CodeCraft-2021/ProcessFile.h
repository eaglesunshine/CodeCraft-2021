#pragma once
#include "Definition.h"

class ProcessFile {
public:
	Servers Servers;
	VirtualMachines VirtualMachines;
	Requests Requests;

public:
	bool ProcessInput(string fild_name);
	bool ProcessInputCin();
};