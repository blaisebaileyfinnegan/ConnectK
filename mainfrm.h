// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__10677EA4_AD29_11D1_B3B2_004F49017471__INCLUDED_)
#define AFX_MAINFRM_H__10677EA4_AD29_11D1_B3B2_004F49017471__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	afx_msg LRESULT OnPlayerOneMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayerTwoMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayerOneInitMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayerTwoInitMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQuitMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__10677EA4_AD29_11D1_B3B2_004F49017471__INCLUDED_)
