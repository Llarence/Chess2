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

    int white_can_castle_kingside;
    int white_can_castle_queenside;
    int black_can_castle_kingside;
    int black_can_castle_queenside;

    int can_en_passent;
    int en_passent_x;
    int en_passent_y;

    int half_move_clock;

    int moves;
} Game;

typedef struct Move{
    int from_x;
    int from_y;
    int to_x;
    int to_y;
} Move;