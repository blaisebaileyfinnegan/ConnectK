#include "StdAfx.h"
#include <iostream>
#include <conio.h>
#include <vector>
#include <math.h>

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

	// If x and y are not -1 then we need to record the move made by the human player
	// If x and y are -1, then this is the first move of the game, the AI moves first.
	if( ( row != -1 ) && ( col != -1 ) )
	{
		if (G)
		{
			row = M - 1; // Have to account for gravity (GUI gives where a mouse is clicked)
			while (board[row][col] != BLANK)
			{
				row--;
			}
		}

		if (computerMark == X)
		{
			board[row][col] = O;
		}
		else
		{
			board[row][col] = X;
		}
	}

	int enemyBestRow = -1;
	int enemyBestColumn = -1;

	int enemyScore = MinimaxV2(board, humanMark, computerMark, 0, enemyBestRow, enemyBestColumn);

	int rowMoveToMake = -1;
	int columnMoveToMake = -1;

	int myScore = MinimaxV2(board, computerMark, humanMark, 0, rowMoveToMake, columnMoveToMake);
		
#ifdef _DEBUG
	_cprintf("Evaluation function for my move (%i, %i) returned: %i\n", rowMoveToMake, columnMoveToMake, this->evaluate(board, computerMark, humanMark, 0));
	_cprintf("Evaluation function for enemy move (%i, %i) returned: %i\n", enemyBestRow, enemyBestColumn, this->evaluate(board, humanMark, computerMark, 0));
#endif
	if (enemyScore > myScore)
	{
		rowMoveToMake = enemyBestRow;
		columnMoveToMake = enemyBestColumn;
	}

	//float idsSearchTime = 2.0f;
	//IDSMinimaxWithABPrune(board, rowMoveToMake, columnMoveToMake, idsSearchTime);

	CharVectorVector fake = board;
	fake[2][5] = computerMark;

	// record the move made by the AI
	board[rowMoveToMake][columnMoveToMake] = computerMark;
	// return the move made by the AI
	row = rowMoveToMake;
	col = columnMoveToMake;


}

// Takes any number of arguments and applies weight to it.
int ConnectK::weigh(int *segments, int depth) const
{
	int points = 0;
	for (int i = 1; i <= K; i++)
	{
		points += (segments[i]*pow(i, 4));
	}
	delete segments;
	return points;
}

// Evaluation function mark agnostic
// (own winning rows) - (opponent's winning rows)
// board: The potential game state
// depth: which move is it?
int ConnectK::evaluate(const CharVectorVector& board, char mark, char enemyMark, int depth, boolean weighted) const
{
	int points = weigh(countSegmentLengths(board, mark), depth);
	int enemyPoints = weigh(countSegmentLengths(board, enemyMark), depth);

	/*
#ifdef _DEBUG
	_cprintf("\tMy points: %i\n", points);
	_cprintf("\tOpponent's points: %i\n", enemyPoints);
#endif
	*/

	return points - enemyPoints;
}

// Returns array of how many of our marks are in each segment of length index. segments[3] > 1 == WIN
// Brute forces every possible segment to allow for gravity off case
int *ConnectK::countSegmentLengths(const CharVectorVector& board, char mark) const
{
	int *segments = new int[K+1];
	for (int i = 0; i <= K; i++)
		segments[i] = 0;

	// We need to manipulate it
	CharVectorVector fakeHorizontalBoard = board;
	CharVectorVector fakeVerticalBoard = board;

	// Start from the bottom left
	for (int i = M - 1; i >= 0; i--)
	{
		for (int j = 0; j < N; j++)
		{
//			if (board[i][j] == mark) 
//			{
				// Start with horizontal (going towards the right)
				if (j <= N-K)
				{
					int b = 0;
					for (int a = 0; a < K; a++) 
					{
						if (fakeHorizontalBoard[i][j+a] == mark)
						{
							fakeHorizontalBoard[i][j+a] = BLANK;
							b++;
						}
						else if (fakeHorizontalBoard[i][j+a] != BLANK) // must be the enemy
						{
							b = -1;
							break;
						}
					}
					if (b != -1)
						segments[b]++;
				}

				// Vertical
				if (i >= K-1)
				{
					int b = 0;
					for (int a = 0; a < K; a++)
					{
						if (fakeVerticalBoard[i-a][j] == mark)
						{
							fakeVerticalBoard[i-a][j] = BLANK;
							b++;
						}
						else if (fakeVerticalBoard[i-a][j] != BLANK) // must be the enemy
						{
							b = -1;
							break;
						}
					}

					if (b != -1)
						segments[b]++;
				}
//			}
		}
	}

	// Right now, the array is inverted. We need to invert it to return an array according the the method signature comment above
	//for (int i = 0, temp = segments[K-1-i]; i < K || K-1-i <= i; i++, temp = segments[K-1-i]) {
	//	segments[K-1-i] = segments[i];
	//	segments[i] = temp;
	//}

	return segments;
}



int ConnectK::MinimaxV2(CharVectorVector &state, char mark, char enemyMark, int depth, int &row, int &col, bool isMaxNode)
{
	// Cut off at arbitrary depth
	if (depth == 2)
		return evaluate(state, mark, enemyMark, depth);

	int bestRow = -1, bestCol = -1, bestScore = -INFINITY;

	for (int i = M - 1; i >= 0; i--)
	{
		for (int j = 0; j < N; j++)
		{
			if (state[i][j] == BLANK)
			{
				// If gravity is ON, check to make sure this isn't a floating block
				if ( G && (i+1) < M && state[i+1][j] == BLANK)
					continue;

				CharVectorVector childState = state;
				childState[i][j] = mark;
				int score = MinimaxV2(childState, enemyMark, mark, depth + 1, bestRow, bestCol, !isMaxNode);

				if (isMaxNode)
					score = -score;

				if (score > bestScore)
				{
					bestScore = score;
					bestRow = i;
					bestCol = j;
				}
			}
		}
	}


	if (bestRow == -1 || bestCol == -1)
		return evaluate(state, mark, enemyMark, depth);

	row = bestRow, col = bestCol;
	return evaluate(state, mark, enemyMark, depth);
}


void ConnectK::IDSMinimaxWithABPrune(CharVectorVector& state, int& rowMoveToMake, int& columnMoveToMake, const float idsSearchTime) const
{
	ExpirationTimer timer(idsSearchTime);
	timer.Start();

	int currentMaxDepth = -1;
	while (!timer.HasExpired())
	{
		++currentMaxDepth;
		int valueOfBestMove = minimax(state, -INFINITY, INFINITY, 0, true, rowMoveToMake, columnMoveToMake, currentMaxDepth, timer);
	}

#ifdef _DEBUG
	_cprintf("AI IDS went to a depth of %i before stopping (may not have finished at this depth).\n", currentMaxDepth);
#endif
}

int ConnectK::minimax(const CharVectorVector& state, int alpha, int beta, int depth, bool isMaxNode, int& rowMoveToMake, int& columnMoveToMake, 
	const int DepthCutoff, const ExpirationTimer& timer) const
{
	int currentBestMoveRow = -1;
	int currentBestMoveColumn = -1;

	if (Cutoff(depth, DepthCutoff, timer))
		return evaluate(state, computerMark, humanMark, depth);

	for (int col = 0; col < N; col++)
	{
		if (state[0][col] == BLANK) //If column is not full
		{
			int currentRow = M - 1;
			while (state[currentRow][col] != BLANK) //Find the row of the column the next piece will be placed, starting at the bottom
				currentRow--;

			CharVectorVector childState = state;
			char markToMake = (isMaxNode) ? computerMark : humanMark;
			childState[currentRow][col] = markToMake; //Add the move for the child state

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

bool ConnectK::Cutoff(const int currentDepth, const int DepthCutoff, const ExpirationTimer& timer) const
{
	return (currentDepth >= DepthCutoff) || timer.HasExpired();
}