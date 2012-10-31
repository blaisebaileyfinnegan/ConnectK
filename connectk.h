#include <afxwin.h>
#include <numeric>

#define BLANK ' '
#define X     'X'
#define O     'O'
#define INFINITY 99999999999

typedef char * CharArray;
typedef CharArray * CharArrayArray; 

class ConnectK
{
private:

	int M;    // row size
	int N;    // column size
	int K;    // length of the straight line to win
	bool G;   // gravity

	char humanMark;
	char computerMark;  // mark played by AI player for board evaluation

	CharArrayArray board;    // board is indexed by [row][column]
	int numOccupied;

public:

	ConnectK();
	~ConnectK();

	void newGame(int M, int N, int K, bool G, char pmark, char hmark);
	void nextMove(int &row, int &col);

private:

	int countWinningRectangles(CharArrayArray board, int row, int col, char mark);
	int evaluate(CharArrayArray board);
	int minimax(CharArrayArray state, int alpha, int beta, int depth, bool isMaxNode);
};