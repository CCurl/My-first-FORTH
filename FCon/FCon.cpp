// FCon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <signal.h>
#include "FCon.h"

#define IS_CONSOLE
#include "..\forthos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

FILE *fp = NULL;
char line[128];
ForthOS *myOS = NULL;

void go()
{
	while (true)
	{
		if (fp == stdin)
		{
			fputs("\nFORTH>", stdout);
		}
		fgets(line, sizeof(line), fp);
		int len = strlen(line);
		if (line[len - 1] == '\n')
		{
			line[--len] = (char)NULL;
		}

		CString input(line);
		if (input.CompareNoCase(_T("bye")) == 0)
		{
			return;
		}

		if (input.CompareNoCase(_T("reset")) == 0)
		{
			delete myOS;
			myOS = NULL;
			line[0] = (char)NULL;
			len = 0;
		}

		if (myOS == NULL)
		{
			myOS = new ForthOS(MEMORY_SIZE);
			myOS->output_fp = stdout;
			myOS->BootStrap();
		}

		if (input.CompareNoCase(_T("dump")) == 0)
		{
			CString ret;
			myOS->Dump(ret);
			fputws(ret, stdout);
			continue;
		}

		myOS->ParseInput(CString(line));

		if (fp == stdin)
		{
			fputs(" ok", stdout);
		}
	}
}

void SigInt_Handler(int action)
{
	fprintf(stdout, "Signal %d caught", action);
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	HMODULE hModule = ::GetModuleHandle(NULL);


	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			signal(SIGABRT, SigInt_Handler);
			signal(SIGBREAK, SigInt_Handler);
			signal(SIGTERM, SigInt_Handler);
			signal(SIGINT, SigInt_Handler);
			fp = stdin;
			go();
		}
	}
	else
	{
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
