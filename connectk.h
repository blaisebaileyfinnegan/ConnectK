#include <afxwin.h>
#include <vector>
#include <array>

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
	int countWinningRectangles(const CharVectorVector& board, int row, int col, char mark) const;
	int evaluate(const CharVectorVector& board) const;
	int minimax(const CharVectorVector& state, int alpha, int beta, int depth, bool isMaxNode, int& rowMoveToMake, int& columnMoveToMake, const int& DepthOfRoot) const;
};