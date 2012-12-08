#include <afxwin.h>
#include <vector>
#include <array>
#include "ExpirationTimer.h"

#define BLANK ' '
#define X     'X'
#define O     'O'
#define INFINITY 99999999999

typedef std::vector<std::vector<char>> CharVectorVector;

class ConnectK
{
private:
	int M;    // row size
	int N;    // column size
	int K;    // length of the straight line to win
	bool G;   // gravity
	const bool ABPruneEnabled;

	char humanMark;
	char computerMark;  // mark played by AI player for board evaluation

	CharVectorVector board;
	
	int numOccupied;

public:

	ConnectK();
	~ConnectK();

	void newGame(int M, int N, int K, bool G, char pmark, char hmark);
	void nextMove(int &row, int &col);

private:
	void IDSMinimax(CharVectorVector& state, int& rowMoveToMake, int& columnMoveToMake, const float idsSearchTime) const;
	int Minimax(const CharVectorVector& state, int alpha, int beta, int depth, bool isMaxNode, int& rowMoveToMake, int& columnMoveToMake, const int DepthCutoff, const ExpirationTimer& timer) const;
	bool ShouldABPrune(const int alpha, const int beta) const;

	bool IsStateFull(const CharVectorVector& state) const;
	bool Cutoff(const CharVectorVector& state, const int currentDepth, const int DepthCutoff, const ExpirationTimer& timer) const;
	bool GameWinningMoveFound(const CharVectorVector& state) const;

	int weigh(std::vector<int> segments) const;
	std::vector<int> countSegmentLengths(const CharVectorVector& board, char mark) const;
	int countWinningRectangles(const CharVectorVector& board, int row, int col, char mark) const;
	int evaluate(const CharVectorVector& board, char mark, char enemyMark, boolean weighted = true) const;
};