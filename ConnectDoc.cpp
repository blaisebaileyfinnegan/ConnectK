// ConnectDoc.cpp : implementation of the CConnectDoc class
//

#include "stdafx.h"
#include <iostream>

using namespace std;

#include "Connect.h"
#include "ConnectK.h"
#include "OptionsBox.h"
#include "ConnectDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectDoc

IMPLEMENT_DYNCREATE(CConnectDoc, CDocument)

BEGIN_MESSAGE_MAP(CConnectDoc, CDocument)
	//{{AFX_MSG_MAP(CConnectDoc)
	ON_COMMAND(ID_FILE_NEWGAME, OnFileNewgame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectDoc construction/destruction

CConnectDoc::CConnectDoc()
	: ck(0), numMoves(1)
{
	ck = new ConnectK();
	board = 0;

	// added for tournament
	m_nPlayerNumber = 0;
}

CConnectDoc::~CConnectDoc()
{
	delete ck;

	for ( int i = 0; i < M; i++ )
		delete [] board[i];

	delete [] board;
}

/////////////////////////////////////////////////////////////////////////////
// CConnectDoc::OnNewDocument -- initializes data for the game
BOOL CConnectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	deleteBoard();

	compTime = 0;
	humanTime = 0;
	
	COptionsBox ob;
	do {
		if ( K > M && K > N )
			AfxMessageBox("The value of K is too large\nPlease choose a smaller K");
		int nResponse = ob.DoModal();
		if (nResponse == IDOK)
		{
			if (ob.m_nXSelected)
			{
				mark = 'X';
				playersmark = 'X';
			}
			else
			{
				mark = 'O';
				playersmark = 'O';
			}
	
			M = ob.m_M;
			N = ob.m_N;
			K = ob.m_K;

			G = ob.m_gravity;
		}
	} while ( K > M && K > N );

	if (playersmark == 'O')
		computerMark = 'X';
	else
		computerMark = 'O';

	newBoard();

	numMoves = 1;

	ck->newGame(M,N,K,G, computerMark, playersmark);

	DeleteFile("gamelog.txt");

	if (playersmark == 'O')
	{
		playersMove = false;
		ComputerMove(-1,-1);
		playersMove = true;
	}
	currTime1 = CTime::GetCurrentTime();

	UpdateAllViews(NULL);

	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CConnectDoc win game check -- added to check for a winner
void CConnectDoc::GameOverCheck()
{
	char winner;
	if (win(winner))
	{
		CString temp = "The winner is ";

		if (winner == 'X')
			temp += "Player 1";
		else
			temp += "Player 2";


		CString com;
		CString hum;

		if (compTime > 0)
			com = compTime.Format("%H:%M:%S");
		else
			com = " < 1 sec.";
		hum = humanTime.Format("%H:%M:%S");

		AfxMessageBox(temp + "\nComputer Time: " + com + "\nHuman Time:    " + hum);
	}
	else if (tie())
	{
		CString temp;
		CString com;
		CString hum;
		if (compTime > 0)
			com = compTime.Format("%H:%M:%S");
		else
			com = " < 1 sec.";
		hum = humanTime.Format("%H:%M:%S");

		AfxMessageBox("It's a tie!\nComputer Time: " + com + "\nHuman Time:    " + hum);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CConnectDoc computer move -- added to make the computer move
bool CConnectDoc::ComputerMove(int row, int col)
{
	char winner;
	CDocument::UpdateAllViews(NULL);
	if (!win(winner) && !tie())
	{	

		currTime1 = CTime::GetCurrentTime();

		playersMove = false;

		BeginWaitCursor();
		CPoint* oppmove = new CPoint(0,0);

		ck->nextMove(row, col);

		oppmove->x = row;
		oppmove->y = col;

		occupy(oppmove->x, oppmove->y, computerMark);
		
		numMoves++;

		GameLog("Computer", oppmove->x, oppmove->y);

		delete oppmove;
		EndWaitCursor();

		playersMove = true;

		currTime2 = CTime::GetCurrentTime();

		compTime += currTime2 - currTime1;

	}
	currTime1 = CTime::GetCurrentTime();

	CDocument::UpdateAllViews(NULL);
	GameOverCheck();
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CConnectDoc player move -- added to place the player's piece
bool CConnectDoc::PlayerMove(int row, int col)
{
	char winner;
	if (!win(winner) && !tie())
	{	
		if (occupy(row,col,playersmark))
		{
			currTime2 = CTime::GetCurrentTime();
			humanTime += currTime2 - currTime1;
			numMoves++;
			CDocument::UpdateAllViews(NULL);

			if (G)
				GameLog("Player", 0, col);
			else
				GameLog("Player", row, col);

			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CConnectDoc serialization

void CConnectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {}
	else{}
}

/////////////////////////////////////////////////////////////////////////////
// CConnectDoc diagnostics

#ifdef _DEBUG
void CConnectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CConnectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConnectDoc commands

void CConnectDoc::OnFileNewgame() 
{
	delete ck;
	CConnectDoc::OnNewDocument();
	CDocument::UpdateAllViews(NULL);
}

void CConnectDoc::OnResetGame()
{
	numMoves = 1;
	
	deleteBoard();

	compTime = 0;
	humanTime = 0;

	newBoard();

	ck->newGame(M,N,K,G, computerMark, playersmark);

	DeleteFile("gamelog.txt");

	if (playersmark == 'O')
	{		
		playersMove = false;
		ComputerMove(-1,-1);
		playersMove = true;
	}

	currTime1 = CTime::GetCurrentTime();

	UpdateAllViews(NULL);

}

void CConnectDoc::newBoard()
{
	numOccupied = 0;

	if ( M > 0 && N > 0 )
	{
		board = new CharArray[M];
		for ( int i = 0; i < M; i++ )
		{
			board[i] = new char[N];
			for ( int j = 0; j < N; j++ )
				board[i][j] = BLANK;
		}
	}
}

bool CConnectDoc::win(char & mark)
{
	switch ( evaluate() )
	{
		case WIN_X:
		    mark = X;
		    return true;
	    case WIN_O: 
		    mark = O;
		    return true;
        default:
			mark = BLANK;
		    return false;
	}
}

bool CConnectDoc::tie()
{
	return numOccupied == M * N; 
}

bool CConnectDoc::occupy(int row, int col, char mark)
{
		int i = 0;

	if ( !isMarkGood(mark) || !inRange(row, col) ) 
		return false;

	if ( G )    // if gravity is true, than we let the token fall...
	{
		for ( i = M - 1; i >= 0; i-- )
		{
			if ( !isOccupied(i, col) )   // ...until it hits another token or... 
			{
				board[i][col] = mark;
				numOccupied++;
				return true;
			}
		}

	    return false;                    // ...until it hits the top
	}
	else								// otherwise the token can occupy
	{									// any position
		if ( isOccupied(row, col) )
			return false;

		board[row][col] = mark;
		numOccupied++;
		return true;
	}
}

char CConnectDoc::at(int row, int col)
{
	return board[row][col];
}

long CConnectDoc::evaluate()
{
	int numX = 0;
	int numO = 0;
	long value = 0;
	int row = 0;
	int col = 0;
	int i = 0;
	int j = 0;

	/* Check all the horizontal groups of K */
	for ( row = 0; row < M; row++ )
	{
		for ( col = 0; col < N-K+1; col++, numX = 0, numO = 0 )
		{
			for ( j = col; j < K + col && j < N; j++ )
			{
                switch ( board[row][j] )
			    {
     			    case X:
						numX++;
					    break;
				    case O:
						numO++;
					    break;
				    default:
					    break;
			    }
            }

            if ( numX == K )
                return WIN_X;
            else if ( numO == K )
                return WIN_O;
            else if ( numX == 0 )
                value = value - numO;
            else if ( numO == 0 )
                value = value + numX;
		}
	}

	/* Check all the vertical groups of K */
	for ( col = 0; col < N; col++ )
	{
		for ( row = 0; row < M-K+1; row++, numX = 0, numO = 0 )
		{
			for ( i = row; i < K + row && i < M; i++ )
			{
			    switch ( board[i][col] )
			    {
				    case X:
					    numX++;
					    break;
				    case O:
				        numO++;
					    break;
				    default:
					    break;
			    }
			}						

            if ( numX == K )
                return WIN_X;
            else if ( numO == K )
                return WIN_O;
            else if ( numX == 0 )
                value = value - numO;
            else if ( numO == 0 )
                value = value + numX;
		}
	}

	/* Check each diagonal forward group of K */
	/*
		  X
		 X
	    X
	*/
	for ( col = 0; col < N-K+1; col++ )
	{
		for ( row = K - 1; row < M; row++, numX = 0, numO = 0 ) 
		{
			for ( i = row, j = col; j < K + col && i >= 0 && j < N; i--, j++ )
			{
				switch ( board[i][j] )
				{
				    case X:
					    numX++;
					    break;
				    case O:
				        numO++;
					    break;
				    default:
					    break;
				}
			}

			if ( numX == K )
                return WIN_X;
            else if ( numO == K )
                return WIN_O;
            else if ( numX == 0 )
                value = value - numO;
            else if ( numO == 0 )
                value = value + numX;
		}
	}
	
	/* Check each diagonal backward group of K */
	/*
	    X
		 X
		  X
	*/
	for ( col = 0; col < N-K+1; col++ )
	{
		for ( row = 0; row < M-K+1; row++, numX = 0, numO = 0 ) 
		{
			for ( i = row, j = col; i < K + row && i < M && j < N; i++, j++ )
			{
				switch ( board[i][j] )
				{
				    case X:
					    numX++;
					    break;
				    case O:
				        numO++;
					    break;
				    default:
					    break;
				}
			}

			if ( numX == K )
                return WIN_X;
            else if ( numO == K )
                return WIN_O;
            else if ( numX == 0 )
                value = value - numO;
            else if ( numO == 0 )
                value = value + numX;
		}
	}

	return value;
}

bool CConnectDoc::isMarkGood(char mark)
{
	switch (mark)
	{
		case X:
		case O:
		case BLANK:
			return true;
		default:
			cerr << "Error: bad marking." << endl;
			return false;
	}
}

bool CConnectDoc::inRange(int row, int col)
{
	return row > -1 && row < M && col > -1 && col < N;
}

bool CConnectDoc::isOccupied(int row, int col)
{
	return board[row][col] != BLANK;
}

void CConnectDoc::deleteBoard()
{
		// re-initialize the board
	if ( board != NULL )
	{
		for ( int i = 0; i < M; i++ )
			delete [] board[i];
		board = 0;	
		delete [] board;

		board = NULL;
	}

}

void CConnectDoc::GameLog(CString currentCompetitor, int x, int y)
{
	CString xs, ys, str;

	xs.Format("%i",x);
	ys.Format("%i",y);

	str = currentCompetitor + "'s move: (" + xs + ", " + ys + ")\n";

	CStdioFile logFile;

	logFile.Open("gamelog.txt", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	logFile.SeekToEnd();
	//logFile.WriteString(str);
	logFile.Close();
}
