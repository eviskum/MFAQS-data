// KAVALUTA.h : main header file for the KAVALUTA DLL
//

#if !defined(AFX_KAVALUTA_H__A96E6F19_2746_11D3_9020_00104BA3042C__INCLUDED_)
#define AFX_KAVALUTA_H__A96E6F19_2746_11D3_9020_00104BA3042C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKAVALUTAApp
// See KAVALUTA.cpp for the implementation of this class
//

class CKAVALUTAApp : public CWinApp
{
public:
	CKAVALUTAApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKAVALUTAApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CKAVALUTAApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KAVALUTA_H__A96E6F19_2746_11D3_9020_00104BA3042C__INCLUDED_)
