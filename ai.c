#ifndef AI
#define AI

#include <stdlib.h>

#include "game.c"

const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 300;
const int BISHOP_VALUE = 300;
const int ROOK_VALUE = 600;
const int QUEEN_VALUE = 900;

typedef struct ValuedMove{
    int value;
    Move move;
} ValuedMove;

int eval(Game *game, int color){
    int over = isOver(game);
    if(over == CHECKMATE){
        if(game->turn == color){
            return 100000;
        }else{
            return -100000;
        }
    }

    if(over == STALEMATE){
        return 0;
    }

    int value = 0;
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            Piece piece = game->board[x][y];
            if(piece.color == color){
                switch(piece.type){
                    case PAWN:
                        value += PAWN_VALUE;
                        break;
                    
                    case KNIGHT:
                        value += KNIGHT_VALUE;
                        break;

                    case BISHOP:
                        value += BISHOP_VALUE;
                        break;

                    case ROOK:
                        value += ROOK_VALUE;
                        break;
                    
                    case QUEEN:
                        value += QUEEN_VALUE;
                        break;
                }
            }else{
                switch(piece.type){
                    case PAWN:
                        value -= PAWN_VALUE;
                        break;
                    
                    case KNIGHT:
                        value -= KNIGHT_VALUE;
                        break;

                    case BISHOP:
                        value -= BISHOP_VALUE;
                        break;

                    case ROOK:
                        value -= ROOK_VALUE;
                        break;
                    
                    case QUEEN:
                        value -= QUEEN_VALUE;
                        break;
                }
            }
        }
    }

    return value + rand() % 10;
}

ValuedMove maxMove(Game *game, int color, int depth);

ValuedMove minMove(Game *game, int color, int depth){
    if(depth == 0){
        return (ValuedMove){eval(game, color), (Move){-1, -1, -1, -1, -1}};
    }

    Move moves[512];
    generateLegalMoves(game, moves);
    if(moves->fromX == -1){
        return (ValuedMove){eval(game, color), (Move){-1, -1, -1, -1, -1}};
    }

    ValuedMove best = {1000000, (Move){-1, -1, -1, -1, -1}};
    for(int i = 0; i < 512; i++){
        if(moves[i].fromX == -1){
            break;
        }
        
        Game newGame = copyGame(game);
        doMove(&newGame, moves[i]);
        ValuedMove curr = maxMove(&newGame, color, depth - 1);

        if(curr.value < best.value){
            best.move = moves[i];
            best.value = curr.value;
        }
    }

    return best;
}

ValuedMove maxMove(Game *game, int color, int depth){
    if(depth == 0){
        return (ValuedMove){eval(game, color), (Move){-1, -1, -1, -1, -1}};
    }

    Move moves[512];
    generateLegalMoves(game, moves);
    if(moves->fromX == -1){
        return (ValuedMove){eval(game, color), (Move){-1, -1, -1, -1, -1}};
    }

    ValuedMove best = {-1000000, (Move){-1, -1, -1, -1, -1}};
    for(int i = 0; i < 512; i++){
        if(moves[i].fromX == -1){
            break;
        }

        Game newGame = copyGame(game);
        doMove(&newGame, moves[i]);
        ValuedMove curr = minMove(&newGame, color, depth - 1);

        if(curr.value > best.value){
            best.move = moves[i];
            best.value = curr.value;
        }
    }

    return best;
}

#endif