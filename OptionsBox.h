#if !defined(AFX_OPTIONSBOX_H__D7327002_5C5D_11D1_A72E_0060B05DEB40__INCLUDED_)
#define AFX_OPTIONSBOX_H__D7327002_5C5D_11D1_A72E_0060B05DEB40__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OptionsBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsBox dialog

class COptionsBox : public CDialog
{
// Construction
public:
	int m_nOSelected;
	int m_nXSelected;
	bool m_gravity;

	COptionsBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsBox)
	enum { IDD = IDD_OPTIONSBOX };
	CButton	m_XorO;
	UINT	m_K;
	UINT	m_M;
	UINT	m_N;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsBox)
	afx_msg void OnO();
	afx_msg void OnX();
	afx_msg void OnGravityOn();
	afx_msg void OnGravityOff();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSBOX_H__D7327002_5C5D_11D1_A72E_0060B05DEB40__INCLUDED_)
