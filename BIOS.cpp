#include "stdafx.h"
#include "vm.h"

HwAbsLayer::HwAbsLayer(int MemorySize, int ReturnStackSize)
{
	memorySize = MemorySize;
	returnStackSize = ReturnStackSize;

	//memory = new int[memorySize];
	returnStack = new int[returnStackSize];

	cpu.memory = memory;
	cpu.returnStack = returnStack;
	cpu.hal = this;
}

HwAbsLayer::~HwAbsLayer()
{
	//free(memory);
	free(returnStack);
}

void HwAbsLayer::BootStrap()
{
	// "ROM" initialization
	for (int i = 0; i < memorySize; i++)
	{
		memory[i] = 0;
	}
	for (int i = 0; i < returnStackSize; i++)
	{
		returnStack[i] = 0;
	}

	// CPU initialization

	// OS Initialization
	// os->BootStrap();

	// Go!
	//cpu.Run();
}
