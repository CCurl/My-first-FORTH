#pragma once

#include "vm.h"

#define FLAG_IS_NORMAL 0x0000
#define FLAG_IS_IMMEDIATE 0x0001

#define HERE_ADDRESS 1
#define LAST_ADDRESS 2
#define STATE_ADDRESS 3
#define BASE_ADDRESS 4
#define SOURCE_ADDRESS 5 // Points to the current input buffer
#define TOIN_ADDRESS 6 // >IN
#define	MEMLAST_ADDRESS 7
#define DEPTH_ADDRESS 8

#define PAD_ADDRESS 800
#define TEMP_REG1 990
#define TEMP_REG2 991

// #define PAD_ADDRESS 950 // PAD
#define INPUT_BUFFER 21 // typed commands go here
#define FILE_BUFFER 600 // commands read from file go here

#define DICT_START 0
#define CODE_START 1000

#define COMPILE_BREAK -999999

#define STATE_INTERPRETING 0
#define STATE_COMPILING 1
#define STATE_IMMEDIATE 2

#define MODE_BOOT 0
#define MODE_BOOTIF 1
#define MODE_RUN 2

#define MEMORY_SIZE (8*(1024))
#define STACK_SIZE 128 // Data stack
#define CSTACK_SIZE 32 // Conditional stack
#define RSTACK_SIZE 32 // Return stack


class CStackEntry
{
public:
	CStackEntry()
	{
		loop_start_addr = 0;
		loop_index = 0;
		loop_increment = 0;
		loop_limit = 0;
	}

	int loop_start_addr;
	int loop_index;
	int loop_increment;
	int loop_limit;

	bool Continue()
	{
		if ((loop_increment > 0) && (loop_index >= loop_limit))
			return false;
		if ((loop_increment < 0) && (loop_index <= loop_limit))
			return false;
		return true;
	}
};


class ForthOS 
{
	public:
	ForthOS(int memSize);
	~ForthOS();

	int xtComma; // Address of runtime code for ,
	int xtDComma; // Address of runtime code for D,
	//int xtPAD; // Address of runtime code for PAD
	int xtCreate; // Address of runtime code for CREATE
	FILE *output_fp;

	int DoExecute();

	int dataSpace[MEMORY_SIZE];
	int memorySize;
	int *memory;
	int IP;
	CString inputStream;
	CString output;

	// Data stack
	int SP, stack[STACK_SIZE];
	int PUSH(int val);
	int POP();

	// Conditional stack
	int CSP;
	CStackEntry cstack[CSTACK_SIZE]; // Conditional stack
	CStackEntry *CPUSH() { return (CSP < CSTACK_SIZE) ? &cstack[CSP++] : NULL; }
	CStackEntry *CPOP() { return (CSP > 0) ? &cstack[--CSP] : NULL; }

	// Return stack
	int RSP, rstack[RSTACK_SIZE];
	int RPUSH(int val) { if (RSP < RSTACK_SIZE) rstack[RSP++] = val; return val; }
	int RPOP() { return (RSP > 0) ? rstack[--RSP] : 0; }

	// Helpers
	int MemGet(int addr) 
	{
		if (addr < memorySize)
			return memory[addr];
		else
			throw CString("memory access of bounds.");
	}

	void MemSet(int addr, int val)
	{ 
		if (addr < memorySize)
			memory[addr] = val;
		else
			throw CString("memory access of bounds.");
	}

	int COMMA(int val);
	int Compile(int mode, ...);
	int HERE() { return MemGet(HERE_ADDRESS); }
	int LAST() { return MemGet(LAST_ADDRESS); }
	int TICK(int name, bool& isImmediate);

	bool StringIsChar(int addr, int& val);
	bool StringIsNumber(int addr, int& val);
	bool StringIsNumber(CString str, int& val);
	bool StringIsString(int addr);
	int StringToMem(int addr, LPCTSTR val);
	int MemToString(int addr, CString& val);
	int CompareStrings(int addr1, int addr2);
	int CollectChar(int addr, CHAR c)
	{
		int len = MemGet(addr)+1;
		MemSet(addr+len, c);
		MemSet(addr, len);
		return addr + len;
	}
	//int GetNextWord_OLD(int& toIN, int stopAddr, int copyTo, CString& name);
	int GetNextWord(int PAD);

	int Create(int name, int imm_flag, int xt);
	//void CommaCall(int addr) { COMMA(I_CALL);  COMMA(addr); }
	//void CommaLiteral(int val) { COMMA(I_LITERAL); COMMA(val); }

	int ParseInput(LPCTSTR commands);
	void AppendOutput(LPCTSTR text);
	void AppendOutput(CHAR ch);
	void Dump(CString& ret);
	void DumpStack(CString& ret);
	int DumpInstr(int xt, CString& ret);
	int DumpWord(int xt, CString& ret, int stopHere);
	void ResolveCall(CString& ret, int addr);
	void Load();
	void Save();
	void BootStrap();
	void BootStrap_FILE();
	bool Include(char *fileName);

	int EXECUTE(int xt);
	void ExecuteWord(int startAddr);
	void FormatNumber(int base, int num, CString& x);
};
