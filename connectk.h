#include <afxwin.h>

#define BLANK ' '
#define X     'X'
#define O     'O'

typedef char * CharArray;
typedef CharArray * CharArrayArray; 

class ConnectK
{
private:

	int M;    // row size
	int N;    // column size
	int K;    // length of the straight line to win
	bool G;   // gravity

	char computerMark;  // mark played by AI player for board evaluation

	CharArrayArray board;    // board is indexed by [row][column]
	int numOccupied;

public:

	ConnectK();

	~ConnectK();

	void newGame(int M, int N, int K, bool G, char pmark);

	void nextMove(int &row, int &col);
};