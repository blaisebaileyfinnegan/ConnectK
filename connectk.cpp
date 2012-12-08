#include "StdAfx.h"
#include <iostream>
#include <conio.h>
#include <vector>
#include <math.h>

using namespace std;

#include "ConnectK.h"

// constructor
ConnectK::ConnectK()
	: M(0), N(0), K(0), G(FALSE), ABPruneEnabled(true)
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

	int rowMoveToMake = -1;
	int columnMoveToMake = -1;
	float idsSearchTime = 5.0f;
	IDSMinimax(board, rowMoveToMake, columnMoveToMake, idsSearchTime);

	// record the move made by the AI
	board[rowMoveToMake][columnMoveToMake] = computerMark;
	// return the move made by the AI
	row = rowMoveToMake;
	col = columnMoveToMake;

#ifdef _DEBUG
	_cprintf("Evaluation function for move (%i, %i) returned: %i\n", rowMoveToMake, columnMoveToMake, this->evaluate(board, computerMark, humanMark));
#endif
}

// Purpose:     Performs a minimax search using IDS. Returns results of the deepest, finished search depth.
//
// Parameters:	&state				The current state of the game
//				&rowMoveToMake		Used as return parameter
//				&columnMoveToMake	Used as return parameter
//				idsSearchTime		Time length IDS should run for
//
// Returns:		&rowMoveToMake		place the row value of the move that should be made by your AI search in this variable
//				&columnMoveToMake	place the column value of the move that should be made by your AI in this variable
void ConnectK::IDSMinimax(CharVectorVector& state, int& rowMoveToMake, int& columnMoveToMake, const float idsSearchTime) const
{
	ExpirationTimer timer(idsSearchTime);
	timer.Start();

	int currentMaxDepth = -1;
	while (!timer.HasExpired())
	{
		++currentMaxDepth;
		int valueOfBestMove = Minimax(state, -INFINITY, INFINITY, 0, true, rowMoveToMake, columnMoveToMake, currentMaxDepth, timer);
	}

#ifdef _DEBUG
	_cprintf("AI IDS went to a depth of %i before stopping (may not have finished at this depth).\n", currentMaxDepth);
#endif
}

// Purpose:     Returns whether or not a prune should take place.
//
// Parameters:	alpha	Alpha value for AB prune check
//				beta	Beta value for AB prune check
//
// Returns:		boolean representing whether an AB prune should take place
//
// Note: ABPruneEnabled must be true for the ConnectK object for this to return true.
bool ConnectK::ShouldABPrune(const int alpha, const int beta) const
{
	if (ABPruneEnabled && alpha >= beta)
	{
#ifdef _DEBUG				
		_cprintf("Pruning with alpha %i and beta %i", alpha, beta);
#endif
		return true;
	}

	return false;
}

// Purpose:     Performs a minimax search to depth specified. Can return early with no results if timer expires before search finishes.
//
// Parameters:	&state				The current state of the game
//				alpha				Alpha value for current state
//				beta				Beta value for current state
//				depth				Current depth of search.
//				isMaxNode			Whether current state is a max node in regards to a minimax search. (max nodes simulate the AI's possible moves)
//				&rowMoveToMake		Used as return parameter
//				&columnMoveToMake	Used as return parameter
//				DepthCutoff			Max depth to search
//				&timer				Timer used to check if the search should continue (will stop searching if timer expires)
//
// Returns:		&rowMoveToMake		place the row value of the move that should be made by your AI search in this variable
//				&columnMoveToMake	place the column value of the move that should be made by your AI in this variable
int ConnectK::Minimax(const CharVectorVector& state, int alpha, int beta, int depth, bool isMaxNode, int& rowMoveToMake, int& columnMoveToMake, 
	const int DepthCutoff, const ExpirationTimer& timer) const
{
	int currentBestMoveRow = -1; // Used to keep track of the move that resulted in the best move at this level
	int currentBestMoveColumn = -1;

	if (Cutoff(state, depth, DepthCutoff, timer))
		return evaluate(state, computerMark, humanMark);

	for (int col = 0; col < N; col++)
	{
		for (int row = M - 1; row >= 0; row--)
		{
			if (state[row][col] == BLANK) //If space is empty
			{
				if (G && (row + 1) < M && state[row + 1][col] == BLANK)
					break; // No more moves can be found in this column with gravity on

				CharVectorVector childState = state; // Create a child state to continue searching on
				char markToMake = (isMaxNode) ? computerMark : humanMark;
				childState[row][col] = markToMake; //Add the move for the child state

				if (isMaxNode)
				{
					int childValue = Minimax(childState, alpha, beta, depth + 1, !isMaxNode, rowMoveToMake, columnMoveToMake, DepthCutoff, timer);
					// If at the top level, and this is the highest valued child so far, record the move to get there
					if (depth == 0 && childValue > alpha)
					{
						currentBestMoveRow = row;
						currentBestMoveColumn = col;
					}
					// Update alpha value
					alpha = max(alpha, childValue);
				}
				else
				{
					// Update beta value
					beta = min(beta, Minimax(childState, alpha, beta, depth + 1, !isMaxNode, rowMoveToMake, columnMoveToMake, DepthCutoff, timer));
				}

				if (ShouldABPrune(alpha, beta)) // Prune the rest of the moves in the current column
				{
					break;
				}
			}
		}

		if (ShouldABPrune(alpha, beta)) // Prune the rest of the moves available at this state
		{
			break;
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

// Purpose:     Determines if the state given has any empty spaces. Returns true if no empty spaces are found.
//
// Parameters:	&state		The current state of the game
//
// Returns:		True if the state given has no empty spaces; otherwise, returns false.
bool ConnectK::IsStateFull(const CharVectorVector& state) const
{
	bool stateFull = true;
	std::vector<char>::const_iterator iter;

	for (int i = 0; i < M; i++)
	{
		iter = find(state[i].begin(), state[i].end(), BLANK);
		if (iter != state[i].end())
		{
			stateFull = false;
			break;
		}
	}

	return stateFull;
}

// Purpose:     To determine if a state should continue to be evaluated (its child states), or to stop searching at this state.
//
// Parameters:	&state				The current state of the game
//				depth				Current depth of search
//				DepthCutoff			Max depth to search
//				&timer				Timer used to check if the search should continue (will signal to Cutoff searching if timer expires)
//
// Returns:		True if the search should stop at this state and not evaluate any of its child states.
bool ConnectK::Cutoff(const CharVectorVector& state, const int currentDepth, const int DepthCutoff, const ExpirationTimer& timer) const
{
	return (currentDepth >= DepthCutoff) || timer.HasExpired() || IsStateFull(state) || GameWinningMoveFound(state);
}

// Purpose:     To determine if there is a winning player on a given state.
//
// Parameters:	&state		The state to check
//
// Returns:		True if a player has K in a row in the state
bool ConnectK::GameWinningMoveFound(const CharVectorVector& state) const
{
	std::vector<int> lengths;
	lengths = countSegmentLengths(state, humanMark); // Get number of consecutive pieces of same player of length K

	return lengths[K] > 0; // Greater than 0 if a winning move was found
}

// Takes any number of arguments and applies weight to it.
int ConnectK::weigh(std::vector<int> segments) const
{
	int points = 0;
	for (int i = 1; i <= K; i++)
	{
		points += (segments[i]*pow(i, 4.0f));
	}

	return points;
}

// Returns array of how many of our marks are in each segment of length index. segments[3] > 1 == WIN
// Brute forces every possible segment to allow for gravity off case
std::vector<int> ConnectK::countSegmentLengths(const CharVectorVector& board, char mark) const
{
	vector<int> segments(K+1, 0);

	// Start from the bottom left
	for (int i = M - 1; i >= 0; i--)
	{
		for (int j = 0; j < N; j++)
		{
			if (j <= N-K)
			{
				int b = 0;
				for (int a = 0; a < K; a++) 
				{
					if (board[i][j+a] == mark)
					{
						//horizontalBoard[i][j+a] = BLANK;
						b++;
					}
					else if (board[i][j+a] != BLANK) // must be the enemy
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
					if (board[i-a][j] == mark)
					{
						//verticalBoard[i-a][j] = BLANK;
						b++;
					}
					else if (board[i-a][j] != BLANK) // must be the enemy
					{
						b = -1;
						break;
					}
				}

				if (b != -1)
					segments[b]++;
			}
				
			// Forward Diagonal
			if (i >= K-1 && j <= N-K)
			{
				int b = 0;
				for (int a = 0; a < K; a++)
				{
					if (board[i-a][j+a] == mark)
					{
						//forwardDiagonalBoard[i-a][j+a] = BLANK;
						b++;
					}
					else if (board[i-a][j+a] != BLANK) // must be the enemy
					{
						b = -1;
						break;
					}
				}

				if (b != -1)
					segments[b]++;
			}

			// Backwards Diagonal
			if (i >= K && j >= K - 1)
			{
				int b = 0;
				for (int a = 0; a < K; a++)
				{
					if (board[i-a][j-a] == mark)
					{
						//backDiagonalBoard[i-a][j-a] = BLANK;
						b++;
					}
					else if (board[i-a][j-a] != BLANK) // must be the enemy
					{
						b = -1;
						break;
					}
				}

				if (b != -1)
					segments[b]++;
			}
		}
	}

	return segments;
}

// Minimal evaluation function
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

// Evaluation function mark agnostic
// (own winning rows) - (opponent's winning rows)
// board: The potential game state
int ConnectK::evaluate(const CharVectorVector& board, char mark, char enemyMark, boolean weighted) const
{
	int points = 0, enemyPoints = 0;
	
	if (!weighted)
	{
		// Search for squares
		for (int row = M - 1; row >= 0 ; row--)
		{
			for (int col = 0; col < N; col++)
			{
				// We've found a piece that belongs to us
				if (board[row][col] == mark) 
					points += countWinningRectangles(board, row, col, mark);
				else if (board[row][col] == enemyMark)
					enemyPoints += countWinningRectangles(board, row, col, enemyMark);
			}
		}
	}
	else
	{
		points = weigh(countSegmentLengths(board, mark));
		enemyPoints = weigh(countSegmentLengths(board, enemyMark));
	}

#ifdef _DEBUG
	_cprintf("\tMy points: %i\n", points);
	_cprintf("\tOpponent's points: %i\n", enemyPoints);
#endif

	return points - enemyPoints;
}