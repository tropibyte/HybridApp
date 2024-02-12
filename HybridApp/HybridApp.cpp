
// HybridApp.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "HybridApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHybridApp

BEGIN_MESSAGE_MAP(CHybridApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHybridApp construction

CHybridApp::CHybridApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CHybridApp object

CHybridApp theApp;


// CHybridApp initialization

BOOL CHybridApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LPCTSTR pClassName = AfxRegisterWndClass(CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW);


//	m_pMainWnd = CWnd::Create(pClassName, "Hybrid App GUI", WS_OVERLAPPEDWINDOW, 0, 0, 200, 200, NULL, NULL, AfxGetInstanceHandle(), NULL);


#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	return m_pMainWnd != NULL ? TRUE : FALSE;
}

