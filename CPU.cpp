#include "stdafx.h"
#include "vm.h"


bool BreakHere(int IP)
{
	switch (IP)
	{
	case 0:
	case 1215:
	case 1181:
		return true;
	}
	return false;
}

void CPU::Run(int startHere)
{
	IP = startHere;

	while (TRUE)
	{
		if (BreakHere(IP))
		{
			int xxx = 1;
		}
		INSTR_T instr = (INSTR_T)memory[IP++];
		switch (instr)
		{
		case CALL:
			returnStack[RSP++] = IP + 1;
			IP = memory[IP];
			break;

		case JUMP:
			IP = memory[IP];
			break;

		case RETURN:
			if (RSP > 0)
			{
				IP = returnStack[--RSP];
			}
			else
			{
				return;
			}
			break;

		default:
			{
				   throw "invalid instruction.";
			}
		}
	}
}
