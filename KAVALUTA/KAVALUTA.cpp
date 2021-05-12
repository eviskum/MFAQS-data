// KAVALUTA.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxtempl.h>
#include "KAVALUTA.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
using namespace std;

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
// CKAVALUTAApp

BEGIN_MESSAGE_MAP(CKAVALUTAApp, CWinApp)
	//{{AFX_MSG_MAP(CKAVALUTAApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKAVALUTAApp construction

CKAVALUTAApp::CKAVALUTAApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKAVALUTAApp object

CKAVALUTAApp theApp;


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

CArray<CString, CString> kavalout;

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

	TCHAR buf[500], buf_str[500];

	LJNORM("KA valuta overfoersels utility")
	if (argc != 2) {
		LJERR("Forkert brug !!!! Korrekt brug KAVAL checkdir ftpdir")
		return(DLLERROR);
	}
	kavalout.RemoveAll();
	CString kafilename;
	kafilename = argv[0];

	Sleep(5000);
	CStdioFile kafile;
	if (!kafile.Open(kafilename, CFile::modeRead | CFile::shareExclusive)) {
		LJERR("Kunne ikke aabne KA-valuta fil")
		return(DLLERROR);
	}
	if (kafile.ReadString(buf, 500) == FALSE) {
		kafile.Close();
		LJERR("fejl i KA-valuta fil")
		return(DLLERROR);
	}
	CommandLexer(buf);
	CString datestmp = (CTime::GetCurrentTime()).Format("%d.%m.%Y");
	CString timestmp = cmd_lex[1];
	timestmp.SetAt(2, '.'); timestmp.SetAt(5, '.');
	timestmp.SetAt(2, '.'); timestmp.SetAt(5, '.');
	kavalout.Add(CString("      Kl " + timestmp + "     UDVALGTE VALUTAER MOD DKK TERMIN"));
	kavalout.Add(CString("    Den " + datestmp));
	kavalout.Add(CString("------------------"));
	while (kafile.ReadString(buf, 500) != FALSE) {
		CommandLexer(buf);
		if (cmd_lex.GetSize() == 3) {
			double bidval[5], askval[5];
			CString val = cmd_lex[0].Left(3);
			double bid = atof(cmd_lex[1]), ask = atof(cmd_lex[2]);
			for (int i = 0; i < 5; i++) {
				if (kafile.ReadString(buf, 500) == FALSE) {
					kafile.Close();
					LJERR("fejl i KA-valuta fil")
					return(DLLERROR);
				}
				CommandLexer(buf);
				bidval[i] = atof(cmd_lex[1]);
				askval[i] = atof(cmd_lex[2]);
			}
			sprintf(buf_str, " % 10.4f% 10.4f", bid, ask);
			kavalout.Add(val + " SPOT " + val + buf_str);
			for (int i = 0; i < 5; i++) {
				CString tempval = " ";
				if (bidval[i] < 0) {
					tempval += "- ";
					bidval[i] = bidval[i] * (-1);
				} else tempval += "  ";
				sprintf(buf_str, "%06.3f", bidval[i]);
				tempval += buf_str;
				if (askval[i] < 0) {
					tempval += "  - ";
					askval[i] = askval[i] * (-1);
				} else tempval += "    ";
				sprintf(buf_str, "%06.3f", askval[i]);
				tempval += buf_str;
				switch (i) {
				case 0:
					kavalout.Add(val + " 01 M " + timestmp.Left(5) + tempval);
					break;
				case 1:
					kavalout.Add(val + " 02 M      " + tempval);
					break;
				case 2:
					kavalout.Add(val + " 03 M      " + tempval);
					break;
				case 3:
					kavalout.Add(val + " 06 M      " + tempval);
					break;
				case 4:
					kavalout.Add(val + " 12 M      " + tempval);
					break;
				}
			}
		}
	}
	kafile.Close();
	int length = kafilename.GetLength();
	while (kafilename[length-1] != '.') length--;
	CString newfilename = kafilename.Left(length) + (CTime::GetCurrentTime()).Format("%Y%m%d%H%M");
	TRY
	{
		CFile::Rename(kafilename, newfilename);
	}
	CATCH( CFileException, e )
	{
		LJERR("Kunne ikke rename' KA-valuta-fil")
		return(DLLERROR);
	}
	END_CATCH
	for (int i = 0; i < kavalout.GetSize(); i++) {
		kavalout[i] += CString(' ', 75 - kavalout[i].GetLength());
		kavalout[i] += CString("\n");
	}
	TRY
	{
		CStdioFile kavalcms(argv[1], CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
		for (int i = 0; i < kavalout.GetSize(); i++) {
			kavalcms.WriteString(kavalout[i]);
		}
		kavalcms.Close();
	}
	CATCH( CFileException, e )
	{
		LJERR("Kunne lave output-valuta-fil")
		return(DLLERROR);
	}
	END_CATCH
	LJNORM("Valuta-kurser overfoert til KA-system ");
	return(DLLSUCCES);
}
