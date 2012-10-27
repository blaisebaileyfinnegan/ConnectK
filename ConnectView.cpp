// ConnectView.cpp : implementation of the CConnectView class
//

#include "stdafx.h"
#include "Connect.h"
#include "ConnectK.h"
#include "ConnectDoc.h"
#include "ConnectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectView

IMPLEMENT_DYNCREATE(CConnectView, CFormView)

BEGIN_MESSAGE_MAP(CConnectView, CFormView)
	//{{AFX_MSG_MAP(CConnectView)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, OnButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_COMMAND(ID_FILE_RESTART, OnFileRestart)
	ON_COMMAND(ID_FILE_GAMELOG, OnFileGamelog)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)

	// added for tournament
	ON_MESSAGE(WM_PLAYER_ONE, OnPlayerOneMessage)
	ON_MESSAGE(WM_PLAYER_TWO, OnPlayerTwoMessage)
	ON_MESSAGE(WM_PLAYER_ONE_INIT, OnPlayerOneInitMessage)
	ON_MESSAGE(WM_PLAYER_TWO_INIT, OnPlayerTwoInitMessage)


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectView construction/destruction

CConnectView::CConnectView()
	: CFormView(CConnectView::IDD)
{
	//{{AFX_DATA_INIT(CConnectView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_pClientDC = NULL;
}

CConnectView::~CConnectView()
{
	delete m_pClientDC;
}

void CConnectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CConnectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CConnectView printing

BOOL CConnectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CConnectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CConnectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CConnectView::OnPrint(CDC* pDC, CPrintInfo*)
{
	// TODO: add code to print the controls
}

/////////////////////////////////////////////////////////////////////////////
// CConnectView diagnostics

#ifdef _DEBUG
void CConnectView::AssertValid() const
{
	CFormView::AssertValid();
}

void CConnectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CConnectDoc* CConnectView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CConnectDoc)));
	return (CConnectDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConnectView message handlers

void CConnectView::OnLButtonUp(UINT nFlags, CPoint point) 
{

	CConnectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect rcClient;
	GetClientRect(rcClient);

	int l,r,t,b;

	l = r = 25;
	t = 35;
	b = 75;
	
	rcClient.DeflateRect(l, t, r, b);

	movePoint = point;

	int M = pDoc->M;
	int N = pDoc->N;
	int row = 0;
	int col = 0;

	if (!rcClient.PtInRect( point ) )
	{
	}
	else
	{
		if (movePoint.x != 0 || movePoint.y != 0)
		{
			for (int r=1; r<=M; r++)
			{
				if ((double)(movePoint.y-t)/(double)rcClient.Height() <= (double)r/(double)M)
				{
					row = r-1;
					break;
				}
			}
			for (int c=1; c<=N; c++)
			{
				if ((double)(movePoint.x-l)/(double)rcClient.Width() <= (double)c/(double)N)
				{
					col = c-1;
					break;
				}
			}
		}
		if (pDoc->PlayerMove(row, col))
		{
			DrawBoard();

			CBrush greenBrush(RGB(0, 255, 0));
			CBrush redBrush(RGB(255, 0, 0));

			CPen greenPen(PS_SOLID, 1, RGB(0, 255, 0));
			CPen redPen(PS_SOLID, 1, RGB(255, 0, 0));


			CBrush* oldBrush;
			CPen* oldPen;

			oldBrush = m_pClientDC->SelectObject(&greenBrush);
			oldPen = m_pClientDC->SelectObject(&greenPen);
			m_pClientDC->Ellipse(12, 10, 26, 24);

			m_pClientDC->SelectObject(&redBrush);
			m_pClientDC->SelectObject(&redPen);
			m_pClientDC->Ellipse(262, 10, 276, 24);

			m_pClientDC->SelectObject(oldBrush);
			m_pClientDC->SelectObject(oldPen);

			DeleteObject(greenBrush);
			DeleteObject(greenPen);
			DeleteObject(redBrush);
			DeleteObject(redPen);

			while ( !pDoc->ComputerMove(row,col)); //wait for computer to move before allowing players move.
		}
	}
	DrawBoard();
	movePoint.x = 0;
	movePoint.y = 0;
	
	CFormView::OnLButtonUp(nFlags, point);
}

void CConnectView::OnDraw(CDC* pDC) 
{
	CConnectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if ( pDoc->board != NULL)
		DrawBoard();
}

void CConnectView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_pClientDC = new CClientDC(this);
	
	human.LoadBitmap( IDB_BITMAP_HUMAN );
	human.GetObject( sizeof(BITMAP), &bmHuman);
	dcMemHuman.CreateCompatibleDC(m_pClientDC);
	dcMemHuman.SelectObject( &human );

	computer.LoadBitmap( IDB_BITMAP_COMPUTER );
	computer.GetObject( sizeof(BITMAP), &bmComputer);
	dcMemComputer.CreateCompatibleDC(m_pClientDC);
	dcMemComputer.SelectObject( &computer );

}

void CConnectView::OnButtonQuit() 
{
	ASSERT_VALID(AfxGetMainWnd());
	AfxGetMainWnd()->PostMessage(WM_CLOSE);	
}


void CConnectView::OnButtonNew() 
{
	CConnectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnNewDocument();		
}

void CConnectView::OnButtonReset() 
{
	CConnectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnResetGame();
}

void CConnectView::DrawBoard()
{
	CConnectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect rcClient;
	CPen penBlue( PS_SOLID, 10, RGB(0,0,255));
	CPen penRed( PS_SOLID, 10, RGB(255,0,0));
	CPen penBlack( PS_SOLID, 6, RGB(0,0,0));
	
	GetClientRect(rcClient);


	int l,r,t,b;

	l = r = 25;
	t = 35;
	b = 75;
	
	rcClient.DeflateRect(l, t, r, b);

	CBrush whiteBrush( RGB(255,255,255));
	CBrush blackBrush( RGB(0,0,0));

	m_pClientDC->FillRect(rcClient, &whiteBrush);
	m_pClientDC->FrameRect(rcClient, &blackBrush);

	int M = pDoc->M;
	int N = pDoc->N;

	//draw the game grid
	m_pClientDC->SelectObject( &penBlack );
	for (int j=0; j<=N; j++)
	{
		//draw vertical lines for the columns (N)
		m_pClientDC->MoveTo(j*rcClient.Width()/N+l,0+t);
		m_pClientDC->LineTo(j*rcClient.Width()/N+l,rcClient.Height()+t);
	}

	for (int k=0; k<=M; k++)
	{
		//draw horizontal lines for the rows (M)
		m_pClientDC->MoveTo(0+l,k*rcClient.Height()/M+t);
		m_pClientDC->LineTo(rcClient.Width()+l,k*rcClient.Height()/M+t);
	}

	for (int q=0; q<N; q++)  //for each column loop
	{
		for (int p=0; p<M; p++)  //for each row loop
		{
			if (pDoc->at(p,q) == 'X')
			{
				/* Draw player 1 bitmap on the board */
				if ( M > 10 || N > 10)
					m_pClientDC->TextOut(((3*q+1)*rcClient.Width()/(3*N))+l-5/*-bmHuman.bmWidth/2*/,((3*p+1)*rcClient.Height()/(3*M))+t-5/*-bmHuman.bmHeight/2*/,"1");
				else
					m_pClientDC->BitBlt(((3*q+1)*rcClient.Width()/(3*N))+l-5/*-bmHuman.bmWidth/2*/,((3*p+1)*rcClient.Height()/(3*M))+t-5/*-bmHuman.bmHeight/2*/,bmHuman.bmWidth,bmHuman.bmHeight,&dcMemHuman,0,0,SRCCOPY);
			}
			else if (pDoc->at(p,q) == 'O')
			{	
				/* Draw player 2 bitmap on the board */
				if ( M > 10 || N > 10)
					m_pClientDC->TextOut(((3*q+1)*rcClient.Width()/(3*N))+l-5/*-bmComputer.bmWidth/2*/,((3*p+1)*rcClient.Height()/(3*M))+t-5/*-bmComputer.bmHeight/2*/,"2");
				else
					m_pClientDC->BitBlt(((3*q+1)*rcClient.Width()/(3*N))+l-5/*-bmComputer.bmWidth/2*/,((3*p+1)*rcClient.Height()/(3*M))+t-5/*-bmComputer.bmHeight/2*/,bmComputer.bmWidth,bmComputer.bmHeight,&dcMemComputer,0,0,SRCCOPY);	
			}
			else if (pDoc->at(p,q) == ' ')
			{			
			}
			else
				AfxMessageBox("Error in board");

		}
	}
	m_pClientDC->TextOut(30,10,pDoc->compTime.Format("Computer Time: %H:%M:%S") );
	m_pClientDC->TextOut(280,10,pDoc->humanTime.Format("Human Time: %H:%M:%S") );

	CBrush greenBrush(RGB(0, 255, 0));
	CBrush redBrush(RGB(255, 0, 0));

	CPen greenPen(PS_SOLID, 1, RGB(0, 255, 0));
	CPen redPen(PS_SOLID, 1, RGB(255, 0, 0));


	CBrush* oldBrush;
	CPen* oldPen;

	oldBrush = m_pClientDC->SelectObject(&greenBrush);
	oldPen = m_pClientDC->SelectObject(&greenPen);
	m_pClientDC->Ellipse(262, 10, 276, 24);

	m_pClientDC->SelectObject(&redBrush);
	m_pClientDC->SelectObject(&redPen);
	m_pClientDC->Ellipse(12, 10, 26, 24);

	m_pClientDC->SelectObject(oldBrush);
	m_pClientDC->SelectObject(oldPen);

	DeleteObject(greenBrush);
	DeleteObject(greenPen);
	DeleteObject(redBrush);
	DeleteObject(redPen);
}

void CConnectView::OnFileRestart() 
{
	CConnectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnResetGame();		
}

void CConnectView::OnFileGamelog() 
{
	::ShellExecute(NULL, "open", _T("gamelog.txt"), NULL, NULL, SW_SHOW);	
}

LRESULT CConnectView::OnPlayerOneMessage(WPARAM wParam, LPARAM lParam)
{
	CConnectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(pDoc->m_nPlayerNumber != 1)
	{
		return 0;
	}

	int x = wParam;
	int y = lParam;

	pDoc->ck->nextMove(x, y);

	BOOL b = ::PostMessage(HWND_BROADCAST, WM_MOVE_RESPONSE, x, y);

	return 0;
}

LRESULT CConnectView::OnPlayerTwoMessage(WPARAM wParam, LPARAM lParam)
{
	CConnectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(pDoc->m_nPlayerNumber != 2)
	{
		return 0;
	}
	
	int x = wParam;
	int y = lParam;

	pDoc->ck->nextMove(x, y);

	BOOL b = ::PostMessage(HWND_BROADCAST, WM_MOVE_RESPONSE, x, y);

	return 0;
}

LRESULT CConnectView::OnPlayerOneInitMessage(WPARAM wParam, LPARAM lParam)
{
	CConnectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(pDoc->m_nPlayerNumber != 0)
	{
		return 0;
	}
	else
	{
		pDoc->m_nPlayerNumber = 1;

		//  lParam layout
		//   ------------------------------------
		//  |HIWORD	 		  |	LOWORD			 |
		//  |------------------------------------|
		//  |HIBYTE	| LOBYTE  |	HIBYTE  | LOBYTE |
		//	|= M	| = N	  |	= K		| = G    |
		//   ------------------------------------

		bool bG;

		LOBYTE(LOWORD(lParam)) ? bG = true : bG = false;

		int m = HIBYTE(HIWORD(lParam));
		int n = LOBYTE(HIWORD(lParam));
		int k = HIBYTE(LOWORD(lParam));
		int g = LOBYTE(LOWORD(lParam));

		pDoc->ck->newGame(	HIBYTE(HIWORD(lParam)), //M
							LOBYTE(HIWORD(lParam)), //N
							HIBYTE(LOWORD(lParam)), //K
							bG, //G
							'X'); //mark for competitor 1
	}

	// just to notify that message was received
	BOOL b = ::PostMessage(HWND_BROADCAST, WM_INIT_RESPONSE, wParam, lParam);

	return 0;
}

LRESULT CConnectView::OnPlayerTwoInitMessage(WPARAM wParam, LPARAM lParam)
{
	CConnectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(pDoc->m_nPlayerNumber != 0)
	{
		return 0;
	}
	else
	{
		pDoc->m_nPlayerNumber = 2;

		//  lParam layout
		//   ------------------------------------
		//  |HIWORD	 		  |	LOWORD			 |
		//  |------------------------------------|
		//  |HIBYTE	| LOBYTE  |	HIBYTE  | LOBYTE |
		//	|= M	| = N	  |	= K		| = G    |
		//   ------------------------------------

		bool bG;

		LOBYTE(LOWORD(lParam)) ? bG = true : bG = false;
		
		pDoc->ck->newGame(	HIBYTE(HIWORD(lParam)), //M
							LOBYTE(HIWORD(lParam)), //N
							HIBYTE(LOWORD(lParam)), //K
							bG, //G
							'O'); //mark for competitor 2
	}

	// just to notify that message was received
	BOOL b = ::PostMessage(HWND_BROADCAST, WM_INIT_RESPONSE, wParam, lParam);

	return 0;
}