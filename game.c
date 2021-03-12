const int FALSE = 0;
const int TRUE = 1;

const int NONE = 0;
const int PAWN = 1;
const int KNIGHT = 2;
const int BISHOP = 3;
const int ROOK = 4;
const int QUEEN = 5;
const int KING = 6;

const int WHITE = 0;
const int BLACK = 1;

typedef struct Piece{
    int type;
    int color;
} Piece;

typedef struct Game{
    Piece board[8][8];

    int  turn;

    int whiteCanCastleKingside;
    int whiteCanCastleQueenside;
    int blackCanCastleKingside;
    int blackCanCastleQueenside;

    int canEnPassent;
    int enPassentX;
    int enPassentY;

    int halfMoveClock;

    int moves;
} Game;

typedef struct Move{
    int fromX;
    int fromY;
    int toX;
    int toY;
} Move;