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


void parse()
{
	if (myOS == NULL)
	{
		myOS = new ForthOS(8192);
		myOS->output_fp = stdout;
		myOS->BootStrap();
	}
	myOS->ParseInput(CString(line));
}

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
			line[--len] = (char)NULL;

		if (strcmp(line, "bye") == 0)
		{
			return;
		}

		if (strcmp(line, "reset") == 0)
		{
			delete myOS;
			myOS = NULL;
		}

		parse();
		//CString output = myOS->output;
		//if (! myOS->output.IsEmpty())
		//{
		//	CT2A ascii(myOS->output);
		//	char *end = NULL;
		//	fputs(ascii.m_psz, stdout);
		//	myOS->output.Empty();
		//}
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
