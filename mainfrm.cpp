// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Connect.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PLAYER_ONE, OnPlayerOneMessage)
	ON_MESSAGE(WM_PLAYER_TWO, OnPlayerTwoMessage)
	ON_MESSAGE(WM_PLAYER_ONE_INIT, OnPlayerOneInitMessage)
	ON_MESSAGE(WM_PLAYER_TWO_INIT, OnPlayerTwoInitMessage)
	ON_MESSAGE(WM_QUIT_GUI, OnQuitMessage)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.cx = 463;
	cs.cy = 478;
	cs.style = SWP_NOSIZE;

	cs.style = WS_OVERLAPPED | WS_CAPTION /*| FWS_ADDTOTITLE*/
		/*| WS_THICKFRAME*/ | WS_SYSMENU | WS_MINIMIZEBOX;

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


LRESULT CMainFrame::OnPlayerOneMessage(WPARAM wParam, LPARAM lParam)
{
	SendMessageToDescendants(WM_PLAYER_ONE, wParam, lParam);

	return 0;
}

LRESULT CMainFrame::OnPlayerTwoMessage(WPARAM wParam, LPARAM lParam)
{
	SendMessageToDescendants(WM_PLAYER_TWO, wParam, lParam);

	return 0;
}

LRESULT CMainFrame::OnPlayerOneInitMessage(WPARAM wParam, LPARAM lParam)
{
	SendMessageToDescendants(WM_PLAYER_ONE_INIT, wParam, lParam);

	return 0;
}

LRESULT CMainFrame::OnPlayerTwoInitMessage(WPARAM wParam, LPARAM lParam)
{
	SendMessageToDescendants(WM_PLAYER_TWO_INIT, wParam, lParam);

	return 0;
}
LRESULT CMainFrame::OnQuitMessage(WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}