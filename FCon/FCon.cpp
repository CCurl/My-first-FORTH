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

int init();
int startup();
void SignalHandler(int action);

int go()
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
			return 0;
		}

		if (input.CompareNoCase(_T("restart")) == 0)
		{
			strcpy_s(line, sizeof(line), ".bs");
			len = 3;
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

void SignalHandler(int action)
{
	fprintf(stdout, "Signal %d caught", action);
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if (startup() != 0)
		return 1;

	CString arg, cmd;
	myOS = NULL;
	init();

	for (int i = 1; i < argc; i++)
	{
		arg = argv[i];
		if ((arg[0] == '-') || (arg[0] == '/'))
		{
			arg = arg.Mid(1);
			if (arg.CompareNoCase(_T("?")) == 0)
			{
				printf("usage: fcon -? -bs -include:fileName");
			}
			else if (arg.CompareNoCase(_T("bs")) == 0)
			{
				cmd.Format(_T(".bs"), arg);
				myOS->ParseInput(cmd);
			}
			else if (arg.Left(8).CompareNoCase(_T("include:")) == 0)
			{
				arg = arg.Mid(8);
				cmd.Format(_T("include %s"), arg);
				myOS->ParseInput(cmd);
			}
		}
	}

	return go();
}

void process_arg(CString& arg)
{
}

int startup()
{
	HMODULE hModule = ::GetModuleHandle(NULL);
	if (hModule == NULL)
	{
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		return 1;
	}
	// initialize MFC and print and error on failure
	if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		return 2;
	}
	return 0;
}

int init()
{

	if (myOS != NULL)
	{
		delete myOS;
		myOS = NULL;
	}
	else
	{
		signal(SIGABRT, SignalHandler);
		signal(SIGBREAK, SignalHandler);
		signal(SIGTERM, SignalHandler);
		signal(SIGINT, SignalHandler);
	}

	fp = stdin;
	myOS = new ForthOS(MEMORY_SIZE);
	myOS->input_fp = stdin;
	myOS->output_fp = stdout;
	myOS->BootStrap();

	return 0;
}
