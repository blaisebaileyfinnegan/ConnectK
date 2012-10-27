// OptionsBox.cpp : implementation file
//

#include "stdafx.h"
#include "Connect.h"
#include "OptionsBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsBox dialog


COptionsBox::COptionsBox(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsBox)
	//}}AFX_DATA_INIT
	m_K = 0;
	m_M = 0;
	m_N = 0;

}


void COptionsBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsBox)
	DDX_Control(pDX, IDC_XorO, m_XorO);
	DDX_Text(pDX, IDC_EDIT_K, m_K);
	DDV_MinMaxUInt(pDX, m_K, 1, 999);
	DDX_Text(pDX, IDC_EDIT_M, m_M);
	DDV_MinMaxUInt(pDX, m_M, 1, 999);
	DDX_Text(pDX, IDC_EDIT_N, m_N);
	DDV_MinMaxUInt(pDX, m_N, 1, 999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsBox, CDialog)
	//{{AFX_MSG_MAP(COptionsBox)
	ON_BN_CLICKED(IDC_O, OnO)
	ON_BN_CLICKED(IDC_X, OnX)
	ON_BN_CLICKED(IDC_GRAVITY_ON, OnGravityOn)
	ON_BN_CLICKED(IDC_GRAVITY_OFF, OnGravityOff)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsBox message handlers

void COptionsBox::OnO() 
{
	m_nOSelected = 1;
	m_nXSelected = 0;
}

void COptionsBox::OnX() 
{
	m_nOSelected = 0;
	m_nXSelected = 1;	
}


void COptionsBox::OnOK() 
{	
	CDialog::OnOK();
}


BOOL COptionsBox::OnInitDialog() 
{
	m_nXSelected = 1;
	m_M = 6;
	m_N = 7;
	m_K = 4;	
	m_gravity = true;

	::SendDlgItemMessage(m_hWnd,  // window handle 
    IDC_X,             // button identifier 
    BM_SETCHECK,          // message 
    1,                    // check state unchecked) 
    0);                   // must be zero        

    ::SendDlgItemMessage(m_hWnd,  // window handle 
    IDC_GRAVITY_ON,             // button identifier 
    BM_SETCHECK,          // message 
    1,                    // check state unchecked) 
    0);                   // must be zero        

	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsBox::OnGravityOn() 
{
	m_gravity = true;
	
}

void COptionsBox::OnGravityOff() 
{
	m_gravity = false;	
}

int COptionsBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
 
	return 0;
}
