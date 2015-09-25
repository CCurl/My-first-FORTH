// FCon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FCon.h"
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
			fputs("\nFORTH> ", stdout);
		}
		fgets(line, sizeof(line), fp);
		int len = strlen(line);
		if (line[len - 1] == '\n')
			line[--len] = (char)NULL;

		if (strcmp(line, "bye") == 0)
		{
			return;
		}
		parse();
		CString output = myOS->output;
		if (! myOS->output.IsEmpty())
		{
			CT2A ascii(myOS->output);
			char *end = NULL;
			fputs(ascii.m_psz, stdout);
			myOS->output.Empty();
		}
		if (fp == stdin)
		{
			fputs(" ok", stdout);
		}
	}
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
			fp = stdin;
			go();
			// TODO: code your application's behavior here.
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
