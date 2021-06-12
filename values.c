#define DEPTH 8
#define MAX_DEPTH 14
#define FULL_DEPTH 4
#define EXTEND_SEARCH_NONPAWNS 6
#define EXTEND_DEPTH 2

#define PAWN_VALUE 100
#define KNIGHT_VALUE 320
#define BISHOP_VALUE 330
#define ROOK_VALUE 500
#define QUEEN_VALUE 900

const int WHITE_PAWN_TABLE[8][8] = 
{{0,  0,  0,  0,  0,  0,  0,  0},
{5, 10, 10,-20,-20, 10, 10,  5},
{5, -5,-10,  0,  0,-10, -5,  5},
{0,  0,  0, 20, 20,  0,  0,  0},
{5,  5, 10, 25, 25, 10,  5,  5},
{10, 10, 20, 30, 30, 20, 10, 10},
{50, 50, 50, 50, 50, 50, 50, 50},
{0,  0,  0,  0,  0,  0,  0,  0}};

const int BLACK_PAWN_TABLE[8][8] = 
{{0,  0,  0,  0,  0,  0,  0,  0},
{50, 50, 50, 50, 50, 50, 50, 50},
{10, 10, 20, 30, 30, 20, 10, 10},
{5,  5, 10, 25, 25, 10,  5,  5},
{0,  0,  0, 20, 20,  0,  0,  0},
{5, -5,-10,  0,  0,-10, -5,  5},
{5, 10, 10,-20,-20, 10, 10,  5},
{0,  0,  0,  0,  0,  0,  0,  0}};

const int WHITE_KNIGHT_TABLE[8][8] = 
{{-50,-40,-30,-30,-30,-30,-40,-50},
{-40,-20,  0,  5,  5,  0,-20,-40},
{-30,  5, 10, 15, 15, 10,  5,-30},
{-30,  0, 15, 20, 20, 15,  0,-30},
{-30,  5, 15, 20, 20, 15,  5,-30},
{-30,  0, 10, 15, 15, 10,  0,-30},
{-40,-20,  0,  0,  0,  0,-20,-40},
{-50,-40,-30,-30,-30,-30,-40,-50}};

const int BLACK_KNIGHT_TABLE[8][8] = 
{{-50,-40,-30,-30,-30,-30,-40,-50},
{-40,-20,  0,  0,  0,  0,-20,-40},
{-30,  0, 10, 15, 15, 10,  0,-30},
{-30,  5, 15, 20, 20, 15,  5,-30},
{-30,  0, 15, 20, 20, 15,  0,-30},
{-30,  5, 10, 15, 15, 10,  5,-30},
{-40,-20,  0,  5,  5,  0,-20,-40},
{-50,-40,-30,-30,-30,-30,-40,-50}};

const int WHITE_BISHOP_TABLE[8][8] = 
{{-20,-10,-10,-10,-10,-10,-10,-20},
{-10,  5,  0,  0,  0,  0,  5,-10},
{-10, 10, 10, 10, 10, 10, 10,-10},
{-10,  0, 10, 10, 10, 10,  0,-10},
{-10,  5,  5, 10, 10,  5,  5,-10},
{-10,  0,  5, 10, 10,  5,  0,-10},
{-10,  0,  0,  0,  0,  0,  0,-10},
{-20,-10,-10,-10,-10,-10,-10,-20}};

const int BLACK_BISHOP_TABLE[8][8] = 
{{-20,-10,-10,-10,-10,-10,-10,-20},
{-10,  0,  0,  0,  0,  0,  0,-10},
{-10,  0,  5, 10, 10,  5,  0,-10},
{-10,  5,  5, 10, 10,  5,  5,-10},
{-10,  0, 10, 10, 10, 10,  0,-10},
{-10, 10, 10, 10, 10, 10, 10,-10},
{-10,  5,  0,  0,  0,  0,  5,-10},
{-20,-10,-10,-10,-10,-10,-10,-20}};

const int WHITE_ROOK_TABLE[8][8] = 
{{0,  0,  0,  5,  5,  0,  0,  0},
{-5,  0,  0,  0,  0,  0,  0, -5},
{-5,  0,  0,  0,  0,  0,  0, -5},
{-5,  0,  0,  0,  0,  0,  0, -5},
{-5,  0,  0,  0,  0,  0,  0, -5},
{-5,  0,  0,  0,  0,  0,  0, -5},
{5, 10, 10, 10, 10, 10, 10,  5},
{0,  0,  0,  0,  0,  0,  0,  0}};

const int BLACK_ROOK_TABLE[8][8] = 
{{0,  0,  0,  0,  0,  0,  0,  0},
{5, 10, 10, 10, 10, 10, 10,  5},
{-5,  0,  0,  0,  0,  0,  0, -5},
{-5,  0,  0,  0,  0,  0,  0, -5},
{-5,  0,  0,  0,  0,  0,  0, -5},
{-5,  0,  0,  0,  0,  0,  0, -5},
{-5,  0,  0,  0,  0,  0,  0, -5},
{0,  0,  0,  5,  5,  0,  0,  0}};

const int WHITE_QUEEN_TABLE[8][8] = 
{{-20,-10,-10, -5, -5,-10,-10,-20},
{-10,  0,  0,  0,  0,  0,  0,-10},
{-10,  0,  5,  5,  5,  5,  0,-10},
{ -5,  0,  5,  5,  5,  5,  0, -5},
{ -5,  0,  5,  5,  5,  5,  0, -5},
{-10,  0,  5,  5,  5,  5,  0,-10},
{-10,  0,  0,  0,  0,  0,  0,-10},
{-20,-10,-10, -5, -5,-10,-10,-20}};

const int BLACK_QUEEN_TABLE[8][8] = 
{{-20,-10,-10, -5, -5,-10,-10,-20},
{-10,  0,  0,  0,  0,  0,  0,-10},
{-10,  0,  5,  5,  5,  5,  0,-10},
{ -5,  0,  5,  5,  5,  5,  0, -5},
{ -5,  0,  5,  5,  5,  5,  0, -5},
{-10,  0,  5,  5,  5,  5,  0,-10},
{-10,  0,  0,  0,  0,  0,  0,-10},
{-20,-10,-10, -5, -5,-10,-10,-20}};

const int WHITE_KING_TABLE[8][8] = 
{{ 20, 30, 10,  0,  0, 10, 30, 20},
{ 20, 20,  0,  0,  0,  0, 20, 20},
{-10,-20,-20,-20,-20,-20,-20,-10},
{-20,-30,-30,-40,-40,-30,-30,-20},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30}};

const int BLACK_KING_TABLE[8][8] = 
{{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-20,-30,-30,-40,-40,-30,-30,-20},
{-10,-20,-20,-20,-20,-20,-20,-10},
{ 20, 20,  0,  0,  0,  0, 20, 20},
{ 20, 30, 10,  0,  0, 10, 30, 20}};
