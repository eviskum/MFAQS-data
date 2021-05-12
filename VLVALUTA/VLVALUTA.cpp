// VLVALUTA.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxtempl.h>
#include "VLVALUTA.h"
#include <stdio.h>
#include <conio.h>
#include <iostream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CVLVALUTAApp

BEGIN_MESSAGE_MAP(CVLVALUTAApp, CWinApp)
	//{{AFX_MSG_MAP(CVLVALUTAApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVLVALUTAApp construction

CVLVALUTAApp::CVLVALUTAApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVLVALUTAApp object

CVLVALUTAApp theApp;


#define DLLSUCCES	1
#define DLLERROR	2
#define LOGNORMAL	1
#define LOGERROR	2
#define LNORM( txt )	Log(txt, LOGNORMAL, FALSE);
#define LERR( txt )		Log(txt, LOGERROR, FALSE);
#define LJNORM( txt )	Log(txt, LOGNORMAL, TRUE);
#define LJERR( txt )	Log(txt, LOGERROR, TRUE);

HANDLE hLogEvent, hLogReady;
TCHAR log_txt[500];
int log_mode, log_job;

CArray<CString, CString> vlvalout;

CArray<CString,CString> cmd_lex;

void CommandLexer(LPCTSTR command)
{
	CString cmd(command);
	cmd_lex.RemoveAll();
	int start_idx = 0, end_idx = 0, length = cmd.GetLength();
	while (end_idx < length-1) {
		while (start_idx < length && cmd[start_idx] == ' ') start_idx++;
		end_idx = start_idx;
		while (end_idx < length && cmd[end_idx] != ' ') end_idx++;
		if (start_idx < length) cmd_lex.Add(cmd.Mid(start_idx, end_idx-start_idx));
		start_idx = end_idx;
	}
}

void Log(LPCTSTR txt, int mode, int job) {
	WaitForSingleObject(hLogReady, INFINITE);
	strcpy(log_txt, txt);
	log_mode = mode;
	log_job = job;
	SetEvent(hLogEvent);
}

extern "C" int PASCAL EXPORT DllGetLog(LPTSTR txt, int *mode, int *job)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	strcpy(txt, log_txt);
	*mode = log_mode;
	*job = log_job;
	return(0);
}

extern "C" int PASCAL EXPORT DllLogReg( HANDLE set_event, HANDLE set_ready )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	hLogEvent = set_event;
	hLogReady = set_ready;
	return(0);
}


extern "C" int PASCAL EXPORT DllWorker ( int argc, char *argv[] )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TCHAR buf[500];

	LJNORM("VL valuta overfoersels utility")
	if (argc != 2) {
		LJERR("Forkert brug !!!! Korrekt brug KAVAL checkdir ftpdir")
		return(DLLERROR);
	}
	vlvalout.RemoveAll();

	// ekstra blank linie
	vlvalout.Add(CString("               \n"));

	CString vlfilename;
	vlfilename = argv[0];

	int i;
	Sleep(5000);

	TRY
	{
		CStdioFile vlfile(vlfilename, CFile::modeRead);
		while (vlfile.ReadString(buf, 500) != FALSE) {
			for (i = 0; i < 500; i++) {
				if (buf[i] == ',') buf[i] = '.';
				else if (buf[i] == '.') buf[i] = ',';
			}
			vlvalout.Add(CString(buf));
		}
		vlfile.Close();
	}
	CATCH( CFileException, e )
	{
		LJERR("FEJL: kunne ikke aabne og laese valuta-fil")
		return(DLLERROR);
	}
	END_CATCH

	TRY
	{
		CStdioFile vlval123(argv[1], CFile::modeCreate | CFile::modeWrite);
		for (i = 0; i < vlvalout.GetSize(); i++) {
			vlval123.WriteString(vlvalout[i]);
		}
		vlval123.Close();
	}
	CATCH( CFileException, e )
	{
		LJERR("FEJL: kunne ikke aabne og skrive output-valuta-fil")
		return(DLLERROR);
	}
	END_CATCH

	TRY
	{
		CFile::Remove(vlfilename);
	}
	CATCH( CFileException, e )
	{
		LJERR("FEJL: kunne ikke slette valuta-trigger-fil")
		return(DLLERROR);
	}
	END_CATCH


	LJNORM("Valuta-kurser overfoert til 123 ");
	return(DLLSUCCES);
}
