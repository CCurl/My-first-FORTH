#include "stdafx.h"
#include "vm.h"
#include "ForthOS.h"

ForthOS::ForthOS(int memSize)
{
	SP = CSP = IP = RSP = 0;
	memorySize = memSize;

#ifdef _DEBUG
	memSize = MEMORY_SIZE;
	memory = dataSpace;
#else
	memory = new int[memorySize];
#endif

	userMode = true;
	memset(memory, 0, memorySize * sizeof(int));
	memset(stack, 0, STACK_SIZE * sizeof(int));
	memset(cstack, 0, CSTACK_SIZE * sizeof(int));
	memset(rstack, 0, RSTACK_SIZE * sizeof(int));
}

ForthOS::~ForthOS()
{
	free(memory);
}

int ForthOS::COMMA(int val)
{
	int here = MemGet(HERE_ADDRESS);
	MemSet(here++, val);
	MemSet(HERE_ADDRESS, here);
	return here;
}

int ForthOS::Create(int name, int flags)
{
	// Create HERE (CUR) ! LAST , <flags> , <counted-name> (CUR) @ , ;
	MemSet(CUR_WORD, HERE());
	COMMA(LAST());
	COMMA(flags);
	int len = MemGet(name++);
	COMMA(len);
	for (int i = 1; i <= len; i++)
	{
		COMMA(MemGet(name++));
	}
	COMMA(MemGet(CUR_WORD));
	return HERE();
}

int ForthOS::EndWord()
{
	MemSet(LAST_ADDRESS, MemGet(CUR_WORD));
	MemSet(STATE_ADDRESS, 0);
	return LAST();
}

int ForthOS::EXECUTE(int xt)
{
	IP = xt;
	bool err = false;

	try
	{
		IP = DoExecute();
	}
	catch (CString msg)
	{
		err = true;
		AppendOutput(msg);
	}
	catch (...)
	{
		err = true;
	}

	if (err)
	{
		SP = 0;
		RSP = 0;
		CSP = 0;
		IP = 0;
		MemSet(STATE_ADDRESS, 0);
		MemSet(IP, I_RETURN);
	}
	return IP;
}

int ForthOS::DoExecute()
{
	int addr, arg1, arg2, val;
	CStackEntry *cse = NULL;

	while (true)
	{
		//if (IP == 1677)
		//{
		//	int ccc = 0;
		//}
		INSTR_T instr = (INSTR_T)MemGet(IP++);
		switch (instr)
		{
		case I_CALL:
			addr = MemGet(IP++);
			RPUSH(IP);
			IP = addr;
			break;

		case I_RETURN:
			if (RSP > 0)
				IP = RPOP();
			else
				return IP;
			break;

		case I_STORE:
			addr = POP();
			val = POP();
			MemSet(addr, val);
			break;

		case I_FETCH:
			addr = POP();
			PUSH(MemGet(addr));
			break;

		case I_LITERAL:
			PUSH(MemGet(IP++));
			break;

		case I_DROP:
			POP();
			break;

		case I_SWAP:
			arg2 = POP();
			arg1 = POP();
			PUSH(arg2);
			PUSH(arg1);
			break;

		case I_DUP:
			arg1 = POP();
			PUSH(arg1);
			PUSH(arg1);
			break;

		case I_ROT:
			arg2 = POP();
			arg1 = POP();
			val = POP();
			PUSH(arg1);
			PUSH(arg2);
			PUSH(val);
			break;

		//case I_IF:
		//	COMMA(I_IF_RT);
		//	IPUSH(HERE());
		//	COMMA(0); // Branches to ELSE or THEN if FALSE
		//	break;

		//case I_ELSE:
		//	addr = IPOP();
		//	COMMA(I_GOTO);
		//	IPUSH(HERE());
		//	COMMA(0);
		//	MemSet(addr, HERE());
		//	break;

		//case I_THEN:
		//	addr = IPOP();
		//	MemSet(addr, HERE());
		//	break;

		case I_IF_RT:
			// JUMP to ELSE or THEN if FALSE
			addr = MemGet(IP++);
			val = POP();
			if (val == 0)
				IP = addr;
			break;

		case I_GOTO:
			IP = MemGet(IP);
			break;

		case I_DO:
			cse = CPUSH();
			cse->loop_start_addr = IP; // LOOP target address
			cse->loop_increment = 1; // Default increment
			cse->loop_index = POP(); // Index
			cse->loop_limit = POP(); // Limit
			break;

		case I_I:
			cse = CPOP(); // Push the index
			CPUSH();
			PUSH(cse->loop_index);
			break;

		case I_LEAVE:
			cse = CPOP(); // Set Limit = Index
			cse = CPUSH();
			cse->loop_limit = cse->loop_index;
			break;

		case I_PLUSLOOP:
			cse = CPOP(); // Index
			CPUSH();
			cse->loop_increment = POP();
			// No break, fall through

		case I_LOOP:
			cse = CPOP(); // Index
			if (cse->loop_increment == 0)
			{
				throw "invalid LOOP increment.";
			}
			cse->loop_index += cse->loop_increment;

			// Did it cross the limit?
			if (cse->Continue())
			{
				// Nope, go back to start
				CPUSH();
				IP = cse->loop_start_addr;
			}
			break;

		case I_ONEPLUS:
			arg1 = POP();
			PUSH(arg1 + 1);
			break;

		case I_PLUS:
			arg2 = POP();
			arg1 = POP();
			PUSH(arg1 + arg2);
			break;

		case I_MINUS:
			arg2 = POP();
			arg1 = POP();
			PUSH(arg1 - arg2);
			break;

		case I_MULT:
			arg2 = POP();
			arg1 = POP();
			PUSH(arg1 * arg2);
			break;

		case I_DIV:
			arg2 = POP();
			arg1 = POP();
			if (arg2 == 0)
				throw "divide by zero.";
			PUSH(arg1 / arg2);
			break;

		case I_EQ:
			PUSH((POP() == POP()) ? -1 : 0);
			break;

		case I_NEQ:
			PUSH((POP() != POP()) ? -1 : 0);
			break;

		case I_GT:
			PUSH((POP() < POP()) ? -1 : 0);
			break;

		case I_LT:
			PUSH((POP() > POP()) ? -1 : 0);
			break;

		case I_OVER:
			PUSH(1);
			// NO break, ... : OVER 1 PICK ;

		case I_PICK:
			arg1 = POP();
			arg2 = SP - arg1 - 1;
			PUSH(arg2 >= 0 ? stack[arg2] : 0);
			break;

		case I_DOT:
			val = POP();
			{
				CString x; 
				int base = MemGet(BASE_ADDRESS);
				if (base == 10)
					x.Format(_T("%d "), val);
				else if (base == 16)
					x.Format(_T("%X "), val);
				else
					FormatNumber(base, val, x);
				AppendOutput(x);
			}
			break;

		case I_TYPE:
			val = POP(); // count
			addr = POP(); // start addr
			{
				CString x;
				for (int i = 0; i < val; i++)
				{
					x.AppendChar(MemGet(addr++));
				}
				AppendOutput(x);
			}
			break;

		case I_TO_R: // >R
			RPUSH(POP());
			break;

		case I_R_FROM: // R>
			PUSH(RPOP());
			break;

		case I_R_AT: // R@
			val = RPOP();
			PUSH(val);
			RPUSH(val);
			break;

		default:
			// Must be a user defined word
		{
				   CString err; err.Format(_T("invalid instruction: %d"), instr);
				   throw err;
		}
			break;
		}
	}

	return 0;
}

void ForthOS::Compile(int num, ...)
{
	int here = HERE();
	va_list code;
	va_start(code, num);
	int val = va_arg(code, int);
	while (val != COMPILE_BREAK)
	{
		MemSet(here++, val);
		val = va_arg(code, int);
	}
	va_end(code);
	MemSet(HERE_ADDRESS, here);
	EndWord();
}

void ForthOS::BootStrap()
{
	// Reset Here pointer
	// MemSet(HERE_ADDRESS, CORE_START);
	MemSet(HERE_ADDRESS, DICT_START);
	MemSet(LAST_ADDRESS, MemGet(HERE_ADDRESS));
	MemSet(BASE_ADDRESS, 10); // Decimal

	COMMA(0); // End of word chain
	COMMA(0); // FLAGS
	COMMA(0); // LEN
	COMMA(1000); // BACK-PTR
	COMMA(I_RETURN); // BACK-PTR

	// Built-In WORD: , ( n -- )
	// : , (HERE) @ SWAP OVER ! 1+ (HERE) ! ;
	xtComma = Create(StringToMem(SOURCE_ADDRESS, _T(",")), FLAG_IS_NORMAL);
	Compile(0, 
		I_LITERAL, HERE_ADDRESS, I_FETCH, 
		I_SWAP, I_OVER, I_STORE,
		I_ONEPLUS,
		I_LITERAL, HERE_ADDRESS, I_STORE,
		I_RETURN, COMPILE_BREAK);

	// Built-In VAR: (HERE)
	Create(StringToMem(SOURCE_ADDRESS, _T("(HERE)")), FLAG_IS_NORMAL);
	Compile(0,
		I_LITERAL, HERE_ADDRESS,
		I_RETURN, COMPILE_BREAK);

	int xtHERE = Create(StringToMem(SOURCE_ADDRESS, _T("HERE")), FLAG_IS_NORMAL);
	Compile(0,
		I_LITERAL, HERE_ADDRESS, I_FETCH,
		I_RETURN, COMPILE_BREAK);

	// Built-In VAR: STATE
	Create(StringToMem(SOURCE_ADDRESS, _T("STATE")), FLAG_IS_NORMAL);
	Compile(0,
		I_LITERAL, STATE_ADDRESS, 
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: SOURCE
	Create(StringToMem(SOURCE_ADDRESS, _T("SOURCE")), FLAG_IS_NORMAL);
	Compile(0,
		I_LITERAL, SOURCE_ADDRESS,
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: >IN
	Create(StringToMem(SOURCE_ADDRESS, _T(">IN")), FLAG_IS_NORMAL);
	Compile(0,
		I_LITERAL, TOIN_ADDRESS,
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: >IN
	Create(StringToMem(SOURCE_ADDRESS, _T("PAD")), FLAG_IS_NORMAL);
	Compile(0,
		// I_CALL, xtHERE, I_LITERAL, 10, I_PLUS,
		I_LITERAL, PAD_ADDRESS,
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: BASE
	Create(StringToMem(SOURCE_ADDRESS, _T("BASE")), FLAG_IS_NORMAL);
	Compile(0,
		I_LITERAL, BASE_ADDRESS,
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: @ (I_FETCH)
	// : <BUILDS I_FETCH , DOES> I_FETCH ; IMMEDIATE
	int xtFetch = Create(StringToMem(SOURCE_ADDRESS, _T("@")), FLAG_IS_IMMEDIATE);
	// STATE @ 1 = IF ELSE THEN ;
	Compile(0,
		I_LITERAL, STATE_ADDRESS, I_FETCH,
		I_IF_RT, HERE() + 11,
		I_LITERAL, I_FETCH,
		I_CALL, xtComma,
		I_GOTO, HERE() + 12,
		I_FETCH,
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: ! (STORE)
	// : ! <BUILDS STORE , DOES> STORE ; IMMEDIATE
	int xtSTORE = Create(StringToMem(SOURCE_ADDRESS, _T("!")), FLAG_IS_IMMEDIATE);
	Compile(0,
		I_LITERAL, STATE_ADDRESS, I_FETCH,
		I_IF_RT, HERE() + 11,
		I_LITERAL, I_STORE,
		I_CALL, xtComma,
		I_GOTO, HERE() + 12,
		I_STORE,
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: +
	// : ! <BUILDS STORE , DOES> STORE ; IMMEDIATE
	int xtPLUS = Create(StringToMem(SOURCE_ADDRESS, _T("+")), FLAG_IS_IMMEDIATE);
	Compile(0,
		I_LITERAL, STATE_ADDRESS, I_FETCH,
		I_IF_RT, HERE() + 11,
		I_LITERAL, I_PLUS,
		I_CALL, xtComma,
		I_GOTO, HERE() + 12,
		I_PLUS,
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: (LAST)
	int xtLastVar = Create(StringToMem(SOURCE_ADDRESS, _T("(LAST)")), FLAG_IS_NORMAL);
	Compile(0,
		I_LITERAL, LAST_ADDRESS,
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: LAST
	// : LAST (LAST) @ ;
	int xtLAST = Create(StringToMem(SOURCE_ADDRESS, _T("LAST")), FLAG_IS_NORMAL);
	Compile(0,
		I_LITERAL, LAST_ADDRESS, I_FETCH,
		I_RETURN, COMPILE_BREAK);

	// Built-In VAR: (cw)
	Create(StringToMem(SOURCE_ADDRESS, _T("(cw)")), FLAG_IS_NORMAL);
	Compile(0,
		I_LITERAL, CUR_WORD,
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: ; (END-WORD)
	// : ; <I_RETURN> , (cw) @ (LAST) ! 0 STATE ! ; IMMEDIATE
	int addrSemiColon = Create(StringToMem(SOURCE_ADDRESS, _T(";")), FLAG_IS_IMMEDIATE);
	Compile(0,
		I_LITERAL, I_RETURN, 
		I_CALL, xtComma,
		I_LITERAL, CUR_WORD, I_FETCH,
		I_LITERAL, LAST_ADDRESS, I_STORE,
		I_LITERAL, 0, I_LITERAL, STATE_ADDRESS, I_STORE,
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: IF ( -- c-addr )
	// Check the STATE; if interpreting, 
	Create(StringToMem(SOURCE_ADDRESS, _T("IF")), FLAG_IS_IMMEDIATE);
	Compile(0,
		I_LITERAL, I_IF_RT,
		I_CALL, xtComma,
		I_CALL, xtHERE,
		I_LITERAL, 0,
		I_CALL, xtComma,
		I_RETURN, COMPILE_BREAK);

	// Built-In WORD: ELSE ( c-addr -- c-addr )
	Create(StringToMem(SOURCE_ADDRESS, _T("ELSE")), FLAG_IS_IMMEDIATE);
	Compile(0,
		I_LITERAL, I_GOTO,
		I_CALL, xtComma,
		I_CALL, xtHERE,
		I_LITERAL, 0,
		I_CALL, xtComma,
		I_CALL, xtHERE,
		I_ROT,
		I_STORE,
		I_RETURN, COMPILE_BREAK);
	//addr = IPOP();
	//COMMA(I_GOTO);
	//IPUSH(HERE());
	//COMMA(0);
	//MemSet(addr, HERE());

	// Built-In WORD: THEN ( c-addr -- )
	Create(StringToMem(SOURCE_ADDRESS, _T("THEN")), FLAG_IS_IMMEDIATE);
	Compile(0,
		I_CALL, xtHERE,
		I_SWAP,
		I_STORE,
		I_RETURN, COMPILE_BREAK);

	PUSH(99999);
	EXECUTE(xtComma);
}

void ForthOS::ResolveCall(CString& ret, int addr)
{
	ret.Append(_T(" ("));
	CString tmp;
	addr = MemGet(addr - 1) + 2;
	int len = MemGet(addr++);
	for (int i = 0; i < len; i++)
	{
		CHAR c = MemGet(addr++);
		ret.AppendChar(c);
	}
	ret.Append(_T(")"));
}

int ForthOS::DumpInstr(int xt, CString& ret)
{
	int addr = 0, val;
	ret.Format(_T("%04d: "), xt);

	INSTR_T instr = (INSTR_T)MemGet(xt++);
	switch (instr)
	{
	case I_CALL:
		addr = MemGet(xt++);
		ret.AppendFormat(_T("I_CALL %04d"), addr);
		ResolveCall(ret, addr);
		break;

	case I_RETURN:
		ret.AppendFormat(_T("I_RETURN"));
		break;

	case I_STORE:
		ret.AppendFormat(_T("I_STORE"));
		break;

	case I_FETCH:
		ret.AppendFormat(_T("I_FETCH"));
		break;

	case I_LITERAL:
		val = MemGet(xt++);
		ret.AppendFormat(_T("I_LITERAL %d"), val);
		break;

	case I_DROP:
		ret.AppendFormat(_T("I_DROP"));
		break;

	case I_SWAP:
		ret.AppendFormat(_T("I_SWAP"));
		break;

	case I_DUP:
		ret.AppendFormat(_T("I_DUP"));
		break;

	case I_ROT:
		ret.AppendFormat(_T("I_ROT"));
		break;

	//case I_IF:
	//	ret.AppendFormat(_T("I_IF"));
	//	break;

	//case I_ELSE:
	//	ret.AppendFormat(_T("I_ELSE"));
	//	addr = MemGet(xt++);
	//	ret.AppendFormat(_T(" (GOTO %d)"), addr);
	//	break;

	//case I_THEN:
	//	ret.AppendFormat(_T("I_THEN"));
	//	break;

	case I_IF_RT:
		ret.AppendFormat(_T("I_IF_RT"));
		// JUMP to ELSE or THEN if FALSE
		addr = MemGet(xt++);
		ret.AppendFormat(_T(" (0 = IF GOTO %d)"), addr);
		break;

	case I_GOTO:
		addr = MemGet(xt++);
		ret.AppendFormat(_T("I_GOTO %04d"), addr);
		break;

	case I_DO:
		ret.AppendFormat(_T("I_DO"));
		break;

	case I_I:
		ret.AppendFormat(_T("I_I"));
		break;

	case I_LEAVE:
		ret.AppendFormat(_T("I_LEAVE"));
		break;

	case I_PLUSLOOP:
		ret.AppendFormat(_T("I_PLUSLOOP"));
		break;

	case I_LOOP:
		ret.AppendFormat(_T("I_LOOP"));
		break;

	case I_ONEPLUS:
		ret.AppendFormat(_T("I_ONEPLUS"));
		break;

	case I_PLUS:
		ret.AppendFormat(_T("I_PLUS"));
		break;

	case I_MINUS:
		ret.AppendFormat(_T("I_MINUS"));
		break;

	case I_MULT:
		ret.AppendFormat(_T("I_MULT"));
		break;

	case I_DIV:
		ret.AppendFormat(_T("I_DIV"));
		break;

	case I_EQ:
		ret.AppendFormat(_T("I_EQ"));
		break;

	case I_NEQ:
		ret.AppendFormat(_T("I_NEQ"));
		break;

	case I_GT:
		ret.AppendFormat(_T("I_GT"));
		break;

	case I_LT:
		ret.AppendFormat(_T("I_LT"));
		break;

	case I_OVER:
		ret.AppendFormat(_T("I_OVER ... 1 "));
		// NO break, ... : OVER 1 PICK ;

	case I_PICK:
		ret.AppendFormat(_T("I_PICK"));
		break;

	case I_DOT:
		ret.AppendFormat(_T("I_DOT"));
		break;

	case I_TYPE:
		ret.AppendFormat(_T("I_TYPE"));
		break;

	case I_TO_R:
		ret.AppendFormat(_T("I_TO_R (>R)"));
		break;

	case I_R_FROM:
		ret.AppendFormat(_T("I_R_FROM (R>)"));
		break;

	case I_R_AT:
		ret.AppendFormat(_T("I_R_AT (R@)"));
		break;

	default:
		// Must be a user defined word
		ret.AppendFormat(_T(".INT %d"), instr);
		break;
	}
	return xt;
}

int ForthOS::DumpWord(int xt, CString& ret, int stopHere)
{
	CString def;
	ret.Empty();

	while (xt < stopHere)
	{
		xt = DumpInstr(xt, def);
		ret.AppendFormat(_T("%s\r\n"), def);
	}
	return xt;
}

void ForthOS::DumpStack(CString& ret)
{
	ret.Format(_T("SP: %d  -  "), SP);
	for (int addr = 0; addr < SP; addr++)
	{
		ret.AppendFormat(_T("%d "), stack[addr]);
	}
}

void ForthOS::Dump(CString& ret)
{
	ret.Empty();

	//t2 = _T(" 0000: ");
	//t3.Empty();
	//for (int addr = 0; addr < 10; addr++)
	//{
	//	int val = MemGet(addr);
	//	DumpHelper(t2, t3, val, true);
	//}
	//ret = t2;
	//ret += "\r\n\r\n";

	//t2.Format(_T(" BASE:"));
	//for (int j = 0; j < 10; j++)
	//{
	//	t1.Format(_T(" %04d"), j);
	//	t2 += t1;
	//}
	//ret += t2;
	//ret += "\r\n";
	for (int i = 0; i < 100; i++)
	{
		ret.AppendFormat(_T("   [%-2d] %-4d"), i, MemGet(i));
		if ((i%10 == 0) && (i > 0))
			ret.Append(_T("\r\n"));
	}
	ret.Append(_T("\r\n\r\n"));

	CString hdr, def;
	int addr = MemGet(LAST_ADDRESS);
	int thisWord = addr;
	int stopHere = HERE();
	while (addr != 0)
	{
		def.Format(_T("%04d: ; "), addr);
		int prev = MemGet(addr++);
		int imm = MemGet(addr++);
		int len = MemGet(addr++);
		def.AppendFormat(_T("%04d %s (%d) "), prev, imm ? _T("(immediate)") : _T("0"), len);
		for (int i = 0; i < len; i++)
		{
			int c = MemGet(addr++);
			def.AppendChar((CHAR)c);
		}
		int wordStart = MemGet(addr++);
		hdr.Format(_T("%s (%04d)"), def, wordStart);
		addr = DumpWord(addr, def, stopHere);
		CString fullDef;
		fullDef.AppendFormat(_T("%s\r\n%s\r\n"), hdr, def);
		ret.Append(fullDef);

		stopHere = thisWord;
		thisWord = MemGet(thisWord);
		addr = thisWord;
	}

	//for (int addr = DICT_START; addr < HERE(); addr++)
	//{
	//	int c = MemGet(addr);
	//	ret.AppendFormat(_T("\r\n %04d: %-4d"), addr, c);
	//	if ((c > 31) && (c < 128))
	//	{
	//		ret.AppendFormat(_T("\t\t'%c'"), c);
	//	}
	//}

	//t2.Format(_T(" %04d: "), DICT_START);
	//int gotoThis = MemGet(HERE_ADDRESS);
	//for (int addr = DICT_START; addr < gotoThis; addr++)
	//{
	//	int val = MemGet(addr);
	//	DumpHelper(t2, t3, val, true);

	//	if (addr % 10 == 9)
	//	{
	//		t2 += _T("    "); t2 += t3;
	//		ret += t2;
	//		ret += "\r\n";
	//		t2.Format(_T(" %04d: "), addr + 1);
	//		t3.Empty();
	//	}
	//}
	//t2 += _T("    "); t2 += t3;
	//ret += t2;
	//ret += "\r\n";
}

void ForthOS::AppendOutput(LPCTSTR text)
{
	output.Append(text);
}

int ForthOS::CompareStrings(int addr1, int addr2)
{
	int len1 = MemGet(addr1);
	int len2 = MemGet(addr2);
	if (len1 == len2)
	{
		for (int i = 1; i <= len1; i++)
		{
			if (MemGet(addr1 + i) != MemGet(addr2 + i))
				return 0;
		}
		return 1;
	}
	return 0;
}

// Returns start of code for the word, 0 if not found
int ForthOS::TICK(int nameAddr, bool& isImmediate)
{
	int thisWord = MemGet(LAST_ADDRESS);
	while (thisWord != 0)
	{
		int addr = thisWord;
		int nextWord = MemGet(addr++);
		int flags = MemGet(addr++);
		isImmediate = (flags & 0x0001) != 0;
		int wordName = addr;
		//{
		//	CString tmp; MemToString(wordName, tmp);
		//}
		if (CompareStrings(wordName, nameAddr))
		{
			int len = MemGet(addr);
			return addr + len + 2;
		}
		thisWord = nextWord;
	}
	return 0;
}

bool ForthOS::StringIsChar(int addr, int& val)
{
	val = 0;
	int len = MemGet(addr);
	if ((len == 3) && (MemGet(addr + 1) == '\'') && (MemGet(addr + 3) == '\''))
	{
		CHAR c = MemGet(addr + 2);
		val = c;
		return true;
	}
	return false;
}

bool ForthOS::StringIsNumber(CString str, int& val)
{
	CT2A ascii(str);
	char *end = NULL;
	int base = MemGet(BASE_ADDRESS);
	val = strtol(ascii.m_psz, &end, base);
	return (*end) == NULL;
}

bool ForthOS::StringIsNumber(int addr, int& val)
{
	val = 0;
	int len = MemGet(addr);
	bool isNegative = false;
	for (int i = 1; i <= len; i++)
	{
		CHAR c = MemGet(addr + i);
		if ((i == 1) && (c == '-'))
		{
			isNegative = true;
			continue;
		}
		if (c < '0' || c > '9')
		{
			return false;
		}
		int t = c - '0';
		val = (val * 10) + t;
	}
	if (isNegative)
		val = -val;
	return true;
}

bool ForthOS::StringIsString(int addr)
{
	int len = MemGet(addr);

	return ((len > 1) && (MemGet(addr + 1) == '"') && (MemGet(addr + len) == '"'));
}

int ForthOS::StringToMem(int addr, LPCTSTR val)
{
	int len = 0;
	while (*val)
	{
		++len;
		MemSet(addr + len, *(val++));
	}
	MemSet(addr, len);
	MemSet(addr + len + 1, ' '); // For now, this is needed by CREATE
	return addr;
}

int ForthOS::MemToString(int addr, CString& val)
{
	int len = MemGet(addr);
	val.Empty();
	for (int i = 1; i <= len; i++)
	{
		CHAR c = MemGet(addr + i);
		val += c;
	}
	return len;
}

int ForthOS::GetNextWord(int& toIN, int stopAddr, int copyTo, CString& name)
{
	int len = 0;
	MemSet(copyTo, len);
	name.Empty();
	int addr = SOURCE_ADDRESS + toIN;

	// Skip leading whitespace
	while ((addr <= stopAddr) && iswspace(MemGet(addr)))
	{
		addr = SOURCE_ADDRESS + (++toIN);
	}

	if (addr > stopAddr)
	{
		return copyTo;
	}

	CHAR match = 0;
	CHAR c = MemGet(addr);
	if (c == '\'') 
	{
		int a1 = addr + 2;
		if ((a1 <= stopAddr) && (MemGet(a1) == '\''))
		{
			toIN += 3;
			name.Format(_T("'%c'"), (CHAR)MemGet(addr + 1));
			StringToMem(copyTo, name);
			return copyTo;
		}
	}
	else if (c == '\"')
	{
		addr = SOURCE_ADDRESS + (++toIN);
		match = c;
		MemSet(copyTo, ++len);
		MemSet(copyTo + len, c);
		name.AppendChar(c);
	}

	while (addr <= stopAddr)
	{
		c = MemGet(addr);
		bool addIt = (!iswspace(c)) || (match != 0);
		if ((match != 0) && (c == match))
		{
			match = 0;
		}
		if (addIt)
		{
			addr = SOURCE_ADDRESS + (++toIN);
			MemSet(copyTo, ++len);
			MemSet(copyTo + len, c);
			name.AppendChar(c);
		}
		else
		{
			break;
		}
	}

	return copyTo;
}

int ForthOS::ParseInput(LPCTSTR commands)
{
	inputStream = commands;
	CString name = commands;

	if (name == _T("load"))
	{
		Load();
		return 0;
	}

	if (name == _T("save"))
	{
		Save();
		return 0;
	}

	if (name == _T(".bs"))
	{
		BootStrap_FILE();
		return 0;
	}

	StringToMem(SOURCE_ADDRESS, inputStream);

	int toIN = 0; // >IN
	MemSet(TOIN_ADDRESS, ++toIN);

	int inputEnd = SOURCE_ADDRESS + MemGet(SOURCE_ADDRESS);
	int tmpBuf = inputEnd + 2;
	int addr = SOURCE_ADDRESS + toIN;

	while (addr <= inputEnd)
	{
		GetNextWord(toIN, inputEnd, tmpBuf, name);
		MemSet(TOIN_ADDRESS, toIN);
		if (!name.IsEmpty())
		{
			int err = false;
			try
			{
				ExecuteWord(tmpBuf, name);
			}
			catch (CString msg)
			{
				err = true;
				AppendOutput(msg);
			}
			catch (...)
			{
				err = true;
			}

			if (err)
			{
				SP = 0;
				RSP = 0;
				CSP = 0;
				MemSet(STATE_ADDRESS, 0);
			}
			toIN = MemGet(TOIN_ADDRESS);
		}
		addr = SOURCE_ADDRESS + toIN;
	}

	// at end of the inputStream
	if (userMode)
		AppendOutput(_T(" ok"));
	return toIN;
}


void ForthOS::BootStrap_FILE()
{
	userMode = false;
	CString cmds;
	FILE *fp = NULL;
	fopen_s(&fp, "BootStrap.4th", "rt");
	if (fp)
	{
		int i = 0;
		char buf[256];
		while (fgets(buf, sizeof(buf), fp) == buf)
		{
			cmds = buf;
			ParseInput(cmds);
		}
		fclose(fp);
	}
	userMode = true;
	AppendOutput(_T(" ok"));
}


void ForthOS::Load()
{
	FILE *fp = NULL;
	fopen_s(&fp, "session.txt", "rt");
	if (fp)
	{
		int i = 0;
		char buf[64];
		while (fgets(buf, sizeof(buf), fp) == buf)
		{
			int num = atoi(buf);
			MemSet(i++, num);
		}
		fclose(fp);
		AppendOutput(_T("Loaded."));
	}
}

void ForthOS::Save()
{
	FILE *fp = NULL;
	fopen_s(&fp, "session.txt", "wt");
	if (fp)
	{
		for (int i = 0; i < memorySize; i++)
		{
			fprintf(fp, "%d\n", MemGet(i));
		}
		fclose(fp);
		AppendOutput(_T("Saved."));
	}
}

// Primitives
void ForthOS::ExecuteWord(int startAddr, CString& name)
{
	if (MemGet(startAddr) == 0)
		return;

	bool isImmediate = false;
	int xt = TICK(startAddr, isImmediate);

	if (xt != 0)
	{
		if (MemGet(STATE_ADDRESS) == STATE_COMPILING) // Compiling?
		{
			if (isImmediate) // Execute it
			{
				EXECUTE(xt);
			}
			else // Build it into the word's definition
			{
				CommaCall(xt);
			}
		}
		else
		{
			EXECUTE(xt);
		}
		return;
	}

	int num = 0;
	if (StringIsNumber(name, num))
	//if (StringIsNumber(startAddr, num))
	{
		if (MemGet(STATE_ADDRESS) == STATE_COMPILING) // Compiling?
		{
			CommaLiteral(num);
		}
		else
		{
			PUSH(num);
		}
		return;
	}

	if (StringIsChar(startAddr, num))
	{
		if (MemGet(STATE_ADDRESS) == STATE_COMPILING) // Compiling?
		{
			CommaLiteral(num);
		}
		else
		{
			PUSH(num);
		}
		return;
	}

	if (StringIsString(startAddr))
	{
		int len = MemGet(startAddr++) - 2;
		MemSet(startAddr, len);
		//throw CString("quoted string not implemented.");
		if (MemGet(STATE_ADDRESS) == STATE_COMPILING) // Compiling?
		{
			// Put addr of string on the stack
			CommaLiteral(HERE()+4);
			COMMA(I_GOTO);
			COMMA(HERE() + len + 2);
			COMMA(len);
			for (int i = 1; i <= len; i++)
			{
				COMMA(MemGet(startAddr + i));
			}
		}
		else
		{
			name = name.Mid(1, name.GetLength() - 2);
			StringToMem(HERE() + 1, name);
			PUSH(HERE()+1);
			// throw CString("quoted string not implemented.");
		}
		return;
	}

	// QUIT logic
	CString msg; msg.Format(_T("'%s' not found."), name);
	throw msg;
}

void ForthOS::FormatNumber(int base, int num, CString& x)
{
	if (num == 0)
	{
		x = " 0";
		return;
	}

	if ((base < 2) || (base > 16))
		throw CString(_T("invalid BASE."));

	char *chars = "0123456789ABCDEF";
	char output[64], *cp = &output[63];
	*cp = (char)NULL;
	int q = num, r;
	while (q != 0)
	{
		r = q % base;
		q = q / base;
		*(--cp) = chars[r];
	}
	*(--cp) = ' ';
	x = cp;
}
