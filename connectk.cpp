#include "StdAfx.h"
#include <iostream>
#include <conio.h>
#include <vector>

using namespace std;

#include "ConnectK.h"

// constructor
ConnectK::ConnectK()
	: M(0), N(0), K(0), G(FALSE)
{
}

// destructor
ConnectK::~ConnectK()
{
}

/////////////////////////////////////////////////////////////////////////////////////////
// newGame(int pM, int pN, int pK, bool pG, char pmark)
//
// Purpose:		This function is to allocate memory and initialize structures necessary 
//				to play the game defined by the parameters, M, N, K and G passed by the
//				GUI (or other driving program to this class).  In this sample code, we are
//				using a two dimensional dynamic array of characters to represent the board
//				and the symbols X, O, and BLANK (defined in the ConnectK.h file) to keep
//				track of game progress.
//
//	Parameters:	pM		number of rows in the game board
//				pN		number of columns in the game board
//				pK		number in a row to win the game
//				pG		Gravity on/off. Determines whether or not game pieces fall to the 
//						lowest available position in a column (as in Connect 4), or remain
//						in the position places (as in Tic-tac-toe).
//				pmark	this is the mark that the computer player has been assigned.
//						it will be either 'X' or 'O'.  It is not actually necessary to use
//						this variable, but you may find it useful to know which player will
//						move first, prior to nextMove() being called.
//
// Returns:		nothing.
//
// NOTE:		This function can be called at any time from the GUI.  Make sure it 
//				can re-allocate memory and re-initialize variables, game after game.
//
//				The board layout used by the GUI has the origin located at the top left corner
//				of the board.  For example:
//
//					N columns
//
//				  0  1  2..N-1
//               +-----------+
//              0|	|  |  |	 |
//               |--+--+--+--|
//				1|	|  |  |	 |
//     M rows    |--+--+--+--|
//				2|  |  |  |	 |
//             ..|--+--+--+--|
//			  M-1|  |  |  |  |
//               +-----------+
//
void ConnectK::newGame(int pM, int pN, int pK, bool pG, char pmark, char hmark)
{	
	board.clear();

	// re-initialize all variables
	// p is for parameter
	M = pM;
	N = pN;
	K = pK;
	G = pG;
	computerMark = pmark;
	humanMark = hmark;

	// initialize the board to all blank characters (see ConnectK.h for definitions)
	if ( M > 0 && N > 0 )
	{
		board.resize(M, vector<char>(N, BLANK));
	}
	else
		cerr << "Error: bad array size." << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////
// nextMove(int &row, int &col)
//
// Purpose:		This function receives the move made by the human player (or other AI
//				player), records it, and returns a move based on an AI search routine.
//
// Parameters:	&row	this holds the row coordinate of the move made by the human player
//				&col	this holds the column coordinate of the move made by the human player
//
// Returns:		&row	place the row value of the move made by your AI search in this variable
//				&col	place the column value of the move made by your AI in this variable
//
// NOTE:		Remember to do all initialization in newGame().  This function must be ready to 
//				return a move when given one, and will determine the time taken by your program
//				in tournament competition.  Make your search as efficient as possible.
//				
void ConnectK::nextMove(int &row, int &col)
{
	if (G)
	{
		row = M - 1; // Have to account for gravity (GUI gives where a mouse is clicked)
		while (board[row][col] != BLANK)
		{
			row--;
		}
	}
	// If x and y are not -1 then we need to record the move made by the human player
	// If x and y are -1, then this is the first move of the game, the AI moves first.
	if( ( row != -1 ) && ( col != -1 ) )
	{
		if (computerMark == X)
			board[row][col] = O;
		else
			board[row][col] = X;
	}

	ExpirationTimer timer(5.0f);
	timer.Start();

	int rowMoveToMake = -1;
	int columnMoveToMake = -1;
	int currentMaxDepth = -1;
	while (!timer.HasExpired())
	{
		++currentMaxDepth;
		int valueOfBestMove = minimax(board, -INFINITY, INFINITY, 0, true, rowMoveToMake, columnMoveToMake, currentMaxDepth, timer);
	}

#ifdef _DEBUG
	_cprintf("AI IDS went to a depth of %i before stopping (may not have finished at this depth).\n", currentMaxDepth);
#endif

	// record the move made by the AI
	board[rowMoveToMake][columnMoveToMake] = computerMark;
	// return the move made by the AI
	row = rowMoveToMake;
	col = columnMoveToMake;

#ifdef _DEBUG
	_cprintf("Evaluation function for move (%i, %i) returned: %i\n", rowMoveToMake, columnMoveToMake, this->evaluate(board));
#endif
}

// AI Evaluation function
// (own winning rows) - (opponent's winning rows)
// board: The potential game state
int ConnectK::evaluate(const CharVectorVector& board) const
{
	int ownWinningRows = 0, opponentWinningRows = 0;

	// Search for squares
	for (int row = M - 1; row >= 0 ; row--)
	{
		for (int col = 0; col < N; col++)
		{
			// We've found a piece that belongs to us
			if (board[row][col] == computerMark) 
				ownWinningRows += countWinningRectangles(board, row, col, computerMark);
			else if (board[row][col] == humanMark)
				opponentWinningRows += countWinningRectangles(board, row, col, humanMark);
		}
	}

#ifdef _DEBUG
	_cprintf("\tMy winning rows: %i\n", ownWinningRows);
	_cprintf("\tOpponent's winning rows: %i\n", opponentWinningRows);
#endif

	return ownWinningRows - opponentWinningRows;
}

int ConnectK::countWinningRectangles(const CharVectorVector& board, int row, int col, char mark) const
{
	int rectangles = 0;

	// Count horizontal winning rectangles
	for (int i = K - 1; i >= 0; i--)
	{
		if ((col - i) < 0)
			continue;

		bool winning = true;
		for (int j = 0; j < K && winning; j++)
		{
			if ((col-i+j) >= N)
			{
				winning = false;
				break;
			}

			if (board[row][col-i+j] != BLANK && board[row][col-i+j] != mark)
				winning = false;
		}

		if (winning)
			rectangles++;
	}

	// Count vertical winning rectangles
	for (int i = K - 1; i >= 0; i--)
	{
		if ((row + i) >= M)
			continue;

		bool winning = true;
		for (int j = 0; j < K && winning; j++)
		{
			if ((row+i-j) < 0)
			{
				winning = false;
				break;
			}

			if (board[row+i-j][col] != BLANK && board[row+i-j][col] != mark)
				winning = false;
		}

		if (winning)
			rectangles++;
	}

	// Count diagonal winning rectangles coming from the bottom left
	for (int i = K-1; i >= 0; i--)
	{
		if ( (row + i) >= M || (col - i) < 0  )
			continue;

		bool winning = true;
		for (int j = 0; j < K && winning; j++)
		{
			if ((row+i-j) < 0 || (col-i+j) >= N)
			{
				winning = false;
				break;
			}

			if (board[row+i-j][col-i+j] != BLANK && board[row+i-j][col-i+j] != mark)
				winning = false;
		}

		if (winning)
			rectangles++;
	}

	// Count diagonal winning rectangles coming from the bottom right
	for (int i = K-1; i >= 0; i--)
	{
		if ( (row + i) >= M || (col + i) >= N  )
			continue;

		bool winning = true;
		for (int j = 0; j < K && winning; j++)
		{
			if ((row+i-j) < 0 || (col+i-j) < 0)
			{
				winning = false;
				break;
			}

			if (board[row+i-j][col+i-j] != BLANK && board[row+i-j][col+i-j] != mark)
				winning = false;
		}

		if (winning)
			rectangles++;
	}

	return rectangles;
}

int ConnectK::minimax(const CharVectorVector& state, int alpha, int beta, int depth, bool isMaxNode, int& rowMoveToMake, int& columnMoveToMake, 
	const int DepthCutoff, const ExpirationTimer& timer) const
{
	int currentBestMoveRow = -1;
	int currentBestMoveColumn = -1;

	if (depth >= DepthCutoff || timer.HasExpired())
		return evaluate(state);

	for (int col = 0; col < N; col++)
	{
		if (state[0][col] == BLANK) //If column is not full
		{
			int currentRow = M - 1;
			while (state[currentRow][col] != BLANK) //Find the row of the column the next piece will be placed, starting at the bottom
				currentRow--;

			CharVectorVector childState = state;
			childState[currentRow][col] = computerMark; //Add the move for the child state

			if (isMaxNode)
			{
				int childValue = minimax(childState, alpha, beta, depth + 1, !isMaxNode, rowMoveToMake, columnMoveToMake, DepthCutoff, timer);
				// If at the top level, and this is the highest valued child so far, record the move to get there
				if (depth == 0 && childValue > alpha)
				{
					currentBestMoveRow = currentRow;
					currentBestMoveColumn = col;
				}
				// Update alpha value
				alpha = max(alpha, childValue);
			}
			else
			{
				beta = min(beta, minimax(childState, alpha, beta, depth + 1, !isMaxNode, rowMoveToMake, columnMoveToMake, DepthCutoff, timer));
			}

			if (alpha >= beta)
			{
#ifdef _DEBUG				
				_cprintf("Pruning with alpha %i and beta %i", alpha, beta);
#endif
				break;
			}
		}
	}

	if (depth == 0) // If this search finished in time, record the result
	{
		if (!timer.HasExpired())
		{
			rowMoveToMake = currentBestMoveRow;
			columnMoveToMake = currentBestMoveColumn;
		}
		else
		{
#ifdef _DEBUG
			_cprintf("Search of depth %i did not finish in time.\n", DepthCutoff);
#endif
		}
	}

	return (isMaxNode) ? alpha : beta;
}