// INTRASTAT.h : main header file for the INTRASTAT DLL
//

#if !defined(AFX_INTRASTAT_H__110D9B86_E2CA_11D3_97FB_00104BA3042C__INCLUDED_)
#define AFX_INTRASTAT_H__110D9B86_E2CA_11D3_97FB_00104BA3042C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CINTRASTATApp
// See INTRASTAT.cpp for the implementation of this class
//

class CINTRASTATApp : public CWinApp
{
public:
	CINTRASTATApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CINTRASTATApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CINTRASTATApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTRASTAT_H__110D9B86_E2CA_11D3_97FB_00104BA3042C__INCLUDED_)
